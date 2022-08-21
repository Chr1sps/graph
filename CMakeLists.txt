cmake_minimum_required(VERSION 3.16.3)

project(Graph VERSION 0.4)

# compiler flags fuckery
option(USE_LIBCPP "Use clang libc++ instead of gnu libstdc++" FALSE)
option(USE_EXPERIMENTAL "Use experimental clang libc++ features" FALSE)

option(BETTER_FLAGS "Use better c++ compiler flags" FALSE)

if(BETTER_FLAGS)
    add_library(compiler_flags INTERFACE)
    target_compile_features(compiler_flags INTERFACE cxx_std_20)

    set(gcc_like_cxx
        "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
    set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
    target_compile_options(compiler_flags INTERFACE
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-Wpedantic;-Werror>>"
        "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
    )
else()
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    set(GCC_WARNINGS_COMPILE_FLAGS "-Wall -Wextra -Wpedantic -Werror -Wunused")
    set(GCC_UB_COMPILE_FLAGS "-fsanitize=undefined")

    set(
        CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} ${GCC_WARNINGS_COMPILE_FLAGS} ${GCC_UB_COMPILE_FLAGS}"
    )

    if(USE_LIBCPP)
        message("Using clang libc++.")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++ -L/usr/lib/x86-64-linux-gnu/libc++.so")

        if(USE_EXPERIMENTAL)
            message("Using experimental features.")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -DLIBCXX_ENABLE_INCOMPLETE_FEATURES=ON")
        endif()
    else()
        message("Using gnu libstdc++.")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libstdc++")
    endif()
endif()

# main target definitions
configure_file(Graph_config.hpp.in Graph_config.hpp)

add_subdirectory(Graph)
add_subdirectory(tests)

add_executable(main.out main.cpp)
add_executable(graph_tests.out tests/graph_tests.cpp)

target_include_directories(
    main.out
    PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/Graph"
)
target_include_directories(
    graph_tests.out
    PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/Graph"
)

target_link_libraries(main.out PUBLIC Graph)
target_link_libraries(graph_tests.out PUBLIC Graph Catch2::Catch2WithMain)

# testing stuff
Include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG origin/devel)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR/extras})
include(CTest)
include(Catch)
catch_discover_tests(graph_tests.out)

set(CMAKE_CTEST_ARGUMENTS "--extra-verbose")

# installation related stuff
option(BUILD_SHARED_LIBS "Build using shared libraries" FALSE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${Graph_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Graph_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

install(TARGETS graph_tests.out DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/Graph_config.hpp" DESTINATION include)
install(
    EXPORT GraphTargets
    FILE GraphTargets.cmake
    DESTINATION lib/cmake/Graph
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/GraphConfig.cmake"
    INSTALL_DESTINATION "lib/cmake/config"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/GraphConfigVersion.cmake"
    VERSION "${Graph_VERSION_MAJOR}.${Graph_VERSION_MINOR}"
    COMPATIBILITY AnyNewerVersion
)
install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/GraphConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/GraphConfigVersion.cmake
    DESTINATION lib/cmake/Graph
)
export(
    EXPORT GraphTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/GraphTargets.cmake"
)