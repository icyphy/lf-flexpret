FROM lsk567/lf-flexpret-base:07.26

# Update RISCV/PATH vars for RISCV toolchain
ENV RISCV="/usr/bin/riscv-tools"
ENV PATH="$RISCV/bin:$PATH"

# Install riscv-tools
RUN git clone https://github.com/riscv/riscv-tools.git /tmp/riscv-tools
RUN cd /tmp/riscv-tools \
    && git checkout b266e97760fbe57feee534941f890d0cde14245a \
    && git submodule update --init --recursive
RUN cd /tmp/riscv-tools \
    && ./build-rv32ima.sh \
    && rm -rf /tmp/riscv-tools

# Install Lingua Franca
RUN mkdir git \
    && cd git \
    && git clone --recursive https://github.com/icyphy/lingua-franca.git
ENV LF="/git/lingua-franca"
ENV PATH="$LF/bin:$PATH"
# Fixing the ssl issue: https://stackoverflow.com/a/29313285/7869204
# RUN update-ca-certificates -f \
#     && ./git/lingua-franca/bin/build-lfc
RUN ./git/lingua-franca/bin/build-lfc

# Clone FlexPRET
RUN git clone https://github.com/pretis/flexpret.git

# Clone Project Repo
RUN git clone https://github.com/edwardcwang/lf-flexpret-scripts.git \
    && echo 'export PATH="/lf-flexpret-scripts/scripts:$PATH"' >> ~/.bashrc
