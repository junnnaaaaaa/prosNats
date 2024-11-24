#include "main.h"
#pragma once
pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::MotorGroup
    left_mg({-4, 3, -11}, pros::v5::MotorGears::blue,
            pros::v5::MotorUnits::rotations); // Creates a motor group with
// forwards
pros::MotorGroup right_mg(
    {8, -9, 19}, pros::v5::MotorGears::blue,
    pros::v5::MotorUnits::rotations); // Creates a motor group with forwards
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
pros::IMU imu(20);
void movefb(float amnt, int spd, int wait) {
  left_mg.move_relative(amnt, spd);
  right_mg.move_relative(amnt, spd);
  pros::delay(wait);
}
void movelr(float amnt, int spd, int wait) {
  left_mg.move_relative(amnt, spd);
  right_mg.move_relative(-amnt, spd);
  pros::delay(wait);
}
void pid(float targ) {
  float error;
  float inter;
  float preverror;
  float deriv;
  float head;
  float kp = 0.5556;
  while (true) {
    head = imu.get_heading();
    error = targ - head;
    if (error > 180) {
      error = error - 360;
    }
    if (error < -180) {
      error = error + 360;
    }
    left_mg.move(error * kp);
    right_mg.move((error * kp) * -1);
  }
}
