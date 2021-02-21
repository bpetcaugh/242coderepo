#include "main.h"
/*

Port     Desc
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20

Current Team Goals:
1.
2.
3.

How do I drive this robot?


What does autonomous do?



*/



void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	pros::lcd::set_text(2, "I'm in Autonomous");
}

void opcontrol() {
    //Motor, Controller, and Sensor Setup
    Controller controller1;
    ControllerButton launch(ControllerDigital::X);
		ControllerButton grab(ControllerDigital::A);
		ControllerButton roll(ControllerDigital::B);
//Green = Roller, Grabber = Red, Launcher = Blue
		MotorGroup grabber({Motor(19, false, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees), Motor(20, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees)});
    MotorGroup launcher({Motor(2, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees), Motor (3, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees)});
		Motor roller(1, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
    std::shared_ptr<ChassisController> myBaseDrive =
    ChassisControllerBuilder()
        .withMotors(9, -10)
        // Green gearset, 4 in wheel diam, 11.5 in wheel track
        .withDimensions(AbstractMotor::gearset::red, {{4_in, 11.5_in}, imev5GreenTPR})
        .build();
    while (true) {
        //Driving Loop
        myBaseDrive->getModel()->arcade(controller1.getAnalog(ControllerAnalog::leftY),
                                                         controller1.getAnalog(ControllerAnalog::leftX));

        if (launch.isPressed()){
            launcher.moveVelocity(720);
        }
        else {
            launcher.moveVelocity(0);
        }
				if (roll.isPressed()){
            roller.moveVelocity(720);
        }
        else {
            roller.moveVelocity(0);
        }
				if (grab.isPressed()){
            grabber.moveVelocity(720);
        }
        else {
            grabber.moveVelocity(0);
        }

        pros::delay(20);
    }
}
