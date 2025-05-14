FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies for OpenCV and Java
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    unzip \
    pkg-config \
    libgtk-3-dev \
    libavcodec-dev \
    libavformat-dev \
    libswscale-dev \
    libv4l-dev \
    libxvidcore-dev \
    libx264-dev \
    libjpeg-dev \
    libpng-dev \
    libtiff-dev \
    gfortran \
    openexr \
    libatlas-base-dev \
    python3-dev \
    openjdk-17-jdk \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Get OpenCV source
WORKDIR /opt
RUN git clone https://github.com/opencv/opencv.git

# Build OpenCV (without contrib for faster build)
WORKDIR /opt/opencv/build
RUN cmake -D CMAKE_BUILD_TYPE=Release \
          -D CMAKE_INSTALL_PREFIX=/usr/local \
          .. && \
    make -j"$(nproc)" && \
    make install && \
    ldconfig

# Set working directory
WORKDIR /app

# Copy the JAR file into the container
COPY target/navigation-0.0.1-SNAPSHOT.jar /app/

# Run the Java application
CMD ["java", "-jar", "navigation-0.0.1-SNAPSHOT.jar"]
