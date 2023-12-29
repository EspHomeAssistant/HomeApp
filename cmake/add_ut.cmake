include(CMakeParseArguments)

function(add_ut)
set(options)
set(oneValueArgs NAME)
set(multiValueArgs SOURCES DEPENDS)
cmake_parse_arguments(ADD_UT "${options}" "${oneValueArgs}"
                             "${multiValueArgs}" ${ARGN} )

add_executable(${ADD_UT_NAME}-ut ${ADD_UT_SOURCES})

target_link_libraries(${ADD_UT_NAME}-ut PRIVATE
    Threads::Threads
    gtest_main
    gtest
    gmock
    ${ADD_UT_NAME}
    ${ADD_UT_DEPENDS}
    )

target_include_directories(${ADD_UT_NAME}-ut PRIVATE ${INCLUDE_DIR} ${TEST_INCLUDE_DIR})

gtest_discover_tests(${ADD_UT_NAME}-ut)

add_custom_target(${ADD_UT_NAME}-ut-run
    COMMAND ${ADD_UT_NAME}-ut
    DEPENDS ${ADD_UT_NAME}-ut
)
endfunction()