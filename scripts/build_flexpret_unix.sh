#!/usr/bin/env bash

# Project root is one up from the bin directory.
PROJECT_ROOT=$LF_BIN_DIRECTORY/..

echo "starting NRF generation script into $LF_SOURCE_GEN_DIRECTORY"
echo "pwd is $(pwd)"

# Parse filename from the src-gen directory name
IFS='/' read -ra ARR <<< "$LF_SOURCE_GEN_DIRECTORY"
LF_FILENAME=${ARR[-1]} # LF filename without the .lf extension
echo "LF filename is $LF_FILENAME.lf."

# Copy c files into /core.
cp "$PROJECT_ROOT/flexpret/programs/lib/start.S" "$LF_SOURCE_GEN_DIRECTORY/core/"
cp "$PROJECT_ROOT/flexpret/programs/lib/syscalls.c" "$LF_SOURCE_GEN_DIRECTORY/core/"
cp "$PROJECT_ROOT/flexpret/programs/lib/startup.c" "$LF_SOURCE_GEN_DIRECTORY/core/"
cp -r "$PROJECT_ROOT/flexpret/programs/lib/tinyalloc" "$LF_SOURCE_GEN_DIRECTORY/core/"
cp "$PROJECT_ROOT/platform/lf_flexpret_support.c" "$LF_SOURCE_GEN_DIRECTORY/core/platform/"

# Copy header files into /include.
cp -r "$PROJECT_ROOT/flexpret/programs/lib/tinyalloc" "$LF_SOURCE_GEN_DIRECTORY/include/core/"
cp "$PROJECT_ROOT/platform/lf_flexpret_support.h" "$LF_SOURCE_GEN_DIRECTORY/include/core/platform/"
cp "$PROJECT_ROOT/platform/platform.h" "$LF_SOURCE_GEN_DIRECTORY/include/core/"

printf '
.DEFAULT_GOAL := all

XLEN ?= 32

# LF variables
LF_PROJECT_ROOT := %s
LF_SOURCE_GEN_DIRECTORY := %s
LF_FILENAME := %s

# FlexPRET scripts directory
FP_C_SCRIPTS_DIR := "$(LF_PROJECT_ROOT)/flexpret/scripts/c/"

# Compilation
RISCV_PREFIX ?= riscv$(XLEN)-unknown-elf-
RISCV_GCC ?= $(RISCV_PREFIX)gcc
RISCV_GCC_OPTS ?= -g -static -O0 -march=rv32i -mabi=ilp32 -nostartfiles -specs=nosys.specs
RISCV_LINK ?= -T "$(LF_PROJECT_ROOT)/flexpret/programs/lib/linker/flexpret.ld"
RISCV_LINK_OPTS ?= -Xlinker -Map=output.map
RISCV_OBJDUMP ?= $(RISCV_PREFIX)objdump
RISCV_OBJDUMP_OPTS ?= -S -d
RISCV_OBJCOPY ?= $(RISCV_PREFIX)objcopy
RISCV_OBJCOPY_OPTS ?= -O binary

INCS  := -I"$(LF_PROJECT_ROOT)/flexpret/programs/lib/include" \
    -I"$(LF_SOURCE_GEN_DIRECTORY)/include/" \
    -I"$(LF_SOURCE_GEN_DIRECTORY)/include/api" \
    -I"$(LF_SOURCE_GEN_DIRECTORY)/include/core" \
    -I"$(LF_SOURCE_GEN_DIRECTORY)/include/core/modal_models" \
    -I"$(LF_SOURCE_GEN_DIRECTORY)/include/core/utils" \
    -I"$(LF_SOURCE_GEN_DIRECTORY)/include/core/platform"

# Usually defined in CMakeLists.txt
# "-DARDUINO" is a hacky way to disable signal() in reactor_common.c.
# A proper way would set an EMBEDDED macro in reactor_common.c
# to disable signal() for all embedded boards.
DEFS  := -DINITIAL_EVENT_QUEUE_SIZE=10 \
	-DINITIAL_REACT_QUEUE_SIZE=10 \
	-DARDUINO

#--------------------------------------------------------------------
# Build application
#--------------------------------------------------------------------

$(LF_FILENAME).riscv:
	# Compile the program into riscv binary.
	$(RISCV_GCC) $(INCS) $(DEFS) $(RISCV_GCC_OPTS) $(RISCV_LINK) \
		$(RISCV_LINK_OPTS) -o $@ \
		"$(LF_SOURCE_GEN_DIRECTORY)/core/start.S" \
		"$(LF_SOURCE_GEN_DIRECTORY)/core/syscalls.c" \
		"$(LF_SOURCE_GEN_DIRECTORY)/core/tinyalloc/tinyalloc.c" \
		"$(LF_SOURCE_GEN_DIRECTORY)/core/startup.c" \
		"$(LF_SOURCE_GEN_DIRECTORY)/core/platform/lf_flexpret_support.c" \
		"$(LF_SOURCE_GEN_DIRECTORY)/core/port.c" \
		"$(LF_SOURCE_GEN_DIRECTORY)/core/tag.c" \
		"$(LF_SOURCE_GEN_DIRECTORY)/core/utils/pqueue.c" \
		"$(LF_SOURCE_GEN_DIRECTORY)/core/utils/util.c" \
		"$(LF_SOURCE_GEN_DIRECTORY)/core/utils/vector.c" \
		"$(LF_SOURCE_GEN_DIRECTORY)/core/reactor_common.c" \
		"$(LF_SOURCE_GEN_DIRECTORY)/core/reactor.c" \
		$(wildcard $(LF_SOURCE_GEN_DIRECTORY)/*.c) # FIXME: Does not handle spaces.

$(LF_FILENAME).dump: $(LF_FILENAME).riscv
	$(RISCV_OBJDUMP) $(RISCV_OBJDUMP_OPTS) $< > $@

$(LF_FILENAME).mem: $(LF_FILENAME).riscv $(LF_FILENAME).dump
	# Extract a binary file from the ELF file.
	$(RISCV_OBJCOPY) $(RISCV_OBJCOPY_OPTS) $< $<.binary.txt
	# Generate a hex file (with a .mem extension) from the binary file.
	xxd -c 4 -e $<.binary.txt | cut -c11-18 > $@
	xxd -c 4 -e $<.binary.txt > $@.orig
	# Delete the binary file.
	rm $<.binary.txt

#--------------------------------------------------------------------
# General
#--------------------------------------------------------------------

all: $(LF_FILENAME).mem

clean:
	rm -f *.riscv *.dump *.mem

cleanall: clean
	cd $(base_dir) && $(MAKE) clean

.PHONY: clean cleanall

' "$PROJECT_ROOT" "$LF_SOURCE_GEN_DIRECTORY" "$LF_FILENAME" > "$LF_SOURCE_GEN_DIRECTORY/Makefile"

echo "Created $LF_SOURCE_GEN_DIRECTORY/Makefile"

cd "$LF_SOURCE_GEN_DIRECTORY"
make

echo ""
echo "**** To get simulation outputs:"
echo "cd $LF_SOURCE_GEN_DIRECTORY; fp-emu +ispm=$LF_FILENAME.mem"
echo ""
