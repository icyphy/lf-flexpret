FROM ubuntu:20.04

RUN apt-get update

# Install common utilities
RUN apt-get install -y vim

# Install JAVA17
RUN apt update \
    && apt install -y openjdk-17-jdk \
    && java -version
ENV JAVA_HOME "/usr/lib/jvm/java-17-openjdk-amd64"
ENV PATH "$JAVA_HOME/bin:$PATH"

# Install SBT
RUN apt-get install -y gnupg curl
RUN echo "deb https://repo.scala-sbt.org/scalasbt/debian all main" | tee /etc/apt/sources.list.d/sbt.list \
    && echo "deb https://repo.scala-sbt.org/scalasbt/debian /" | tee /etc/apt/sources.list.d/sbt_old.list \
    && curl -sL "https://keyserver.ubuntu.com/pks/lookup?op=get&search=0x2EE0EA64E40A89B84B2DF73499E82A75642AC823" | apt-key add \
    && apt-get update \
    && apt-get install -y sbt

# Install Verilator dependencies
RUN apt-get install -y git perl python3 make g++ libfl2 libfl-dev zlibc zlib1g zlib1g-dev
RUN apt-get install -y ccache libgoogle-perftools-dev numactl perl-doc
RUN apt-get install -y autoconf flex bison
RUN git clone https://github.com/verilator/verilator \
    && cd verilator \
    && git checkout v4.038 \
    && autoconf \
    && ./configure \
    && make -j4 \
    && make install

