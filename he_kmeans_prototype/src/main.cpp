/**
 * @file main.cpp
 * @brief Plaintext K-Means Prototype with Accuracy Evaluation
 * 
 * MIT License
 * Copyright (c) 2024-2026 Lakshmi R. Kiran Pasumarthy
 * SPDX-License-Identifier: MIT
 * 
 * TIPS-HECluster — Privacy-Preserving Threat Intelligence Clustering
 * Trusted Privacy-Preserving Threat Information Platform for Sharing (TIPS)
 * Edinburgh Napier University — PhD Research
 * 
 * @details
 * Proof-of-concept plaintext k-means implementation used to establish
 * the baseline for encrypted clustering accuracy comparison.
 * 
 * Functionality:
 * 1. Read threat feature vectors from CSV
 * 2. Run standard k-means algorithm (plaintext)
 * 3. Read ground-truth labels from separate CSV
 * 4. Compute Adjusted Rand Index (ARI) metric
 * 5. Compare with encrypted HE clustering results
 * 
 * This unencrypted version is essential for:
 * - Validating algorithm correctness
 * - Establishing baseline performance metrics  
 * - Computing ARI >= 0.85 threshold validation
 * - Benchmarking against HE-MPC overhead
 * 
 * @author Lakshmi R. Kiran Pasumarthy
 * @version 1.0
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
#include <random>
#include <stdexcept>
#include <chrono>

/**
 * @brief Parse numeric CSV file into 2D vector
 * 
 * @param path Path to CSV file (with header row)
 * 
 * @return 2D vector where each inner vector is a row (data point)
 * 
 * @details
 * Reads CSV with numeric values only. First line is treated as header
 * and skipped. Each subsequent line is parsed as comma-separated floats.
 * Empty rows and cells are skipped.
 */
static std::vector<std::vector<double>> readcsv(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open " + path);

    std::string line;
    std::vector<std::vector<double>> X;

    // header
    if (!std::getline(in, line)) throw std::runtime_error("empty csv");

    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string cell;
        std::vector<double> row;
        while (std::getline(ss, cell, ',')) {
            if (cell.empty()) continue;
            row.push_back(std::stod(cell));
        }
        if (!row.empty()) X.push_back(row);
    }

    if (X.empty()) throw std::runtime_error("no data rows");
    return X;
}

/**
 * @brief Compute squared Euclidean distance between two points
 * 
 * @param a First point (feature vector)
 * @param b Second point (feature vector)
 * 
 * @return Squared L2 distance: sum((a[i] - b[i])^2)
 * 
 * @details
 * Used for k-means cluster assignment and centroid comparison.
 * Squared distance avoids sqrt() computation cost.
 */
static double sqdist(const std::vector<double>& a, const std::vector<double>& b) {
    double s = 0.0;
    for (size_t i = 0; i < a.size(); i++) {
        double d = a[i] - b[i];
        s += d * d;
    }
    return s;
}

/**
 * @brief Standard k-means clustering algorithm
 * 
 * @param X Input data points (n x d matrix)
 * @param k Number of clusters
 * @param iters Maximum iterations
 * @param labels Output: cluster assignment for each point (modified in-place)
 * 
 * @return Final centroids as k x d matrix
 * 
 * @details
 * Lloyd's algorithm: iteratively assign points to nearest centroid
 * and update centroids as cluster means. Continues for @p iters iterations
 * or until convergence.
 * 
 * Initialization: Randomly sample k points from data as initial centroids
 */
static std::vector<std::vector<double>> kmeans(const std::vector<std::vector<double>>& X,
                                               int k,
                                               int iters,
                                               std::vector<int>& labels) {
    const int n = (int)X.size();
    const int d = (int)X[0].size();
    labels.assign(n, 0);

    // Initialize centroids by randomly sampling k points from data
    // Fixed seed (424242) for reproducibility across runs
    std::mt19937_64 rng(424242);
    std::uniform_int_distribution<int> uni(0, n - 1);
    std::vector<std::vector<double>> C(k, std::vector<double>(d, 0.0));
    for (int j = 0; j < k; j++) C[j] = X[uni(rng)];

    for (int t = 0; t < iters; t++) {
        // Assignment step: assign each point to nearest centroid
        for (int i = 0; i < n; i++) {
            double best = std::numeric_limits<double>::infinity();
            int bestj = 0;
            for (int j = 0; j < k; j++) {
                double dist = sqdist(X[i], C[j]);
                if (dist < best) {
                    best = dist;
                    bestj = j;
                }
            }
            labels[i] = bestj;
        }

        // Update step: recompute centroids as cluster means
        std::vector<std::vector<double>> newC(k, std::vector<double>(d, 0.0));
        std::vector<int> cnt(k, 0);

        for (int i = 0; i < n; i++) {
            int j = labels[i];
            cnt[j]++;
            for (int p = 0; p < d; p++) newC[j][p] += X[i][p];
        }
        for (int j = 0; j < k; j++) {
            if (cnt[j] == 0) continue;
            for (int p = 0; p < d; p++) newC[j][p] /= (double)cnt[j];
        }
        C.swap(newC);
    }

    return C;
}

