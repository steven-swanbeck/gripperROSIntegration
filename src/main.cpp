#include <Arduino.h>
#include <stdlib.h> 
#include <ros.h>
#include <std_msgs/String.h>


ros::NodeHandle arduinoNode;

char command {};

void turnOnLight();
void turnOffLight();

void remoteControl_cb(const std_msgs::String& cmd_msg) { 
    command = cmd_msg.data[0];
}

void turnOnLight() {
    digitalWrite(13, HIGH);
}

void turnOffLight() {
    digitalWrite(13, LOW);
}

const char on_msg[7] = "led on";
const char off_msg[8] = "led off";

ros::Subscriber<std_msgs::String> sub("arduino", remoteControl_cb);

std_msgs::String return_msg;
ros::Publisher chatter("chatter", &return_msg);

// sub.getNumPublishers() const;
// chatter.getNumSubscribers() const;

void remoteControl () {
    if (command == 'q') {
        turnOnLight();
        return_msg.data = on_msg;
        chatter.publish(&return_msg);
    }
    else if (command == 'a') {
        turnOffLight();
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    
}

void setup() {
    pinMode(13, OUTPUT);
    arduinoNode.initNode();
    arduinoNode.advertise(chatter);
    arduinoNode.subscribe(sub);
}

void loop() {
    arduinoNode.spinOnce();
    remoteControl();
    delay(1);
}