# Full prototype: clustering + findings (ARI)

This prototype **runs k-means clustering on data**, then reports a quantitative finding (**ARI**) and writes centroids and labels to `results/`.

## Build & run

```bash
rm -rf build && mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j10
./kmeans
```

Expected output:

```
ARI = 0.8
```

Outputs:
- `results/centroids.csv`
- `results/pred_labels.csv`

## How to adapt to homomorphic encryption

Use this plaintext prototype as the baseline for Chapter 6 (ARI + runtime). Then add an HE version that:
1) encrypts vectors,
2) computes distances homomorphically,
3) updates centroids via MPC (secure sum),
4) decrypts final centroids for evaluation.

The key is: **you must report findings** (ARI, runtime, accuracy loss vs plaintext).





-----
##### 📖 User Manual



Quick start:
```bash
git clone https://github.com/plrkiran51-ship-it/TIPS-HE-MPC-Clustering.git
cd TIPS-HE-MPC-Clustering && git switch HE
cd he_kmeans_prototype
rm -rf build && mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release && make -j10
./kmeans                # Plaintext baseline
./he_hybrid_kmeans      # HE-hybrid version

