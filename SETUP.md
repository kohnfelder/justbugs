# Setup guide

[source chat](https://gemini.google.com/app/a4d5a19ad6a2134e) 3 August 2026 LMK

## **Directory Structure**

```
Code/
├── cybergym/          # Test harness, proxy service, evaluation logic
└── justbugs/          # Your experiment workspace (standalone)
    ├── cjson/         # Upstream cJSON repository
    ├── cxxopts/       # Upstream cxxopts repository
    ├── fast_float/    # Upstream fast_float repository
    └── cpp-httplib/   # Upstream cpp-httplib repository
```

## **Fedora Environment & CyberGym Harness Setup**

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

# Step 4: Setup and run CyberGym (including reset)

# 1. Stop any lingering containers/networks
docker rm -f cybergym-proxy cybergym-server 2>/dev/null
docker network rm cybergym-internal 2>/dev/null

# 2. Activate virtual environment and install full backend suite
source venv/bin/activate
pip install uvicorn fastapi pydantic pydantic-settings docker requests httpx jinja2 sqlalchemy sqlmodel python-multipart

# 3. Start the proxy container
python3 -m cybergym.firewall start

# 4. Get gateway IP and launch evaluation server
HOST=$(docker network inspect cybergym-internal -f '{{(index .IPAM.Config 0).Gateway}}')

python3 -m cybergym.server \
  --host $HOST \
  --port 8666 \
  --mask_map_path mask_map.json \
  --log_dir ./server_poc \
  --db_path ./server_poc/poc.db

```

## Test a project

### Prepare context

```
cd /home/lmk/Code/justbugs/cjson
source ../../cybergym/venv/bin/activate

HOST=$(docker network inspect cybergym-internal -f '{{(index .IPAM.Config 0).Gateway}}')

# Submit evaluation run using a valid task ID from mask_map.json (e.g. arvo:10013)
python3 -m cybergym eval \
  --server "http://${HOST}:8666" \
  --task-id "arvo:10013" \
  --repo-path .
```

