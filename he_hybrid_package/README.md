# HE hybrid k-means (validation artifact)

This is a **hybrid HE–MPC** validation prototype:
- HE (CKKS/OpenFHE) computes squared distances on encrypted vectors.
- "MPC" step is represented by plaintext argmin + centroid recomputation (replace with secure comparison + secure sum).

## Build

```bash
rm -rf build && mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j10
```

## Run (writes logs)

```bash
mkdir -p ../results
./he_hybrid_kmeans | tee ../results/he_hybrid_run.txt
```

Outputs:
- `results/he_hybrid_centroids.csv`
- `results/he_hybrid_pred_labels.csv`
- `results/metrics.csv`
