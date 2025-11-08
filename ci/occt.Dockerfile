# Base builder
FROM ubuntu:24.04 AS base

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y\
    build-essential \
    git \
    cmake \
    tcl tcl-dev \
    tk tk-dev \
    libfreeimage-dev \
    libxmu-dev libxi-dev \
    libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/Open-Cascade-SAS/OCCT.git opencascade
WORKDIR /opencascade
RUN git checkout V7_4_0 -b dev-branch
RUN mkdir -p build
WORKDIR /opencascade/build

RUN cmake .. \
    -DCMAKE_BUILD_TYPE=release \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DUSE_FREEIMAGE=OFF \
    -DUSE_FFMPEG=OFF \
    -DUSE_VTK=OFF \
    -DUSE_TBB=OFF
RUN make -j$(nproc)
RUN make install

WORKDIR /app
COPY src/occt/ ./src
COPY test/occt/ ./test

RUN cmake -S src -B build -DCMAKE_PREFIX_PATH=/usr/local && cmake --build build

COPY test/occt/ ./test

# Unit test runner
FROM ubuntu:24.04 AS unit-testing

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    catch2 \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

COPY --from=base /usr/local /usr/local
COPY --from=base /app/build /app/build
COPY --from=base /app/src /app/src
COPY --from=base /app/test /app/test

WORKDIR /app

RUN cmake -S test -B build/tests -DCMAKE_PREFIX_PATH=/usr/local
RUN cmake --build build/tests

CMD ["ctest", "--test-dir", "build/tests", "-V"]
