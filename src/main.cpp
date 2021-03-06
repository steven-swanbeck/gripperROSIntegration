// Arduino dependencies
#include <Arduino.h>
#include <Wire.h>

#define USE_USBCON

// ROS dependencies
#include <stdlib.h> 
#include <ros.h>
#include <std_msgs/String.h>

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
void remoteControl_cb(const std_msgs::String& cmd_msg) { 
    command = cmd_msg.data[1];
}

// ROS subscriber and publisher declarations
ros::Subscriber<std_msgs::String> sub("arduino", remoteControl_cb);
std_msgs::String return_msg;
ros::Publisher chatter("chatter", &return_msg);

//////////////////////////////////////////////////////////////////////////
// RC function prototypes
int selectMotor(char command);
float incrementTurns(int motorNumber);
float decrementTurns(int motorNumber);
void incrementAll();
void decrementAll();
void resetHand();
void cutPower();
void restorePower();
void drive1s(int motorNumber);
void drive1sReverse(int motorNumber);
void remoteControl();

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
    remoteControl();
    actuateMotors();
    delay(10);
}

//////////////////////////////////////////////////////////////////////////
// Remote control function
void remoteControl() {
    if (command == '=' || command == '1' || command == '2' || command == '3' || command == '4' || command == '5' || command == '6' || command == '7' || command == '8' || command == '9' || command == '0' || command == '-') {
        selectMotor(command);
    }
    else if (command == 'q') {
        incrementTurns(motorNumber);
        const char on_msg[30] = "increase selected motor turns";
        return_msg.data = on_msg;
        chatter.publish(&return_msg);
    } 
    else if (command == 'a') {
        decrementTurns(motorNumber);
        const char off_msg[30] = "decrease selected motor turns";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == '.') {
        incrementAll();
        const char off_msg[25] = "increase all motor turns";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == ',') {
        decrementAll();
        const char off_msg[25] = "decrease all motor turns";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == 'r') {
        resetHand();
        const char off_msg[22] = "reset all motor turns";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == 'x') {
        cutPower();
        const char off_msg[10] = "power cut";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == 'z') {
        restorePower();
        const char off_msg[15] = "power restored";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == 'm') {
        drive1s(motorNumber);
        const char off_msg[25] = "selected motor driven 1s";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == 'n') {
        drive1sReverse(motorNumber);
        const char off_msg[33] = "selected motor driven reverse 1s";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
}

int selectMotor(char command){
    if (command == '='){
        motorNumber = 0;
        const char off_msg[17] = "selected motor 0";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == '1'){
        motorNumber = 1;
        const char off_msg[17] = "selected motor 1";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == '2'){
        motorNumber = 2;
        const char off_msg[17] = "selected motor 2";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == '3'){
        motorNumber = 3;
        const char off_msg[17] = "selected motor 3";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == '4'){
        motorNumber = 4;
        const char off_msg[17] = "selected motor 4";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == '5'){
        motorNumber = 5;
        const char off_msg[17] = "selected motor 5";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == '6'){
        motorNumber = 6;
        const char off_msg[17] = "selected motor 6";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == '7'){
        motorNumber = 7;
        const char off_msg[17] = "selected motor 7";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == '8'){
        motorNumber = 8;
        const char off_msg[17] = "selected motor 8";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == '9'){
        motorNumber = 9;
        const char off_msg[17] = "selected motor 9";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == '0'){
        motorNumber = 10;
        const char off_msg[18] = "selected motor 10";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    else if (command == '-'){
        motorNumber = 11;
        const char off_msg[18] = "selected motor 11";
        return_msg.data = off_msg;
        chatter.publish(&return_msg);
    }
    return motorNumber;
}

float incrementTurns(int motorNumber){
    float motorIncrement = (maxMotorTurns - minMotorTurns) / maxMotorSteps;
    r[motorNumber] = r[motorNumber] + motorIncrement;
    if (r[motorNumber] > maxMotorTurns){
        r[motorNumber] = maxMotorTurns;
    }
    return r[motorNumber];
}

float decrementTurns(int motorNumber){
    float motorIncrement = (maxMotorTurns - minMotorTurns) / maxMotorSteps;
    r[motorNumber] = r[motorNumber] - motorIncrement;
    if (r[motorNumber] < minMotorTurns){
        r[motorNumber] = minMotorTurns;
    }
    return r[motorNumber];
}

void incrementAll(){
    float motorIncrement = (maxMotorTurns - minMotorTurns) / maxMotorSteps;
    for (int i = 0; i < num_motors; i++){
        r[i] = r[i] + motorIncrement;
        if (r[i] > maxMotorTurns){
            r[i] = maxMotorTurns;
        }
    }
}

void decrementAll(){
    float motorIncrement = (maxMotorTurns - minMotorTurns) / maxMotorSteps;
    for (int i = 0; i < num_motors; i++){
        r[i] = r[i] - motorIncrement;
        if (r[i] < minMotorTurns){
            r[i] = minMotorTurns;
        }
    }
}

void resetHand(){
    for (int i = 0; i < num_motors; i++){
        r[i] = rInit[i];
    } 
}

void cutPower() {
    digitalWrite(relay, LOW);
}

void restorePower() {
    digitalWrite(relay, HIGH);
}

void drive1s(int motorNumber){
    float timer1s = millis();
    while (millis() - timer1s < 100){
        digitalWrite(dir[motorNumber],HIGH);
        analogWrite(pwm[motorNumber],255);
    } 
    analogWrite(pwm[motorNumber],0);
    digitalWrite(dir[motorNumber],LOW);
}

void drive1sReverse(int motorNumber){
    float timer1s = millis();
    while (millis() - timer1s < 100){
        digitalWrite(dir[motorNumber],LOW);
        analogWrite(pwm[motorNumber],255);
    } 
    analogWrite(pwm[motorNumber],0);
    digitalWrite(dir[motorNumber],LOW);
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