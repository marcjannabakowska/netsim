cmake_minimum_required(VERSION 3.17)
project(net_sim)

set(CMAKE_CXX_STANDARD 14)
include_directories(
        include
)

add_executable(net_sim main.cpp src/package.cpp
        include/factory.hpp include/reports.hpp include/simulation.hpp
        include/helpers.hpp include/nodes.hpp include/package.hpp
        include/storage_types.hpp include/types.hpp src/storage_types.cpp
        src/nodes.cpp src/helpers.cpp src/factory.cpp
        src/reports.cpp src/simulation.cpp)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)


# Ustaw zmienną `SOURCES_FILES_TESTS`, która będzie przechowywać ścieżki do
# plików źródłowych z testami jednostkowymi.
set(SOURCES_FILES_TESTS
        netsim_tests/test/test_package.cpp
        )

# Dodaj konfigurację typu `Test`.
add_executable(netsim_tests ${SOURCE_FILES} ${SOURCES_FILES_TESTS} netsim_tests/test/main_gtest.cpp)

# Dodaj katalogi z plikami nagłówkowymi wyłącznie do konkretnej konfiguracji (tu: `Test`).
target_include_directories(netsim_tests PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
        mocks
        )


# Podlinkuj bibliotekę o identyfikatorze `gmock` (w pliku CMake) wyłącznie do konkretnej
# konfiguracji (tu: `Test`).
target_link_libraries(netsim_tests gmock)
# Dodaj podkatalog `googletest-master`.
add_subdirectory(googletest-master)

target_compile_definitions(net_sim PUBLIC EXERCISE_ID=EXERCISE_ID_PACKAGES)