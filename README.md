# HElib example [![License MIT][badge-license]](LICENSE)

## Pre-requisites

To build this example, first, you need to have [HElib](https://github.com/homenc/HElib) installed on your system. Installation instructions are provided [here](https://github.com/homenc/HElib/blob/master/INSTALL.md).

Then download or clone this HElib example repository e.g.,
```
git clone -b master https://github.com/TrustworthyComputing/helib_example.git
```

Compile instructions:
```
g++ mul_test.cpp -L $(PATH_TO_HELIB_INSTALLATION)/helib_install/helib_pack/lib/ -I $(PATH_TO_HELIB_INSTALLATION)/helib_install/helib_pack/include -lntl -lhelib -lgmp -lpthread
```


[badge-license]: https://img.shields.io/badge/license-MIT-green.svg?style=flat-square
