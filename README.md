You need: 
* msvc
* git 
* cmake

build the project:
```
git submodule update --init --recursive
mkdir build
cd build
cmake .. # add -DBUILD_SHARED_LIBS=OFF to link statically
cmake --build . --target 
```
