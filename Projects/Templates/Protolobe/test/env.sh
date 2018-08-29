#!/bin/bash
# Setup a base directory:
BASE=$(dirname "$0")

# SJSUOne Board Settings:

# Project Target Settings:
# Sets the binary name, defaults to "firmware" (Optional)
PROJ=Protolobe
# Sets which DBC to generate, defaults to "DBG"

# Compiler and library settings:
# Selects compiler version to use
PATH=$PATH:$BASE/esp-idf/xtensa-esp32-elf/bin
LIB_DIR="$BASE/esp-idf/components"

# Make system settings:
# Number of jobs = 4
# Tune this to nthreads (f.eks. my system has a quad core with two threads per core = 4 threads)
MAKEFLAGS=" -j 4"

# Export everything to the environment
export PATH
export PROJ
export LIB_DIR
