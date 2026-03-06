/**
 * @file scheme-selection.cpp
 * @brief Interactive Encryption Scheme Selector (CKKS vs BGV)
 * 
 * MIT License
 * Copyright (c) 2024-2026 Lakshmi R. Kiran Pasumarthy
 * SPDX-License-Identifier: MIT
 * 
 * TIPS-HECluster — Privacy-Preserving Threat Intelligence Clustering
 * Trusted Privacy-Preserving Threat Information Platform for Sharing (TIPS)
 * Edinburgh Napier University — PhD Research
 * 
 * @details
 * This interactive utility demonstrates how to select and initialize different
 * homomorphic encryption schemes based on data type and use case.
 * 
 * Scheme Selection Criteria:
 * - CKKS: For floating-point threat scores (severity, confidence, normalized time)
 *         Supports approximate arithmetic with high precision
 * - BGV: For integer-based counts (attack frequency, packet counts)
 *        Supports exact arithmetic on integers
 * 
 * The program:
 * 1. Prompts user to choose data type (floating-point or integer)
 * 2. Selects appropriate HE scheme
 * 3. Configures scheme-specific parameters
 * 4. Enables required HE capabilities (PKE, KEYSWITCH, LEVELEDSHE)
 * 5. Reports successful setup
 * 
 * @author Lakshmi R. Kiran Pasumarthy
 * @version 1.0
 */

#include <iostream>
#include <vector>
#include "openfhe.h"

using namespace std;
using namespace lbcrypto;

int main() {
    string scheme_choice;
    int dataset_type;

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
        CCParams<CryptoContextCKKSRNS> parameters;
        parameters.SetMultiplicativeDepth(6);
        parameters.SetScalingModSize(50);
        parameters.SetBatchSize(8192);
        parameters.SetRingDim(32768);  // 🔥 FIX: Set ring dimension to 32768
        cc = GenCryptoContext(parameters);
    } else {
        CCParams<CryptoContextBGVRNS> parameters;
        parameters.SetMultiplicativeDepth(6);
        parameters.SetPlaintextModulus(65537);
        parameters.SetRingDim(16384);  // 🔥 No change for BGV
        cc = GenCryptoContext(parameters);
    }

    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);

    cout << "Homomorphic encryption setup completed with scheme: " << scheme_choice << endl;
    return 0;
}
