FROM ubuntu:20.04
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update

# Install common utilities
RUN apt-get install -y curl vim wget git

# Install JAVA17
RUN apt-get install -y openjdk-17-jdk \
    && java -version
ENV JAVA_HOME "/usr/lib/jvm/java-17-openjdk-amd64"
ENV PATH "$JAVA_HOME/bin:$PATH"

# Install Lingua Franca 0.3.0
RUN curl -L https://github.com/lf-lang/lingua-franca/releases/download/v0.3.0/lfc_0.3.0.tar.gz --output /opt/lfc_0.3.0.tar.gz
RUN cd /opt \
    && tar xvf lfc_0.3.0.tar.gz \
    && rm lfc_0.3.0.tar.gz 
ENV LFC_HOME "/opt/lfc_0.3.0"
ENV PATH "$LFC_HOME/bin:$PATH"

# Install RISCV tools see: https://github.com/riscv-collab/riscv-gnu-toolchain
RUN apt-get install -y autoconf automake autotools-dev curl python3 
RUN apt-get install -y libmpc-dev libmpfr-dev libgmp-dev gawk build-essential 
RUN apt-get install -y bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev
ENV RISCV="/usr/bin/riscv-tools"
RUN git clone --recursive https://github.com/riscv-collab/riscv-gnu-toolchain /tmp/riscv-gnu-toolchain \
    && cd /tmp/riscv-gnu-toolchain \
    && ./configure --prefix=$RISCV --with-arch=rv32i --with-abi=ilp32 \
    && make

ENV PATH="$RISCV/bin:$PATH"