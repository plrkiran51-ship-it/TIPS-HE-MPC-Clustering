# TIPS-HE-MPC-Clustering

Privacy-preserving threat clustering from STIX 2.1 data using OpenFHE CKKS + MPC.

## Thesis
PhD Cybersecurity, Edinburgh Napier University: https://github.com/plrkiran123/Experimentation_HE/tree/HE

## Data
`threat_stix.json` - STIX 2.1 bundle (threat-actors, IPs, relationships)

## Parameters (Ch5.2.10)
- N=16384, Δ=50, L=6, k=5, n=3 parties

## Run
g++ src/main.cpp -o cluster
./cluster threat_stix.json results/table6.4.csv

## Results Table 6.4
ARI=0.872 (HE-MPC), 127s runtime
