/**
 * @file data-ingestion.cpp
 * @brief Threat Data Ingestion and CKKS Encryption
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
 * This module handles the initial stage of the HE pipeline: fetching raw
 * threat data from a Flask API and converting it to encrypted form.
 * 
 * Workflow:
 * 1. Initialize CKKS cryptographic context with HE parameters
 * 2. Generate public/secret key pair
 * 3. Fetch threat records via HTTP from Flask server
 * 4. Extract threat_score field from each record
 * 5. Encrypt all scores under public key
 * 6. Return vector of ciphertexts for downstream clustering
 * 
 * Key Functions:
 * - FetchThreatData(): HTTP GET to API, returns JSON array
 * - EncryptThreatData(): Converts JSON scores to CKKS ciphertexts
 * 
 * Assumptions:
 * - Flask API running at http://127.0.0.1:5000/get_threat_data
 * - Each threat record has a "threat_score" field (double)
 * - Network connectivity with API server established
 * 
 * @author Lakshmi R. Kiran Pasumarthy
 * @version 1.0
 */

#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "openfhe.h"

using json = nlohmann::json;
using namespace lbcrypto;

/**
 * @brief Callback for accumulating HTTP response body
 * 
 * @param contents Buffer containing HTTP response chunk
 * @param size Size of each element
 * @param nmemb Number of elements
 * @param output Pointer to std::string accumulating response
 * 
 * @return Size of data processed
 */
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

/**
 * @brief Fetches threat intelligence records from Flask API
 * 
 * @return nlohmann::json Array of threat records
 * 
 * @details
 * Issues HTTP GET to http://127.0.0.1:5000/get_threat_data
 * and returns parsed JSON response. Each record should contain
 * at minimum a "threat_score" field (floating-point).
 */
json FetchThreatData() {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5000/get_threat_data");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return json::parse(response);
}

/**
 * @brief Encrypts threat scores from JSON records using CKKS
 * 
 * @param cc Initialized CKKS cryptographic context
 * @param keyPair Public/secret key pair for encryption
 * @param threat_data JSON array of threat records
 * 
 * @return Vector of CKKS ciphertexts (one per threat record)
 * 
 * @details
 * Extracts the "threat_score" field from each JSON record,
 * wraps it in a CKKS plaintext, and encrypts under the public key.
 * The resulting ciphertexts are ready for homomorphic processing
 * in the clustering stage.
 */
vector<Ciphertext<DCRTPoly>> EncryptThreatData(
    CryptoContext<DCRTPoly> cc, KeyPair<DCRTPoly> keyPair, json &threat_data) {

    vector<Ciphertext<DCRTPoly>> encrypted_data;
    for (const auto& item : threat_data) {
        double score = item["threat_score"];
        auto plaintext = cc->MakePackedPlaintext({score});
        encrypted_data.push_back(cc->Encrypt(keyPair.publicKey, plaintext));
    }
    return encrypted_data;
}

int main() {
    json threat_data = FetchThreatData();

    CryptoContext<DCRTPoly> cc = InitializeEncryption();
    KeyPair<DCRTPoly> keyPair = cc->KeyGen();
    cc->EvalMultKeyGen(keyPair.secretKey);
    
    vector<Ciphertext<DCRTPoly>> encrypted_data = EncryptThreatData(cc, keyPair, threat_data);
    
    std::cout << "Successfully encrypted " << encrypted_data.size() << " threat scores." << std::endl;
    return 0;
}
