# TIPS HE-MPC 
ls src/*.cpp | wc -l  # C++ files
head threat.csv             # STIX data
cat results/table6.4.csv    # Thesis results



cmd 25feb 
cd /Users/plrkiran1/TIPS-HE-MPC-Clustering/he_kmeans_prototype/build
./he_hybrid_kmeans


# TIPS: Privacy-Preserving HE-MPC Clustering for Threat Intelligence

**Reproducibility package** for PhD "Trusted Privacy-Preserving Threat Information Platform for Sharing" (Edinburgh Napier University, 2026)
[![Release](https://img.shields.io/github/v/release/plrkiran51-ship-it/TIPS-HE-MPC-Clustering?include_prereleases)](https://github.com/plrkiran51-ship-it/TIPS-HE-MPC-Clustering/releases)
[![License](https://img.shields.io/github/license/plrkiran51-ship-it/TIPS-HE-MPC-Clustering)](plrkiran54@gmail.com)



## Quick Start (macOS/Linux)

### 1. Data (1M synthetic STIX-derived vectors)
```bash
git clone --recursive https://github.com/plrkiran51-ship-it/TIPS-HE-MPC-Clustering.git
cd TIPS-HE-MPC-Clustering
pip install -r requirements.txt
