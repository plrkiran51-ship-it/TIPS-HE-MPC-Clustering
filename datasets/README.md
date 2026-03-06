# Datasets for TIPS-HECluster

This directory contains threat intelligence datasets used for benchmarking and evaluation.

## Dataset Format

All datasets should be CSV files with the following structure:

```
severity,confidence,attack_type,timestamp
75,0.92,DDoS,2026-01-01T08:15:00
45,0.65,phishing,2026-01-01T09:30:00
88,0.98,ransomware,2026-01-01T10:45:00
```

### Fields

- **severity**: Threat severity score (0-100 or 0-1.0)
- **confidence**: Confidence in threat assessment (0-1.0)
- **attack_type**: Classification code or string ID
- **timestamp**: ISO-8601 timestamp of threat event

## Included Datasets

- `threat_vectors.csv` — STIX feature vectors (placeholder for your data)
- `ground_truth_labels.csv` — Ground truth clustering assignments (optional)

## Generating Test Data

To generate synthetic test data, use the provided script:

```bash
python ../generate_test_threat_data.py
```

This creates `threat_dataset_1M.csv` with 1 million synthetic threat records.

## Downloading Public Datasets

For real-world evaluation, consider:

- **STIX 2.1 Indicator Examples**: https://github.com/oasis-open/cti-stix-examples
- **CTI Threat Feeds**: Public threat intelligence feeds (see project README for links)

## CSV Requirements

- **Header row required**: First line must contain column names
- **Consistent datatypes**: All rows must have same number of columns
- **UTF-8 encoding**: Use UTF-8 character encoding
- **No special characters**: Avoid commas in field values (use JSON if needed)

## Data Privacy

⚠️ **Important**: When using real threat data:
- Remove or anonymize sensitive indicators (IP ranges, domain names)
- Ensure data sharing compliance with organizational policies
- Follow GDPR/CCPA requirements for PII handling

## Preprocessing

Before running the pipeline, ensure:

1. **Normalization**: Features should be scaled (hint: use `config.yaml` normalization option)
2. **Consistency**: All numeric fields properly formatted
3. **Size**: Test with 10K records first, then scale to 100K/1M

See `config/config.yaml` for dataset path configuration.
