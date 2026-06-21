// Build & run:
//   make build
//   make run STEP=hello_05_service_server
//
// Step 5: a service server offering "/add_two_ints" (example_interfaces'
// stock AddTwoInts type -- no custom .srv needed), logging each request.

#include <example_interfaces/srv/add_two_ints.hpp>
#include <rclcpp/rclcpp.hpp>

using AddTwoInts = example_interfaces::srv::AddTwoInts;

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("hello_05_service_server");

  auto service = node->create_service<AddTwoInts>(
      "add_two_ints",
      [node](const std::shared_ptr<AddTwoInts::Request> request,
             std::shared_ptr<AddTwoInts::Response> response) {
        response->sum = request->a + request->b;
        RCLCPP_INFO(node->get_logger(), "%ld + %ld = %ld", request->a,
                    request->b, response->sum);
      });

  RCLCPP_INFO(node->get_logger(), "ready on /add_two_ints");
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
