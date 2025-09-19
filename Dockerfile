# Use Ubuntu 22.04 as base image
FROM ubuntu:22.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install essential tools
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    python3 \
    python3-pip \
    python3-dev \
    libpython3-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Install Python packages for benchmarking and plotting
RUN pip3 install --no-cache-dir \
    matplotlib \
    numpy \
    pandas

# Create working directory
WORKDIR /app

# Copy the entire project
COPY . .

# Create bin directory for compiled executables
RUN mkdir -p bin

# Set permissions for scripts
RUN chmod +x benchmark_update.py

# Default command (will be overridden by docker-compose or manual run)
CMD ["python3", "benchmark_update.py", "--help"]