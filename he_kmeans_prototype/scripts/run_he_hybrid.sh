#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT/build/he_hybrid_kmeans"
OUTDIR="$ROOT/results"

mkdir -p "$OUTDIR"

ts="$(date +%Y%m%d_%H%M%S)"
iso="$(date -u +%Y-%m-%dT%H:%M:%SZ)"
gitsha="$(git -C "$ROOT" rev-parse --short HEAD 2>/dev/null || echo nogit)"

log="$OUTDIR/run_he_hybrid_kmeans_${ts}.log"

# Run and capture output
"$BIN" 2>&1 | tee "$log" >/dev/null

# Parse values (assumes your program prints these exact tokens)
mode="$(grep -Eo 'mode=[^ ]+' "$log" | head -n1 | cut -d= -f2)"
n="$(grep -Eo 'n=[0-9]+' "$log" | head -n1 | cut -d= -f2)"
d="$(grep -Eo 'd=[0-9]+' "$log" | head -n1 | cut -d= -f2)"
k="$(grep -Eo 'k=[0-9]+' "$log" | head -n1 | cut -d= -f2)"
iters="$(grep -Eo 'iters=[0-9]+' "$log" | head -n1 | cut -d= -f2)"
ari="$(grep -Eo 'ARI_[^=]+=([0-9]+(\.[0-9]+)?)' "$log" | tail -n1 | sed -E 's/.*=//')"
runtime_ms="$(grep -Eo 'runtime_ms=([0-9]+(\.[0-9]+)?)' "$log" | tail -n1 | cut -d= -f2)"

csv="$OUTDIR/metrics.csv"
if [ ! -f "$csv" ]; then
  echo "utc_ts,git_sha,mode,n,d,k,iters,ari,runtime_ms,logfile" > "$csv"
fi

echo "${iso},${gitsha},${mode},${n},${d},${k},${iters},${ari},${runtime_ms},$(basename "$log")" >> "$csv"

echo "Wrote: $log"
echo "Appended: $csv"
