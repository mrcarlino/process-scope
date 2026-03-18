# Use an Ubuntu base image
FROM ubuntu:24.04

# Prevent interaction prompts from stalling the build
ENV DEBIAN_FRONTEND=noninteractive

# Install C++ compiler, cmake, and Qt5 dev libraries
RUN apt update && apt install -y \
    build-essential \
    cmake \
    qt6-base-dev \
    libgl1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /usr/src/app

# Copy entire project into the container
COPY . .

# Create a build directory and compile using CMake
RUN mkdir build && cd build && cmake .. && make -j12

# Set command to run
CMD ["./build/ProcessScope"]