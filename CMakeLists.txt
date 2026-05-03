cmake_minimum_required(VERSION 3.16.3)
project(Lab3 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -march=native -ffast-math -funroll-loops -flto=auto -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pg -g -O1")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg -g")
# set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg -g")

include_directories(${CMAKE_SOURCE_DIR}/include)

# Fetch googletest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()


add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(demo)

# Documentation Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Génération de la documentation Doxygen"
        VERBATIM
    )
else()
    message(WARNING "Doxygen non trouvé - la cible 'doc' ne sera pas disponible.")
endif()

