# CMakeList.txt : CMake project for SAOD_Lab8, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (SAOD_Lab8 "SAOD_Lab8.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SAOD_Lab8 PROPERTY CXX_STANDARD 23)
endif()

# TODO: Add tests and install targets if needed.
