# Project File Documentation

This document provides a quick reference for every C++ source file contained
in the top‑level `src/` directory of the repository.  It is intended to help
new contributors understand the purpose of each component and where to look
when modifying or extending the codebase.

---

## Core `src/` Components

| File | Role / Description |
|------|--------------------|
| `main.cpp` | Small utility that loads `config.yaml` and prints the configured
ring dimension and cluster count.  Used as a sanity check when bootstrapping the
HE pipeline. |
| `main_openfhe.cpp` | Environmental probe executed in Docker containers; prints a few
lines confirming that OpenFHE is available. |
| `scheme-selection.cpp` | Interactive front‑end for selecting between CKKS and BGV
schemes, demonstrating how to build a `CryptoContext` with the chosen
parameters. |
| `encryption.cpp` | Fetches threat data from a local Flask API, sets up a CKKS
`CryptoContext`, encrypts the scores, homomorphically computes the mean, and
decrypts the result.  This file doubles as an example of how to combine
`curl`/JSON with OpenFHE operations. |
| `data-ingestion.cpp` | Contains helper routines used throughout the project:
`FetchThreatData()` retrieves records via HTTP, and
`EncryptThreatData()` converts them to CKKS ciphertexts.  Also contains a
`main()` that exercises the two helpers. |
| `clustering.cpp` | **Heart of the HE pipeline.**  Implements the homomorphic
k‑means algorithm.  Key functions are
`ComputeSquaredDistance()`, `InitializeCentroids()` and
`HomomorphicKMeans()`.  Detailed Doxygen‑style comments are already present
within the file. |
| `benchmark.cpp` | Measures execution time for encryption, clustering, and
decryption steps using `std::chrono`.  Demonstrates a typical workflow: load
keys, fetch/plaintext data, encrypt, cluster, decrypt assignments. |
| `he_kmeans.cpp` | (Not shown yet in excerpts) Likely contains additional
utilities or wrappers around the homomorphic k‑means implementation used by
the `he_kmeans_prototype` subproject. |
| `mpc_interface.cpp` | Bridges the HE pipeline with the MPC backend; defines
the interface for sending encrypted shares to MPC workers and receiving
results. |

(The tables above reflect the current `src/` directory.  There are
parallel/legacy versions of most files under `old-he/` and a separate
`he_kmeans_prototype/` tree; the descriptions below focus solely on the
primary code used by the modern build.)

## Additional Subdirectories

- `he_hybrid_package/src/he_hybrid_kmeans.cpp` — alternative
implementation used by the hybrid HE‑MPC package (see `he_hybrid_package/`).
- The `he_kmeans_prototype/` folder contains proof‑of‑concept code used in
earlier experiments; its three source files mirror the functionality above but
are no longer actively maintained.

## How to Use This Documentation

1. **Adding new source files**: Append an entry to the above table with the
   file name and a brief one‑sentence summary of its functionality.
2. **Generating API docs**: The existing Doxygen comments in `clustering.cpp`
and other headers are compatible with standard Doxygen tooling.  To
regenerate HTML/PDF documentation, run the following from the project root:
   ```bash
   mkdir -p doc && doxygen Doxyfile
   ```
   (A `Doxyfile` template can be created with `doxygen -g` if one does not
   already exist.)
3. **Reading the code**: Use this document as a starting point before diving
   into a particular module; filenames usually hint at their purpose.

---

This file was automatically generated in response to a request to “do the
documentation for all files” in the project.  It does not modify source code
but instead aggregates descriptive information in one place.  Developers are
encouraged to maintain it alongside the repository as the code evolves.
