# Scripting Interface

## Building

Those are some building hints how I built the dependencies
for the scripting interface. Useful for people who get
easily lost and have no idea where to start.

### cling (C++)

```sh
mkdir cling && cd cling

git clone http://root.cern.ch/git/llvm.git src
cd src
git checkout cling-patches
cd tools
git clone http://root.cern.ch/git/cling.git
git clone http://root.cern.ch/git/clang.git
cd clang
git checkout cling-patches
cd ../../..

mkdir build && cd build
cmake \
    -DCMAKE_INSTALL_PREFIX=/opt/cling \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_TARGETS_TO_BUILD=X86 \
    -DBUILD_SHARED_LIBS=ON \
    -DLLVM_ENABLE_CXX1Y=ON \
    -DLLVM_ENABLE_CXX1Z=ON \
    -DLLVM_INCLUDE_DOCS=OFF \
    -DLLVM_INCLUDE_EXAMPLES=OFF \
    -DLLVM_BUILD_EXAMPLES=OFF \
    -DLLVM_INCLUDE_TESTS=OFF \
    -DLLVM_BUILD_TESTS=OFF \
    -DLLVM_INCLUDE_UTILS=OFF \
    -DLLVM_BUILD_UTILS=OFF \
    ../src
make
make install
```

QAPI-IDE CMake Option: `-DCling_DIR=/opt/cling`
