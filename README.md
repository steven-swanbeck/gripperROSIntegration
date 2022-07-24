PlatformIO Arduino code to allow the University of Nevada, Reno Smart Robotics Lab's TSA-Driven Soft Robotic Hand to send and receive ROS messages for external control. 

The experimental branch shows efforts to parse long-form messages to control inidivual motors, their setpoints, and trigger macro commands. 

The pickplacePoC branch shows simplified, non-complex messaging in which messages consist of a single character sent via ROS to trigger exclusively high-level commands for purpose of a basic pick-and-place demonstration using the hand mounted to a Universal Robots UR3e manipulator.

Messages can be sent to the Arduino via ROS in the form:
```console
rostopic pub arduino std_msgs/String <command>
```
Callback messages from the Arduino can be seen using:
```console
rostopic echo /chatter
```
Use of this code requires installation of the ROSserial packages, which can be installed with:
```console
sudo apt install ros-<distro>-rosserial ros-<distro>-rosserial-arduino ros-<distro>-rosserial-python
```
and the rosserial client library for Arduino, which can be installed with:
```console
git clone https://github.com/ros-drivers/rosserial.git
```
