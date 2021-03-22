#include "main.h"

void on_center_button() {
    static bool pressed = false;
    pressed = !pressed;
    if (pressed) {
        pros::lcd::set_text(2, "I was pressed!");
    } else {
        pros::lcd::clear_line(2);
    }
}

#define HUE 360
#define DEFAULT 3
#define AUTONS "One Goal", "Two Goals", "Three Goals"

namespace selector{

const char *b[] = {AUTONS, ""};
int auton;
int autonCount;
int color;
const char *btnmMap[] = {"","","","","","","","","","",""}; // up to 10 autons

lv_obj_t *tabview;
lv_obj_t *redBtnm;
lv_obj_t *blueBtnm;
lv_obj_t *colorBtnm;

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

lv_res_t colorBtnmAction(lv_obj_t *btnm, const char *txt)
{
	//printf("blue button: %s released\n", txt);

	for(int i = 0; i < autonCount; i++){
		if(strcmp(txt, btnmMap[i]) == 0){
			color = (i+1);
		}
	}

  return LV_RES_OK;
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
	lv_obj_t *redTab = lv_tabview_add_tab(tabview, "Left");
	lv_obj_t *blueTab = lv_tabview_add_tab(tabview, "Right");
	lv_obj_t *skillsTab = lv_tabview_add_tab(tabview, "Skills");
  lv_obj_t *colorTab = lv_tabview_add_tab(tabview, "Colors");

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
  //
  colorBtnm = lv_btnm_create(colorTab, NULL);
  	lv_btnm_set_map(colorBtnm, btnmMap);
  	lv_btnm_set_action(colorBtnm, colorBtnmAction);
  	lv_btnm_set_toggle(colorBtnm, true, abs(color)-1);//3
  	lv_obj_set_size(colorBtnm, 450, 50);
  	lv_obj_set_pos(colorBtnm, 0, 100);
  	lv_obj_align(colorBtnm, NULL, LV_ALIGN_CENTER, 0, 0);

	// start tab watcher
	pros::Task tabWatcher_task(tabWatcher);

}

} // namespace selector

std::shared_ptr<OdomChassisController> autoChassis =
    ChassisControllerBuilder()
        .withMotors ({18, 19}, //left motors
										{-9, -17})  //right motors
        .withDimensions(AbstractMotor::gearset::green, {{3.125_in, 12.75_in}, imev5GreenTPR}) //Blue, 4in wheel, 12.5 in wheel track
        .withSensors(ADIEncoder{'A', 'B', false}, //Right
                     ADIEncoder{'C', 'D', true}) //Left
        .withOdometry({{2.75_in, 4.5_in}, quadEncoderTPR}) //2.75in with distance of 4.5 apart
        .withMaxVelocity(225) //cap velocity at 300 to reduce jerk and cut down on PID correction time
        .buildOdometry();

std::shared_ptr<ChassisController> chassis =
   ChassisControllerBuilder()
   .withMotors ({18, 19}, //left motors
               {-9, -17})  //right motors
.withDimensions(AbstractMotor::gearset::blue, {{3.125_in, 12.75_in}, imev5GreenTPR})
// .withMaxVelocity(600)
.build();

  std::shared_ptr<AsyncMotionProfileController> profileController =
    AsyncMotionProfileControllerBuilder()
      .withLimits({
        1.0, //Maximum linear velocity in m/s
        2.0, //Maximum linear acceleration in m/s/s
        10.0 //Maximum linear jerk in m/s/s/s
      })
      .withOutput(autoChassis)
      .buildMotionProfileController();

    void autoIntake(int a){
      leftIntake.moveVelocity(a);
      rightIntake.moveVelocity(a);
    }

    void autoIndexThree(int a){
      indexer3.moveVelocity(a);
    }

    void autoIndexTop(int a){
      indexerTop.moveVelocity(a);
    }

void initialize() {
  selector::init(HUE, DEFAULT, selector::b);
  // pros::lcd::initialize();
  // pros::lcd::set_text(1, "Hello PROS User!");
  //
  // pros::lcd::register_btn1_cb(on_center_button);

  inertialSensor.reset(); //Calibrates IMU
  while(inertialSensor.is_calibrating()){
    pros::delay(20);
  }

  frontRight.setBrakeMode(AbstractMotor::brakeMode::hold);
  frontLeft.setBrakeMode(AbstractMotor::brakeMode::hold);
  backRight.setBrakeMode(AbstractMotor::brakeMode::hold);
  backLeft.setBrakeMode(AbstractMotor::brakeMode::hold);
}

void disabled() {}

void competition_initialize() {
  inertialSensor.reset();
  while(inertialSensor.is_calibrating()){
    pros::delay(20);
  }
}

