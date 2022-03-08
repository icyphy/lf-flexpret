rm -r build
mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..
make
riscv64-unknown-elf-objdump -S -d Minimal > Minimal.dump.txt
~/lf_flexpret/flexpret/scripts/c/parse_disasm.py Minimal.dump.txt readmemh > Minimal.hex
