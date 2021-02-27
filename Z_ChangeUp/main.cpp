#include "main.h"

/*
-------Motor Ports-------
Port	Desc		Port	Desc
1			left1		11		middleRoller right1
2			left2		12		right2
3			topRoller				13
4			lift1*	14		lift2*
5							15		spinner1
6							16
7							17		spinner2
8							18
9		righty		19
10	lefty			20

*RED, high torque
^BLUE, high speed

-------ADI-------
Port	Desc		Port	Desc
A							E
B							F
C							G
D							H

*/

//Set up Sensors
ADIButton ballSensor('A');
Motor topRoller(3,false,AbstractMotor::gearset::blue,AbstractMotor::encoderUnits::degrees);
Motor middleRoller(-11);
Motor baseMotor1(9);
Motor baseMotor2(10);


//Helper Functions
void isThereBall_fn() {
	while(true) {
		if (ballSensor.isPressed()) {
			topRoller.moveVelocity(100);
			middleRoller.moveVelocity(100);
			pros::delay(500);
			topRoller.moveVelocity(0);
			middleRoller.moveVelocity(0);
		}
	}
}

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "My name is Nathaniel...TIME TO SUFFER");
}

void disabled() {}

void competition_initialize() {}

void autonomous() {

	pros::lcd::set_text(2, "I'm in autonomous!");
	Motor spinner1(-15);
	Motor spinner2(17);
	Motor topRoller(3,false,AbstractMotor::gearset::blue,AbstractMotor::encoderUnits::degrees);
	

	//pros::Task isThereBall(isThereBall_fn);
	//Motor topRoller(-3,false,AbstractMotor::gearset::blue,AbstractMotor::encoderUnits::degrees);
	//MotorGroup myLift ({4 , -14});

	std::shared_ptr<ChassisController> myBaseDrive =
		ChassisControllerBuilder()
				.withMotors(10, -9)
				.withDimensions(AbstractMotor::gearset::green, {{4_in, 10.5_in}, imev5GreenTPR})
				.build();

		myBaseDrive->setMaxVelocity(75);

		//spinner1.moveVelocity(-200);
		//spinner2.moveVelocity(-200);
		topRoller.moveVelocity(100);
		pros::delay(1000);
		//spinner1.moveVelocity(0);
		//spinner2.moveVelocity(0);
		topRoller.moveVelocity(0);

		//isThereBall.resume();
		/*
		Begin Main Movement Routine
		*/

		//Top intake OUT, 1 sec
		topRoller.moveVelocity(-600); //start motor
		middleRoller.moveVelocity(200);
		pros::delay(2000); //wait 1 sec
		topRoller.moveVelocity(0); //stop motor
		middleRoller.moveVelocity(200);

		//Forward, 2 in
		myBaseDrive->moveDistance(4_in);

		//Intake IN, 5 sec; UNCOMMENT WHEN DONE TESTING

		middleRoller.moveVelocity(-600);
		topRoller.moveVelocity(-600);
		pros::delay(3000);
		middleRoller.moveVelocity(0);
		topRoller.moveVelocity(0);

		//Backward, 3.5 inches
		myBaseDrive->moveDistance(-4_in);

		//Turn, 150 deg counterclock
		myBaseDrive->turnAngle(180_deg);

		//Bring ball in system and move Forward 8 in
		spinner1.moveVelocity(100);
		spinner2.moveVelocity(100);
		myBaseDrive->moveDistance(15_in);

		//Turn 15 degrees
		myBaseDrive->turnAngle(-25_deg);
		spinner1.moveVelocity(0);
		spinner2.moveVelocity(0);

		//isThereBall.suspend(); //SEE IF TASK STOPS RUNNING HERE

		//Straight 37 inches
		myBaseDrive->moveDistance(37_in);

		//Turn 90 degrees toward goal
		//Straight 37 inches
		myBaseDrive->turnAngle(-90_deg);
}

void opcontrol() {
	//Set up motors/sensors/controller/etc for driving
	Controller myController;
	ControllerButton rollIn (ControllerDigital::R2);
	ControllerButton rollOut (ControllerDigital::R1);
	ControllerButton deployButt (ControllerDigital::up);

	//Motor topRoller(3);
	Motor middleRoller(-11);
	Motor spinner1(-15);
	Motor spinner2(17);
	Motor topRoller(3,false,AbstractMotor::gearset::blue,AbstractMotor::encoderUnits::degrees);
	//MotorGroup myLift ({4 , -14});

	//Setting coast for motors
	topRoller.setBrakeMode(AbstractMotor::brakeMode::coast);
	middleRoller.setBrakeMode(AbstractMotor::brakeMode::coast);
	spinner1.setBrakeMode(AbstractMotor::brakeMode::coast);
	spinner2.setBrakeMode(AbstractMotor::brakeMode::coast);
	baseMotor1.setBrakeMode(AbstractMotor::brakeMode::coast);
	baseMotor2.setBrakeMode(AbstractMotor::brakeMode::coast);



	std::shared_ptr<ChassisController> myBaseDrive =
    ChassisControllerBuilder()
        .withMotors(10, 9)
        // Green gearset, 4 in wheel diam, 11.5 in wheel track
        .withDimensions(AbstractMotor::gearset::green, {{4_in, 14_in}, imev5GreenTPR})
        .build();



	while (true) {
		//Loops while I'm driving
		myBaseDrive->getModel()->arcade(myController.getAnalog(ControllerAnalog::rightX),
													myController.getAnalog(ControllerAnalog::leftY));



		if (deployButt.isPressed()) {
			spinner1.moveVelocity(-200);
			spinner2.moveVelocity(-200);
			topRoller.moveVelocity(100);
			pros::delay(1000);
			spinner1.moveVelocity(0);
			spinner2.moveVelocity(0);
			topRoller.moveVelocity(0);

		}


		//Lift the arms up and down
		if (rollIn.isPressed()) {
			middleRoller.moveVelocity(200);
			topRoller.moveVelocity(600);
			spinner1.moveVelocity(200);
			spinner2.moveVelocity(200);

		} else if (rollOut.isPressed()) {
			middleRoller.moveVelocity(-200);
			topRoller.moveVelocity(-600);
			spinner1.moveVelocity(-200);
			spinner2.moveVelocity(-200);

		}
		else {
			middleRoller.moveVelocity(0);
			topRoller.moveVelocity(0);
			spinner1.moveVelocity(0);
			spinner2.moveVelocity(0);
		}

		pros::delay(20);
	}

}
