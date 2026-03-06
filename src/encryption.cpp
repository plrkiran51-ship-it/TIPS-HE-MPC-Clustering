/**
 * @file encryption.cpp
 * @brief CKKS Homomorphic Encryption for Threat Scores
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
 * Demonstrates complete CKKS encryption workflow for floating-point threat scores:
 * 1. Fetch threat data from locally-running Flask API
 * 2. Initialize OpenFHE CKKS context with HE parameters
 * 3. Generate public/secret key pair
 * 4. Encrypt threat severity scores
 * 5. Perform homomorphic operations (compute mean without decryption)
 * 6. Decrypt results and display
 * 
 * Uses libcurl for HTTP communication with Flask backend and nlohmann/json
 * for JSON parsing. This module serves as both example code and integration test.
 * 
 * CKKS scheme properties:
 * - Supports approximate real/complex arithmetic
 * - Efficient for floating-point threat metrics
 * - Allows addition and multiplication of ciphertexts
 * - Requires scaling factor management for precision
 * 
 * @author Lakshmi R. Kiran Pasumarthy
 * @version 1.0
 */

#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "openfhe.h"

using namespace lbcrypto;
using json = nlohmann::json;

/**
 * @brief Callback function for libcurl HTTP response handling
 * 
 * @param contents Pointer to response buffer from curl
 * @param size Element count in response
 * @param nmemb Element size in bytes
 * @param output Pointer to std::string to append response data to
 * 
 * @return Total bytes processed
 * 
 * @details
 * Called by curl_easy_perform() to accumulate HTTP response body.
 * Appends received data to the output string for subsequent JSON parsing.
 */
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

/**
 * @brief Fetches threat data from a remote HTTP API endpoint
 * 
 * @param api_url URL of the threat data API (e.g., "http://127.0.0.1:5000/data")
 * 
 * @return nlohmann::json Parsed JSON response containing threat records
 * 
 * @details
 * Uses libcurl to issue HTTP GET request to @p api_url.
 * Response body is accumulated via WriteCallback and parsed as JSON.
 * Caller is responsible for ensuring API is running and returns valid JSON.
 * 
 * Error handling is minimal; ensure API is reachable before calling.
 */
json fetchDataFromAPI(const std::string& api_url) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return json::parse(response);
}

int main() {
    std::string api_url = "http://127.0.0.1:5000/data";

    // Step 1: Fetch Data from Flask API
    json threatData = fetchDataFromAPI(api_url);
    std::cout << "Fetched Threat Data: " << threatData.dump(4) << std::endl;

    // Step 2: Initialize OpenFHE CKKS Context
    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetMultiplicativeDepth(4);
    parameters.SetScalingModSize(50);
    parameters.SetRingDim(8192);

    CryptoContext<DCRTPoly> cryptoContext = GenCryptoContext(parameters);

    cryptoContext->Enable(PKE);  // Enable Public Key Encryption
    cryptoContext->Enable(FHE);  // Enable Fully Homomorphic Encryption

    // Step 3: Generate Key Pair
    auto keyPair = cryptoContext->KeyGen();
    cryptoContext->EvalMultKeyGen(keyPair.secretKey);

    // Step 4: Encrypt Threat Data (Severity Scores)
    std::vector<double> severityScores;
    for (const auto& entry : threatData) {
        severityScores.push_back(entry["severity"]);
    }

    Plaintext plaintext = cryptoContext->MakeCKKSPackedPlaintext(severityScores);
    Ciphertext<DCRTPoly> encryptedData = cryptoContext->Encrypt(keyPair.publicKey, plaintext);

    std::cout << "Data Encrypted Successfully!" << std::endl;

    // Step 5: Perform Homomorphic Computations (Compute Average Severity)
    Ciphertext<DCRTPoly> encryptedSum = encryptedData;
    for (size_t i = 1; i < severityScores.size(); i++) {
        encryptedSum = cryptoContext->EvalAdd(encryptedSum, encryptedData);
    }

    std::vector<double> scalingFactor = {1.0 / severityScores.size()};
    Plaintext scalar = cryptoContext->MakeCKKSPackedPlaintext(scalingFactor);
    Ciphertext<DCRTPoly> encryptedMean = cryptoContext->EvalMult(encryptedSum, scalar);

    // Step 6: Decrypt & Display Result
    Plaintext decryptedMean;
    cryptoContext->Decrypt(keyPair.secretKey, encryptedMean, &decryptedMean);
    decryptedMean->SetLength(1);

    std::cout << "Decrypted Mean Severity Score: " << decryptedMean->GetCKKSPackedValue()[0] << std::endl;

    return 0;
}
