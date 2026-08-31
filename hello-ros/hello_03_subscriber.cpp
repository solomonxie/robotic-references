// Build & run (needs step 2's publisher running too -- see README):
//   make build
//   make run STEP=hello_03_subscriber
//
// Step 3: standalone node that subscribes to "hello_chatter" and logs
// whatever it receives. Pairs with step 2's publisher, run in another
// terminal, but is its own complete node.

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class HelloSubscriber : public rclcpp::Node {
public:
  HelloSubscriber() : Node("hello_03_subscriber") {
    subscription_ = create_subscription<std_msgs::msg::String>(
        "hello_chatter", 10,
        [this](const std_msgs::msg::String &msg) {
          RCLCPP_INFO(get_logger(), "heard: '%s'", msg.data.c_str());
        });
  }

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<HelloSubscriber>());
  rclcpp::shutdown();
  return 0;
}
