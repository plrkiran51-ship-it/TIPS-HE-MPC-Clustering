/**
 * @file benchmark.cpp
 * @brief Performance Benchmarking for HE-MPC Clustering Pipeline
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
 * This module measures the execution time of key HE operations:
 * - Encryption of threat data
 * - Homomorphic k-means clustering
 * - Decryption of cluster assignments
 * 
 * Uses std::chrono high_resolution_clock for microsecond-precision timing.
 * Output is printed to stdout in human-readable format.
 * 
 * Typical workflow:
 * 1. Initialize encryption context and key pair
 * 2. Fetch threat data from API
 * 3. Time and execute: encryption → clustering → decryption
 * 4. Report execution costs for each stage
 * 
 * @author Lakshmi R. Kiran Pasumarthy
 * @version 1.0
 */

#include <chrono>
#include "openfhe.h"

using namespace lbcrypto;
using namespace std::chrono;

/**
 * @brief Measures and prints execution time for an operation
 * 
 * @param operation Descriptive name of the operation being timed (e.g., "Encryption")
 * @param start Time point captured before the operation began
 * 
 * @details
 * Computes elapsed time in microseconds from @p start to the current time,
 * then outputs a human-readable message like:
 *   "Encryption Execution Time: 45230 microseconds."
 */
void BenchmarkExecution(const std::string& operation, high_resolution_clock::time_point start) {
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << operation << " Execution Time: " << duration << " microseconds." << std::endl;
}

int main() {
    CryptoContext<DCRTPoly> cc = InitializeEncryption();
    KeyPair<DCRTPoly> keyPair = cc->KeyGen();
    
    json threat_data = FetchThreatData();

    auto start_encrypt = high_resolution_clock::now();
    vector<Ciphertext<DCRTPoly>> encrypted_data = EncryptThreatData(cc, keyPair, threat_data);
    BenchmarkExecution("Encryption", start_encrypt);

    auto start_cluster = high_resolution_clock::now();
    vector<Ciphertext<DCRTPoly>> clustered_data = HomomorphicKMeans(cc, encrypted_data, 3, 5);
    BenchmarkExecution("Clustering", start_cluster);

    auto start_decrypt = high_resolution_clock::now();
    for (const auto& cipher : clustered_data) {
        Plaintext plaintext;
        cc->Decrypt(keyPair.secretKey, cipher, &plaintext);
        std::cout << "Decrypted Cluster Label: " << plaintext << std::endl;
    }
    BenchmarkExecution("Decryption", start_decrypt);

    return 0;
}
