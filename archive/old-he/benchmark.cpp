/**
 * @file benchmark.cpp
 * @brief HE Pipeline Performance Measurement (Legacy)
 * 
 * MIT License
 * Copyright (c) 2024-2026 Lakshmi R. Kiran Pasumarthy
 * SPDX-License-Identifier: MIT
 * 
 * TIPS-HECluster — Privacy-Preserving Threat Intelligence Clustering
 * Trusted Privacy-Preserving Threat Information Platform for Sharing (TIPS)
 * Edinburgh Napier University — PhD Research
 * 
 * @deprecated Legacy implementation; see src/benchmark.cpp for current version
 * 
 * @details
 * Measures end-to-end execution time for the HE clustering pipeline:
 * 1. Initialize cryptographic context and key pair
 * 2. Fetch threat data from Flask API
 * 3. Encrypt threat scores (time this stage)
 * 4. Run homomorphic k-means on encrypted data (time this stage)
 * 5. Decrypt cluster assignments (time this stage)
 * 6. Report timing for each stage in microseconds
 * 
 * This archived version demonstrates timing instrumentation used for
 * performance analysis and parameter sweep experiments.
 * 
 * @author Lakshmi R. Kiran Pasumarthy
 * @version 0.9 (legacy)
 */

#include <chrono>
#include "openfhe.h"

using namespace lbcrypto;
using namespace std::chrono;

/**
 * @brief Measure and report execution time
 * 
 * Computes elapsed time from @p start to now and prints result.
 */
void BenchmarkExecution(const std::string& operation, high_resolution_clock::time_point start) {
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();
    std::cout << operation << " Execution Time: " << duration << " microseconds." << std::endl;
}

/**
 * @brief Main benchmarking workflow
 * 
 * Orchestrates encryption, clustering, and decryption with timing.
 * External dependencies (InitializeEncryption, FetchThreatData, etc.)
 * must be linked from other modules.
 */
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