void autonomous() {

  if(selector::auton == 1){
    profileController->generatePath(
       {{0_ft, 0_ft, 0_deg}, {2_ft, 0_ft, 0_deg}}, "moveOne");
     profileController->setTarget("moveOne");
     indexerTop.moveVelocity(600);
     pros::delay(500);
     indexerTop.moveVelocity(0);
     profileController->waitUntilSettled();

     turn(230, 'l', 120);

     profileController->generatePath(
        {{0_ft, 0_ft, 0_deg}, {2.25_ft, 0_ft, 0_deg}}, "goalOne");
      profileController->setTarget("goalOne");
      autoIntake(400);
      profileController->waitUntilSettled();

      autoIntake(0);

      autoIndexThree(600);
      autoIndexTop(600);
      pros::delay(1000);
      autoIndexTop(0);
      autoIndexThree(0);

     profileController->generatePath(
       {{0_ft, 0_ft, 0_deg}, {2.4_ft, 0_ft, 0_deg}}, "backOne");
     profileController->setTarget("backOne", true);
     autoIntake(-200);
     profileController->waitUntilSettled();
     autoIntake(0);
  }

  if(selector::auton == -1){
    profileController->generatePath(
       {{0_ft, 0_ft, 0_deg}, {1_ft, 0_ft, 0_deg}}, "moveOne");
     profileController->setTarget("moveOne");
     indexerTop.moveVelocity(600);
     pros::delay(500);
     indexerTop.moveVelocity(0);
     profileController->waitUntilSettled();

     turn(98, 'r', 120);

     profileController->generatePath(
        {{0_ft, 0_ft, 0_deg}, {1.7_ft, 0_ft, 0_deg}}, "goalOne");
      profileController->setTarget("goalOne");
      autoIntake(400);
      profileController->waitUntilSettled();

      autoIntake(0);

      autoIndexThree(600);
      autoIndexTop(600);
      pros::delay(1550);
      autoIndexTop(0);
      autoIndexThree(200);

      profileController->generatePath(
         {{0_ft, 0_ft, 0_deg}, {1.35_ft, 0_ft, 0_deg}}, "backOne");
       profileController->setTarget("backOne", true);
       autoIntake(-400);
       profileController->waitUntilSettled();
       autoIntake(0);
       autoIndexThree(0);
  }

  if(selector::auton == -2){
    profileController->generatePath(
       {{0_ft, 0_ft, 0_deg}, {1_ft, 0_ft, 0_deg}}, "moveOne");
     profileController->setTarget("moveOne");
     indexerTop.moveVelocity(600);
     pros::delay(500);
     indexerTop.moveVelocity(0);
     profileController->waitUntilSettled();

     turn(98, 'r', 120);

     profileController->generatePath(
        {{0_ft, 0_ft, 0_deg}, {1.7_ft, 0_ft, 0_deg}}, "goalOne");
      profileController->setTarget("goalOne");
      autoIntake(400);
      profileController->waitUntilSettled();

      autoIntake(0);

      autoIndexThree(600);
      autoIndexTop(600);
      pros::delay(550);
      autoIndexTop(0);
      autoIndexThree(200);

      profileController->generatePath(
         {{0_ft, 0_ft, 0_deg}, {1.35_ft, 0_ft, 0_deg}}, "backOne");
       profileController->setTarget("backOne", true);
       autoIntake(-400);
       profileController->waitUntilSettled();
       autoIntake(0);
       autoIndexThree(0);

       turn(258, 'r', 120);

       profileController->generatePath(
          {{0_ft, 0_ft, 0_deg}, {3.7_ft, 0_ft, 0_deg}}, "forwardTwo");
        profileController->setTarget("forwardTwo");
        profileController->waitUntilSettled();

        turn(193, 'l', 120);

        profileController->generatePath(
           {{0_ft, 0_ft, 0_deg}, {0.5_ft, 0_ft, 0_deg}}, "goalTwo");
         profileController->setTarget("goalTwo");
         profileController->waitUntilSettled();

         autoIndexThree(600);
         autoIndexTop(600);
         pros::delay(1400);
         autoIndexTop(0);
         autoIndexThree(0);

         profileController->generatePath(
            {{0_ft, 0_ft, 0_deg}, {2.2_ft, 0_ft, 0_deg}}, "backTwo");
          profileController->setTarget("backTwo", true);
          profileController->waitUntilSettled();
  }

  if(selector::auton == -3){
  profileController->generatePath(
     {{0_ft, 0_ft, 0_deg}, {1_ft, 0_ft, 0_deg}}, "moveOne");
   profileController->setTarget("moveOne");
   indexerTop.moveVelocity(600);
   pros::delay(500);
   indexerTop.moveVelocity(0);
   profileController->waitUntilSettled();

   turn(98, 'r', 120);

   profileController->generatePath(
      {{0_ft, 0_ft, 0_deg}, {1.7_ft, 0_ft, 0_deg}}, "goalOne");
    profileController->setTarget("goalOne");
    autoIntake(400);
    profileController->waitUntilSettled();

    autoIntake(0);

    autoIndexThree(600);
    autoIndexTop(600);
    pros::delay(550);
    autoIndexTop(0);
    autoIndexThree(200);

    profileController->generatePath(
       {{0_ft, 0_ft, 0_deg}, {1.35_ft, 0_ft, 0_deg}}, "backOne");
     profileController->setTarget("backOne", true);
     autoIntake(-400);
     profileController->waitUntilSettled();
     autoIntake(0);
     autoIndexThree(0);

     turn(258, 'r', 120);

     profileController->generatePath(
        {{0_ft, 0_ft, 0_deg}, {3.7_ft, 0_ft, 0_deg}}, "forwardTwo");
      profileController->setTarget("forwardTwo");
      profileController->waitUntilSettled();

      turn(193, 'l', 120);

      profileController->generatePath(
         {{0_ft, 0_ft, 0_deg}, {0.5_ft, 0_ft, 0_deg}}, "goalTwo");
       profileController->setTarget("goalTwo");
       profileController->waitUntilSettled();

       autoIndexThree(600);
       autoIndexTop(600);
       pros::delay(400);
       autoIndexTop(0);
       autoIndexThree(0);

       profileController->generatePath(
          {{0_ft, 0_ft, 0_deg}, {2.2_ft, 0_ft, 0_deg}}, "backTwo");
        profileController->setTarget("backTwo", true);
        profileController->waitUntilSettled();

        turn(215, 'r', 120);
        indexerTop.setBrakeMode(AbstractMotor::brakeMode::coast);

        profileController->generatePath(
           {{0_ft, 0_ft, 0_deg}, {6.5_ft, 0_ft, 0_deg}}, "goalThree");
         profileController->setTarget("goalThree");
         autoIntake(600);
         autoIndexTop(600);
         autoIndexThree(600);
         profileController->waitUntilSettled();

         autoIntake(0);
       }

   if(selector::auton == 3){
     profileController->generatePath(
        {{0_ft, 0_ft, 0_deg}, {2_ft, 0_ft, 0_deg}}, "moveOne");
      profileController->setTarget("moveOne");
      indexerTop.moveVelocity(600);
      pros::delay(500);
      indexerTop.moveVelocity(0);
      profileController->waitUntilSettled();

      turn(230, 'l', 120);

      profileController->generatePath(
         {{0_ft, 0_ft, 0_deg}, {2.25_ft, 0_ft, 0_deg}}, "goalOne");
       profileController->setTarget("goalOne");
       autoIntake(400);
       profileController->waitUntilSettled();

       autoIntake(0);

       autoIndexThree(600);
       autoIndexTop(600);
       pros::delay(550);
       autoIndexTop(0);
       autoIndexThree(0);

      profileController->generatePath(
        {{0_ft, 0_ft, 0_deg}, {2.4_ft, 0_ft, 0_deg}}, "backOne");
      profileController->setTarget("backOne", true);
      autoIntake(-200);
      profileController->waitUntilSettled();
      autoIntake(0);

      turn(350.5, 'r', 120);

      profileController->generatePath(
        {{0_ft, 0_ft, 0_deg}, {2.8_ft, 0_ft, 0_deg}}, "moveTwo");
      profileController->setTarget("moveTwo");
      autoIntake(400);
      profileController->waitUntilSettled();

      autoIntake(0);

      profileController->generatePath(
        {{0_ft, 0_ft, 0_deg}, {2.75_ft, 0_ft, 0_deg}}, "backTwo");
      profileController->setTarget("backTwo", true);
      profileController->waitUntilSettled();

      turn(27, 'r', 120);

      profileController->generatePath(
        {{0_ft, 0_ft, 0_deg}, {4.5_ft, 0_ft, 0_deg}}, "goalTwo");
      profileController->setTarget("goalTwo");
      profileController->waitUntilSettled();

      autoIntake(200);
      autoIndexThree(600);
      autoIndexTop(600);

      pros::delay(1250);

      profileController->generatePath(
        {{0_ft, 0_ft, 0_deg}, {4.25_ft, 0_ft, 0_deg}}, "backThree");
      profileController->setTarget("backThree", true);
      profileController->waitUntilSettled();
   }

   if(selector::auton == 2){
     autoIndexTop(600);
     pros::delay(1000);
     autoIndexTop(0);
   }
}

void opcontrol() {

    while (true) {

      frontRight.setBrakeMode(AbstractMotor::brakeMode::coast);
      frontLeft.setBrakeMode(AbstractMotor::brakeMode::coast);
      backRight.setBrakeMode(AbstractMotor::brakeMode::coast);
      backLeft.setBrakeMode(AbstractMotor::brakeMode::coast);

      chassis->getModel()->arcade(controller.getAnalog(ControllerAnalog::leftY),
                             controller.getAnalog(ControllerAnalog::rightX));

      index();
      intake();
      intakeOut();
      eject();
      deploy();

      pros::lcd::set_text(2, autoChassis->getState().str().c_str());


     		pros::delay(20);
      }
    }
