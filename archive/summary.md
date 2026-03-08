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





### 1. Clone the repository 
```bash
git clone --recursive https://github.com/plrkiran51-ship-it/TIPS-HE-MPC-Clustering.git
cd TIPS-HE-MPC-Clustering


test with threat_dataset_1M.csv

##Results are in Results folder

# TIPS-HECluster

> Trusted Intelligence Privacy-preserving Secure Clustering — a hybrid Homomorphic Encryption (HE) and Multi-Party Computation (MPC) framework for privacy-preserving k-means clustering of cyber threat intelligence data, built for researchers and practitioners.

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

## Architecture


## Architecture

┌─────────────────────────────────────────────────────────────────────┐
│ TIPS-HECluster Pipeline │
│ │
│ STIX Feed ──► Vectorise (4D) ──► CKKS Encrypt ──► HE Distance │
│ (Flask API) Algorithm 3 OpenFHE BatchSize: ──► Computation │
│ severity,conf, 1024–16384 (Algorithm 1) │
│ attack,time │
│ │
│ HE Distance ──► MPC ArgMin ──► Cluster ──► Centroid │
│ Shares (Kubernetes Pods) Assignment Update │
│ (semi-honest) Ai (plaintext) │
│ │
│ Centroid ──► Re-encrypt ──► Next Iteration │
│ Update (CKKS) (max 10 iters) │
└─────────────────────────────────────────────────────────────────────┘


## Encryption Schemes

| Scheme | Use Case in TIPS-HECluster | Operations Supported | Precision |
|--------|---------------------------|---------------------|-----------|
| CKKS | Floating-point threat scores (severity, confidence, normalised timestamp) | Add, Multiply, Rotate | Approximate real/complex |
| BGV | Integer-based threat counts, attack type codes | Add, Multiply | Exact integers |

## Repository Structure
TIPS-HE-MPC-Clustering/
├── src/
│ ├── he_pipeline/ # C++ OpenFHE encryption pipeline (CKKS/BGV)
│ ├── mpc_workers/ # MPC argmin microservice (Kubernetes pods)
│ ├── stix_vectoriser/ # STIX-to-4D feature vector encoder (Python)
│ └── api/ # Flask threat intelligence API (data ingestion)
├── config/
│ └── config.yaml # Runtime HE parameters (N, depth, batch size, k)
├── datasets/
│ ├── threats_10k.csv # Synthetic STIX threat records (10K rows)
│ ├── threats_100k.csv # 100K row dataset
│ └── threats_1m.csv # 1M row dataset
├── results/
│ ├── plaintext_kmeans_benchmarks.csv
│ ├── ckks_kmeans_benchmarks.csv
│ ├── bgv_benchmarks.csv
│ └── parameter_sweep_results.csv
├── figures/
│ ├── execution_time_vs_ring_dimension.png
│ ├── encrypted_vs_plaintext_clustering.png
│ ├── elapsed_time_vs_depth.png
│ └── ari_accuracy_results.png
├── k8s/
│ └── mpc-worker-deployment.yaml # Kubernetes MPC worker deployment
├── CMakeLists.txt
├── requirements.txt # Python dependencies
├── Dockerfile
└── README.md

## Prerequisites and Dependencies (It helps to setup )
### C++ Build (OpenFHE Pipeline) - [OpenFHE](https://github.com/openfheorg/openfhe-development) | ≥ 1.1.0,
###CKKS and BGV schemes - CMake | ≥ 3.16 | Build system
###yaml-cpp | ≥ 0.7.0 | Runtime config loading

### Python (Vectoriser + API)
```bash
pip install -r requirements.txt

### Python (Vectoriser + API)

```bash
pip install -r requirements.txt
# Key packages: flask, pandas, scikit-learn, numpy

Infrastructure (MPC Workers)
Docker ≥ 20.x

Kubernetes ≥ 1.24 (AWS EKS or local minikube)
#.
# Installation Instructions

## System Requirements

| Resource | Minimum | Recommended |
|----------|---------|-------------|
| RAM | 16 GB | 32 GB |
| CPU | 4 cores | 8+ cores |
| OS | macOS 12+ / Ubuntu 20.04+ | Apple M1/M2 or Linux |
| Disk | 10 GB free | 20 GB free |

## Dependencies

| Dependency | Version | Purpose |
|-----------|---------|---------|
| [OpenFHE](https://github.com/openfheorg/openfhe-development) | ≥ 1.1.0 | CKKS and BGV schemes |
| CMake | ≥ 3.16 | C++ build system |
| GCC / Clang | ≥ 11 | C++17 required |
| yaml-cpp | ≥ 0.7.0 | Runtime config loading |
| Python | ≥ 3.9 | STIX vectoriser and API |
| Docker | ≥ 20.x | MPC worker containers |
| Kubernetes | ≥ 1.24 | MPC worker orchestration |

---

## Step 1 — Clone the Repository

```bash
git clone https://github.com/plrkiran51-ship-it/TIPS-HE-MPC-Clustering.git
cd TIPS-HE-MPC-Clustering
git checkout HE

##  Step 2 — Install OpenFHE
git clone https://github.com/openfheorg/openfhe-development.git
cd openfhe-development
mkdir build && cd build
cmake .. -DBUILD_EXTRAS=ON -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
cd ../..

Verify: ls /usr/local/lib | grep OpenFHE

## Step 3 — Install yaml-cpp
brew install yaml-cpp

## Step 4 — Build the HE Pipeline
mkdir build && cd build
cmake .. -DOpenFHE_DIR=/usr/local/lib/cmake/OpenFHE
make -j$(nproc)

##Step 4 — Build the HE Pipeline
mkdir build && cd build
cmake .. -DOpenFHE_DIR=/usr/local/lib/cmake/OpenFHE
make -j$(nproc)

Expected: ./build/tips_he_cluster binary created.

##Step 5 — Python Dependencies

pip install -r requirements.txt

## Step 6 — Docker (MPC Workers)
docker build -t tips-mpc-worker:latest .

##Troubleshooting examples
If error persists OpenFHE not found by CMake:
export OpenFHE_DIR=/usr/local/lib/cmake/OpenFHE
cmake .. -DOpenFHE_DIR=$OpenFHE_DIR

Port conflict on Flask API:
Edit src/api/app.py and change port=5000 to port=5001

Apple M1 — OpenFHE build fails:
cmake .. -DBUILD_EXTRAS=ON -DCMAKE_OSX_ARCHITECTURES=arm64
#.
