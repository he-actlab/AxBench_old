
#include <cmath>
#include "kinematics.hpp"


float l1 = 0.5 ;
float l2 = 0.5 ;

void forwardk2j(float theta1, float theta2, float* x, float* y) {
	*x = l1 * cos(theta1) + l2 * cos(theta1 + theta2) ;
	*y = l1 * sin(theta1) + l2 * sin(theta1 + theta2) ;
}

void inversek2j(float x, float y, float* theta1, float* theta2) {
	*theta2 = (float)acos(((x * x) + (y * y) - (l1 * l1) - (l2 * l2))/(2 * l1 * l2)) ;
	*theta1 = (float)asin((y * (l1 + l2 * cos(*theta2)) - x * l2 * sin(*theta2))/(x * x + y * y)) ;
}