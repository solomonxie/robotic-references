// Build & run:
//   make build
//   make run STEP=hello_04_parameters
//   # override the rate: docker exec -it hello-ros-dev bash -lc \
//   #   "source install/setup.bash && ./install/hello_ros/lib/hello_ros/hello_04_parameters --ros-args -p publish_rate_hz:=5.0"
//
// Step 4: full copy of step 2's publisher, plus a declared parameter
// "publish_rate_hz" (default 1.0) that sets the timer period at startup.

#include <chrono>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

class HelloParamPublisher : public rclcpp::Node {
public:
  HelloParamPublisher() : Node("hello_04_parameters"), count_(0) {
    declare_parameter<double>("publish_rate_hz", 1.0);
    double rate_hz = get_parameter("publish_rate_hz").as_double();
    auto period = std::chrono::duration<double>(1.0 / rate_hz);

    publisher_ = create_publisher<std_msgs::msg::String>("hello_chatter", 10);
    timer_ = create_wall_timer(
        std::chrono::duration_cast<std::chrono::nanoseconds>(period),
        std::bind(&HelloParamPublisher::tick, this));

    RCLCPP_INFO(get_logger(), "publish_rate_hz = %.2f", rate_hz);
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
  rclcpp::spin(std::make_shared<HelloParamPublisher>());
  rclcpp::shutdown();
  return 0;
}
