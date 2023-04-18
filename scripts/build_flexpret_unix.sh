#!/usr/bin/env bash

# Project root is one up from the bin directory.
PROJECT_ROOT=$LF_BIN_DIRECTORY/..

echo "Generating a Makefile for FlexPRET in $LF_SOURCE_GEN_DIRECTORY"
echo "Current directory is $(pwd)"

# Parse filename from the src-gen directory name
# https://stackoverflow.com/questions/3162385/how-to-split-a-string-in-shell-and-get-the-last-field
LF_FILENAME=${LF_SOURCE_GEN_DIRECTORY##*/} # Get the LF filename without the .lf extension.
echo "The LF filename is $LF_FILENAME.lf."

# Copy c files into /core.
cp "$PROJECT_ROOT/platform/lf_flexpret_support.c" "$LF_SOURCE_GEN_DIRECTORY/core/platform/"

# Copy header files into /include.
cp "$PROJECT_ROOT/platform/lf_flexpret_support.h" "$LF_SOURCE_GEN_DIRECTORY/include/core/platform/"
cp "$PROJECT_ROOT/platform/platform.h" "$LF_SOURCE_GEN_DIRECTORY/include/core/"

printf '
# LF variables
LF_PROJECT_ROOT := %s
LF_SOURCE_GEN_DIRECTORY := %s
LF_FILENAME := %s

NAME = $(LF_FILENAME)

APP_INCS := -I$(LF_PROJECT_ROOT)/flexpret/programs/lib/include \
    -I$(LF_SOURCE_GEN_DIRECTORY)/include/ \
    -I$(LF_SOURCE_GEN_DIRECTORY)/include/api \
    -I$(LF_SOURCE_GEN_DIRECTORY)/include/core \
    -I$(LF_SOURCE_GEN_DIRECTORY)/include/core/modal_models \
    -I$(LF_SOURCE_GEN_DIRECTORY)/include/core/utils \
    -I$(LF_SOURCE_GEN_DIRECTORY)/include/core/platform

APP_DEFS  := -DINITIAL_EVENT_QUEUE_SIZE=10 \
	-DINITIAL_REACT_QUEUE_SIZE=10 \
	-DNO_TTY \
	-DPLATFORM_FLEXPRET \
	-DLF_UNTHREADED

GENERAL_SOURCES := \
		$(LF_SOURCE_GEN_DIRECTORY)/core/port.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/tag.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/mixed_radix.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/reactor_common.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/trace.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/lf_token.c \
		$(LF_SOURCE_GEN_DIRECTORY)/lib/schedule.c

UTIL_SOURCES := \
		$(LF_SOURCE_GEN_DIRECTORY)/core/utils/pqueue.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/utils/util.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/utils/hashset/hashset.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/utils/hashset/hashset_itr.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/utils/vector.c

THREADED_SOURCES := \
		$(LF_SOURCE_GEN_DIRECTORY)/core/threaded/reactor_threaded.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/threaded/scheduler_adaptive.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/threaded/scheduler_GEDF_NP_CI.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/threaded/scheduler_GEDF_NP.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/threaded/scheduler_NP.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/threaded/scheduler_PEDF_NP.c \
		$(LF_SOURCE_GEN_DIRECTORY)/core/threaded/scheduler_sync_tag_advance.c 

UNTHREADED_SOURCES := \
		$(LF_SOURCE_GEN_DIRECTORY)/core/reactor.c

MODAL_SOURCES := \
		$(LF_SOURCE_GEN_DIRECTORY)/core/modal_models/modes.c

PLATFORM_SOURCES := \
		$(LF_SOURCE_GEN_DIRECTORY)/core/platform/lf_flexpret_support.c

APP_SOURCES := \
		$(GENERAL_SOURCES) \
		$(UTIL_SOURCES) \
		$(UNTHREADED_SOURCES) \
		$(THREADED_SOURCES) \
		$(MODAL_SOURCES) \
		$(PLATFORM_SOURCES) \
		$(wildcard $(LF_SOURCE_GEN_DIRECTORY)/*.c) # FIXME: Does not handle spaces.

include $(FLEXPRET_ROOT_DIR)/Makefrag

' "$PROJECT_ROOT" "$LF_SOURCE_GEN_DIRECTORY" "$LF_FILENAME" > "$LF_SOURCE_GEN_DIRECTORY/Makefile"

echo "Created $LF_SOURCE_GEN_DIRECTORY/Makefile"

cd "$LF_SOURCE_GEN_DIRECTORY"
make

echo ""
echo "**** To get simulation outputs:"
echo "cd $LF_SOURCE_GEN_DIRECTORY; fp-emu +ispm=$LF_FILENAME.mem"
echo ""
