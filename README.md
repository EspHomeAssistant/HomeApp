# Dependencies
- googletest
- libgtest-dev
- google-mock
- libgmock-dev
- nlohmann-json
- oatpp
- oatpp-swagger
- mongo-cxx-driver
- mosquitto
- spdlog

```bash
$ add-apt-repository ppa:mosquitto-dev/mosquitto-ppa
$ sudo apt install -y \
     googletest \
     libgtest-dev \
     google-mock \
     libgmock-dev \
     libspdlog-dev \
     libspdlog-dev \
     mosquitto \
     libmosquitto-dev \
     pkg-config

# oat++ lib:
$ git clone https://github.com/oatpp/oatpp.git
$ cd oatpp
$ cmake -Bbuild
# cmake --build --target install

# oat++-swagger lib: 
$ git clone https://github.com/oatpp/oatpp-swagger
$ cd oatpp-swagger
$ cmake -Bbuild
# cmake --build --target install

# download json:
$ git clone https://github.com/nlohmann/json.git
$ cd json
$ cmake -Bbuild
# cmake --build --target install

# MongoDB driver:
$ curl -OL https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.9.0/mongo-cxx-driver-r3.9.0.tar.gz
$ tar -xzf mongo-cxx-driver-r3.9.0.tar.gz
$ cd mongo-cxx-driver-r3.9.0
$ cmake -Bbuild                           \
     -DCMAKE_BUILD_TYPE=Release           \
     -DMONGOCXX_OVERRIDE_DEFAULT_INSTALL_PREFIX=OFF
# cmake --build build --target install
```
