#include "main.h"

/*
Port description
1
2 Distance Sensor
3
4
5
6
7 Top Color Sensor
8 Bottom Color Sensor
9 Front right drive
10 Right Intake
11 Top two indexers
12 Bottom three indexers
13
14
15
16 IMU
17 Back Right Drive
18 Back Left Drive
19 Front Left Drive
20 Left Intake

A Right track wheel
B Right track wheel
C Left track wheel
D Left track wheel
E
F
G
H

Current Goals:
106-126 auton

How do I drive it?
Left stick forward back is forward back
Right stick left right is turn left right

R1 intake only intake
R2 all rollers out
L1 indexers up
L2 ejector
A Deploy button

What does Auton do?
As of right now 3 balls into bottom right goal starting with ball in middle
and with the ball straight in front of it
*/


//The code to setup the four wheels for the drive train
Motor frontRight(9);
Motor frontLeft(18);
Motor backRight(17);
Motor backLeft(19);

//For the motor that runs the bottom 3 intakes
Motor indexer3(12, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

//For the top two rollers
Motor indexerTop(11, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

//For the intakes
Motor leftIntake(20, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor rightIntake(10, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

//Tracking wheels
pros::ADIEncoder leftTrack ('C', 'D');
pros::ADIEncoder rightTrack ('A', 'B');

//Color sensors
OpticalSensor topColor(7);
OpticalSensor bottomColor(8);

//IMU
pros::Imu inertialSensor(16);

//Distance sensors
pros::Distance distanceSensor(2);

//Define the controller under the name controller
Controller controller;

//Setup the intake buttons in and out on the right side triggers
ControllerButton intakeIn(ControllerDigital::R1);

ControllerButton allOut(ControllerDigital::R2);

ControllerButton indexIn(ControllerDigital::L1);

//Setup the button for ejecting the balls out of the back
ControllerButton ejector(ControllerDigital::L2);

ControllerButton deploybutton(ControllerDigital::A);
