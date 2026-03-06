# TIPS-HE-MPC Docker Image
# Multi-stage build for MPC worker deployment

# Build stage - OpenFHE and dependencies
FROM ubuntu:22.04 AS builder

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    libgmp-dev \
    libomp-dev \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Clone and build OpenFHE
RUN git clone https://github.com/openfheorg/openfhe-development.git && \
    cd openfhe-development && \
    mkdir build && cd build && \
    cmake .. -DBUILD_EXTRAS=OFF -DBUILD_BENCHMARKS=OFF && \
    make -j$(nproc) && \
    make install

# Runtime stage
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    libgomp1 \
    && rm -rf /var/lib/apt/lists/*

# Copy OpenFHE libraries from builder
COPY --from=builder /usr/local/lib/libOPENFHE*.so* /usr/local/lib/
COPY --from=builder /usr/local/include/openfhe* /usr/local/include/

# Set library path
ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

# Create app directory
WORKDIR /app

# Copy application code
COPY . /app/

# Default command
CMD ["python3", "mpc_worker.py"]
