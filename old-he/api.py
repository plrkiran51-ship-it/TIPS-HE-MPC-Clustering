#!/usr/bin/env python3
"""
api.py - Flask API for Threat Intelligence Data Serving

MIT License
Copyright (c) 2024-2026 Lakshmi R. Kiran Pasumarthy
SPDX-License-Identifier: MIT

TIPS-HECluster — Privacy-Preserving Threat Intelligence Clustering
Trusted Privacy-Preserving Threat Information Platform for Sharing (TIPS)
Edinburgh Napier University — PhD Research

Description:
    Provides HTTP endpoints for fetching synthetic threat intelligence data.
    This Flask application is used by the C++ pipeline to retrieve threat
    records that are subsequently encrypted and clustered.
    
    Endpoints:
    - GET /threat_data: Returns threat dataset as CSV
    
    The dataset contains:
    - ThreatID: Unique identifier
    - ThreatScore: Severity metric (0.0-1.0)
    - AttackType: Classification (DDoS, Malware, Phishing, Ransomware)
    
    Usage:
        python api.py
        # API listens on http://0.0.0.0:5000
        # Client: curl http://127.0.0.1:5000/threat_data
    
Author: Lakshmi R. Kiran Pasumarthy
Version: 1.0
"""

from flask import Flask, Response
import pandas as pd
import random

app = Flask(__name__)

# Generate synthetic threat intelligence dataset
# This simulates a real threat feed with 10,000 records
data = {
    # Unique threat identifier
    "ThreatID": range(1, 10001),  
    
    # Threat severity score (normalized to [0.0, 1.0])
    "ThreatScore": [round(random.uniform(0, 1), 2) for _ in range(10000)], 
    
    # Attack classification type
    "AttackType": [random.choice(["DDoS", "Malware", "Phishing", "Ransomware"]) for _ in range(10000)] 
}

df = pd.DataFrame(data)

@app.route('/threat_data', methods=['GET'])
def get_csv():
    """
    HTTP GET endpoint for threat intelligence data.
    
    Method: GET /threat_data
    Returns: Threat dataset as CSV (text/csv)
    
    CSV Format:
        ThreatID,ThreatScore,AttackType
        1,0.73,DDoS
        2,0.42,Malware
        ...
    
    This endpoint is called by the C++ encryption pipeline
    to fetch raw threat records for homomorphic clustering.
    """
    csv_data = df.to_csv(index=False)
    return Response(csv_data, mimetype="text/csv")

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5000, debug=True)
