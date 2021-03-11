#include "main.h"

#define HUE 96
#define DEFAULT 1
#define AUTONS "one", "two", "Do Nothing"

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
	lv_obj_t *redTab = lv_tabview_add_tab(tabview, "left");
	lv_obj_t *blueTab = lv_tabview_add_tab(tabview, "right");
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
----motorports----
port desc		  port desc
1		 				  11 BackRight
2						  12
3						  13 frontright
4	leftspinny	14
5 rightspinny 15 rightarmupdown
6						  16
7						  17
8 backleft	  18
9						  19 leftarmupdown
10 frontleft  20

----ADI----
A				E
B				F
C				G
D				H
*/

void initialize() {
	selector::init(HUE, DEFAULT, selector::b);
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
				// Green gearset, 4 in wheel diam, 11.5 in whe	el track
				.withDimensions(AbstractMotor::gearset::green, {{4_in, 13_in}, imev5GreenTPR})
				.build();

if(selector::auton == 1){ //left side one goal
	// duo auton
	myBaseDrive->setMaxVelocity(150);
	pros::delay(1000);
	myBaseDrive->moveDistance (24_in);
	pros::delay(500);
	myBaseDrive->turnAngle(-155_deg);
	pros::delay(500);
	myBaseDrive->moveDistance (15_in);
	myLift.moveVelocity(200);
	pros::delay(1000);
	pros::delay(1000);
	myBaseDrive->moveDistance (6.5_in);
	spinny.moveVelocity(200);
	pros::delay(1000);
	spinny.moveVelocity(0);
	myBaseDrive->moveDistance (-34_in);
	//myBaseDrive->turnAngle(-135_deg);
	//myBaseDrive->moveDistance (20.5_in);
	//myLift.moveVelocity(0);
}

if(selector::auton == 2){ //left side two goals
	// duo auton
	myBaseDrive->setMaxVelocity(150);
	pros::delay(1000);
	myBaseDrive->moveDistance (24_in);
	pros::delay(500);
	myBaseDrive->turnAngle(-155_deg);
	pros::delay(500);
	myBaseDrive->moveDistance (15_in);
	myLift.moveVelocity(200);
	pros::delay(1000);
	pros::delay(1000);
	myBaseDrive->moveDistance (6.5_in);
	spinny.moveVelocity(200);
	pros::delay(1000);
	spinny.moveVelocity(0);
	myBaseDrive->moveDistance (-34_in);
	myBaseDrive->turnAngle(-135_deg);
	myBaseDrive->moveDistance (20.5_in);
	myLift.moveVelocity(0);
}

if(selector::auton == -1){ //left side one goal
	// duo auton
	myBaseDrive->setMaxVelocity(150);
	pros::delay(1000);
	myBaseDrive->moveDistance (24_in);
	pros::delay(500);
	myBaseDrive->turnAngle(155_deg);
	pros::delay(500);
	myBaseDrive->moveDistance (15_in);
	myLift.moveVelocity(200);
	pros::delay(1000);
	pros::delay(1000);
	myBaseDrive->moveDistance (6.5_in);
	spinny.moveVelocity(200);
	pros::delay(1000);
	spinny.moveVelocity(0);
	myBaseDrive->moveDistance (-34_in);
	//myBaseDrive->turnAngle(-135_deg);
	//myBaseDrive->moveDistance (20.5_in);
	//myLift.moveVelocity(0);
}

if(selector::auton == 0){ //
myBaseDrive->setMaxVelocity(150);
//up
myLift.moveVelocity(200);
pros::delay(1000);
myLift.moveVelocity(0);
//forewoards
myBaseDrive->moveDistance (6_in);
//spin ball
spinny.moveVelocity(200);
pros::delay(500);
spinny.moveVelocity(0);

myBaseDrive->moveDistance (-9_in);
pros::delay(1000);
myBaseDrive->turnAngle(195_deg);
pros::delay(1000);
myBaseDrive->moveDistance (-7_in);
pros::delay(1000);
myLift.moveVelocity(-200);
pros::delay(1000);
myLift.moveVelocity(-200);
pros::delay(1000);
myLift.moveVelocity(0);
spinny.moveVelocity(-200);
pros::delay(500);
myBaseDrive->setMaxVelocity(200);
myBaseDrive->moveDistance (30_in);
spinny.moveVelocity(0);
pros::delay(1000);
myBaseDrive->turnAngle(100_deg);
pros::delay(1000);
myLift.moveVelocity(200);
pros::delay(1000);
myLift.moveVelocity(0);
myBaseDrive->moveDistance (24_in);
spinny.moveVelocity(200);
pros::delay(1000);
spinny.moveVelocity(0);
pros::delay(1000);
myBaseDrive->moveDistance (-10_in);
pros::delay(1000);
myBaseDrive->turnAngle(-130_deg);
myLift.moveVelocity(-200);
pros::delay(1000);
myLift.moveVelocity(0);
spinny.moveVelocity(-200);
pros::delay(1000);
myBaseDrive->moveDistance (46_in);
spinny.moveVelocity(200);
pros::delay(1000);
spinny.moveVelocity(0);
pros::delay(1000);
myBaseDrive->turnAngle(30_deg);
myBaseDrive->moveDistance (24_in);
spinny.moveVelocity(200);
pros::delay(500);
spinny.moveVelocity(0);
}


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
        // Green gearset, 4 in wheel diam, 11.5 in whe	el track
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
