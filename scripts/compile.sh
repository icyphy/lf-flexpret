#!/bin/bash
# Compile a RISC-V C program with a start script and create an objdump.
# Copyright 2021 Edward Wang <edwardw@eecs.berkeley.edu>

set -ex
set -euo pipefail

dirname "$0"

output_name="$1"
shift

riscv64-unknown-elf-gcc -I$(dirname "$0")/include -g -static -O0 -march=rv32i -mabi=ilp32 -specs=nosys.specs -nostartfiles -Wl,-Ttext=0x00000000 -o "$output_name" $(dirname "$0")/start.S "$@"
riscv64-unknown-elf-objdump -S -d "$output_name" > "$output_name.dump.txt"
