# Local CyberGym Small-Scale LLM Vulnerability Experiment

This workspace provides a lightweight harness for evaluating LLM patch generation and vulnerability verification against 4 small-footprint CyberGym target projects: `cJSON`, `cxxopts`, `fast_float`, and `cpp-httplib`.

## Prerequisites
- Fedora Linux (38+)
- Docker service enabled and active
- Python 3.10+

## Directory Structure

```

.
├── cybergym/               # Cloned CyberGym evaluation framework
├── justbugs/               # sibling directory
│   ├── README.md SETUP.md  # documentation (such as it is)
│   ├── cjson/              # cJSON repo (branches: vulnerable, fixed)
│   ├── cxxopts/            # cxxopts repo (branches: vulnerable, fixed)
│   ├── fast_float/         # fast_float repo (branches: vulnerable, fixed)
│   └── cpp-httplib/        # cpp-httplib repo (branches: vulnerable, fixed)

```

## Quick Start Guide

### 1. Initialize CyberGym Harness
```bash
(cd ../cybergym; source venv/bin/activate; python3 -m cybergym.firewall start)

```

### 2. Switch Project Versions

To test an LLM against a vulnerable baseline or verify a fixed version:

```bash
# Check out vulnerable code for LLM analysis
cd cjson
git checkout vulnerable

# Generate LLM patch onto source code...

cd ..
```

### 3. Start up evaluation service (Terminal session 1)

#### 1. Go to your cybergym installation directory
cd cybergym

#### 2. Activate the virtual environment
source venv/bin/activate

#### 3. Obtain the Docker network gateway IP address
HOST=$(docker network inspect cybergym-internal -f '{{(index .IPAM.Config 0).Gateway}}')

#### 4. Launch the evaluation server
python3 -m cybergym.server \
  --host $HOST \
  --port 8666 \
  --mask_map_path mask_map.json \
  --log_dir ./server_poc \
  --db_path ./server_poc/poc.db

### 5. Evaluate

#### 1. Open a new terminal and navigate to your target project
cd /home/lmk/Code/justbugs/cjson

#### 2. Activate the CyberGym virtual environment from the sibling directory
source ../../cybergym/venv/bin/activate

#### 3. Check out the baseline (vulnerable) version to test
git checkout vulnerable

#### 4. Get the same host gateway IP
HOST=$(docker network inspect cybergym-internal -f '{{(index .IPAM.Config 0).Gateway}}')

#### 5. Submit a PoC or modified source test case to the evaluation server
python3 -m cybergym.client \
  --server "http://${HOST}:8666" \
  --task-id "cjson_bug_01" \
  --poc-path ./test_poc.bin


**FOLLOWING DEPRECATED**


### 3. Evaluate Patch via CyberGym Engine

```bash
cd ../cybergym
python3 -m cybergym.eval --task-id cjson_bug_01 --source-path ../justbugs/cjson

```

### 4. Teardown Environment

```bash
(cd ../cybergem; python3 -m cybergym.firewall stop-all)

```

---

**Sources / References:**
- CyberGym Repository & Setup Instructions: [https://github.com/sunblaze-ucb/cybergym](https://github.com/sunblaze-ucb/cybergym)
- CyberGym E2E Evaluation Methodology: [https://arxiv.org/abs/2606.04460](https://arxiv.org/abs/2606.04460)

