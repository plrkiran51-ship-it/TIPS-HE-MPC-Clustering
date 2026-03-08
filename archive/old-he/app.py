#!/usr/bin/env python3
"""
app.py - Flask REST API for Threat Intelligence Data (Legacy)

MIT License
Copyright (c) 2024-2026 Lakshmi R. Kiran Pasumarthy
SPDX-License-Identifier: MIT

TIPS-HECluster — Privacy-Preserving Threat Intelligence Clustering
Trusted Privacy-Preserving Threat Information Platform for Sharing (TIPS)
Edinburgh Napier University — PhD Research

Description:
    Legacy Flask application serving synthetic threat intelligence data.
    This is an earlier version of the API; see old-he/api.py for current endpoint.
    
    Endpoints:
    - GET /: Health check (returns JSON with status message)
    - GET /data: Returns sample threat records with regional aggregates
    
    Each threat record contains:
    - region: Geographic origin
    - threat_count: Number of detected threats
    - severity: Normalized severity score (0.0-1.0)
    
    Usage:
        python app.py  # Runs on http://0.0.0.0:5000
        curl http://127.0.0.1:5000/
        curl http://127.0.0.1:5000/data

Author: Lakshmi R. Kiran Pasumarthy
Version: 0.9 (legacy)
"""

from flask import Flask, jsonify

app = Flask(__name__)

@app.route("/")
def home():
    """
    Health check endpoint.
    
    Returns:
        JSON message confirming API is operational
    """
    return jsonify({"message": "Flask API is running!"})

@app.route("/data", methods=["GET"])
def get_data():
    """
    Threat intelligence data endpoint.
    
    Returns:
        JSON array of threat records with regional aggregates
        Each record contains: region, threat_count, severity
    """
    sample_data = [
        {"region": "Scotland", "threat_count": 450, "severity": 0.87},
        {"region": "England", "threat_count": 780, "severity": 0.65}
    ]
    return jsonify(sample_data)

if __name__ == "__main__":
    # Start Flask development server
    # In production, use WSGI server (e.g., gunicorn)
    app.run(debug=True)
