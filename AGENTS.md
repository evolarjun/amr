# AGENTS.md

This document explains how to set up the development environment, install dependencies, and run tests for AMRFinderPlus.

## Option 1: Build from Source (C++)

### Prerequisites (Linux)

Install system packages and BLAST+:

```bash
sudo apt-get update
sudo apt-get install -y hmmer git libcurl4-openssl-dev build-essential curl

BLAST_VERSION=2.17.0
ARCH=$(uname -m)
if [ "$ARCH" = "x86_64" ]; then
  BLAST_ARCH="x64"
elif [ "$ARCH" = "aarch64" ]; then
  BLAST_ARCH="aarch64"
fi

curl -L -O https://ftp.ncbi.nlm.nih.gov/blast/executables/blast+/${BLAST_VERSION}/ncbi-blast-${BLAST_VERSION}+-${BLAST_ARCH}-linux.tar.gz
tar -zxvf ncbi-blast-${BLAST_VERSION}+-${BLAST_ARCH}-linux.tar.gz
sudo cp -r ncbi-blast-${BLAST_VERSION}+/bin/* /usr/local/bin/
rm ncbi-blast-${BLAST_VERSION}+-${BLAST_ARCH}-linux.tar.gz
rm -rf ncbi-blast-${BLAST_VERSION}+
```

### Prerequisites (macOS)

```bash
brew install blast hmmer
```

### Build

```bash
git submodule update --init --recursive
make -j -O
```

### Download the AMRFinderPlus database

```bash
./amrfinder -u
```

### Run Tests

```bash
make test
```

This runs `./test_amrfinder.sh -n` (using local test data) and the stxtyper tests.

You can also run the test script directly:

```bash
# Use local test data (no download)
./test_amrfinder.sh -n

# Download fresh test data from GitHub before running
./test_amrfinder.sh
```

---

## Option 2: Install via Conda / Bioconda

### Prerequisites

Install [Micromamba](https://mamba.readthedocs.io/en/latest/installation/micromamba-installation.html) or [Conda](https://docs.conda.io/en/latest/miniconda.html).

### Install AMRFinderPlus

```bash
micromamba create -n amrfinder -c conda-forge -c bioconda -c defaults ncbi-amrfinderplus
micromamba activate amrfinder
```

Or with conda:

```bash
conda create -n amrfinder -c conda-forge -c bioconda ncbi-amrfinderplus
conda activate amrfinder
```

### Download the AMRFinderPlus database

```bash
amrfinder --force_update
```

### Run Tests

Download the test script and run it with the `-p` flag to test the `amrfinder` command found in your `$PATH`:

```bash
curl -L -O https://raw.githubusercontent.com/ncbi/amr/master/test_amrfinder.sh
bash ./test_amrfinder.sh -p
```

---

## Test Script Options

| Flag | Description |
|------|-------------|
| `-p` | Test `amrfinder` found in `$PATH` instead of `./amrfinder` |
| `-n` | Skip downloading fresh test data; use files in the current directory |
| `-h` | Print help message |
