#include "PID.h"
#include <limits>

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

#define INVALID_CTE std::numeric_limits<double>::lowest()

void PID::Init(double Kp, double Ki, double Kd) {
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    cte_sum = 0;
    p_error = i_error = d_error = 0;
    last_cte = INVALID_CTE;
}

void PID::UpdateError(double cte) {

    p_error = cte;
    i_error = cte_sum;
    d_error = last_cte==INVALID_CTE ? 0 : cte - last_cte;  //assuming constant time difference between CTE samples

    cte_sum += cte;
    last_cte = cte;
}

double PID::TotalError() {
    return - Kp * p_error - Ki * i_error - Kd * d_error;
}

