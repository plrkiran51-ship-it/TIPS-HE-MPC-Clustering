#!/usr/bin/env python3
"""
generate_test_threat_data.py - Synthetic Threat Intelligence Dataset Generator

MIT License
Copyright (c) 2024-2026 Lakshmi R. Kiran Pasumarthy
SPDX-License-Identifier: MIT

TIPS-HECluster — Privacy-Preserving Threat Intelligence Clustering
Trusted Privacy-Preserving Threat Information Platform for Sharing (TIPS)
Edinburgh Napier University — PhD Research

Description:
    Generates a synthetic dataset of 1 million threat intelligence records
    in CSV format for testing the HE-MPC clustering pipeline.
    
    Each record simulates a threat event with:
    - Unique threat ID
    - Timestamp (distributed across 2026)
    - Severity score (1-100, numeric)
    - Source IP address (random IPv4)
    - Attack origin country
    - Attack type classification (DDoS, phishing, ransomware, etc.)
    
    This synthetic data is used for:
    - Performance benchmarking
    - Correctness validation
    - Parameter sweep experiments
    - Reproduction of research results
    
Output File:
    threat_dataset_1M.csv (comma-separated, 1M rows)

Author: Lakshmi R. Kiran Pasumarthy
Version: 1.0
"""

# It helps to test with mock data initially
import csv
import random
from datetime import datetime, timedelta

# Configuration for synthetic data generation
OUTPUT_FILE = "threat_dataset_1M.csv"  # Output CSV filename
NUM_ROWS = 1_000_000  # Total threat records to generate

# Realistic attack origin countries (representing attack vectors)
countries = ["UK", "NL", "DE", "FR", "US", "IN", "CN", "BR", "CA", "AU"]

# Common cyber attack classifications
attack_types = ["DDoS", "phishing", "ransomware", "malware", "SQLi", "XSS"]

# Time range for threat event timestamps
start_date = datetime(2026, 1, 1)

def random_ip():
    """Generate a random IPv4 address string.
    
    Returns:
        str: Randomly generated IP address (e.g., "192.168.1.42")
    """
    return ".".join(str(random.randint(1, 255)) for _ in range(4))

with open(OUTPUT_FILE, mode="w", newline="") as file:
    writer = csv.writer(file)
    
    # Header
    writer.writerow(["threat_id", "timestamp", "severity", "source_ip", "country", "attack_type"])
    
    for i in range(1, NUM_ROWS + 1):
        timestamp = start_date + timedelta(minutes=random.randint(0, 525600))
        severity = random.randint(1, 100)
        source_ip = random_ip()
        country = random.choice(countries)
        attack = random.choice(attack_types)
        
        writer.writerow([i, timestamp.strftime("%Y-%m-%d %H:%M"), severity, source_ip, country, attack])

print("1 Million threat records generated successfully!")
