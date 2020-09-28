cmake_minimum_required(VERSION 3.15)
project(cprApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3 -std=c++17")

message("Pistache LIB -> " ${PISTACHE_LIB})
message("Pistache INC -> " ${PISTACHE_INC})

aux_source_directory(${CMAKE_CURRENT_BINARY_DIR}/../src ccmControlSrcs)
message(INFO " sources: " ${ccmControlSrcs})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../include)

add_library(ccmControl SHARED ${ccmControlSrcs})
target_include_directories(ccmControl PUBLIC ${PISTACHE_INC})
target_link_libraries( ccmControl 
   ${PISTACHE_LIB}/libpistache-0.0.002-git20200802.so
)

add_executable (ccmApp ${CMAKE_CURRENT_BINARY_DIR}/../test/ccm_app.cpp )
target_include_directories( ccmApp PUBLIC ${PISTACHE_INC})
target_link_libraries(ccmApp 
  ccmControl
  pthread
)

add_executable (ccmPerf ${CMAKE_CURRENT_BINARY_DIR}/../test/ccm_perf.cpp ) 
target_include_directories( ccmPerf PUBLIC ${PISTACHE_INC})
target_link_libraries(ccmPerf
  ccmControl
  pthread
)
