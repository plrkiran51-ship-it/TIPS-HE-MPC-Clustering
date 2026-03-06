#!/usr/bin/env python3
"""
convert_threat_data.py - Convert synthetic threat data to TIPS-HECluster format

MIT License
Copyright (c) 2024-2026 Lakshmi R. Kiran Pasumarthy
SPDX-License-Identifier: MIT

Converts threat_dataset_1M.csv to datasets/threat_vectors.csv with proper format:
severity,confidence,attack_type,timestamp
"""

import csv
import random
import os

def convert_dataset():
    """Convert the generated threat data to the expected format."""

    input_file = "threat_dataset_1M.csv"
    output_file = "datasets/threat_vectors.csv"

    # Create datasets directory if it doesn't exist
    os.makedirs("datasets", exist_ok=True)

    with open(input_file, 'r') as infile, open(output_file, 'w', newline='') as outfile:
        reader = csv.DictReader(infile)
        writer = csv.writer(outfile)

        # Write header
        writer.writerow(["severity", "confidence", "attack_type", "timestamp"])

        count = 0
        for row in reader:
            # Convert severity to int, generate random confidence
            severity = int(row['severity'])
            confidence = round(random.uniform(0.1, 1.0), 2)
            attack_type = row['attack_type']
            timestamp = row['timestamp']

            writer.writerow([severity, confidence, attack_type, timestamp])
            count += 1

            # For testing, limit to 10,000 records initially
            if count >= 10000:
                break

    print(f"Converted {count} threat records to datasets/threat_vectors.csv")

if __name__ == "__main__":
    convert_dataset()