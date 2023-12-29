find_package(GTest REQUIRED)
include(GoogleTest)
include(CTest)
include(add_ut)

add_subdirectory(api)
add_subdirectory(core)