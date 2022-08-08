# ROS Integration for TSA-Driven Soft Gripper

## Acknowledgment
This repository was created for the Smart Robotics Lab at the University of Nevada, Reno.
It is the companion repository to [ROS_ur3ehand](https://github.com/steven-swanbeck/ROS_ur3ehand).
---

## Purpose
PlatformIO Arduino code to allow the University of Nevada, Reno Smart Robotics Lab's TSA-Driven Soft Robotic Hand to send and receive ROS messages for external control. 

The experimental branch shows efforts to parse long-form messages to control individual motors, their setpoints, and trigger macro commands. 

The pickplacePoC branch shows simplified, non-complex messaging in which messages consist of a single character sent via ROS to trigger exclusively high-level commands for purpose of a basic pick-and-place demonstration using the hand mounted to a Universal Robots UR3e manipulator.

<img alt="Soft Gripper" src="/include/SoftGripper.jpg">

## Dependencies
Use of this code requires installation of the ROSserial packages, which can be installed with:
```console
sudo apt install ros-<distro>-rosserial ros-<distro>-rosserial-arduino ros-<distro>-rosserial-python
```
and the rosserial client library for Arduino, which can be installed to the target directory with:
```console
git clone https://github.com/ros-drivers/rosserial.git
```
**Note: The roslib Arduino library is already included in the 'lib' folder in this repo**
***
## General Use
Once configured, a rosserial Python node can be launched using:
```console
rosrun rosserial_python serial_node.py <port_name>
```
In general, this will be:
```console
rosrun rosserial_python serial_node.py /dev/ttyACM0
```
Messages can be sent to the Arduino via ROS in the form:
```console
rostopic pub arduino std_msgs/String <command>
```
Callback messages from the Arduino can be seen using:
```console
rostopic echo /chatter
```
---
## Integration with UR3e
For full details of the integration of the gripper with the UR3e manipulator, view this repository's [companion repository](https://github.com/steven-swanbeck/ROS_ur3ehand).
---
## Troubleshooting
If ROS informs the user that permission to access a port is denied, try running:
```console
sudo chmod a+rw <port_name>
```
ex:
```console
sudo chmod a+rw /dev/ttyACM0
```
