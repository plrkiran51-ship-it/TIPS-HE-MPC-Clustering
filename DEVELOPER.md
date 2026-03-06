# Developer Setup & Contribution Guide

## Getting Started

### 1. Prerequisites

**macOS:**
```bash
brew install cmake python3 llvm git
```

**Linux (Ubuntu):**
```bash
apt-get install build-essential cmake python3 git
```

### 2. Clone the Repository

```bash
git clone https://github.com/plrkiran51-ship-it/TIPS-HE-MPC-Clustering.git
cd TIPS-HE-MPC-Clustering
```

### 3. Build the Project

**Plaintext Baseline:**
```bash
cd he_kmeans_prototype
mkdir -p build && cd build
cmake .. && make -j$(nproc)
```

**HE-MPC Production:**
```bash
cd he_hybrid_package
mkdir -p build && cd build
cmake .. && make -j$(nproc)
```

### 4. Run Tests

```bash
cd he_kmeans_prototype/build
./kmeans  # Baseline ARI ~0.44

cd ../../he_hybrid_package/build
./he_hybrid_kmeans  # Expected ARI = 1.0
```

## Project Architecture

### Key Components

#### 1. Encryption Pipeline (`src/encryption.cpp`)
- CKKS scheme initialization
- Public/private key generation
- Data encryption and decryption

#### 2. Clustering Algorithm (`src/clustering.cpp`)
- Homomorphic distance computation
- Lloyd's iteration
- Centroid updates

#### 3. Hybrid Implementation (`he_hybrid_package/src/he_hybrid_kmeans.cpp`)
- Combines HE pipeline with MPC integration
- Production-ready with result persistence
- Includes ARI validation

### Data Flow

```
Input (STIX CSV)
    ↓
Generate Test Data (Python)
    ↓
Convert to Format (Python)
    ↓
Encrypt (C++ - CKKS)
    ↓
Compute Distances (Encrypted)
    ↓
Distributed Argmin (MPC)
    ↓
Output (Cluster Labels)
```

## Configuration

Edit `config/config.yaml`:

```yaml
openfhe:
  ring_dim: 16384              # Ring dimension (N)
  mult_depth: 4                # Multiplicative depth (D)
  scaling_mod_bits: 40         # Scaling factor (B)
  batch_size: 4096             # SIMD packing (BS)

clustering:
  k: 8                         # Cluster count
  max_iterations: 10           # Max iterations
  dataset: datasets/threat_vectors.csv
```

## Making Changes

### Adding New Features

1. **Find the right location:**
   - Core algorithm → `src/clustering.cpp`
   - Encryption logic → `src/encryption.cpp`
   - Tests → same directory as code

2. **Follow existing patterns:**
   - Use same coding style
   - Add inline comments
   - Document with Doxygen fmt

3. **Update CMakeLists.txt** if adding new files

4. **Test locally:**
   ```bash
   cd build && make
   ./kmeans  # Should pass validation
   ```

### Code Style

- **Language:** C++17 (modern features OK)
- **Comments:** Doxygen format with @brief, @param, @return
- **Naming:** CamelCase for classes, snake_case for functions

Example:
```cpp
/**
 * @brief Compute encrypted distance between two vectors
 * @param a Encrypted vector A
 * @param b Encrypted vector B
 * @return Squared Euclidean distance (encrypted)
 */
auto computeDistance(const vector_t& a, const vector_t& b) {
    // Implementation
}
```

## Testing

### Unit Tests
No formal test suite yet. Validation happens via:
- `./kmeans` for plaintext
- `./he_hybrid_kmeans` for HE-MPC
- `python3 scripts/validate.py` for comparison

### Creating Test Data

```bash
python3 generate_test_threat_data.py  # Creates threat_dataset_1M.csv
python3 convert_threat_data.py        # Converts to proper format
```

### Validating Code

```bash
python3 scripts/validate.py
# Output: ARI scores and validation status
```

## Deployment

### Docker

```bash
# Build image
docker build -t tips-he-mpc:latest .

# Run locally
docker run -it tips-he-mpc:latest
```

### Kubernetes

```bash
# Deploy MPC workers
kubectl apply -f k8s/mpc-worker-deployment.yaml

# Check status
kubectl get pods -l app=mpc-worker

# View logs
kubectl logs -f deployment/mpc-worker-0
```

## Debugging

### Compilation Issues

```bash
# Check compiler version
clang++ --version  # Should support C++17

# Clean rebuild
rm -rf build/
mkdir build && cd build
cmake .. && make VERBOSE=1
```

### Runtime Issues

**Out of memory:**
```bash
# Reduce ring dimension
sed -i 's/ring_dim: 16384/ring_dim: 8192/' config/config.yaml
```

**Dataset format error:**
```bash
# Check data format
head datasets/threat_vectors.csv
# Should have: severity,confidence,attack_type,timestamp
```

## Performance Optimization

### Faster Predictions

- Reduce `ring_dim` to 8192
- Reduce `mult_depth` to 2
- Use smaller batches

### More Accurate Results

- Increase `ring_dim` to 32768
- Increase `mult_depth` to 6
- Use higher `scaling_mod_bits`

## Contributing

1. **Fork** the repository
2. **Create** feature branch: `git checkout -b feature/my-feature`
3. **Commit** changes: `git commit -m "Add feature description"`
4. **Push** to branch: `git push origin feature/my-feature`
5. **Open** pull request

## Documentation

- Main docs → `README.md`
- This guide → `DEVELOPER.md`
- Experiments → `EXPERIMENTATION_GUIDE.md`
- Structure → `REPOSITORY_STRUCTURE.md`
- Config → `config/config.yaml`

## Common Tasks

### Update Configuration

```bash
vim config/config.yaml
# Modify parameters, then rebuild
```

### Generate New Test Data

```bash
python3 generate_test_threat_data.py
python3 convert_threat_data.py
# Data appears in datasets/threat_vectors.csv
```

### Run Full Experiment

```bash
# Generate data
python3 generate_test_threat_data.py
python3 convert_threat_data.py

# Build & run baselines
cd he_kmeans_prototype/build && cmake .. && make && ./kmeans
cd ../../he_hybrid_package/build && cmake .. && make && ./he_hybrid_kmeans

# Validate
cd ../.. && python3 scripts/validate.py
```

## Questions?

See:
- `README.md` - Project overview
- `EXPERIMENTATION_GUIDE.md` - Detailed experiments
- `k8s/README.md` - Kubernetes deployment
- `config/config.yaml` - Parameter documentation
