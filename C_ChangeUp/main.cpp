#include "main.h"

#define HUE 360
#define DEFAULT 1
#define AUTONS "Front", "Back", "Do Nothing"

namespace selector{

const char *b[] = {AUTONS, ""};
int auton;
int autonCount;
const char *btnmMap[] = {"","","","","","","","","","",""}; // up to 10 autons

lv_obj_t *tabview;
lv_obj_t *redBtnm;
lv_obj_t *blueBtnm;

lv_res_t redBtnmAction(lv_obj_t *btnm, const char *txt){
	//printf("red button: %s released\n", txt);

	for(int i = 0; i < autonCount; i++){
		if(strcmp(txt, btnmMap[i]) == 0){
			auton = i+1;
		}
	}

	return LV_RES_OK; // return OK because the button matrix is not deleted
}

lv_res_t blueBtnmAction(lv_obj_t *btnm, const char *txt)
{
	//printf("blue button: %s released\n", txt);

	for(int i = 0; i < autonCount; i++){
		if(strcmp(txt, btnmMap[i]) == 0){
			auton = -(i+1);
		}
	}

	return LV_RES_OK; // return OK because the button matrix is not deleted
}

lv_res_t skillsBtnAction(lv_obj_t *btn){
  //printf("skills pressed");
	auton = 0;
	return LV_RES_OK;
}

int tabWatcher() {
	int activeTab = lv_tabview_get_tab_act(tabview);
	while(1){
		int currentTab = lv_tabview_get_tab_act(tabview);

		if(currentTab != activeTab){
			activeTab = currentTab;
			if(activeTab == 0){
				if(auton == 0) auton = 1;
				auton = abs(auton);
				lv_btnm_set_toggle(redBtnm, true, abs(auton)-1);
			}else if(activeTab == 1){
				if(auton == 0) auton = -1;
				auton = -abs(auton);
				lv_btnm_set_toggle(blueBtnm, true, abs(auton)-1);
			}else{
				auton = 0;
			}
		}

		pros::delay(20);
	}
}

void init(int hue, int default_auton, const char **autons){

	int i = 0;
	do{
		memcpy(&btnmMap[i], &autons[i], sizeof(&autons));
		i++;
	}while(strcmp(autons[i], "") != 0);

	autonCount = i;
	auton = default_auton;

	// lvgl theme
	lv_theme_t *th = lv_theme_alien_init(hue, NULL); //Set a HUE value and keep font default RED
	lv_theme_set_current(th);

	// create a tab view object
	tabview = lv_tabview_create(lv_scr_act(), NULL);

	// add 3 tabs (the tabs are page (lv_page) and can be scrolled
	lv_obj_t *redTab = lv_tabview_add_tab(tabview, "Red");
	lv_obj_t *blueTab = lv_tabview_add_tab(tabview, "Blue");
	lv_obj_t *skillsTab = lv_tabview_add_tab(tabview, "Skills");

	//set default tab
	if(auton < 0){
		lv_tabview_set_tab_act(tabview, 1, LV_ANIM_NONE);
	}else if(auton == 0){
		lv_tabview_set_tab_act(tabview, 2, LV_ANIM_NONE);
	}

	// add content to the tabs
	// button matrix
	redBtnm = lv_btnm_create(redTab, NULL);
	lv_btnm_set_map(redBtnm, btnmMap);
	lv_btnm_set_action(redBtnm, redBtnmAction);
	lv_btnm_set_toggle(redBtnm, true, abs(auton)-1);//3
	lv_obj_set_size(redBtnm, 450, 50);
	lv_obj_set_pos(redBtnm, 0, 100);
	lv_obj_align(redBtnm, NULL, LV_ALIGN_CENTER, 0, 0);

	// blue tab
	blueBtnm = lv_btnm_create(blueTab, NULL);
	lv_btnm_set_map(blueBtnm, btnmMap);
	lv_btnm_set_action(blueBtnm, *blueBtnmAction);
	lv_btnm_set_toggle(blueBtnm, true, abs(auton)-1);
	lv_obj_set_size(blueBtnm, 450, 50);
	lv_obj_set_pos(blueBtnm, 0, 100);
	lv_obj_align(blueBtnm, NULL, LV_ALIGN_CENTER, 0, 0);

	// skills tab
	lv_obj_t *skillsBtn = lv_btn_create(skillsTab, NULL);
	lv_obj_t *label = lv_label_create(skillsBtn, NULL);
	lv_label_set_text(label, "Skills");
	lv_btn_set_action(skillsBtn, LV_BTN_ACTION_CLICK, *skillsBtnAction);
	// lv_btn_set_state(skillsBtn, LV_BTN_STATE_TGL_REL);
	lv_obj_set_size(skillsBtn, 450, 50);
	lv_obj_set_pos(skillsBtn, 0, 100);
	lv_obj_align(skillsBtn, NULL, LV_ALIGN_CENTER, 0, 0);

	// start tab watcher
	pros::Task tabWatcher_task(tabWatcher);

}

} // namespace selector


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

Current Team Goals?
1. Score higher than 108

How do I drive this robot?
Left joystick is forward back to move; right joystick to turn
L1 is lower arms; L2 to raise
R1 is spinners in; R2 is spinners out

What does autonomous do?
Score 1 ball; go to next post; score 2nd ball

*/
Controller controller;
ControllerButton armsUp (ControllerDigital::L1);
ControllerButton armsDown (ControllerDigital::L2);
ControllerButton spinnerIntake (ControllerDigital::R1);
ControllerButton spinnerOuttake (ControllerDigital::R2);

Motor liftLeft(8);
Motor liftRight(-10);
Motor spinLeft(1);
Motor spinRight(-4);
Motor leftDrive(2);
Motor rightDrive(-9);

void initialize()
{
selector::init(HUE, DEFAULT, selector::b);
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

  rightDrive.setBrakeMode(AbstractMotor::brakeMode::brake);
  leftDrive.setBrakeMode(AbstractMotor::brakeMode::brake);

  liftLeft.setBrakeMode(AbstractMotor::brakeMode::brake);
  liftRight.setBrakeMode(AbstractMotor::brakeMode::brake);

std::shared_ptr<ChassisController> drive =
   ChassisControllerBuilder()
       .withMotors(leftDrive, rightDrive)
.withDimensions(AbstractMotor::gearset::green, {{4_in, 15_in}, imev5GreenTPR})
.build();

//drive->moveDistance(4.25_in);

if(selector::auton == 1){ //skills auton

autoArmRaiseLower(1700, 'r');

drive->moveDistance(9.5_in);

autoArmRaiseLower(500, 'r');

drive->turnAngle(10_deg);

drive->moveDistance(2_in);

autoSpinnerInOut(2000, 'o');

drive->moveDistance(-8_in);

autoArmRaiseLower(1000, 'l');

drive->turnAngle(170_deg);
}

if(selector::auton == -1){
	autoArmRaiseLower(1700, 'r');

	drive->moveDistance(9.5_in);

	autoArmRaiseLower(500, 'r');

	drive->turnAngle(-10_deg);

	drive->moveDistance(2_in);

	autoSpinnerInOut(2000, 'o');

	drive->moveDistance(-8_in);

	autoArmRaiseLower(1000, 'l');

	drive->turnAngle(-170_deg);
}

// drive->moveDistance();



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
rightDrive.setBrakeMode(AbstractMotor::brakeMode::brake);
leftDrive.setBrakeMode(AbstractMotor::brakeMode::brake);

liftLeft.setBrakeMode(AbstractMotor::brakeMode::brake);
liftRight.setBrakeMode(AbstractMotor::brakeMode::brake);


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
