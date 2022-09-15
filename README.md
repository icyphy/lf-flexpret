# Running LF on FlexPRET
[Lingua Franca (LF)](https://github.com/lf-lang/lingua-franca) is a polyglot coordination language featuring time as a first-class citizen. [FlexPRET](https://github.com/pretis/flexpret) is a 5-stage RISC-V processor with timing-predictable and repeatable execution. This repository explores how the high-level timing semantics and constraints at the LF level are fulfilled by FlexPRET such that hard real-time deadlines are guaranteed to be satisfied.

## Setup

(Optional) To skip the setup, you can use a pre-built [Ubuntu VM image](https://drive.google.com/file/d/1-up7AZVIdNj_yFmq8U6EUC5M5qc8zlSB/view?usp=sharing) with most of the dependencies installed. Follow [these instructions](https://leaf-enquiry-8f7.notion.site/Adjusting-the-VM-6ef662f5649d43128417c4f8acab0b6e) to adjust the VM image.

1. Install the [RISC-V GNU toolchain](https://github.com/pretis/flexpret#risc-v-compiler)

2. Install [Lingua Franca](https://github.com/lf-lang/lingua-franca/wiki/Downloading-and-Building)

3. Clone this repo with the `flexpret` submodule
```bash
git clone https://github.com/lsk567/lf-flexpret.git
git submodule update --init --recursive
```

## Quick start

1. Build a FlexPRET emulator
```bash
cd flexpret
make emulator
cd ..
```

2. Build and run a minimal LF program
```bash
cd lf/Minimal/src-gen-cpy/Minimal_no_cmake
sh rebuild.sh
./Minimal
```
