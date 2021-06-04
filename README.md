# Matmul (FPGA devices version)

**Name**: Matrix Multiplication Kernel  
**Contact Person**: OmpSs@FPGA Team, ompss-fpga-support@bsc.es  
**License Agreement**: GPL  
**Platform**: OmpSs@FPGA


### Description
This application performs the multiplication of two square matrices. The matrices are allocated by blocks of contiguous memory.

The task implementation may be changed if support for some external library is enabled at compile time. Otherwise, a basic implementation is provided. See the *Build variables* section for more information.

### Build instructions
Clone the repository:
```
git clone https://github.com/bsc-pm-ompss-at-fpga/Matmul.git
cd matmul
```

Build the application binaries:
```
make BOARD=zedboard CROSS_COMPILE=arm-linux-gnueabihf-
```
##### Build variables
You can change the build process defining or modifying some environment variables.
The supported ones are:
  - `CFLAGS`. Compiler flags. The following preprocessor variables can be defined to modify the application:
    - `-DUSE_DOUBLE`. The matrix elements are of type `double` instead of `float`.
    - `-DUSE_IMPLEMENTS`. Enable the implements feature. Then, matmulBlock function will have two targets: FPGA and SMP (implemented using OPENBLAS, MKL, or basic C code).
  - `LDFLAGS`
  - `MCC`. If not defined, the default value is: `fpgacc`.
  - `CROSS_COMPILE`
  - `MKL_DIR`. Installation directory of MKL library (only used for the implements feature). The default value is: `$MKLROOT`.
    - `MKL_INC_DIR`. Installation directory of includes for MKL library. The default value is: `$MKL_DIR/include`.
    - `MKL_LIB_DIR`. Installation directory of OS libraries for MKL library. The default value is: `$MKL_DIR/lib`.
  - `OPENBLAS_DIR`. Installation directory of OpenBLAS library (only used for the implements feature). The default value is: `$OPENBLAS_HOME`.
    - `OPENBLAS_INC_DIR`. Installation directory of includes for OpenBLAS library. The default value is: `$OPENBLAS_DIR/include`.
    - `OPENBLAS_LIB_DIR`. Installation directory of OS libraries for OpenBLAS library. The default value is: `$OPENBLAS_DIR/lib`.
  - `BOARD`. Board option used when generating the bitstreams.
  - `FPGA_CLOCK`. Target frequency of FPGA accelerators in the bitstreams. The default value is: `200`.
  - `FPGA_MEMORY_PORT_WIDTH`. Bit-width of accelerators memory port to access main memory. The default value is: `128`.
  - `MATMUL_BLOCK_SIZE`. Dimension of matrix blocks that FPGA accelerators deal with. The default value is: `64`.
  - `MATMUL_NUM_ACCS`. Number of FPGA accelerators for matmulBlock task. The default value is: `1`.
  - `MATMUL_BLOCK_II`. Initiation interval, in cycles, for matmulBlock middle loop. The default value is: `2`.

To check the correct support detection of backend libraries, you can use the `make info` target once the environment variables are properly set.

For example, the build step to cross-compile the application for ARM may be:
```
export CROSS_COMPILE=arm-linux-gnueabihf-
make matmul-d bitstream-p
```

### Run instructions
The name of each binary file created by build step ends with a suffix which determines the version:
 - program-p: performance version
 - program-i: instrumented version
 - program-d: debug version

All versions use the same arguments structure:
```
./matmul-p <matrix size> <check> <create from>
```
where:
 - `matrix size` (Mandatory) is the dimension of the matrices.
 - `check` (Mandatory) defines if the result must be checked.
   The result is checked against a reference solution file which must be available inside the `ref` folder.
   To generate those files, you can run the application using the value `2` of check argument.
 - `create from` (Mandatory) defines where tasks will be created.
   0 means create from FPGA and 1 from SMP.
