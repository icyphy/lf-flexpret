#!/bin/bash

set -euxo pipefail

SCRIPT_DIR=$(dirname "$0")

$SCRIPT_DIR/compile.sh $1 $2
$SCRIPT_DIR/parse_disasm.py $1.dump.txt readmemh > imem.hex.txt
