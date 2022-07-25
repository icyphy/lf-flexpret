# Helper fragment to help run flexpret with verilator.
# Copyright 2021 Edward Wang <edwardw@eecs.berkeley.edu>

EMULATOR_BIN = $(EMULATOR_DIR)/fp-emu
HDL_SCRIPTS = $(SCRIPTS_DIR)/hdl

$(EMULATOR_BIN): $(VERILOG_RAW) $(EMULATOR_DIR)/main.cpp $(HDL_SCRIPTS)/simify_verilog.py
	# Inject the right simulation constructs
	$(HDL_SCRIPTS)/simify_verilog.py $(VERILOG_RAW) > $(EMULATOR_DIR)/Core.sim.v

	(cd $(EMULATOR_DIR) && verilator --cc Core.sim.v --exe --trace --build main.cpp)

	cp $(EMULATOR_DIR)/obj_dir/VCore $(EMULATOR_BIN)

	echo "Emulator usage: Run $(EMULATOR_BIN) in a folder with imem.hex.txt. You can generate it with scripts/parse_disasm.py using an objdump. It will generate a VCD in Core.vcd. Writing 0xbaaabaaa will cause the next value to be printed. Writing 0xdeaddead to the tohost CSR will terminate the simulation."
