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
  float inter = 0;
  float preverror;
  float deriv;
  float head;
  float kp = 0.05;
  float ki = 0.3;
  float kd = 1;
  float power;
  float dt = 20;
  std::string headstr = "kill me please";
  while (true) {
    headstr = std::to_string(imu.get_heading());
    head = imu.get_heading();
    error = targ - head;
    if (error > 180) {
      error = error - 360;
    }
    if (error < -180) {
      error = error + 360;
    }
    if (error > -2 && error < 2) {
      inter = error;
    }
    if (inter > 175 | inter < -175) {
      inter = 2;
    }
    inter += error;
    deriv = error - preverror;
    power = error * kp + inter * ki + deriv * kd;
    left_mg.move(power);
    right_mg.move(-power);
    pros::lcd::set_text(2, headstr);
    pros::delay(dt);
  }
}
