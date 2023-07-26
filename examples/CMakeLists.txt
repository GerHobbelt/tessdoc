cmake_minimum_required(VERSION 3.0.0)
project(tessdoc VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

### Basic_example
add_executable(Basic_example Basic_example.cc)
target_link_libraries (Basic_example lept tesseract)

# Steps to install leptonica： 
# download http://www.leptonica.org/source/leptonica-x.xx.x.tar.gz
# tar -xzvf leptonica-x.xx.x.tar.gz
# ./configure
# make
# sudo make install

### SetRectangle_example
add_executable(SetRectangle_example SetRectangle_example.cc)
target_link_libraries(SetRectangle_example leptonica lept tesseract)

### GetComponentImages_example
add_executable(GetComponentImages_example GetComponentImages_example.cc)
target_link_libraries(GetComponentImages_example leptonica lept tesseract)

### ResultIterator_example
add_executable(ResultIterator_example ResultIterator_example.cc)
target_link_libraries(ResultIterator_example leptonica lept tesseract)

### OSD_example
add_executable(OSD_example OSD_example.cc)
target_link_libraries(OSD_example leptonica lept tesseract)


### LSTM_Choices_example
add_executable(LSTM_Choices_example LSTM_Choices_example.cc)
target_link_libraries(LSTM_Choices_example leptonica lept tesseract)

### OpenCV_example
# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

if(CMAKE_VERSION VERSION_LESS "2.8.11")
  # Add OpenCV headers location to your include paths
  include_directories(${OpenCV_INCLUDE_DIRS})
endif()

# Declare the executable target built from your sources
add_executable(OpenCV_example OpenCV_example.cc)

# Link your application with OpenCV libraries
target_link_libraries(OpenCV_example PRIVATE ${OpenCV_LIBS} leptonica lept tesseract)

### UserPatterns_example