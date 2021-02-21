#include "main.h"

//The code to setup the four wheels for the drive train
Motor frontRight(-9);
Motor frontLeft(19);
Motor backRight(17);
Motor backLeft(-18);

//For the motor that runs the bottom 3 intakes
Motor indexer3(12, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

//For the top two rollers
Motor indexerTop(11, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

//For the intakes
Motor leftIntake(20, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor rightIntake(10, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

auto imuz = IMU(16, IMUAxes::z);
auto imux = IMU(16, IMUAxes::x);
auto imuy = IMU(16, IMUAxes::y);

pros::ADIEncoder leftTrack ('C', 'D');
pros::ADIEncoder rightTrack ('A', 'B');

//Define the controller under the name controller
Controller controller;

//Setup the intake buttons in and out on the right side triggers
ControllerButton intakeIn(ControllerDigital::R1);

ControllerButton allOut(ControllerDigital::R2);

ControllerButton indexIn(ControllerDigital::L1);

//Setup the button for ejecting the balls out of the back
ControllerButton ejector(ControllerDigital::L2);


void on_center_button() {
    static bool pressed = false;
    pressed = !pressed;
    if (pressed) {
        pros::lcd::set_text(2, "I was pressed!");
    } else {
        pros::lcd::clear_line(2);
    }
}

std::shared_ptr<OdomChassisController> chassis =
    ChassisControllerBuilder()
        .withMotors ({-18, -19}, //left motors
										{9, 17})  //right motors
        .withDimensions(AbstractMotor::gearset::blue, {{1.7324_in, 12.5_in}, imev5BlueTPR}) //Blue, 4in wheel, 12.5 in wheel track
        .withSensors(ADIEncoder{'A', 'B', false}, //Right
                     ADIEncoder{'C', 'D', true}) //Left
        .withOdometry({{2.75_in, 4.5_in}, quadEncoderTPR}) //2.75in with distance of 4.5 apart
        .buildOdometry();

  //Initialize Motion Profiling
  std::shared_ptr<AsyncMotionProfileController> profileController =
    AsyncMotionProfileControllerBuilder()
      .withLimits({
        1.0, //Maximum linear velocity in m/s
        2.0, //Maximum linear acceleration in m/s/s
        10.0 //Maximum linear jerk in m/s/s/s
      })
      .withOutput(chassis)
      .buildMotionProfileController();

void index(){
  if(indexIn.isPressed()){
    indexer3.moveVelocity(600);
    indexerTop.moveVelocity(600);
  }
  else if (!indexIn.isPressed() || !allOut.isPressed() || !ejector.isPressed()){
    indexer3.moveVelocity(0);
    indexerTop.moveVelocity(0);
  }
}

void intake(){
    if(intakeIn.isPressed()){
      leftIntake.moveVelocity(600);
      rightIntake.moveVelocity(600);
    }
    else if (!intakeIn.isPressed() || !allOut.isPressed()){
      leftIntake.moveVelocity(0);
      rightIntake.moveVelocity(0);
    }
}

void intakeOut(){
  if(allOut.isPressed()){
      indexer3.moveVelocity(-600);
      indexerTop.moveVelocity(-600);
      leftIntake.moveVelocity(-600);
      rightIntake.moveVelocity(-600);
  }
}

void eject(){
  if(ejector.isPressed()){
    indexer3.moveVelocity(600);
    indexerTop.moveVelocity(-600);
  }
}

void initialize() {
  pros::lcd::initialize();
  pros::lcd::set_text(1, "Hello PROS User!");

  pros::lcd::register_btn1_cb(on_center_button);
}

void disabled() {}

void competition_initialize() {
}

void autonomous() {

    chassis->setState({0_ft, 0_ft, 0_deg});
    pros::delay(1000);

  pros::Task([&] {while(true){
    printf("%s\n", chassis->getState().str().c_str());
    printf("Left encoder value: %d", leftTrack.get_value());
    printf("Right encoder value: %d", rightTrack.get_value());
  }});

  chassis->turnAngle(90_deg);

  // indexerTop.moveVelocity(600);
  // pros::delay(500);
  // indexerTop.moveVelocity(0);
  //
  // profileController->generatePath(
  //   {{0_ft, 0_ft, 0_deg}, {4_ft, 0_ft, 0_deg}}, "testStraight");
  // profileController->setTarget("testStraight"); //Drive up to the first goal
  // profileController->waitUntilSettled(); //Wait until finished



//Showing how to do second movement
  // profileController->generatePath(
  //   {{0_ft, 0_ft, 0_deg}, {1.4_ft, 0_ft, 0_deg}}, "Goal1Straight1");
  // profileController->setTarget("Goal1Straight1");
  // profileController->waitUntilSettled();

  //Showing of the turn function
  // chassis->turnToAngle(90_deg);


  // turnAim(90, 2, 0, .3, 1);
}

void opcontrol() {
  frontRight.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
  frontLeft.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
  backRight.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
  backLeft.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);

    while (true) {
      index();
      intake();
      intakeOut();
      eject();

      chassis->getModel()->arcade(controller.getAnalog(ControllerAnalog::leftY),
                             controller.getAnalog(ControllerAnalog::rightX));
     		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
     		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
     		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
        pros::lcd::print(3, "Left Encoder: %d", (leftTrack.get_value()));
        pros::lcd::print(5, "Right Encoder: %d", (rightTrack.get_value()));
        pros::lcd::print(7, "%s\n", chassis->getState().str().c_str());
     		pros::delay(20);
      }
}
