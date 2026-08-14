# Code flaw reduction via quality improvement

All script commands start in project root directory

* ./
  * ./cjson/   # cJSON repo
  * ./poc/     # proof of concept testing

## Setup cJSON

Install dependencies (Fedora):
`bash sudo dnf install libasan libubsan`

### Install repo

```
git clone https://github.com/DaveGamble/cJSON.git
cd cjson
mkdir build
```

### Compile and test

Both vulnerable version and later version fixing known vulnerabilities

```
cd cjson
git checkout v1.7.15 # vulnerable release, or v1.7.18 with fixes
cd build
cmake .. -DENABLE_CJSON_TEST=ON
make
ctest --output-on-failure
cd ..
```

## Experiments

### Proof of concept attempt

```
cd poc
gcc -g -O0 -I../cjson ../cjson/cJSON.c poc1.c -o poc1 -lm
```

