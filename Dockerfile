# Use an official Ubuntu image as the base
FROM patkaczm/esphomeassistant:0.0.2

# Set the working directory
WORKDIR /app

COPY . .

RUN cmake -B build && \
    cmake --build build --target example-main

# CMD "/app/build/src/example-main"
CMD ["ctest", "--test-dir", "build"]
