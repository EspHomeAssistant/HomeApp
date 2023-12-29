include(CMakeParseArguments)

function(add_lib)
set(options)
set(oneValueArgs NAME)
set(multiValueArgs SOURCES DEPENDS)
cmake_parse_arguments(ADD_LIB "${options}" "${oneValueArgs}"
                             "${multiValueArgs}" ${ARGN} )

add_library(${ADD_LIB_NAME} ${ADD_LIB_SOURCES})

target_link_libraries(${ADD_LIB_NAME} PRIVATE
    ${ADD_LIB_DEPENDS}
    )

target_include_directories(${ADD_LIB_NAME} PRIVATE ${INCLUDE_DIR})

endfunction()