#include "main.h"
#include "okapi/api.hpp"
using namespace okapi;

/*
-------Motor Parts-------
Port Desc    Port   Desc
1            11
2 leftdrive  12
3 leftspin   13
4  rightspin 12
5            15
6 				   16
7             17
8 leftlift   18
9 rightdrive 19
10 rightlift 20

*RED, high torque
^GREEN, normal

-------ADI-------
Port Desc  Port Desc
A          E
B          F
C          G
D          H

*/
Controller controller;
ControllerButton armsUp (ControllerDigital::L1);
ControllerButton armsDown (ControllerDigital::L2);
ControllerButton spinnerIntake (ControllerDigital::R1);
ControllerButton spinnerOuttake (ControllerDigital::R2);

Motor liftLeft(8);
Motor liftRight(-10);
Motor spinLeft(3);
Motor spinRight(-4);
Motor leftDrive(2);
Motor rightDrive(-9);

void initialize()
{
pros::lcd::initialize();
pros::lcd::set_text(1, "Hello PROS User!");

rightDrive.setBrakeMode(AbstractMotor::brakeMode::brake);
leftDrive.setBrakeMode(AbstractMotor::brakeMode::brake);

liftLeft.setBrakeMode(AbstractMotor::brakeMode::brake);
liftRight.setBrakeMode(AbstractMotor::brakeMode::brake);
}

void disabled() {}

void competition_initialize() {}

void autoArmRaiseLower(int time, char direction) {
if(direction == 'r') { //raise
liftLeft.moveVelocity(-50);
liftRight.moveVelocity(-50);
pros::delay(time);
} else if(direction == 'l') { //lower
liftLeft.moveVelocity(50);
liftRight.moveVelocity(50);
pros::delay(time);
}
liftLeft.moveVelocity(0);
liftRight.moveVelocity(0);
}

void autoSpinnerInOut(int time, char direction) {
if(direction == 'o') { //in
spinLeft.moveVelocity(-100);
spinRight.moveVelocity(-100);
pros::delay(time);
} else if(direction == 'i') { //out
spinLeft.moveVelocity(100);
spinRight.moveVelocity(100);
pros::delay(time);
}
spinLeft.moveVelocity(0);
spinRight.moveVelocity(0);
}

void autonomous() {

std::shared_ptr<ChassisController> drive =
   ChassisControllerBuilder()
       .withMotors(leftDrive, rightDrive)
.withDimensions(AbstractMotor::gearset::green, {{4_in, 15_in}, imev5GreenTPR})
.build();

//drive->moveDistance(4.25_in);

autoArmRaiseLower(1700, 'r');

drive->moveDistance(9.5_in);

autoArmRaiseLower(500, 'r');

autoSpinnerInOut(2000, 'o');

drive->moveDistance(-8.25_in);

autoArmRaiseLower(1000, 'l');

// drive->moveDistance(24_in);
//
// drive->turnAngle(90_deg);
//
// drive->moveDistance(48_in);
//
// drive->moveDistance(24_in);
//
// drive->turnAngle(180_deg);
//
// drive->moveDistance(24_in);

// autoArmRaiseLower(500, 'r');
// pros::delay(1000);
// autoArmRaiseLower(500, 'l');
//
// autoSpinnerInOut(500, 'i');
// pros::delay(1000);
// autoSpinnerInOut(500, 'o');
}

void armsMove() {
if(armsUp.isPressed()) {
liftLeft.moveVelocity(50);
liftRight.moveVelocity(50);
} else if(armsDown.isPressed()) {
liftLeft.moveVelocity(-50);
liftRight.moveVelocity(-50);
} else {
liftLeft.moveVelocity(0);
liftRight.moveVelocity(0);
}
}

void spinnersMove() {
//Intake
if(spinnerIntake.isPressed()) {
spinLeft.moveVelocity(100);
spinRight.moveVelocity(100);

//Spit out
} else if(spinnerOuttake.isPressed()) {
spinLeft.moveVelocity(-100);
spinRight.moveVelocity(-100);
} else {
spinLeft.moveVelocity(0);
spinRight.moveVelocity(0);
}

}

void opcontrol() {
//Set up motors/sensors/controller/etc for driving



//Controller controller;
std::shared_ptr<ChassisController> drive =
    ChassisControllerBuilder()
        .withMotors(leftDrive, rightDrive)
.withDimensions(AbstractMotor::gearset::green, {{4_in, 15_in}, imev5GreenTPR})
.build();
// Chassis Controller - lets us drive the robot around with open- or closed-loop control

 //.withDimensions(AbstractMotor::gearset::green, {{4_in, 12_in}, imev5GreenTPR})

while (true) {
//Loops while I'm driving
drive -> getModel() -> arcade(controller.getAnalog(ControllerAnalog::leftY),
controller.getAnalog(ControllerAnalog::rightX));

armsMove();
spinnersMove();

}
pros::delay(20);
}
