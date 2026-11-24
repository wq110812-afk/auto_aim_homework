cmake_minimum_required(VERSION 3.16)

project(auto_aim_homework)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)

find_package(OpenCV REQUIRED)
find_package(fmt REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

add_library(auto_aim OBJECT
    tasks/auto_aim/armor.cpp
    tasks/auto_aim/classifier.cpp
    tasks/auto_aim/detector.cpp
    tasks/auto_aim/solver.cpp
)
target_link_libraries(auto_aim PUBLIC ${OpenCV_LIBS} fmt::fmt yaml-cpp spdlog::spdlog)

add_library(tools OBJECT
    tools/exiter.cpp
    tools/img_tools.cpp
    tools/logger.cpp
    tools/plotter.cpp
)
target_link_libraries(tools PUBLIC ${OpenCV_LIBS} fmt::fmt yaml-cpp spdlog::spdlog nlohmann_json::nlohmann_json)

add_subdirectory(io)

# TODO(作业): 在 src/main.cpp 中实现主程序后，加回 standard 可执行目标，例如：
# add_executable(standard src/main.cpp)
# target_link_libraries(standard PRIVATE ${OpenCV_LIBS} fmt::fmt yaml-cpp spdlog::spdlog auto_aim tools io)

add_executable(smoke_test tests/smoke_test.cpp)
target_link_libraries(smoke_test PRIVATE ${OpenCV_LIBS} fmt::fmt yaml-cpp spdlog::spdlog auto_aim tools)
