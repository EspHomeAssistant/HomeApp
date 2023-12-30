FROM patkaczm/esphomeassistant:0.0.3

# Set the working directory
WORKDIR /app

COPY . .

RUN cmake -B build && \
    cmake --build build --target tests -- -j4

# CMD "/app/build/src/example-main"
CMD ["ctest", "--test-dir", "build"]
