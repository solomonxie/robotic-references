// Build & run:
//   make build
//   make run STEP=hello_02_publisher
//
// Step 2: full copy of step 1's node, plus a publisher on topic
// "hello_chatter" and a 1Hz timer that publishes an incrementing message.
// Keeps running (rclcpp::spin) instead of exiting immediately.

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

using namespace std::chrono_literals;

class HelloPublisher : public rclcpp::Node {
public:
  HelloPublisher() : Node("hello_02_publisher"), count_(0) {
    publisher_ = create_publisher<std_msgs::msg::String>("hello_chatter", 10);
    timer_ = create_wall_timer(1s, std::bind(&HelloPublisher::tick, this));
  }

private:
  void tick() {
    auto msg = std_msgs::msg::String();
    msg.data = "Hello, ROS 2! #" + std::to_string(count_++);
    RCLCPP_INFO(get_logger(), "publishing: '%s'", msg.data.c_str());
    publisher_->publish(msg);
  }

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t count_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<HelloPublisher>());
  rclcpp::shutdown();
  return 0;
}
