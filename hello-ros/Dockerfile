FROM ros:jazzy-ros-base

SHELL ["/bin/bash", "-c"]

WORKDIR /ws/src/hello_ros
COPY package.xml CMakeLists.txt ./
COPY *.cpp ./

WORKDIR /ws
RUN source /opt/ros/jazzy/setup.bash && colcon build

RUN echo "source /opt/ros/jazzy/setup.bash" >> /root/.bashrc && \
    echo "source /ws/install/setup.bash" >> /root/.bashrc

CMD ["bash"]
