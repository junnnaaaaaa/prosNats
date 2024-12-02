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
pros::Motor arm(-14, pros::v5::MotorGears::red,
                pros::v5::MotorUnits::rotations);
// mogomech: port g
pros::adi::Pneumatics mogoMech('g', false, true);
// flap: port h
pros::adi::Pneumatics flap('h', false);
// inertia moment: port 20
pros::IMU imu(20);
// me when i rotate: port 18
pros::Rotation odoRotation(-18);
void movefb(float amnt, int spd, int time) {
  const double finalLeftPos = amnt - left_mg.get_position();
  const double finalRightPos = amnt - right_mg.get_position();
  left_mg.move_relative(amnt, spd);
  right_mg.move_relative(amnt, spd);
  pros::delay(time);
}
void movelr(float amnt, int spd, int wait) {
  left_mg.move_relative(amnt, spd);
  right_mg.move_relative(-amnt, spd);
  pros::delay(wait);
}
void pidTurn(float targ) {
  float error;
  float inter = 0;
  float preverror = 0;
  float deriv;
  float head;
  float kp = 0.01;
  float ki = 0.2;
  float kd = 1.1;
  float power = 100;
  float dt = 20;
  int count = 0;
  std::string headstr = "kill me please";
  std::string derivstr = "kill me please";
  deriv = 2;
  while (true) {
    headstr = std::to_string(imu.get_heading());
    head = imu.get_heading();
    error = targ - head;
    if (error > 180) {
      error = error - 360;
    }
    if (error < -180) {
    https: // www.vexrobotics.com/276-7417.html
      error = error + 360;
    }
    if (error > -2 && error < 2) {
      inter = error;
    }
    if (inter > 150 | inter < -150) {
      inter = 0;
    }
    inter += error;
    deriv = error - preverror;
    power = error * kp + inter * ki + deriv * kd;
    left_mg.move(power);
    right_mg.move(-power);
    pros::lcd::set_text(2, headstr);
    derivstr = std::to_string(deriv);
    pros::lcd::set_text(3, derivstr);
    derivstr = std::to_string(power);
    pros::lcd::set_text(4, derivstr);
    if (std::abs(power) < 2.5 || std::abs(deriv) < 2.5) {
      count += 1;
      if (count > 4) {
        pros::lcd::set_text(4, "broken");
        break;
      }
    }
    pros::delay(dt);
  }

  pros::lcd::set_text(5, "turn complete");
}
void pidMove(float targ) {
  float error, deriv, amt, inter;
  float preverror = 0;
  float kp = 0.001;
  float ki = 0.2;
  float kd = 0.7;
  float power = 100;
  float dt = 20;
  int count = 0;
  odoRotation.reset_position();
  std::string amtstr = "killing myself";
  std::string errorstr = "killing myself";
  targ = (targ / 220.0) * 360.0;
  while (true) {
    amt = odoRotation.get_position();
    amt = amt / 100.0;
    amtstr = std::to_string(amt);
    error = targ - amt;
    errorstr = std::to_string(error);
    if (std::abs(inter) > 100) {
      inter = 0;
    }
    inter += error;
    deriv = error - preverror;
    power = error * kp + inter * ki + deriv * kd;
    left_mg.move(power * 0.6);
    right_mg.move(power * 0.6);
    pros::lcd::set_text(2, amtstr);
    pros::lcd::set_text(3, errorstr);
    pros::delay(dt);
  }
}
