#include "stdafx.h"
#include <stdio.h>   
#include <math.h>  

/*

Calculate factor for determination of muscle force upper bound given length and velocity properties
Calculate factor for determination of muscle force lower bound from passive element given length properties

*/
void Muscle::mus_length_velocity_factors(int muscle) {



	double optimal = this->optimal_length; 
	double fibre_length = this->length - this->tendon_length;

	double velocity = this->length_change;
	double v_max = this->optimal_length * 4;

	this->f_l = exp(-17.33*(pow(abs(((fibre_length) / optimal) - 1), 3.0))); //checked: f_l values good
	this->f_v = 1.8 - (1.8 / (1 + (exp((0.04 - (velocity / v_max*optimal)) / 0.18)))); // checked f_v values good
	
	this->active_force_factor = f_l*f_v;

	this->passive_force_factor = 30000 * exp(6 * (((fibre_length) / optimal) - 3.2));


	return;


}
