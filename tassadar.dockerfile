FROM ubuntu:18.04
ENV LANG=C.UTF-8
ARG APT_INSTALL="apt-get install -y --no-install-recommends"
ARG GIT_CLONE="git clone --depth 10"
ARG THRIFT_VERSION="0.11.0"
ARG TESSDATA_GIT_SHA="590567f20dc044f6948a8e2c61afc714c360ad0e"

RUN apt-get update && DEBIAN_FRONTEND=noninteractive ${APT_INSTALL} \
        build-essential \
        ca-certificates \
        cmake \
        git \
        automake \
        bison \
        flex \
        g++ \
        libboost-all-dev \
        libevent-dev \
        libssl-dev \
        libtool \
        make \
        curl \
        pkg-config \
        python3-pip \
        tesseract-ocr \
        libtesseract-dev

ENV TESSDATA_PREFIX="/usr/share/tesseract-ocr/4.00/tessdata/"
RUN curl -L -o $TESSDATA_PREFIX/chi_sim.traineddata https://github.com/tesseract-ocr/tessdata/raw/${TESSDATA_GIT_SHA}/chi_sim.traineddata

RUN curl -L -o /root/thrift-${THRIFT_VERSION}.tar.gz http://archive.apache.org/dist/thrift/${THRIFT_VERSION}/thrift-${THRIFT_VERSION}.tar.gz
RUN cd /root/ && \
        tar -xzf thrift-${THRIFT_VERSION}.tar.gz && cd thrift-${THRIFT_VERSION} && \
        ./bootstrap.sh && \
        ./configure --without-qt4 --without-qt5 --without-csharp --without-erlang --without-nodejs --without-lua --without-perl --without-php --without-php_extention --without-dart --without-ruby --without-haskell --without-rs --without-haxe --without-dotnetcore --without-d && \
        make -j2 && make install

ARG TASSADAR_VERSION="v0.2.1"
RUN ${GIT_CLONE} --branch ${TASSADAR_VERSION} https://github.com/shenfei/tassadar_ocr.git /root/tassadar_ocr && \
        cd /root/tassadar_ocr && make
RUN pip3 --no-cache-dir install setuptools
RUN pip3 --no-cache-dir install -e /root/tassadar_ocr/python/

RUN rm -rf /root/thrift-${THRIFT_VERSION}*
RUN ldconfig && \
        apt-get clean && \
        apt-get autoremove && \
        rm -rf /var/lib/apt/lists/* /tmp/*

EXPOSE 9090
