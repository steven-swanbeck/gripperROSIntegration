// Arduino dependencies
#include <Arduino.h>
#include <Wire.h>

#define USE_USBCON // *** MUST BE UN-COMMENTED FOR USE WITH ARDUINO DUE***

// ROS dependencies
#include <stdlib.h> 
#include <ros.h>
#include <std_msgs/String.h>

// #include <string.h> // *** cstd string class fails to recognize ***
// #include <cstring>
// #include <iostream>
// #include <string>

// Other dependencies
#include <QuadratureEncoder.h>

//////////////////////////////////////////////////////////////////////////
// Declaration of ROS node
ros::NodeHandle arduinoNode;

//////////////////////////////////////////////////////////////////////////
// Miscellaneous declarations and initializations
#define num_motors 12
int motorNumber {0};
const int maxMotorSteps {15};
const int minMotorTurns {0};
const int maxMotorTurns {15};
bool haltCondition {false};
const float rInit[num_motors] {0.0f}; // used for reset functionality

byte pwm[num_motors] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2};
byte dir[num_motors] = {14, 15, 16, 17, 18, 19, 27, 29, 31, 33, 35, 37};
byte ena[num_motors] = {24, 26, 32, 34, 40, 42, 48, 50, 51, 49, 43, 41};
byte enb[num_motors] = {22, 28, 30, 36, 38, 44, 46, 52, 53, 47, 45, 39};
const byte relay = 65;

Encoders en1(ena[0], enb[0]);
Encoders en2(ena[1], enb[1]);
Encoders en3(ena[2], enb[2]);
Encoders en4(ena[3], enb[3]);
Encoders en5(ena[4], enb[4]);
Encoders en6(ena[5], enb[5]);
Encoders en7(ena[6], enb[6]);
Encoders en8(ena[7], enb[7]); // 8 does not work properly
Encoders en9(ena[8], enb[8]);
Encoders en10(ena[9], enb[9]);
Encoders en11(ena[10], enb[10]);
Encoders en12(ena[11], enb[11]);

float r[num_motors] {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}; // reference signal
float y[num_motors] {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}; // plant output
float e[num_motors] {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}; // error signal
float v[num_motors] {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}; // voltage input
float v_pwm[num_motors] {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}; // voltage (pwm) input
float Kp[num_motors] {100,100,100,100,100,100,100,100,100,100,100,100}; // proportional gain

const int cpr = 12;
const float gear_ratio = 29.861;

//////////////////////////////////////////////////////////////////////////
// Ros remote control callback function
char command {};
char macroCommand {};
int motorSelectionCommand {};
int motorSetpointCommand {};
char command2 {};
char command3 {};
char command4 {};
char command5 {};

int concat(int num1, int num2);

void remoteControl_cb(const std_msgs::String& cmd_msg) { 
    // command = cmd_msg.data[1];
    macroCommand = cmd_msg.data[0];
    if (macroCommand == 'm') {
        command2 = cmd_msg.data[1];
        command3 = cmd_msg.data[2];
        command4 = cmd_msg.data[3];
        command5 = cmd_msg.data[4];
        int command2int = command2 - 48;//'0';
        int command3int = command3 - 48;//'0';
        int command4int = command4 - '0';
        int command5int = command5 - '0';
        motorSelectionCommand = concat(command2int, command3int);
        motorSetpointCommand = concat(command4int, command5int);
    } 
}

int concat(int num1, int num2) {
    int cat = num1 * 10 + num2;
    return cat;
}

// ROS subscriber and publisher declarations
ros::Subscriber<std_msgs::String> sub("arduino", remoteControl_cb);
std_msgs::String return_msg;
ros::Publisher chatter("chatter", &return_msg);

//////////////////////////////////////////////////////////////////////////
// RC function prototypes
void resetHand();
void cutPower();
void restorePower();
void selectMacro(char macroCommand);
void selectMotor(int motorSelectionCommand);
void selectSetpoint(int motorSetpointCommand, bool setAll = false);

