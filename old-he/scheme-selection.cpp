/**
 * @file scheme-selection.cpp
 * @brief Interactive CKKS/BGV Scheme Selector (Legacy)
 * 
 * MIT License
 * Copyright (c) 2024-2026 Lakshmi R. Kiran Pasumarthy
 * SPDX-License-Identifier: MIT
 * 
 * TIPS-HECluster — Privacy-Preserving Threat Intelligence Clustering
 * Trusted Privacy-Preserving Threat Information Platform for Sharing (TIPS)
 * Edinburgh Napier University — PhD Research
 * 
 * @deprecated Legacy implementation; see src/scheme-selection.cpp for current version
 * 
 * @details
 * Interactive menu-driven utility for selecting between CKKS and BGV schemes.
 * User selects data type, program initializes appropriate context and parameters.
 * 
 * Scheme Rationale:
 * - CKKS (Cheon-Kim-Kim-Song): Floating-point threat scores, approximate arithmetic
 * - BGV (Brakerski-Gentry-Vaikuntanathan): Integer counts, exact arithmetic
 * 
 * This archived version demonstrates the early exploration of scheme selection
 * for different threat data types within the research.
 * 
 * @author Lakshmi R. Kiran Pasumarthy
 * @version 0.9 (legacy)
 */

#include <iostream>
#include <vector>
#include "openfhe.h"

using namespace std;
using namespace lbcrypto;

int main() {
    string scheme_choice;
    int dataset_type;

    // Prompt user for data type selection
    cout << "Select Dataset Type: \n1. Floating-point Threat Scores \n2. Integer-based Threat Counts\n";
    cin >> dataset_type;

    if (dataset_type == 1) {
        scheme_choice = "CKKS";
    } else if (dataset_type == 2) {
        scheme_choice = "BGV";
    } else {
        cout << "Invalid selection. Defaulting to CKKS." << endl;
        scheme_choice = "CKKS";
    }

    cout << "Selected Encryption Scheme: " << scheme_choice << endl;

    CryptoContext<DCRTPoly> cc;
    if (scheme_choice == "CKKS") {
        // CKKS: Floating-point arithmetic on encrypted data
        // Suitable for threat scores (severity 0-100, confidence 0-1)
        CCParams<CryptoContextCKKSRNS> parameters;
        parameters.SetMultiplicativeDepth(6);      // Circuit depth for multiplications
        parameters.SetScalingModSize(50);          // Precision bits per level
        parameters.SetBatchSize(8192);             // SIMD vector size
        parameters.SetRingDim(32768);              // Security parameter N
        cc = GenCryptoContext(parameters);
    } else {
        // BGV: Exact integer arithmetic on encrypted data
        // Suitable for threat counts and categorical features
        CCParams<CryptoContextBGVRNS> parameters;
        parameters.SetMultiplicativeDepth(6);
        parameters.SetPlaintextModulus(65537);     // Prime modulus for plaintext space
        parameters.SetRingDim(16384);
        cc = GenCryptoContext(parameters);
    }

    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);

    cout << "Homomorphic encryption setup completed with scheme: " << scheme_choice << endl;
    return 0;
}
