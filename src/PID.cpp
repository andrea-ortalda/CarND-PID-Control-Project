#include "PID.h"

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_)
{

  //Initialize PID coefficients (and errors, if needed)
  this->p_error = 0.0;
  this->i_error = 0.0;
  this->d_error = 0.0;
  this->Kp = Kp_;
  this->Ki = Ki_;
  this->Kd = Kd_;
}

void PID::UpdateError(double cte)
{

  //Update PID errors based on cte.
  // d_error
  d_error = cte - p_error;

  // i_error
  i_error += cte;

  //p_error
  p_error = cte;
}

double PID::TotalError()
{

  //Calculate and return the total error
  double tot_error = (-Kp * p_error) + (-Ki * i_error) + (-Kd * d_error);
  return tot_error;
}