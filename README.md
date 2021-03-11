# 242coderepo
All code used for the Firebirds VEX robots. Also included in this repo will be robot/controller setups, and team objectives.

# Using Auton Selector
1. Copy the code from the AutonSelector.c file, and paste it in your main.ccp file directly below the first line, include('main.h').

2. Add this line to your initialize function:

    void initialize() {
	    selector::init(HUE, DEFAULT, selector::b);
    }

3. To use, the buttons, set up different auton code in IF statements inside the autonomous() {} function. For example:

    - selector::auton == 1 : Red Front
    - selector::auton == 2 : Red Back
    - selector::auton == 3 : Do Nothing
    - selector::auton == -1 : Blue Front
    - selector::auton == -2 : Blue Back
    - selector::auton == -3 : Do Nothing
    - selector::auton == 0 : Skills

  if (selector::auton == 1) {
  //run this code
  
  } else if (selector::auton == 2) {
  //run this code
  
  }

3. For more information, visit: <http://github.com/kunwarsahni01/Vex-Autonomous-Selector>
