/**
 * @file mpc_interface.cpp
 * @brief MPC Secure Argmin Program (Legacy)
 * 
 * MIT License
 * Copyright (c) 2024-2026 Lakshmi R. Kiran Pasumarthy
 * SPDX-License-Identifier: MIT
 * 
 * TIPS-HECluster — Privacy-Preserving Threat Intelligence Clustering
 * Trusted Privacy-Preserving Threat Information Platform for Sharing (TIPS)
 * Edinburgh Napier University — PhD Research
 * 
 * @deprecated Legacy implementation; see src/mpc_interface.cpp for current version
 * 
 * @details
 * SCALE-MAMBA MPC program for secure argmin computation.
 * This program runs on 3 semi-honest parties who cooperatively compute
 * cluster assignments from encrypted distance metrics.
 * 
 * Algorithm:
 * 1. Each party inputs a subset of n threat records
 * 2. For each record, compute argmin over k encrypted distances
 * 3. Output final cluster assignment (plaintext)
 * 
 * Security: Semi-honest adversary, no collusion
 * 
 * @author Lakshmi R. Kiran Pasumarthy
 * @version 0.9 (legacy)
 */

/**
 * @brief MPC Argmin Program for Cluster Assignment
 * 
 * Variables:
 * - n: Number of threat records (100)
 * - k: Number of clusters (3)
 * - x: Encrypted threat records (n-element array)
 * - centroids: Encrypted cluster centroids (k-element array)
 * 
 * For each record, find the closest centroid and assign its cluster label.
 */
program.p

/**
 * Number of threat records being clustered
 */
n = 100

/**
 * Number of clusters k
 */
k = 3  

/**
 * Input: encrypted threat records from party 0
 * Each x[i] is a shared secret-share of encrypted data point
 */
x = Array(n, sint)
for i in range(n):
    x[i] = sint.get_input_from(0)

/**
 * Initialize centroids from first k data points
 */
centroids = Array(k, sint)
for j in range(k):
    centroids[j] = x[j]

/**
 * Main loop: For each data point, find closest centroid
 */
for i in range(n):
    # Initialize minimum distance to large value
    min_dist = sint(1e9)
    # Track cluster assignment
    best_cluster = sint(0)

    # Check distance to each centroid
    for j in range(k):
        # Compute squared Euclidean distance (x[i] - centroid[j])^2
        diff = x[i] - centroids[j]
        dist = diff * diff
        # Update minimum if closer
        is_closer = dist < min_dist
        min_dist = sint.if_else(is_closer, dist, min_dist)
        best_cluster = sint.if_else(is_closer, j, best_cluster)

    # Output cluster assignment for this record
    print_ln("Threat %s assigned to cluster %s", i, best_cluster)
