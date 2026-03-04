import csv
import random
from datetime import datetime, timedelta

OUTPUT_FILE = "threat_dataset_1M.csv"
NUM_ROWS = 1_000_000

countries = ["UK", "NL", "DE", "FR", "US", "IN", "CN", "BR", "CA", "AU"]
attack_types = ["DDoS", "phishing", "ransomware", "malware", "SQLi", "XSS"]

start_date = datetime(2026, 1, 1)

def random_ip():
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
