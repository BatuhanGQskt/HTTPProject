ARG VERSION=latest

FROM ubuntu:$VERSION

# Install wget first
RUN apt-get update && apt-get install -y wget

# Install build dependencies first
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    libssl-dev \
    libtcmalloc-minimal4 \
    libboost-all-dev \
    libboost-dev

# install cmake
ENV CMAKE_VERSION=3.28.1

WORKDIR /root
RUN mkdir temp
WORKDIR /root/temp
RUN wget -c https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}.tar.gz
RUN tar -xzvf cmake-${CMAKE_VERSION}.tar.gz

WORKDIR /root/temp/cmake-${CMAKE_VERSION}
RUN ./bootstrap -- -DCMAKE_BUILD_TYPE:STRING=Release
RUN make -j4
RUN make install

WORKDIR /root
RUN rm -rf temp

WORKDIR /app
COPY . /app/

CMD ["/bin/bash"]