add_executable(registration-ut RegistrationTest.cpp)

target_include_directories(registration-ut PRIVATE 
    ${INCLUDE_DIR}
)

target_link_libraries(registration-ut
    PRIVATE
    gtest_main
    gtest
    gmock
    registration
)

gtest_discover_tests(registration-ut)

add_custom_target(registration-ut-run
    COMMAND registration-ut
    DEPENDS registration-ut
)