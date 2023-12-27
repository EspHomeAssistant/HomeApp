# Use an official Ubuntu image as the base
FROM patkaczm/esphomeassistant:0.0.2

# Set the working directory
WORKDIR /app

COPY . .

RUN apt update && \
    apt install -y \
    software-properties-common &&\
    add-apt-repository ppa:mosquitto-dev/mosquitto-ppa &&\
    apt update && \
    apt install -y \
    libspdlog-dev \
    mosquitto \
    libmosquitto-dev \
    pkg-config && \
    rm -rf /var/lib/apt/lists/*

RUN cmake -B build && \
    cmake --build build --target example-main

# CMD "/app/build/src/example-main"
CMD ["ctest", "--test-dir", "build"]
