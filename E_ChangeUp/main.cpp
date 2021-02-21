#include "main.h"

/*
----motorports----
port desc  port desc
1  						11 BackRight
2  						12
3  						13 frontright
4 leftspinny 	14
5 rightspinny 15 rightarmupdown
6  						16
7  						17
8 backleft  	18
9  						19 leftarmupdown
10 frontleft  20

----ADI----
A E
B F
C G
D H
*/

void initialize() {
pros::lcd::initialize();
pros::lcd::set_text(1, "Hello PROS User!");
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
pros::lcd::set_text(2, "I'm in auto!");
MotorGroup myLift({15, -19});
MotorGroup spinny({4, -5});

std::shared_ptr<ChassisController> myBaseDrive =
    ChassisControllerBuilder()
        .withMotors({8,10}, {-11,-13})
        // Green gearset, 4 in wheel diam, 11.5 in whe el track
        .withDimensions(AbstractMotor::gearset::green, {{4_in, 13_in}, imev5GreenTPR})
 .build();

 myBaseDrive->setMaxVelocity(150);

//MOVE LIFT UP
 myLift.moveVelocity(200);
 pros::delay(2000);
 myLift.moveVelocity(0);
 
//MOVE FORWARD
 myBaseDrive->moveDistance(7_in);

//SPIN BALL OUT
 spinny.moveVelocity(200);
 pros::delay(2000);
 spinny.moveVelocity(0);

 //myBaseDrive->turnAngle(90_deg);

}

void opcontrol() {
//sets up parts
Controller myController;
  ControllerButton liftUp(ControllerDigital::L1);
ControllerButton liftDown(ControllerDigital::R1);
ControllerButton inTake(ControllerDigital::R2);
ControllerButton outTake(ControllerDigital::L2);

MotorGroup myLift({15, -19});
MotorGroup spinny({4, -5});

std::shared_ptr<ChassisController> myBaseDrive =
    ChassisControllerBuilder()
        .withMotors({8,10}, {-11,-13})
        // Green gearset, 4 in wheel diam, 11.5 in whe el track
        .withDimensions(AbstractMotor::gearset::green, {{4_in, 13_in}, imev5GreenTPR})
 .build();

while (true) {


myBaseDrive->getModel()->arcade(myController.getAnalog(ControllerAnalog::leftY),
myController.getAnalog(ControllerAnalog::leftX));

if (liftUp.isPressed()) {
myLift.moveVelocity(150);
} else if (liftDown.isPressed()) {
myLift.moveVelocity(-150);
}
 else {
myLift.moveVelocity(0);
}

if (inTake.isPressed()) {
spinny.moveVelocity(-200);
} else if (outTake.isPressed()){
spinny.moveVelocity(200);
}
else {
spinny.moveVelocity(0);
}

pros::delay(20);
}
}
