## Quick Start (macOS/Linux)

# TIPS HE-MPC 
ls src/*.cpp | wc -l  # C++ files
head threat.csv             # STIX data
cat results/table6.4.csv    # Thesis results



cmd 25feb 
cd /Users/plrkiran1/TIPS-HE-MPC-Clustering/he_kmeans_prototype/build
./he_hybrid_kmeans


# TIPS: Privacy-Preserving HE-MPC Clustering for Threat Intelligence

**Reproducibility package**  "Trusted Privacy-Preserving Threat Information Platform for Sharing" 
plrkiran51-ship-it/TIPS-HE-MPC-Clustering?include_prereleases(https://github.com/plrkiran51-ship-it/TIPS-HE-MPC-Clustering/releases)
plrkiran51-ship-it/TIPS-HE-MPC-Clustering(plrkiran54@gmail.com)





### 1. Data 
```bash
git clone --recursive https://github.com/plrkiran51-ship-it/TIPS-HE-MPC-Clustering.git
cd TIPS-HE-MPC-Clustering


test with threat_dataset_1M.csv

##Results are in Results folder

# TIPS-HECluster

> **T**rusted **I**ntelligence **P**rivacy-preserving **S**ecure Clustering — a hybrid Homomorphic Encryption (HE) and Multi-Party Computation (MPC) framework for privacy-preserving k-means clustering of cyber threat intelligence data, built for researchers and practitioners.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![OpenFHE](https://img.shields.io/badge/OpenFHE-v1.x-blue)](https://github.com/openfheorg/openfhe-development)
[![Platform: macOS / Linux](https://img.shields.io/badge/Platform-macOS%20%7C%20Linux-lightgrey)]()
[![Build: CMake](https://img.shields.io/badge/Build-CMake-green)]()

---

####### Overview  

TIPS-HECluster implements a hybrid HE-MPC k-means clustering pipeline that allows multiple organisations to collaboratively analyse cyber threat intelligence (CTI) data encoded as STIX indicators — without ever exposing raw threat records to any party.

- Encrypts 4D STIX feature vectors `v = (s, c, a, t)` using CKKS (floating-point threat scores) and BGV (integer-based threat counts) via OpenFHE
- Computes homomorphic squared Euclidean distances without decryption
- Offloads the `argmin` cluster assignment to Kubernetes-orchestrated MPC workers (semi-honest model), avoiding prohibitive multiplicative depth
- Scales from 10K to 10M threat records with configurable ring dimensions, multiplicative depth, and batch sizes
- Achieves ARI ≥ 0.85 against plaintext k-means baseline, with 59% latency reduction via hybrid MPC design
- Fully GDPR/RTBF compliant — plaintext threat features never leave the encrypted pipeline


## Encryption Schemes

| Scheme | Use Case in TIPS-HECluster | Operations Supported | Precision |
|--------|---------------------------|---------------------|-----------|
| CKKS | Floating-point threat scores (severity, confidence, normalised timestamp) | Add, Multiply, Rotate | Approximate real/complex |
| BGV | Integer-based threat counts, attack type codes | Add, Multiply | Exact integers |


## Prerequisites and Dependencies (It helps to setup )
### C++ Build (OpenFHE Pipeline) - [OpenFHE](https://github.com/openfheorg/openfhe-development) | ≥ 1.1.0,
###CKKS and BGV schemes - CMake | ≥ 3.16 | Build system
###yaml-cpp | ≥ 0.7.0 | Runtime config loading

