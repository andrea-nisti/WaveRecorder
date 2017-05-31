cmake_minimum_required(VERSION 2.6)
project (WaveRecorder CXX)
set (CMAKE_CXX_STANDARD 11)

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (LIB_DIR ./build)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PROJECT_SRCS include/utils/TimeHelpers.hpp include/utils/SimpleLog.hpp)
set(QT_LIB Qt5::Widgets Qt5::Core Qt5::Network)

include_directories("include")
include_directories("/home/andreanistico/Qt/5.8/gcc_64/include")
include_directories(${LIB_DIR}/landingPlat/include)

#Set Qt
# Find the QtWidgets library
set(CMAKE_PREFIX_PATH /home/andreanistico/Qt/5.8/gcc_64/lib/cmake)
find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)
MESSAGE(STATUS ${CMAKE_PREFIX_PATH})

#Set LCM
set ( PROJECT_LINK_LIBS liblcm.so)
add_library(wavegen include/WaveGen.h lib/WaveGen.cpp)
link_directories( ~/usr/local/lib )

add_executable(wave ${PROJECT_SOURCE_DIR}/main.cpp   ${PROJECT_SRCS})
add_executable(read ${PROJECT_SOURCE_DIR}/reader.cpp ${PROJECT_SRCS} include/utils/QtTcpSender.hpp)
#add_executable(tcp src/tcp.cpp ${PROJECT_SRCS})

target_link_libraries(wave ${PROJECT_LINK_LIBS} wavegen)
target_link_libraries(read ${PROJECT_LINK_LIBS} ${QT_LIB})
#target_link_libraries(tcp ${PROJECT_LINK_LIBS} ${QT_LIB})



