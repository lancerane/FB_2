#include "stdafx.h"
#include <stdio.h>   
#include <math.h>  

/*

Calculate factor for determination of muscle force upper bound given length and velocity properties

*/
void Muscle::mus_length_velocity_factors(int muscle) {

	double horseman_opt_lengths[163] = { 0.095, 0.095, 0.104, 0.104, 0.112, 0.112, 0.106, 0.106, 0.106, 0.106, 0.106, 0.106, 0.108, 0.108, 0.108, 0.104, 0.104, 0.104, 0.104, 0.104, 0.104, 0.107, 0.107, 0.107, 0.107, 0.085, 0.091, 0.091, 0.091, 0.06, 0.06, 0.06, 0.06, 0.06, 0.06, 0.038, 0.038, 0.038, 0.026, 0.026, 0.026, 0.057, 0.06, 0.034, 0.034, 0.12, 0.12, 0.12, 0.12, 0.12, 0.12, 0.151, 0.151, 0.151, 0.151, 0.151, 0.151, 0.038, 0.038, 0.038, 0.038, 0.038, 0.038, 0.045, 0.045, 0.045, 0.045, 0.045, 0.045, 0.028, 0.034, 0.037, 0.181, 0.181, 0.103, 0.103, 0.103, 0.052, 0.052, 0.052, 0.089, 0.089, 0.089, 0.069, 0.069, 0.028, 0.028, 0.028, 0.021, 0.021, 0.021, 0.115, 0.115, 0.115, 0.115, 0.027, 0.027, 0.027, 0.034, 0.034, 0.034, 0.043, 0.043, 0.043, 0.039, 0.048, 0.024, 0.024, 0.059, 0.099, 0.099, 0.099, 0.034, 0.034, 0.034, 0.034, 0.078, 0.078, 0.347, 0.347, 0.081, 0.142, 0.024, 0.024, 0.024, 0.026, 0.026, 0.026, 0.095, 0.095, 0.046, 0.046, 0.046, 0.024, 0.024, 0.024, 0.024, 0.024, 0.024, 0.077, 0.077, 0.077, 0.077, 0.077, 0.077, 0.042, 0.042, 0.042, 0.042, 0.042, 0.042, 0.091, 0.091, 0.076, 0.076, 0.076, 0.076, 0.083, 0.083, 0.083, 0.083, 0.083, 0.083 };
	
	double horseman_tendon_lengths[163];
	std::fill_n(horseman_tendon_lengths, 163, 0.1);

	this->optimal_length = horseman_opt_lengths[muscle];//*((_parameters->subject_height)/1.74);
		this->tendon_length = horseman_tendon_lengths[muscle];//*((_parameters->subject_height) / 1.74);

	int v_factor = 4.0;

	double optimal = this->optimal_length;
	double tendon = this->tendon_length;
	double length = this->length;
	double v_max = optimal*v_factor;

	double f_l = exp(-17.33);//*(fabs(((length - tendon) / optimal) - 1) ^ 3));
	double f_v = 1.8 - ((1.8) / (1 + exp(0.04 - (length - tendon) / (v_max - optimal) / 0.18)));

	this->l_v_factor = f_l*f_v;




	return;


}
