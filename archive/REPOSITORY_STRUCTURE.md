# Repository Organization

## Core Directories

### `/src` - Main HE Pipeline
Core C++ implementation of the Homomorphic Encryption clustering pipeline.

**Key Files:**
- `he_kmeans.cpp` - Main k-means clustering implementation
- `encryption.cpp` - CKKS encryption routines
- `clustering.cpp` - Distance computation (encrypted)
- `main.cpp` - CLI entry point

### `/he_kmeans_prototype` - Plaintext Baseline
Reference implementation using standard k-means clustering without encryption.

**Purpose:** Validate accuracy of HE-MPC results against plaintext baseline.

**Key Files:**
- `src/main.cpp` - Plaintext k-means implementation
- `data/` - Test datasets
- `build/` - Generated build artifacts (not in git)

### `/he_hybrid_package` - Production Implementation
Production-ready hybrid HE-MPC clustering combining encrypted distance computation with distributed MPC.

**Key Files:**
- `src/he_hybrid_kmeans.cpp` - Complete HE-MPC pipeline
- `data/` - Test data
- `build/` - Generated build artifacts (not in git)

### `/config` - Configuration Templates
Runtime parameters for OpenFHE and k-means clustering.

**Files:**
- `config.yaml` - Complete parameter documentation

### `/datasets` - Test Data
Sample threat intelligence datasets in STIX-compatible CSV format.

**Files:**
- `threat_vectors.csv` - 10K sample threat indicators
- `README.md` - Dataset format specification

### `/k8s` - Kubernetes Deployment
Configuration and documentation for Kubernetes-based MPC worker deployment.

**Files:**
- `mpc-worker-deployment.yaml` - 3-party MPC worker manifests
- `README.md` - Deployment documentation

### `/scripts` - Utility Scripts
Validation and testing scripts.

**Files:**
- `validate.py` - Result validation script

## Root-Level Files

| File | Purpose |
|------|---------|
| `README.md` | Main project documentation |
| `Dockerfile` | Container image for deployment |
| `CMakeLists.txt` | CMake build configuration |
| `generate_test_threat_data.py` | Generate synthetic test datasets |
| `convert_threat_data.py` | Convert raw data to proper format |
| `mpc_worker.py` | MPC worker service (placeholder) |
| `.gitignore` | Git ignore rules |

## Files NOT in Git

The `.gitignore` excludes:
- `build/` - CMake build artifacts
- `*.o`, `*.a`, `*.so` - Compiled objects
- `*.csv`, `*.json` - Data files (generated)
- `doc/html/`, `doc/latex/` - Generated documentation
- `openfhe-development/` - External dependency (build locally)
- `.DS_Store` - OS files
- `*.pem`, `*.key` - Credentials

## Build Process

```
CMakeLists.txt (root)
├── src/ (HE pipeline)
├── he_kmeans_prototype/ (baseline)
└── he_hybrid_package/ (production)

Build command:
cd <directory>/build
cmake .. && make
```

## Adding New Code

1. **Source code** → `/src` or appropriate subdirectory
2. **Tests** → same directory as code
3. **Configuration** → `/config`
4. **Documentation** → `README.md` or directory-specific `README.md`

Never commit:
- Build artifacts
- Generated documentation
- Large data files
- IDE files (.vscode, .idea)
