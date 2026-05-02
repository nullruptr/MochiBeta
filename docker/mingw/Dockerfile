FROM ubuntu:26.04

# Setting TZ and Japanese Locale
ENV DEBIAN_FRONTEND=noninteractive \
    LANG=ja_JP.UTF-8 \
    LANGUAGE=ja_JP:ja \
    LC_ALL=ja_JP.UTF-8

# locales のインストールと日本語ロケール生成
RUN apt update && \
    apt-get upgrade -y && \
    apt install -y locales && \
    locale-gen ja_JP.UTF-8 && \
    update-locale LANG=ja_JP.UTF-8

# install
RUN apt update &&\
    apt-get upgrade -y &&\
    apt install -y \
    curl \
    wget \
    tar \
    gettext \
    tree \
    make \
    cmake \
    git \
    gcc \
    g++ \
    gcc-mingw-w64 \
    g++-mingw-w64 \
    sqlite3 \
    libsqlite3-dev

# Setup wxWidgets (mingw)
WORKDIR /opt
RUN git clone https://github.com/wxWidgets/wxWidgets.git &&\
    cd wxWidgets &&\
    git checkout v3.2.10 &&\
    git submodule update --init --recursive

# 1. Debug 版のビルド
RUN mkdir -p /opt/wxWidgets/build-debug
WORKDIR /opt/wxWidgets/build-debug
RUN ../configure \
    --prefix=/usr/x86_64-w64-mingw32/debug \
    --host=x86_64-w64-mingw32 \
    --with-msw \
    --disable-shared \
    --enable-unicode \
    --enable-debug \
    && make -j$(nproc) && make install

# 2. Release 版のビルド
RUN mkdir -p /opt/wxWidgets/build-release
WORKDIR /opt/wxWidgets/build-release
RUN ../configure \
    --prefix=/usr/x86_64-w64-mingw32/release \
    --host=x86_64-w64-mingw32 \
    --with-msw \
    --disable-shared \
    --enable-unicode \
    --disable-debug \
    --enable-optimise \
    && make -j$(nproc) && make install

# SQLite3 for MinGW
RUN cd /opt && \
    wget https://www.sqlite.org/2026/sqlite-autoconf-3510200.tar.gz && \
    tar xvf sqlite-autoconf-3510200.tar.gz && \
    cd sqlite-autoconf-3510200 && \
    # クロスコンパイル。-g を含めることでデバッグを可能にしつつ、-O2 で速度も維持
    CFLAGS="-g -O2" ./configure --host=x86_64-w64-mingw32 --prefix=/usr/x86_64-w64-mingw32 --enable-static --disable-shared && \
    make -j$(nproc) && \
    make install

# Setup SOCI (MinGW)
WORKDIR /opt
RUN git clone https://github.com/SOCI/soci.git && \
    cd soci && \
    git checkout v4.1.2

# SOCIもwxWidgetsに合わせて build ディレクトリを分けて管理
RUN mkdir -p /opt/soci/build-mingw
WORKDIR /opt/soci/build-mingw

# CMAKE_FIND_ROOT_PATH を指定して、MinGW側のライブラリを優先的に探させる
RUN cmake .. \
    -DCMAKE_SYSTEM_NAME=Windows \
    -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
    -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
    -DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32 \
    -DCMAKE_FIND_ROOT_PATH=/usr/x86_64-w64-mingw32 \
    -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
    -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
    -DSOCI_TESTS=OFF \
    -DSOCI_CXX11=ON \
    -DSOCI_STATIC=ON \
    -DSOCI_SHARED=OFF \
    -DWITH_SQLITE3=ON \
    -DSQLITE3_INCLUDE_DIR=/usr/x86_64-w64-mingw32/include \
    -DSQLITE3_LIBRARY=/usr/x86_64-w64-mingw32/lib/libsqlite3.a && \
    make -j$(nproc) && \
    make install