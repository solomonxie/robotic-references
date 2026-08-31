// Build & run:
//   make build
//   make run STEP=hello_01_node
//
// Step 1: the smallest possible ROS 2 node -- init, create a Node, log once,
// shut down. No publishers/subscribers/timers yet.

#include <rclcpp/rclcpp.hpp>

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("hello_01_node");
  RCLCPP_INFO(node->get_logger(), "Hello, ROS 2!");
  rclcpp::shutdown();
  return 0;
}
