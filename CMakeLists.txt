#cmake_minimum_required(VERSION 3.17)
#project(net_sim)
#
#set(CMAKE_CXX_STANDARD 14)
#include_directories(
#        include
#)
#
#add_executable(net_sim main.cpp src/package.cpp
#        include/factory.hpp include/reports.hpp include/simulation.hpp
#        include/helpers.hpp include/nodes.hpp include/package.hpp
#        include/storage_types.hpp include/types.hpp src/storage_types.cpp
#        src/nodes.cpp src/helpers.cpp src/factory.cpp
#        src/reports.cpp src/simulation.cpp)
#
#add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)
#
#add_subdirectory(googletest-master)
#
## Ustaw zmienną `SOURCES_FILES_TESTS`, która będzie przechowywać ścieżki do
## plików źródłowych z testami jednostkowymi.
#set(SOURCES_FILES_TESTS
#        netsim_tests/test/test_package.cpp
#        )
#
## Dodaj konfigurację typu `Test`.
#add_executable(netsim_tests ${SOURCE_FILES} ${SOURCES_FILES_TESTS} netsim_tests/test/main_gtest.cpp)
#target_compile_definitions(netsim_tests PUBLIC EXERCISE_ID=EXERCISE_ID_PACKAGES)
## Dodaj katalogi z plikami nagłówkowymi wyłącznie do konkretnej konfiguracji (tu: `Test`).
#
#target_include_directories(netsim_tests PUBLIC
#        googletest-master/googlemock/include
#        googletest-master/googletest/include
#        mocks
#        )
#
## Podlinkuj bibliotekę o identyfikatorze `gmock` (w pliku CMake) wyłącznie do konkretnej
## konfiguracji (tu: `Test`).
#target_link_libraries(netsim_tests gmock)
## Dodaj podkatalog `googletest-master`.
#
#
cmake_minimum_required(VERSION 3.17)
project(net_sim)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors -Wconversion)

include_directories(
        include
)

set(SOURCE_FILES
        src/factory.cpp
        src/helpers.cpp
        src/nodes.cpp
        src/package.cpp
        src/reports.cpp
        src/simulation.cpp
        src/storage_types.cpp
        )


add_executable(net_sim ${SOURCE_FILES} main.cpp)

set(SOURCE_FILES_TESTS
#        netsim_tests/test/test_Factory.cpp
#        netsim_tests/test/test_factory_io.cpp
       netsim_tests/test/test_nodes.cpp
       netsim_tests/test/test_package.cpp
#        netsim_tests/test/test_reports.cpp
#        netsim_tests/test/test_simulate.cpp
#        netsim_tests/test/test_storage_types.cpp
        )

add_executable(netsim_tests ${SOURCE_FILES} ${SOURCE_FILES_TESTS} netsim_tests/test/main_gtest.cpp)

target_include_directories(netsim_tests
         PUBLIC
        googletest-master/googlemock/include
        googletest-master/googletest/include
        mocks
        netsim_tests/mocks

        )

add_subdirectory(googletest-master)

target_link_libraries(netsim_tests gmock)

#target_compile_definitions(netsim_test PUBLIC EXERCISE_ID=1)

