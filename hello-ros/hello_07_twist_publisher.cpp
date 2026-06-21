// Build & run:
//   make build
//   make run STEP=hello_07_twist_publisher
//
// Step 7: publishes geometry_msgs/Twist on "/cmd_vel" -- the standard ROS 2
// message shape for commanding a mobile robot's velocity (linear.x forward
// speed, angular.z turn rate). This is the message type robo-car's drive
// firmware would consume if it grows a ROS 2 layer later.

#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>

using namespace std::chrono_literals;

class TwistPublisher : public rclcpp::Node {
public:
  TwistPublisher() : Node("hello_07_twist_publisher") {
    publisher_ = create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    timer_ = create_wall_timer(500ms, std::bind(&TwistPublisher::tick, this));
  }

private:
  void tick() {
    auto msg = geometry_msgs::msg::Twist();
    msg.linear.x = 0.2;   // m/s forward
    msg.angular.z = 0.0;  // rad/s turn
    RCLCPP_INFO(get_logger(), "cmd_vel: linear.x=%.2f angular.z=%.2f",
                msg.linear.x, msg.angular.z);
    publisher_->publish(msg);
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TwistPublisher>());
  rclcpp::shutdown();
  return 0;
}
