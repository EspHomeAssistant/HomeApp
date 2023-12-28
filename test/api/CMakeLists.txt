add_executable(signal-ut SignalTest.cpp)

target_include_directories(signal-ut PRIVATE 
    ${INCLUDE_DIR}
)

target_link_libraries(signal-ut
    PRIVATE
    gtest_main
    gtest
    gmock
)

gtest_discover_tests(signal-ut)

add_custom_target(signal-ut-run
    COMMAND signal-ut
    DEPENDS signal-ut
)


add_subdirectory(mqtt)