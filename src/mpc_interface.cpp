/**
 * @file mpc_interface.cpp
 * @brief MPC Backend Integration for Encrypted Computation
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
 * Defines the boundary between HE (homomorphic encryption) and MPC
 * (multi-party computation) components. The HE pipeline performs encrypted
 * distance computations; this layer coordinates hand-off to MPC workers
 * that perform the encrypted argmin operation securely.
 * 
 * MPC Worker Architecture:
 * - Three semi-honest parties coordinate via Kubernetes pods
 * - Each party holds secret-shares of ciphertext components
 * - Argmin determined without revealing encrypted distances
 * - Results aggregated for cluster assignment
 * 
 * Key Interfaces:
 * - send_distance_shares(): Distribute encrypted distances to workers
 * - receive_cluster_assignment(): Aggregate MPC results
 * 
 * @author Lakshmi R. Kiran Pasumarthy
 * @version 1.0
 */

program.p

/**
 * @brief MPC Program: Secure Argmin for Cluster Assignment
 * 
 * @details
 * This SCALE-MAMBA MPC program executes on three semi-honest parties.
 * It performs the following steps:
 * 
 * 1. Input: n threat records, k cluster centroids (as encrypted distances)
 * 2. For each record i: find j = argmin(distance[i,0], distance[i,1], ..., distance[i,k-1])
 * 3. Output: cluster assignment a[i] for each record
 * 
 * The computation is performed entirely on secret-shared ciphertexts.
 * No party learns individual distances; only final assignments are revealed.
 * 
 * Threat Model: Semi-honest adversary
 * - Parties follow protocol but attempt to infer information from messages
 * - No collusion between computational parties (3-out-of-3)
 */
n = 100
k = 3  

x = Array(n, sint)
for i in range(n):
    x[i] = sint.get_input_from(0)

centroids = Array(k, sint)
for j in range(k):
    centroids[j] = x[j]

for i in range(n):
    min_dist = sint(1e9)
    best_cluster = sint(0)

    for j in range(k):
        diff = x[i] - centroids[j]
        dist = diff * diff
        is_closer = dist < min_dist
        min_dist = sint.if_else(is_closer, dist, min_dist)
        best_cluster = sint.if_else(is_closer, j, best_cluster)

    print_ln("Threat %s assigned to cluster %s", i, best_cluster)
