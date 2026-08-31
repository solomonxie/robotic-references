cmake_minimum_required(VERSION 3.8)
project(hello_ros)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(hello_01_node hello_01_node.cpp)
ament_target_dependencies(hello_01_node rclcpp)

add_executable(hello_02_publisher hello_02_publisher.cpp)
ament_target_dependencies(hello_02_publisher rclcpp std_msgs)

add_executable(hello_03_subscriber hello_03_subscriber.cpp)
ament_target_dependencies(hello_03_subscriber rclcpp std_msgs)

add_executable(hello_04_parameters hello_04_parameters.cpp)
ament_target_dependencies(hello_04_parameters rclcpp std_msgs)

add_executable(hello_05_service_server hello_05_service_server.cpp)
ament_target_dependencies(hello_05_service_server rclcpp example_interfaces)

add_executable(hello_06_service_client hello_06_service_client.cpp)
ament_target_dependencies(hello_06_service_client rclcpp example_interfaces)

add_executable(hello_07_twist_publisher hello_07_twist_publisher.cpp)
ament_target_dependencies(hello_07_twist_publisher rclcpp geometry_msgs)

install(TARGETS
  hello_01_node
  hello_02_publisher
  hello_03_subscriber
  hello_04_parameters
  hello_05_service_server
  hello_06_service_client
  hello_07_twist_publisher
  DESTINATION lib/${PROJECT_NAME})

ament_package()
