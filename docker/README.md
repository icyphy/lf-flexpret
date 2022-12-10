# Docker containers for FlexPRET project
Building the docker images on your machine will take hours. The fastest way is to just pull prebuilt images from Dockerhub:

https://hub.docker.com/r/erlingrj/lf_chisel3
https://hub.docker.com/repository/docker/erlingrj/lf_riscv

## chisel3 docker
This Dockerfile builds the dependencies needed to generate verilog and verilator simulations from the FlexPRET CPU.
It does *not* include RISCV GCC needed to build programs to run on FlexPRET.
It does not pull any repos from Github and the idea is that the user has the FlexPRET repo on the *host* system and mounts it into the docker container.

### Usage
Build an image
`docker build -f chisel3.Dockerfile -t lf_chisel3 .`

Start a container with this repository mounted into. Please update the command to reflect the setup on your machine
`docker run -it -v C:\Users\erling\dev\lf-flexpret:/lf-flexpret --rm lf_chisel3 /bin/bash` 

Test that you can build FlexPRET

```
cd /lf-flexpret/flexpret
make clean
make emulator
```

## lf+riscv toolchain docker
### Usage
Build an image
`docker build -f chisel3.Dockerfile -t lf_riscv .`

Start a container with this repository mounted into. Please update the command to reflect the setup on your machine
`docker run -it -v C:\Users\erling\dev\lf-flexpret:/lf-flexpret --rm lf_riscv /bin/bash` 