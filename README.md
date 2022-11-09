# Lingua Franca on FlexPRET
[Lingua Franca (LF)](https://github.com/lf-lang/lingua-franca) is a polyglot coordination language featuring time as a first-class citizen. [FlexPRET](https://github.com/pretis/flexpret) is a 5-stage RISC-V processor with timing-predictable and repeatable execution. This repository explores how the high-level timing semantics and constraints at the LF level are fulfilled by FlexPRET such that hard real-time deadlines are guaranteed to be satisfied.

## Setup

(Optional) To skip the setup, you can use a pre-built [Ubuntu VM image](https://drive.google.com/file/d/1-up7AZVIdNj_yFmq8U6EUC5M5qc8zlSB/view?usp=sharing) with most of the dependencies installed. Follow [these instructions](https://sklin.notion.site/FlexPRET-VM-setup-notes-6ef662f5649d43128417c4f8acab0b6e) to adjust the VM image.

1. Install the RISC-V GNU toolchain by following these [instructions](https://github.com/pretis/flexpret#risc-v-compiler).

2. Clone this repo with submodules.
```bash
git clone https://github.com/icyphy/lf-flexpret.git
git submodule update --init --recursive
```

3. Install Lingua Franca (Java 17 or up required).
```bash
cd lingua-franca
./bin/build-lf-cli
```

4. Add `./bin/lfc` to `PATH`.

## Quick start

1. Build a FlexPRET emulator.
```bash
cd flexpret
make emulator
cd ..
```

2. Build and run a simple LF program
```bash
lfc src/Timer.lf
cd src-gen/Timer/
../../flexpret/emulator/fp-emu +ispm=Timer.mem
```

You should see the following outputs
```
$ ../../flexpret/emulator/fp-emu +ispm=Timer.mem
1
2
3
4
5
6
7
8
9
10
- Core.sim.v:3154: Verilog $finish
```

It is recommended to add `flexpret/emulator/fp-emu` to `PATH` so that `fp-emu` can be directly called anywhere.
