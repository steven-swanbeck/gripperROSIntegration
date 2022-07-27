// Arduino dependencies
#include <Arduino.h>
#include <Wire.h>

#define USE_USBCON // *** MUST BE UN-COMMENTED FOR USE WITH ARDUINO DUE***

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
float Kp[num_motors] {50,50,50,50,50,50,50,50,50,50,50,50}; // proportional gain

const int cpr = 12;
const float gear_ratio = 29.861;

//////////////////////////////////////////////////////////////////////////
// Ros remote control callback function
char command {};

int concat(int num1, int num2);

void remoteControl_cb(const std_msgs::String& cmd_msg) { 
    command = cmd_msg.data[0];
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
void selectMacro(char command);
void assumeGrasp();

void handshake();
void fist();
void pinkieOut();
void pinkieClosed();
void snap();
void fingerGun();
void peace();
void rockPaperScissors();

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
    digitalWrite(relay, LOW);

    // Seeding RNG
    randomSeed(analogRead(0));

    // Setting up ROS communication
    arduinoNode.initNode();
    arduinoNode.advertise(chatter);
    arduinoNode.subscribe(sub);
}

// Loop
void loop() {
    arduinoNode.spinOnce();
    selectMacro(command);
    actuateMotors();
    delay(10);
}

//////////////////////////////////////////////////////////////////////////
// Remote control function
void selectMacro(char command) {
    switch (command) {
        case 'g':
            assumeGrasp();
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
        case 'h':
            handshake();
            break;
        case 'f':
            fist();
            break;
        case 'm':
            pinkieOut();
            break;
        case 'n':
            pinkieClosed();
            break;
        case 's':
            snap();
            break;
        case 'j':
            fingerGun();
            break;
        case 'p':
            peace();
            break;
        case 'k':
            rockPaperScissors();
            break;
    }
}

void assumeGrasp() {
    // for (int i = 0; i < num_motors; i++) {
    //     r[i] = 8.0;
    // }
    // r[0] = 8.0;
    // r[1] = 8.0;
    // r[2] = 8.0;
    // r[3] = 8.0;
    // r[4] = 8.0;

    r[4] = 10.0;
    r[5] = 10.0;
    r[7] = 12.0;
    r[9] = 12.0;
    r[11] = 10.0;

    // r[10] = 17.0;
    // r[0] = 16.0;
    // r[1] = 18.0;
    // r[2] = 18.0;
    // r[3] = 19.0;

    r[10] = 17.0;
    r[0] = 16.0;
    r[1] = 18.0;
    r[2] = 21.0;
    r[3] = 21.0;

    const char off_msg[28] = "preprogrammed grasp assumed";
    return_msg.data = off_msg;
    chatter.publish(&return_msg); 
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

// additional hand positions for handshake
void handshake() {
    r[4] = 10.0;
    r[5] = 10.0;
    r[7] = 9.0;
    r[9] = 8.0;
    r[11] = 10.0;

    r[10] = 16.0;
    r[0] = 10.0;
    r[1] = 15.0;
    r[2] = 16.0;
    r[3] = 17.0;

    const char off_msg[18] = "handshake assumed";
    return_msg.data = off_msg;
    chatter.publish(&return_msg); 
}
void fist() {
    r[4] = 12.0;
    r[5] = 12.0;
    r[7] = 12.0;
    r[9] = 12.0;
    r[11] = 12.0;

    r[10] = 18.0;
    r[0] = 18.0;
    r[1] = 18.0;
    r[2] = 18.0;
    r[3] = 18.0;

    const char off_msg[13] = "fist assumed";
    return_msg.data = off_msg;
    chatter.publish(&return_msg); 
}
void pinkieOut() {
    r[4] = 0.0;
    r[5] = 12.0;
    r[7] = 12.0;
    r[9] = 12.0;
    r[11] = 12.0;

    r[10] = 18.0;
    r[0] = 18.0;
    r[1] = 18.0;
    r[2] = 18.0;
    r[3] = 0.0;

    const char off_msg[19] = "pinkie out assumed";
    return_msg.data = off_msg;
    chatter.publish(&return_msg); 
}
void pinkieClosed() {
    r[4] = 8.0;
    r[5] = 12.0;
    r[7] = 12.0;
    r[9] = 12.0;
    r[11] = 12.0;

    r[10] = 18.0;
    r[0] = 18.0;
    r[1] = 18.0;
    r[2] = 18.0;
    r[3] = 10.0;

    const char off_msg[22] = "pinkie closed assumed";
    return_msg.data = off_msg;
    chatter.publish(&return_msg); 
}
void snap() {
    r[4] = 12.0;
    r[5] = 12.0;
    r[7] = 12.0;
    r[9] = 12.0;
    r[11] = 12.0;

    r[10] = 0.0;
    r[0] = 0.0;
    r[1] = 0.0;
    r[2] = 18.0;
    r[3] = 18.0;

    const char off_msg[13] = "snap assumed";
    return_msg.data = off_msg;
    chatter.publish(&return_msg); 
}
void fingerGun() {
    r[4] = 10.0;
    r[5] = 10.0;
    r[7] = 10.0;
    r[9] = 0.0;
    r[11] = 0.0;

    r[10] = 0.0;
    r[0] = 0.0;
    r[1] = 18.0;
    r[2] = 18.0;
    r[3] = 18.0;

    const char off_msg[19] = "finger gun assumed";
    return_msg.data = off_msg;
    chatter.publish(&return_msg); 
}
void peace() {
    r[4] = 10.0;
    r[5] = 10.0;
    r[7] = 0.0;
    r[9] = 0.0;
    r[11] = 10.0;

    r[10] = 18.0;
    r[0] = 0.0;
    r[1] = 0.0;
    r[2] = 18.0;
    r[3] = 18.0;

    const char off_msg[14] = "peace assumed";
    return_msg.data = off_msg;
    chatter.publish(&return_msg); 
}
void rockPaperScissors() {
    int sign = random(1,4);
    if (sign == 1) {
        fist();
    }
    else if (sign == 2) {
        resetHand();
    }
    else if (sign == 3) {
        peace();
    }
    else if (sign == 4) {
        fingerGun();
    }
    const char off_msg[12] = "sign chosen";
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