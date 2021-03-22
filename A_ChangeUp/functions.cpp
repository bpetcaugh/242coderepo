#include "main.h"


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

void deploy(){
  if(deploybutton.isPressed()){
    leftIntake.moveVelocity(50);
    rightIntake.moveVelocity(50);
    indexerTop.moveVelocity(600);
  }
}

void intake(){
    if(intakeIn.isPressed()){
      leftIntake.moveVelocity(600);
      rightIntake.moveVelocity(600);
    }
    else if (!intakeIn.isPressed() || !allOut.isPressed() || !indexIn.isPressed()){
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

void turn (double degToTurn, char t, int speed){
  //normal was 90
  if(t == 'r'){ //turn right
    while(inertialSensor.get_heading() < degToTurn - 5 || inertialSensor.get_heading() > 350){
        frontLeft.moveVelocity(speed);
        backLeft.moveVelocity(speed);
        frontRight.moveVelocity(speed);
        backRight.moveVelocity(speed);
    }
    frontLeft.moveVelocity(0);
    backLeft.moveVelocity(0);
    frontRight.moveVelocity(0);
    backRight.moveVelocity(0);
  }
  if(t == 'l'){ //turn left
    while(inertialSensor.get_heading() > degToTurn + 5 || inertialSensor.get_heading() < 10){
      frontLeft.moveVelocity(-speed);
      backLeft.moveVelocity(-speed);
      frontRight.moveVelocity(-speed);
      backRight.moveVelocity(-speed);
    }
    frontLeft.moveVelocity(0);
    backLeft.moveVelocity(0);
    frontRight.moveVelocity(0);
    backRight.moveVelocity(0);
  }
}

void autoDeploy(){
  leftIntake.moveVelocity(50);
  rightIntake.moveVelocity(50);
  indexerTop.moveVelocity(600);
  pros::delay(500);
  leftIntake.moveVelocity(0);
  rightIntake.moveVelocity(0);
  indexerTop.moveVelocity(0);
}
