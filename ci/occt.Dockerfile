# Builds Open CASCADE Technology (OCCT) v7.9.2
# Licensed under LGPL-2.1-only with OCCT Exception v1.0
# Source: https://github.com/Open-Cascade-SAS/OCCT/tree/c5f20409c52bf8f658314d205a0e5d6f0be0969c

# Base builder
FROM ubuntu:24.04 AS base

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential git cmake \
    tcl tcl-dev tk tk-dev \
    libfreeimage-dev \
    libxmu-dev libxi-dev \
    libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/Open-Cascade-SAS/OCCT.git opencascade
WORKDIR /opencascade
RUN rm -rf /opencascade/build && mkdir build
RUN git checkout c5f20409c52bf8f658314d205a0e5d6f0be0969c #Tag V7_9_2
RUN mkdir -p build
WORKDIR /opencascade/build

RUN cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DBUILD_MODULE_FoundationClasses=ON \
    -DBUILD_MODULE_ModelingData=ON \
    -DBUILD_MODULE_ModelingAlgorithms=ON \
    -DBUILD_MODULE_TKernel=ON \
    -DBUILD_MODULE_TKMath=ON \
    -DBUILD_MODULE_TKBRep=ON \
    -DBUILD_MODULE_TKGeomBase=ON \
    -DBUILD_MODULE_TKPrim=ON \
    -DBUILD_MODULE_TKBO=ON \
    -DBUILD_MODULE_TKG2d=ON \
    -DBUILD_MODULE_TKG3d=ON \
    -DBUILD_MODULE_TKShHealing=ON \
    -DBUILD_MODULE_TKTopAlgo=ON \
    -DBUILD_MODULE_TKBool=ON \
    -DBUILD_MODULE_TKFillet=ON \
    -DBUILD_MODULE_TKOffset=ON \
    -DBUILD_MODULE_TKFeat=ON \
    -DBUILD_MODULE_TKMesh=ON \
    -DBUILD_MODULE_TKHLR=ON \
    -DBUILD_MODULE_TKCDF=ON \
    -DBUILD_MODULE_TKLCAF=OFF \
    -DBUILD_MODULE_TKCAF=OFF \
    -DBUILD_MODULE_TKXCAF=OFF \
    -DBUILD_MODULE_TKXDEDRAW=OFF \
    -DBUILD_MODULE_TKDRAW=OFF \
    -DBUILD_MODULE_TKService=OFF \
    -DBUILD_MODULE_TKV3d=OFF \
    -DBUILD_MODULE_TKOpenGl=OFF \
    -DBUILD_MODULE_TKIVtk=OFF \
    -DBUILD_MODULE_TKIVtkDraw=OFF \
    -DBUILD_MODULE_TKViewerTest=OFF \
    -DBUILD_MODULE_TKD3DHost=OFF \
    -DBUILD_MODULE_TKVRML=OFF \
    -DBUILD_MODULE_TKXSDRAW=OFF \
    -DBUILD_MODULE_TKTObj=OFF \
    -DBUILD_MODULE_TKTObjDRAW=OFF \
    -DBUILD_MODULE_TKStd=OFF \
    -DBUILD_MODULE_TKStdL=OFF \
    -DBUILD_MODULE_TKShapeSchema=OFF \
    -DBUILD_MODULE_TKXml=OFF \
    -DBUILD_MODULE_TKXmlL=OFF \
    -DBUILD_MODULE_TKBin=OFF \
    -DBUILD_MODULE_TKBinL=OFF \
    -DBUILD_MODULE_TKOCAF=OFF \
    -DBUILD_MODULE_TKXDECAF=OFF \
    -DBUILD_MODULE_TKQADraw=OFF \
    -DBUILD_MODULE_TKTopTest=OFF \
    -DBUILD_MODULE_TKGeomBaseTest=OFF \
    -DUSE_FREEIMAGE=OFF \
    -DUSE_FREETYPE=OFF \
    -DUSE_FFMPEG=OFF \
    -DUSE_TBB=OFF \
    -DUSE_VTK=OFF \
    -DUSE_OPENGL=OFF
RUN make -j$(nproc)
RUN make install

WORKDIR /app
COPY src/occt/ ./src
RUN cmake -S src -B build -DCMAKE_PREFIX_PATH=/usr/local
RUN cmake --build build

COPY test/occt/ ./test

# Unit test runner stage
FROM ubuntu:24.04 AS unit-testing

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential cmake git \
    catch2 \
    libfreetype-dev \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

COPY --from=base /usr/local /usr/local
COPY --from=base /app/build /app/build
COPY --from=base /app/src /app/src
COPY --from=base /app/test /app/test
COPY --from=base /usr/local/include /usr/local/include
COPY --from=base /usr/local/lib /usr/local/lib

WORKDIR /app

ENV LD_LIBRARY_PATH=/usr/local/lib

RUN rm -rf build/unit-tests/*
RUN cmake -S test -B build/unit-tests -DCMAKE_PREFIX_PATH=/usr/local -DCMAKE_CXX_FLAGS="-I/usr/local/include/opencascade"
RUN cmake --build build/unit-tests
