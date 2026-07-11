FROM debian:trixie

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
 && apt-get dist-upgrade -y \
 && apt-get install -y locales sudo

RUN echo 'en_US.UTF-8 UTF-8' >> /etc/locale.gen \
 && locale-gen en_US.UTF-8 \
 && update-locale LANG=en_US.UTF-8 LANGUAGE=en_US:en
ENV LANG=en_US.UTF-8 \
    LANGUAGE=en_US:en \
    LC_ALL=en_US.UTF-8

RUN useradd docker -U -G sudo -m -s /bin/bash \
 && echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

RUN sed -i 's/\(Suites: trixie trixie-updates\)/\1 trixie-backports/' /etc/apt/sources.list.d/debian.sources

RUN apt-get update \
 && apt-get install -y \
    curl bash bc gcc sed patch patchutils tar bzip2 gzip xz-utils zstd perl gawk gperf zip \
      unzip diffutils lzop make file g++ xfonts-utils xsltproc default-jre-headless python3 \
      libc6-dev libncurses5-dev libjson-perl libxml-parser-perl libparse-yapp-perl rdfind \
      golang-1.24-go git openssh-client rsync nodejs npm \
    --no-install-recommends \
    && ln -s /usr/lib/go-1.24 /usr/lib/go \
    && ln -s /usr/lib/go-1.24/bin/go /usr/bin/go \
    && ln -s /usr/lib/go-1.24/bin/gofmt /usr/bin/gofmt

RUN rm -rf /var/lib/apt/lists/*

USER docker
