FROM ubuntu:resolute

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
 && apt-get dist-upgrade -y \
 && apt-get install -y locales sudo

RUN locale-gen en_US.UTF-8 \
 && update-locale LANG=en_US.UTF-8 LANGUAGE=en_US:en
ENV LANG=en_US.UTF-8 \
    LANGUAGE=en_US:en \
    LC_ALL=en_US.UTF-8

RUN useradd docker -U -G sudo -m -s /bin/bash \
 && echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

RUN apt-get update \
 && apt-get install -y \
    curl bash bc gcc-16 cpp-16 sed patch patchutils tar bzip2 gzip xz-utils zstd perl gawk gperf zip \
      unzip diffutils lzop make file g++-16 xfonts-utils xsltproc default-jre-headless python3 \
      libc6-dev libcrypt-dev libncurses5-dev libjson-perl libxml-parser-perl libparse-yapp-perl rdfind \
      golang-1.26-go git openssh-client rsync upx-ucl nodejs npm cpio xxd \
    --no-install-recommends \
    && ln -s /usr/lib/go-1.26 /usr/lib/go \
    && ln -s /usr/lib/go-1.26/bin/go /usr/bin/go \
    && ln -s /usr/lib/go-1.26/bin/gofmt /usr/bin/gofmt

RUN if [ "$(uname -m)" = "aarch64" ]; then \
  apt-get install -y libc6-amd64-cross qemu-user-binfmt --no-install-recommends; \
 fi

RUN rm -rf /var/lib/apt/lists/*

# Only the versioned gcc-16/g++-16/cpp-16 are installed, so create the
# unversioned names (gcc, cc, cpp, g++, c++, gcov) as plain symlinks.
RUN for t in gcc cpp g++ gcov; do \
      ln -sf /usr/bin/$t-16 /usr/bin/$t; \
    done \
 && ln -sf /usr/bin/gcc-16 /usr/bin/cc \
 && ln -sf /usr/bin/g++-16 /usr/bin/c++ \
 && cc --version && gcc --version && cpp --version && c++ --version

USER docker
