#include "main.h"
#include "functions.hpp"
#include "pros/misc.h"
#pragma

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
  static bool pressed = false;
  pressed = !pressed;
  if (pressed) {
    pros::lcd::set_text(2, "I was pressed!");
  } else {
    pros::lcd::clear_line(2);
  }
}
//
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  pros::lcd::initialize();
  pros::lcd::set_text(1, "Hello noah! I am watching you ");
  imu.reset(true);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  pros::delay(20);
  mogoMech.retract();
  pros::lcd::set_text(1, "auto initialized");
  pidMove(-920);
  pidTurn(145);
  movefb(-1.3, 100, 800);
  mogoMech.toggle();
  movelr(-0.1, 100, 200);
  movefb(0.7, 600, 400);
  pidTurn(270);
  intake.move(200);
  pros::delay(900);
  mogoMech.toggle();
  movefb(-0.1, 400, 500);
  movefb(2, 400, 1000);
  pidTurn(40);
  movefb(-2.3, 100, 1000);
  mogoMech.toggle();
  pidTurn(90);
  movefb(2, 300, 2000);
  pidTurn(270);
  movefb(4, 200, 2000);
}
/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connec:w
 * ted, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  std::string headingstr = "killing myself";
  arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  bool intakeToggle = false;
  bool canFlap = true;
  bool canMogo = true;
  bool canIntake = true;
  std::string odostr = "i hate this";
  while (true) {
    // Arcade control scheme
    int dir = master.get_analog(
        ANALOG_LEFT_Y); // Gets amount forward/backward from left joystick
    int turn = master.get_analog(
        ANALOG_RIGHT_X);       // Gets the turn left/right from right joystick
    left_mg.move(dir + turn);  // Sets left motor voltage
    right_mg.move(dir - turn); // Sets right motor voltage
    if (master.get_digital(
            pros::E_CONTROLLER_DIGITAL_R2)) { // intake stuff with toggle
      intakeToggle = false;
      intake.move(-200);
    } else if (master.get_digital(
                   pros::E_CONTROLLER_DIGITAL_R1)) { // intake stuff with toggle
      intakeToggle = false;
      intake.move(200);
    } else {
      if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) && canIntake) {
        canIntake = false;
        if (!intakeToggle) {
          intakeToggle = true;
        } else if (intakeToggle) {
          intakeToggle = false;
        }
      } else if (!master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
        canIntake = true;
      }
      if (intakeToggle) { // intake toggle basic logic
        intake.move(200);
      } else {
        intake.move(0);
      }
    }
    if (master.get_digital(
            pros::E_CONTROLLER_DIGITAL_L1)) { // arm logic and stuff
      arm.move(100);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
      arm.move(-100);
    } else {
      arm.move(0);
    }
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) &&
        canMogo) { // mogoMech toggle
      mogoMech.toggle();
      canMogo = false;
    } else if (!master.get_digital(pros::E_CONTROLLER_DIGITAL_A)) {
      canMogo = true;
    }
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B) &&
        canFlap) { // mogoMech toggle
      flap.toggle();
      canFlap = false;
    } else if (!master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) {
      canFlap = true;
    }
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
      odoRotation.reset_position();
    }
    headingstr = std::to_string(imu.get_heading());
    odostr = std::to_string(odoRotation.get_position());
    pros::lcd::set_text(2, headingstr);
    pros::lcd::set_text(3, odostr);
    pros::delay(20); // Run for 20 ms then update
  }
}
