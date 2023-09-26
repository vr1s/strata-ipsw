cmake_minimum_required(VERSION 3.24)
project(strata_ipsw)

set(CMAKE_CXX_STANDARD 17)

add_executable(strata_ipsw main.cpp ipsw.hpp)

execute_process(COMMAND rm ${CMAKE_CURRENT_BINARY_DIR}/libipsw.dylib)
execute_process(COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/libipsw.dylib)

add_custom_target(
        build_libipsw
        COMMAND ${CMAKE_COMMAND} -E echo "Building 'libipsw'..."
        COMMAND make -C ${CMAKE_SOURCE_DIR}/libipsw build-c
        COMMENT "Building 'libipsw' library..."
        COMMAND cp ${CMAKE_SOURCE_DIR}/libipsw/ipsw.dylib ${CMAKE_CURRENT_BINARY_DIR}/ipsw.dylib
)
add_dependencies(strata_ipsw build_libipsw)

target_link_libraries(strata_ipsw PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/ipsw.dylib)