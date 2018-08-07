cmake_minimum_required(VERSION 2.8.3)
project(cam_calibration)
set (CMAKE_CXX_STANDARD 11)

ADD_LIBRARY(calibration_module src/calibration.cpp )


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  cv_bridge
  positioning_parameter_msgs
  apriltags_ros
)
find_package(OpenCV)

catkin_package(

)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)


include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(opencv_testcam src/opencv_testcam.cpp)
add_dependencies(opencv_testcam ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(opencv_testcam calibration_module ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})



add_executable(catch_frame src/catch_frame.cpp)
add_dependencies(catch_frame ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(catch_frame ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(calibration_main src/calibration_main.cpp)
add_dependencies(calibration_main ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(calibration_main calibration_module ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(opencv_cam src/opencv_cam.cpp)
add_dependencies(opencv_cam ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(opencv_cam ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

install(DIRECTORY include/
  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
)