/**
 * @brief Adjusted Rand Index (ARI) for clustering accuracy
 * 
 * @param a Predicted cluster labels (from k-means)
 * @param b Ground-truth labels
 * 
 * @return ARI score in range [-1, 1]
 *   - ARI = 1: Perfect clustering match
 *   - ARI = 0: Random clustering
 *   - ARI < 0: Worse than random
 * 
 * @details
 * ARI measures agreement between two labelings accounting for chance.
 * In thesis validation, ARI >= 0.85 indicates HE clustering accuracy
 * is equivalent to plaintext baseline (within acceptable bounds).
 */
static double ari(const std::vector<int>& a, const std::vector<int>& b) {
    const int n = (int)a.size();
    if ((int)b.size() != n) throw std::runtime_error("label size mismatch");

    int maxa = 0, maxb = 0;
    for (int i = 0; i < n; i++) {
        if (a[i] > maxa) maxa = a[i];
        if (b[i] > maxb) maxb = b[i];
    }
    int ra = maxa + 1;
    int rb = maxb + 1;

    std::vector<std::vector<int>> nij(ra, std::vector<int>(rb, 0));
    std::vector<int> ai(ra, 0), bj(rb, 0);

    for (int i = 0; i < n; i++) {
        nij[a[i]][b[i]]++;
        ai[a[i]]++;
        bj[b[i]]++;
    }

    auto comb2 = [](double x) { return x * (x - 1.0) / 2.0; };

    double sumnij = 0.0;
    for (int i = 0; i < ra; i++)
        for (int j = 0; j < rb; j++)
            sumnij += comb2((double)nij[i][j]);

    double sumai = 0.0;
    for (int i = 0; i < ra; i++) sumai += comb2((double)ai[i]);

    double sumbj = 0.0;
    for (int j = 0; j < rb; j++) sumbj += comb2((double)bj[j]);

    double total = comb2((double)n);
    double expected = (sumai * sumbj) / total;
    double maxindex = 0.5 * (sumai + sumbj);

    if (maxindex - expected == 0.0) return 0.0;
    return (sumnij - expected) / (maxindex - expected);
}

/**
 * @brief Load ground-truth cluster labels from CSV
 * 
 * @param path Path to labels CSV file
 * 
 * @return Vector of integer labels
 * 
 * @details
 * Reads single-column CSV with one label per line (header skipped).
 * Used for comparing k-means predictions against known classifications.
 */
static std::vector<int> readlabels(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open " + path);

    std::string line;
    std::vector<int> y;

    // header
    if (!std::getline(in, line)) throw std::runtime_error("empty labels csv");

    while (std::getline(in, line)) {
        if (line.empty()) continue;
        y.push_back(std::stoi(line));
    }
    return y;
}

int main(int argc, char** argv) {
    try {
        std::string datapath = "../data/stix_vectors.csv";
        std::string labelspath = "../data/labels.csv";
        int k = 3;
        int iters = 10;

        auto X = readcsv(datapath);
        std::vector<int> truth = readlabels(labelspath);

        std::vector<int> pred;

        auto t0 = std::chrono::high_resolution_clock::now();
        auto C = kmeans(X, k, iters, pred);
        auto t1 = std::chrono::high_resolution_clock::now();

        double runtime_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        double score = ari(truth, pred);

        std::cout << "ARI = " << score << std::endl;
        std::cout << "runtime_ms=" << runtime_ms << std::endl;

        // Save centroids
        {
            std::ofstream out("../results/centroids.csv");
            out << "c0,c1,c2\n";
            for (int j = 0; j < k; j++) {
                for (size_t p = 0; p < C[j].size(); p++) {
                    out << C[j][p];
                    if (p + 1 < C[j].size()) out << ",";
                }
                out << "\n";
            }
        }

        // Save predicted labels
        {
            std::ofstream out2("../results/predlabels.csv");
            out2 << "label\n";
            for (int v : pred) out2 << v << "\n";
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
}
