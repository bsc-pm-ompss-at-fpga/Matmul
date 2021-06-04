/*
* Copyright (c) 2020, Barcelona Supercomputing Center
*                     Centro Nacional de Supercomputacion
*
* This program is free software: you can redistribute it and/or modify  
* it under the terms of the GNU General Public License as published by  
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _MATMUL_H_
#define _MATMUL_H_

#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#ifdef USE_MKL
#  include <mkl.h>
#elif USE_OPENBLAS
#  include <cblas.h>
#endif

#ifndef RUNTIME_MODE
#  define RUNTIME_MODE "unknown"
#endif
#ifndef MATMUL_BLOCK_SIZE
#  error MATMUL_BLOCK_SIZE variable not defined
#endif
#ifndef MATMUL_BLOCK_II
#  error MATMUL_BLOCK_II variable not defined
#endif
#ifndef MATMUL_NUM_ACCS
#  error MATMUL_NUM_ACCS variable not defined
#endif
#ifndef FPGA_MEMORY_PORT_WIDTH
#  error FPGA_MEMORY_PORT_WIDTH variable not defined
#endif

// Global variables
const float THRESHOLD = 1e-4;
const unsigned int BSIZE = MATMUL_BLOCK_SIZE;
#pragma omp target device(fpga)
const unsigned int MBLOCK_II = MATMUL_BLOCK_II;
#pragma omp target device(fpga)
const unsigned int MBLOCK_FPGA_PWIDTH = FPGA_MEMORY_PORT_WIDTH;
const unsigned int MBLOCK_NUM_ACCS = MATMUL_NUM_ACCS;

// Elements type
#if defined(USE_DOUBLE)
   typedef double     elem_t;
#  define  ELEM_T_STR "double"
#else
   typedef float      elem_t;
#  define  ELEM_T_STR "float"
#endif /* defined(USE_FLOAT) */

// MKL/OpenBLAS interface
#if defined(USE_DOUBLE)
#  define  GEMM       DGEMM
#  define  cblas_gemm cblas_dgemm
#else
#  define  GEMM       SGEMM
#  define  cblas_gemm cblas_sgemm
#endif /* defined(USE_FLOAT) */

void usage (char* argv0) {
   fprintf(stderr, "USAGE:\t%s <matrix size> <check> <create from>\n", argv0);
   fprintf(stderr, "      \t<block size> is fixed to %u\n", BSIZE);
   fprintf(stderr, "      \t<check> values:\n");
   fprintf(stderr, "      \t  - 0 to disable checking\n");
   fprintf(stderr, "      \t  - 1 to enable checking\n");
   fprintf(stderr, "      \t  - 2 to generate checking reference\n");
   fprintf(stderr, "      \t<create from> values:\n");
   fprintf(stderr, "      \t  - 0 to create block tasks in FPGA\n");
   fprintf(stderr, "      \t  - 1 to create block tasks in SMP\n");
}

double wall_time () {
   struct timespec ts;
   clock_gettime(CLOCK_MONOTONIC,&ts);
   return (double) (ts.tv_sec) + (double) ts.tv_nsec * 1.0e-9;
}

#endif /* _MATMUL_H_ */
