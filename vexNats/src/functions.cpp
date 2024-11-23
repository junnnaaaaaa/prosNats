#include "functions.h"
#include "main.h"
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
void movefb(int amnt, int spd) {
  left_mg.move_relative(amnt, spd);
  right_mg.move_relative(amnt, spd);
}
void movelr(int amnt, int spd) {
  left_mg.move_relative(amnt, spd);
  right_mg.move_relative(amnt, -spd);
}
