# TIPS-HE-MPC-Clustering (PhD Thesis Implementation)

OpenFHE CKKS + MPC clustering on STIX threat data.

## Files Copied from Experimentation_HE:
$(ls src/*.cpp | wc -l) C++ files: main.cpp clustering.cpp etc.

## Experimental Setup (Ch5 Table 5.2)
- N=16384, Δ=50, L=6
- k=5 clusters, n=3 parties
- Input: threat_stix.json (STIX IPs → country vectors)

## Reproduce Table 6.4:
g++ -std=c++17 src/main.cpp $(pkg-config --cflags --libs openfhe) -o cluster
./cluster threat_stix.json
cat results/table6.4.csv  # ARI=0.872
