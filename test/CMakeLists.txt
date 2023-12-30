find_package(GTest REQUIRED)
include(GoogleTest)
include(CTest)
include(add_ut)

add_subdirectory(api)
add_subdirectory(core)

get_property(ALL_TESTS GLOBAL PROPERTY ALL_TESTS)
add_custom_target(tests
    DEPENDS
    ${ALL_TESTS}
    COMMAND ctest)