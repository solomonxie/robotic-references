# hello-ros

Progressive learning series for ROS 2 (`rclcpp`, C++). Each step is a standalone
node file that builds on the previous one, adding exactly one new concept. Unlike
the other `hello-*` folders, all steps share one colcon package (`package.xml` +
`CMakeLists.txt`) because that's the minimum a ROS 2 C++ node needs to build --
each step is still its own self-contained `.cpp` file and its own executable.

Runs in Docker (ROS 2 Jazzy) rather than on any board here: macOS has no native
ROS 2 support, and the Pi Zero W's ARMv6 CPU predates ROS 2's supported
architectures.

## Running

Single-node steps build and run in one shot:

```sh
make build
make run STEP=hello_01_node
make run STEP=hello_07_twist_publisher
```

Steps that need two nodes talking to each other (2+3, 5+6) run inside one
shared container so they see each other on localhost -- start it once, then
exec into it per node from separate terminals:

```sh
make shell                              # terminal 1: leaves you in the container
make exec STEP=hello_02_publisher       # terminal 2
make exec STEP=hello_03_subscriber      # terminal 3
make exec STEP=hello_06_service_client ARGS="3 4"   # after hello_05_service_server
make stop                               # tear down when done
```

## Phase plan

1. Node basics -- init, log, shutdown
2. Publisher -- topic + timer
3. Subscriber -- pairs with step 2
4. Parameters -- runtime-configurable publish rate
5. Service server -- `/add_two_ints`
6. Service client -- pairs with step 5
7. `geometry_msgs/Twist` publisher on `/cmd_vel` -- the message shape
   [robo-car](../robo-car)'s drive firmware would consume if it grows a
   ROS 2 layer later
