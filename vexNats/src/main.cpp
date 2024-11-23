#include "main.h"
#include "pros/misc.h"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup left_mg({-4, 3, -11}); // Creates a motor group with forwards
pros::MotorGroup right_mg({8, -9, 20}); // Creates a motor group with forwards
// intake: 12
pros::Motor intake(-12, pros::v5::MotorGears::green,
                   pros::v5::MotorUnits::rotations);
// arm: 13
pros::Motor arm(-13, pros::v5::MotorGears::red,
                pros::v5::MotorUnits::rotations);
// mogomech: port g
pros::adi::Pneumatics mogoMech('g', false, true);
// flap: port h
pros::adi::Pneumatics flap('h', false);
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

  pros::lcd::register_btn1_cb(on_center_button);
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
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

  arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  bool intakeToggle = false;
  bool canFlap = true;
  bool canMogo = true;
  bool canIntake = true;
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
      intake.move(-100);
    } else if (master.get_digital(
                   pros::E_CONTROLLER_DIGITAL_R1)) { // intake stuff with toggle
      intakeToggle = false;
      intake.move(100);
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
        intake.move(100);
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
    pros::delay(20); // Run for 20 ms then update
  }
}
