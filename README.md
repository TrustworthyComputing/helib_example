# HElib example [![License MIT][badge-license]](LICENSE)

## Pre-requisites

To build this example, first, you need to have [HElib](https://github.com/homenc/HElib) installed on your system. Installation instructions are provided [here](https://github.com/homenc/HElib/blob/master/INSTALL.md). NOTE: replace the HElib source files with the modified source files provided in this repo
before installing.

Then download or clone this HElib example repository e.g.,
```
git clone -b master https://github.com/TrustworthyComputing/helib_example.git
```

Create a new environmental variable to point to the HElib installation directory:
```
export HELIB_INSTALL_DIR="/home/...your-custom-path.../helib_install/helib_pack/"
```

Point the `LD_LIBRARY_PATH` environmental variable to the helib installation directory:
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HELIB_INSTALL_DIR/lib
```

## Build this example

Compile instructions:
```
g++ -o mul_test.out mul_test.cpp -L $HELIB_INSTALL_DIR/lib/ -I $HELIB_INSTALL_DIR/include -lntl -lhelib -lgmp -lpthread
```

## Run HElib example
Finally, to run the example type (first argument is the number of multiplications, second argument is the size of the total bits of the ctxt primes):
```
./mul_test.out 15 500
```

## PALISADE example
Also, check out our [PALISADE example](https://github.com/TrustworthyComputing/palisade_example).

[badge-license]: https://img.shields.io/badge/license-MIT-green.svg?style=flat-square
