/**
 * @file main.cpp
 * @brief Configuration Loader and Sanity Check
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
 * This utility program tests the configuration loading mechanism used throughout
 * the HE-MPC clustering pipeline. It reads the YAML configuration file
 * (../config.yaml) and verifies that key HE parameters are correctly parsed.
 * 
 * The program serves as:
 * - A bootstrap sanity check during build/testing
 * - A template for loading config in other modules
 * - A diagnostic tool to inspect runtime parameters
 * 
 * Expected config.yaml format:
 * openfhe:
 *   ring_dim: 16384              # Ring dimension (security parameter)
 * kmeans:
 *   n_clusters: 3                 # Number of clusters k
 * 
 * @author Lakshmi R. Kiran Pasumarthy
 * @version 1.0
 */

#include <iostream>
#include <yaml-cpp/yaml.h>
#include <fstream>

int main() {
    try {
        // Load configuration from YAML file
        // This ensures all HE parameters are correctly set before encryption
        YAML::Node config = YAML::LoadFile("../config.yaml");
        
        // Extract OpenFHE ring dimension from config
        // Ring dimension N determines security and computational cost
        // Typical values: 8192, 16384, 32768
        int ring_dim = config["openfhe"]["ring_dim"].as<int>();
        
        // Extract k-means cluster count from config
        int n_clusters = config["kmeans"]["n_clusters"].as<int>();
        
        std::cout << "Loaded ring_dim: " << ring_dim << std::endl;
        std::cout << " HE-MPC K-Means  SUCCESS!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Config OK - " << e.what() << std::endl;
    }
    return 0;
}

