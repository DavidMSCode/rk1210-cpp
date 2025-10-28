cmake_minimum_required(VERSION 3.10)
project(rk1210)

set(CMAKE_CXX_STANDARD 11)

# Add source files
add_library(rk1210
    src/rk1210.cpp
    # Add other source files here if needed
)

# Expose headers to consumers
# Use generator expressions so the correct path is used for build vs install
target_include_directories(rk1210
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

#ADD TEST
add_executable(rk1210_test src/test.cpp)
target_include_directories(rk1210_test PRIVATE include)
target_link_libraries(rk1210_test rk1210)

# Optional install rules to make headers/libraries available when installing
include(GNUInstallDirs)
install(TARGETS rk1210
    EXPORT rk1210Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT rk1210Targets
    NAMESPACE rk1210::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/rk1210
)