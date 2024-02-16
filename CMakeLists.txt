cmake_minimum_required(VERSION 3.5.0)
project(main VERSION 0.1.0 LANGUAGES C CXX)

# SQLiteC++ 3.x requires C++11 compiler
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/thirdparty/SQLiteCpp)

add_library(Factory
	src/Factory.cpp 
	src/Building.cpp
)

target_link_libraries(Factory
	SQLiteCpp
)

target_include_directories(Factory
	PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/thirdparty"
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_executable(main src/main.cpp)
target_link_libraries(main
  Factory
  SQLiteCpp
  sqlite3
  )

target_include_directories(main
  PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/lib/thirdparty"
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)