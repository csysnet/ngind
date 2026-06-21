FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    g++ \
    clang \
    gdb \
    make \
    cmake \
    git \
    valgrind \
    strace \
    ltrace

WORKDIR /workspace
