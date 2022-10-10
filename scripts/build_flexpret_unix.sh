#!/usr/bin/env bash

# Project root is one up from the bin directory.
PROJECT_ROOT=$LF_BIN_DIRECTORY/..

echo "starting NRF generation script into $LF_SOURCE_GEN_DIRECTORY"
echo "pwd is $(pwd)"

# Parse filename from the src-gen directory name
ARR=(${LF_SOURCE_GEN_DIRECTORY//\// })
LF_FILENAME=${ARR[-1]} # Without the .lf extension
echo "LF filename is $LF_FILENAME.lf."

# Copy platform into /core
cp $PROJECT_ROOT/flexpret/programs/lib/start.S $LF_SOURCE_GEN_DIRECTORY/core/
cp $PROJECT_ROOT/flexpret/programs/lib/syscalls.c $LF_SOURCE_GEN_DIRECTORY/core/
cp $PROJECT_ROOT/flexpret/programs/lib/startup.c $LF_SOURCE_GEN_DIRECTORY/core/
cp $PROJECT_ROOT/platform/lf_flexpret_support.c $LF_SOURCE_GEN_DIRECTORY/core/platform/
cp $PROJECT_ROOT/platform/lf_flexpret_support.h $LF_SOURCE_GEN_DIRECTORY/core/platform/
cp $PROJECT_ROOT/platform/platform.h $LF_SOURCE_GEN_DIRECTORY/core/
cp $PROJECT_ROOT/platform/tinyalloc/tinyalloc.c $LF_SOURCE_GEN_DIRECTORY/core/
cp $PROJECT_ROOT/platform/tinyalloc/tinyalloc.h $LF_SOURCE_GEN_DIRECTORY/core/
# FIXME: Try to remove the following by building support in the compiler.
cp $PROJECT_ROOT/platform/reactor.c $LF_SOURCE_GEN_DIRECTORY/core/
cp $PROJECT_ROOT/platform/reactor_common.c $LF_SOURCE_GEN_DIRECTORY/core/
cp $PROJECT_ROOT/platform/tag.c $LF_SOURCE_GEN_DIRECTORY/core/

# Copy platform into /include/core
# TODO: Why are there two generated core dirs
cp $PROJECT_ROOT/flexpret/programs/lib/start.S $LF_SOURCE_GEN_DIRECTORY/include/core/
cp $PROJECT_ROOT/flexpret/programs/lib/syscalls.c $LF_SOURCE_GEN_DIRECTORY/include/core/
cp $PROJECT_ROOT/flexpret/programs/lib/startup.c $LF_SOURCE_GEN_DIRECTORY/include/core/
cp $PROJECT_ROOT/platform/lf_flexpret_support.c $LF_SOURCE_GEN_DIRECTORY/include/core/platform/
cp $PROJECT_ROOT/platform/lf_flexpret_support.h $LF_SOURCE_GEN_DIRECTORY/include/core/platform/
cp $PROJECT_ROOT/platform/platform.h $LF_SOURCE_GEN_DIRECTORY/include/core/
cp $PROJECT_ROOT/platform/tinyalloc/tinyalloc.c $LF_SOURCE_GEN_DIRECTORY/include/core/
cp $PROJECT_ROOT/platform/tinyalloc/tinyalloc.h $LF_SOURCE_GEN_DIRECTORY/include/core/
# FIXME: Try to remove the following by building support in the compiler.
cp $PROJECT_ROOT/platform/reactor.c $LF_SOURCE_GEN_DIRECTORY/include/core/
cp $PROJECT_ROOT/platform/reactor_common.c $LF_SOURCE_GEN_DIRECTORY/include/core/
cp $PROJECT_ROOT/platform/tag.c $LF_SOURCE_GEN_DIRECTORY/include/core/

printf '
.DEFAULT_GOAL := all

XLEN ?= 32

# LF variables
LF_SOURCE_GEN_DIRECTORY := %s
LF_FILENAME := %s

# FlexPRET scripts directory
FP_C_SCRIPTS_DIR := $(LF_SOURCE_GEN_DIRECTORY)/../../flexpret/scripts/c/

# Compilation
RISCV_PREFIX ?= riscv$(XLEN)-unknown-elf-
RISCV_GCC ?= $(RISCV_PREFIX)gcc
RISCV_GCC_OPTS ?= -g -static -O0 -march=rv32i -mabi=ilp32 -nostartfiles -specs=nosys.specs
RISCV_LINK ?= -T $(LF_SOURCE_GEN_DIRECTORY)/../../flexpret/programs/lib/linker/flexpret.ld
RISCV_LINK_OPTS ?= -Xlinker -Map=output.map
RISCV_OBJDUMP ?= $(RISCV_PREFIX)objdump
RISCV_OBJDUMP_OPTS ?= -S -d
RISCV_OBJCOPY ?= $(RISCV_PREFIX)objcopy
RISCV_OBJCOPY_OPTS ?= -O binary

INCS  += -I$(LF_SOURCE_GEN_DIRECTORY)/../../flexpret/programs/lib/include
OBJS  :==

#--------------------------------------------------------------------
# Build application
#--------------------------------------------------------------------

$(LF_FILENAME).riscv:
	# Compile the program into riscv binary.
	$(RISCV_GCC) $(INCS) $(RISCV_GCC_OPTS) $(RISCV_LINK) $(RISCV_LINK_OPTS) -o $@ $(LF_SOURCE_GEN_DIRECTORY)/core/start.S $(LF_SOURCE_GEN_DIRECTORY)/core/syscalls.c $(LF_SOURCE_GEN_DIRECTORY)/core/startup.c $(LF_SOURCE_GEN_DIRECTORY)/core/tinyalloc.c $(LF_SOURCE_GEN_DIRECTORY)/core/platform/lf_flexpret_support.c $(wildcard $(LF_SOURCE_GEN_DIRECTORY)/*.c)

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

' $LF_SOURCE_GEN_DIRECTORY $LF_FILENAME > $LF_SOURCE_GEN_DIRECTORY/Makefile

echo "Created $LF_SOURCE_GEN_DIRECTORY/Makefile"

cd $LF_SOURCE_GEN_DIRECTORY
make

echo ""
echo "**** To get simulation outputs:"
echo "cd $LF_SOURCE_GEN_DIRECTORY; fp-emu +ispm=$LF_FILENAME.mem"
echo ""
