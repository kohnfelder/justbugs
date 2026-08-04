# Setup guide

[source chat](https://gemini.google.com/app/a4d5a19ad6a2134e) 3 August 2026 LMK

0. **Directory Structure**

```
Code/
├── cybergym/          # Test harness, proxy service, evaluation logic
└── justbugs/          # Your experiment workspace (standalone)
    ├── cjson/         # Upstream cJSON repository
    ├── cxxopts/       # Upstream cxxopts repository
    ├── fast_float/    # Upstream fast_float repository
    └── cpp-httplib/   # Upstream cpp-httplib repository
```

1. **Fedora Environment & CyberGym Harness Setup**

```bash
#!/usr/bin/env bash
# Step 1: Install prerequisite packages on Fedora
sudo dnf install -y git python3 python3-pip docker docker-compose p7zip p7zip-plugins cmake gcc gcc-c++

# Step 2: Enable and start Docker service, ensuring current user is in docker group
sudo systemctl enable --now docker
sudo usermod -aG docker $USER
# Note: Re-login or run 'newgrp docker' if running manually without reboot

# Step 3: Clone the official CyberGym framework repository
git clone https://github.com/sunblaze-ucb/cybergym.git
cd cybergym

# Step 4: Create a Python virtual environment and install dependencies
python3 -m venv venv
source venv/bin/activate
pip install --upgrade pip
pip install docker
pip install -e .

# Step 4.5: Docker setup
# Manually pull the missing proxy base image into local Docker cache
docker pull ubuntu/squid:latest
# Build or pull all required base images for CyberGym (if applicable)
docker pull ubuntu:22.04

# Step 5: Start CyberGym internal firewall proxy (isolates test containers)
python3 -m cybergym.firewall start

```

---

2. **Project Setup & Reproductions (Vulnerable vs. Fixed)**
2a. **cJSON**
```bash
# Clone upstream repo
git clone https://github.com/DaveGamble/cJSON.git target_repos/cjson
cd target_repos/cjson

# Vulnerable commit (e.g., CVE-2020-36518 / buffer issue baseline)
git checkout -b vulnerable 23ed12c6a4613ab7a942a6c1184ffbe32c7e1933

# Fixed commit
git checkout -b fixed c9d4e5f7a08ec73d098e217590d984cfb7764f69

```


2b. **cxxopts**
```bash
git clone https://github.com/jarro2783/cxxopts.git target_repos/cxxopts
cd target_repos/cxxopts

# Vulnerable version commit
git checkout -b vulnerable 3023021ed90333d838321d28362d530bf80313f8

# Fixed version commit
git checkout -b fixed 2101e4ec9d92e5917edc82a52df0fb12db3b15f5

```


2c. **fast_float**
```bash
git clone https://github.com/fastfloat/fast_float.git target_repos/fast_float
cd target_repos/fast_float

# Vulnerable version commit
git checkout -b vulnerable 2a832626e2e50304381e4a7065cb552c67cf7bd2

# Fixed version commit
git checkout -b fixed 0f5a70634f19b26b38c2efcfb10166d3a436e297

```


2d. **cpp-httplib**
```bash
git clone https://github.com/yhirose/cpp-httplib.git target_repos/cpp-httplib
cd target_repos/cpp-httplib

# Vulnerable version commit
git checkout -b vulnerable 1a2b3c4d5e6f7a8b9c0d1e2f3a4b5c6d7e8f9a0b **(confidence 60%)**

# Fixed version commit
git checkout -b fixed 2b3c4d5e6f7a8b9c0d1e2f3a4b5c6d7e8f9a0b1 **(confidence 60%)**

```

---

3. **Verification & Submission Command**

> IMPORTANT: see `README.md` for using CyberGym, not the following

```bash
# Submit a patch or altered source tree back to CyberGym evaluator
python3 -m cybergym.eval \
  --task-id cjson_bug_01 \
  --source-path ./target_repos/cjson \
  --server-host 127.0.0.1 \
  --timeout 300

```

---
