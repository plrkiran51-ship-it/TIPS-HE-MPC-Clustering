/**
 * @file main1.cpp
 * @brief Plaintext K-Means Variant (Experimental)
 * 
 * MIT License
 * Copyright (c) 2024-2026 Lakshmi R. Kiran Pasumarthy
 * SPDX-License-Identifier: MIT
 * 
 * TIPS-HECluster — Privacy-Preserving Threat Intelligence Clustering
 * Trusted Privacy-Preserving Threat Information Platform for Sharing (TIPS)
 * Edinburgh Napier University — PhD Research
 * 
 * @deprecated Experimental variant; see he_kmeans_prototype/src/main.cpp for primary version
 * 
 * @details
 * Alternative implementation of plaintext k-means used for testing
 * different initialization strategies or convergence criteria.
 * 
 * This version maintains identical functionality to main.cpp but may
 * have been used to evaluate parameter variations during the research phase.
 * 
 * @author Lakshmi R. Kiran Pasumarthy
 * @version 1.0 (experimental)
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

static double sqdist(const std::vector<double>& a, const std::vector<double>& b) {
    double s = 0.0;
    for (size_t i = 0; i < a.size(); i++) {
        double d = a[i] - b[i];
        s += d * d;
    }
    return s;
}

static std::vector<std::vector<double>> kmeans(const std::vector<std::vector<double>>& X,
                                               int k,
                                               int iters,
                                               std::vector<int>& labels) {
    const int n = (int)X.size();
    const int d = (int)X[0].size();
    labels.assign(n, 0);

    // init centroids by sampling k points
    std::mt19937_64 rng(42);
    std::uniform_int_distribution<int> uni(0, n - 1);
    std::vector<std::vector<double>> C(k, std::vector<double>(d, 0.0));
    for (int j = 0; j < k; j++) C[j] = X[uni(rng)];

    for (int t = 0; t < iters; t++) {
        // assign
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

        // update
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

// Adjusted Rand Index (ARI) for two labelings of n items.
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