// Other function prototypes
float getMotorTurns(int motorNumber);
bool sgn(float x, int motorNumber);
int sat(float pwm_val);
void actuateMotors();

//////////////////////////////////////////////////////////////////////////
// Setup
void setup() {
    // Configuring Outputs
    for (int i = 0; i < num_motors; i ++) {
        pinMode(pwm[i], OUTPUT);
        pinMode(dir[i], OUTPUT);
    }
    pinMode(relay, OUTPUT);
    digitalWrite(relay, HIGH);

    // Setting up ROS communication
    arduinoNode.initNode();
    arduinoNode.advertise(chatter);
    arduinoNode.subscribe(sub);
}

// Loop
void loop() {
    arduinoNode.spinOnce();
    selectMacro(macroCommand);
    actuateMotors();
    delay(10);
}

//////////////////////////////////////////////////////////////////////////
// Remote control function
void selectMacro(char macroCommand) {
    switch (macroCommand) {
        case 'm':
            selectMotor(motorSelectionCommand);
            // selectSetpoint(motorSetpointCommand);
            break;
        case 'r':
            resetHand();
            break;
        case 'x':
            cutPower();
            break;
        case 'z':
            restorePower();
            break;
    }
}

void selectMotor(int motorSelectionCommand){
    if (command == 0){
        motorNumber = 0;
        const char off_msg[17] = "selected motor 0";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
        selectSetpoint(motorSetpointCommand);
    } else if (command == 1){
        motorNumber = 1;
        const char off_msg[17] = "selected motor 1";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
        selectSetpoint(motorSetpointCommand);
    } else if (command == 2){
        motorNumber = 2;
        const char off_msg[17] = "selected motor 2";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
        selectSetpoint(motorSetpointCommand);
    } else if (command == 3){
        motorNumber = 3;
        const char off_msg[17] = "selected motor 3";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
        selectSetpoint(motorSetpointCommand);
    } else if (command == 4){
        motorNumber = 4;
        const char off_msg[17] = "selected motor 4";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
        selectSetpoint(motorSetpointCommand);
    } else if (command == 5){
        motorNumber = 5;
        const char off_msg[17] = "selected motor 5";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
        selectSetpoint(motorSetpointCommand);
    } else if (command == 6){
        motorNumber = 6;
        const char off_msg[17] = "selected motor 6";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
        selectSetpoint(motorSetpointCommand);
    } else if (command == 7){
        motorNumber = 7;
        const char off_msg[17] = "selected motor 7";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
        selectSetpoint(motorSetpointCommand);
    } else if (command == 8){
        motorNumber = 8;
        const char off_msg[17] = "selected motor 8";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
        selectSetpoint(motorSetpointCommand);
    } else if (command == 9){
        motorNumber = 9;
        const char off_msg[17] = "selected motor 9";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
        selectSetpoint(motorSetpointCommand);
    } else if (command == 10){
        motorNumber = 10;
        const char off_msg[18] = "selected motor 10";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
        selectSetpoint(motorSetpointCommand);
    } else if (command == 11){
        motorNumber = 11;
        const char off_msg[18] = "selected motor 11";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
        selectSetpoint(motorSetpointCommand);
    } else if (command == -110) {
        motorNumber = 12;
        const char off_msg[20] = "selected all motors";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
        selectSetpoint(motorSetpointCommand, true);
    } else if (command == -33) {
        // keep previous motor number
        selectSetpoint(motorSetpointCommand);
    } else {
        const char off_msg[14] = "unknown motor";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
}

void selectSetpoint(int motorSetpointCommand, bool setAll) {
    // //verify no conflict with set bounds
    // if (motorSetpointCommand > maxMotorTurns) {
    //     motorSetpointCommand = maxMotorTurns;
    //     const char off_msg[58] = "motor setpoint exceeded maximum value; reset to max turns";
    //     return_msg.data = off_msg;
    //     chatter.publish(&return_msg);
    // } else if (motorSetpointCommand < minMotorTurns) {
    //     motorSetpointCommand = minMotorTurns;
    //     const char off_msg[55] = "motor setpoint below minimum value; reset to min turns";
    //     return_msg.data = off_msg;
    //     chatter.publish(&return_msg);
    // }
    //check to set all motors or just one
    if (setAll == true) {
        for (int i = 0; i < num_motors; i++) {
            r[i] = motorSetpointCommand;
        }
    } else {
        r[motorNumber] = motorSetpointCommand;
    }
    // const char off_msg[20] = "motor setpoints set";
    // return_msg.data = off_msg;
    // chatter.publish(&return_msg);
}

void resetHand(){
    for (int i = 0; i < num_motors; i++){
        r[i] = rInit[i];
    }
    const char off_msg[11] = "hand reset";
    return_msg.data = off_msg;
    chatter.publish(&return_msg); 
}

void cutPower() {
    digitalWrite(relay, LOW);
    const char off_msg[10] = "power cut";
    return_msg.data = off_msg;
    chatter.publish(&return_msg); 
}

void restorePower() {
    digitalWrite(relay, HIGH);
    const char off_msg[15] = "power restored";
    return_msg.data = off_msg;
    chatter.publish(&return_msg); 
}

// This function allows us to get the current motor angle of each motor
float getMotorTurns(int motorNumber){
    float yy = 0.0;
    switch (motorNumber) {
        case 0:
            yy = en1.getEncoderCount();
            break;
        case 1:
            yy = en2.getEncoderCount();
            break;
        case 2:
            yy = en3.getEncoderCount();
            break;
        case 3:
            yy = en4.getEncoderCount();
            break;
        case 4:
            yy = en5.getEncoderCount();
            break;
        case 5:
            yy = en6.getEncoderCount();
            break;
        case 6:
            yy = en7.getEncoderCount();
            break;
        case 7:
            yy = en8.getEncoderCount();
            break;
        case 8:
            yy = en9.getEncoderCount();
            break;
        case 9:
            yy = en10.getEncoderCount();
            break;
        case 10:
            yy = en11.getEncoderCount();
            break;
        case 11:
            yy = en12.getEncoderCount();
            break;
    }
    yy = yy/cpr/gear_ratio;
    return yy;
}

// sgn() defines the direction that the motor should spin
bool sgn(float x, int motorNumber){
    bool s = 0;
    if(x == 0) {
        s = 0;
    }
    if (abs(x) == x) {
        s = 1;
      }
    else {
        s = 0;
    }
    // Based on how the encoders were soldered onto each motor, we may need to customize the motor spin direction for each one.
      // This is why the switch case is necessary.
    switch (motorNumber) {
        case 0:
            s=s;
            break;
        case 1:
            s=s;
            break;
        case 2:
            s=s;
            break;
        case 3:
            s=!s;
            break;
        case 4:
            s=s;
            break;
        case 5:
            s=!s;
            break;
        case 6:
            s=s;
            break;
        case 7:
            s=s;
            break;
        case 8:
            s=s;
            break;
        case 9:
            s=!s;
            break;
        case 10:
            s=!s;
            break;
        case 11:
            s=s;
            break;
        default:
            s=!s;
    }
    return s;
}

// sat() caps the min and max values for the motor pwm signal
int sat(float pwm_val) {
    int min_pwm = 0;
    int max_pwm = 255;
    if (pwm_val > max_pwm) {
        pwm_val = max_pwm;
    }
    else if (pwm_val < min_pwm) {
        pwm_val = min_pwm;
    }
    return pwm_val;
}

// set the position of each motor to specified values from remote control function
void actuateMotors(){
    for (int i = 0; i < num_motors; i++) {
        y[i] = getMotorTurns(i);
        e[i] = r[i] - y[i];
        v[i] = Kp[i]*e[i];
        v_pwm[i] = sat(abs(v[i])/6*255);
        analogWrite(pwm[i],v_pwm[i]);
        digitalWrite(dir[i],sgn(v[i],i));
    }
}