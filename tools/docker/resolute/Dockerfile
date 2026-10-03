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
      golang-1.26-go git openssh-client rsync upx-ucl nodejs npm pigz cpio xxd libgirepository-1.0-1 xmlstarlet wget \
    --no-install-recommends \
    && ln -s /usr/lib/go-1.26 /usr/lib/go \
    && ln -s /usr/lib/go-1.26/bin/go /usr/bin/go \
    && ln -s /usr/lib/go-1.26/bin/gofmt /usr/bin/gofmt

RUN if [ "$(uname -m)" = "aarch64" ]; then \
  apt-get install -y libc6-amd64-cross qemu-user-binfmt --no-install-recommends; \
 fi

RUN rm -rf /var/lib/apt/lists/*

RUN [ ! -f /usr/bin/cc ] && ln -s /usr/bin/gcc /usr/bin/cc || echo "/usr/bin/cc already exists"

USER docker
