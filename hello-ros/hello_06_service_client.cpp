// Build & run (needs step 5's server running too -- see README):
//   make build
//   docker exec -it hello-ros-dev bash -lc \
//     "source install/setup.bash && ./install/hello_ros/lib/hello_ros/hello_06_service_client 3 4"
//
// Step 6: standalone client node that calls "/add_two_ints" once with two
// command-line ints and prints the result.

#include <cstdlib>
#include <example_interfaces/srv/add_two_ints.hpp>
#include <rclcpp/rclcpp.hpp>

using AddTwoInts = example_interfaces::srv::AddTwoInts;

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("hello_06_service_client");
  auto client = node->create_client<AddTwoInts>("add_two_ints");

  auto request = std::make_shared<AddTwoInts::Request>();
  request->a = argc > 1 ? std::atoll(argv[1]) : 2;
  request->b = argc > 2 ? std::atoll(argv[2]) : 3;

  while (!client->wait_for_service(std::chrono::seconds(1))) {
    RCLCPP_INFO(node->get_logger(), "waiting for /add_two_ints...");
  }

  auto future = client->async_send_request(request);
  if (rclcpp::spin_until_future_complete(node, future) ==
      rclcpp::FutureReturnCode::SUCCESS) {
    RCLCPP_INFO(node->get_logger(), "%ld + %ld = %ld", request->a, request->b,
                future.get()->sum);
  } else {
    RCLCPP_ERROR(node->get_logger(), "service call failed");
  }

  rclcpp::shutdown();
  return 0;
}
