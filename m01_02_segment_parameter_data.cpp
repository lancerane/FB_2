// Input Data.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "m01_02_segment_parameter_data.h"


#include "io_datainput.h"
#include "m01_01_mapcoord.h"
#include "m01_03_save_raw_positions.h"
#include "io_outop.h"

using namespace std;

/*

Calculate segment parameters based on DE LEVA's method
Alternative methods for segment parameter estimations should be implemented here  

Re-writtedn by Ziyun Ding 2015

*/

void m01_02_segment_parameter_data(Structure *calibrate_pos[], int segments, XML_Parameters* _parameters)
{

	//////////////////////////////////////////////////////////////////////////////////////////
	////////*****************************************************************/////////////////
    /* DE LEVA METHOD OF SEGMENT PARAMETER ESTIMATION
		
			Reference: de Leva, P. (1996).  "Adjustments to 
			Zatsiorsky-Seluyanov's segment inertia parameters."  
			Journal of Biomechanics, 29(9), 1223-1230.
		
			Coded here by Ziyun Ding August 2014.
			
			Notes:

				ROG = radius of gyration

				segment LCS x-axis = segment's sagittal axis
				segment LCS y-axis = segment's longitudinal axis
				segment LCS z-axis = segment's transverse axis
	*/

	if ( _parameters->subject_sex == "female")
	{
		//**********FEMALE************
        // FEMALE FOOT
		calibrate_pos[0]->mass   = _parameters->subject_mass *0.0129;
		calibrate_pos[0]->com[1] = 0.4014;
		calibrate_pos[0]->com[0] = calibrate_pos[0]->com[2] = 0;
		double assumed_foot_length	 = _parameters->subject_height * 0.132;
		double foot_saggital_ROG	 = assumed_foot_length * 0.299; 
		double foot_longitudinal_ROG = assumed_foot_length * 0.139; 
		double foot_transverse_ROG	 = assumed_foot_length * 0.279; 
		calibrate_pos[0]->inertia[0] =  foot_saggital_ROG * foot_saggital_ROG * calibrate_pos[0]->mass;
		calibrate_pos[0]->inertia[1] =  foot_longitudinal_ROG * foot_longitudinal_ROG * calibrate_pos[0]->mass;
		calibrate_pos[0]->inertia[2] =  foot_transverse_ROG * foot_transverse_ROG * calibrate_pos[0]->mass;

		// FEMALE SHANK
		calibrate_pos[1]->mass   = _parameters->subject_mass *0.048;
		calibrate_pos[1]->com[1] = 0.4416;
		calibrate_pos[1]->com[0] = calibrate_pos[0]->com[2] = 0;
		double assumed_shank_length	  = _parameters->subject_height * 0.249;
		double shank_saggital_ROG	  = calibrate_pos[1]->com[1] * 0.271; 
		double shank_longitudinal_ROG = calibrate_pos[1]->com[1] * 0.093; 
		double shank_transverse_ROG	  = calibrate_pos[1]->com[1] * 0.267; 
		calibrate_pos[1]->inertia[0]  =  shank_saggital_ROG * shank_saggital_ROG * calibrate_pos[1]->mass;
		calibrate_pos[1]->inertia[1]  =  shank_longitudinal_ROG * shank_longitudinal_ROG * calibrate_pos[1]->mass;
		calibrate_pos[1]->inertia[2]  =  shank_transverse_ROG * shank_transverse_ROG * calibrate_pos[1]->mass;

		//FEMALE THIGH
		calibrate_pos[2]->mass   = _parameters->subject_mass *0.148;
		calibrate_pos[2]->com[1] = 0.3612;
		calibrate_pos[2]->com[0] = calibrate_pos[2]->com[2] = 0;
		double assumed_thigh_length   = _parameters->subject_height * 0.395;
		double thigh_saggital_ROG	  = assumed_thigh_length * 0.369; 
		double thigh_longitudinal_ROG = assumed_thigh_length * 0.162; 
		double thigh_transverse_ROG	  = assumed_thigh_length * 0.364; 
		calibrate_pos[2]->inertia[0]  =  thigh_saggital_ROG * thigh_saggital_ROG * calibrate_pos[2]->mass;
		calibrate_pos[2]->inertia[1]  =  thigh_longitudinal_ROG * thigh_longitudinal_ROG * calibrate_pos[2]->mass;
		calibrate_pos[2]->inertia[2]  =  thigh_transverse_ROG * thigh_transverse_ROG * calibrate_pos[2]->mass;

		//FEMALE PELVIS
		calibrate_pos[3]->mass   = _parameters->subject_mass *0.125;
		calibrate_pos[3]->com[1] = 0.4920;
		calibrate_pos[3]->com[0] = calibrate_pos[3]->com[2] = 0;
		double assumed_pelvis_length   = _parameters->subject_height * 0.105;
		double pelvis_saggital_ROG	   = assumed_pelvis_length * 0.433; 
		double pelvis_longitudinal_ROG = assumed_pelvis_length * 0.444; 
		double pelvis_transverse_ROG   = assumed_pelvis_length * 0.402; 
		calibrate_pos[3]->inertia[0] =  pelvis_saggital_ROG * pelvis_saggital_ROG * calibrate_pos[3]->mass;
		calibrate_pos[3]->inertia[1] =  pelvis_longitudinal_ROG * pelvis_longitudinal_ROG * calibrate_pos[3]->mass;
		calibrate_pos[3]->inertia[2] =  pelvis_transverse_ROG * pelvis_transverse_ROG * calibrate_pos[3]->mass;

		//FEMALE PATELLA
		calibrate_pos[4]->mass   = 0;
		calibrate_pos[4]->com[1] = 0;
		calibrate_pos[4]->com[0] = calibrate_pos[4]->com[2] = 0;
		calibrate_pos[4]->inertia[0] =  0;
		calibrate_pos[4]->inertia[1] =  0;
		calibrate_pos[4]->inertia[2] =  0;
	}
	
	else
	{
		//**********MALE************
        // MALE FOOT
		calibrate_pos[0]->mass   = _parameters->subject_mass *0.0137;
		calibrate_pos[0]->com[1] = 0.4415;
		calibrate_pos[0]->com[0] = calibrate_pos[0]->com[2] = 0;
		double assumed_foot_length	 = _parameters->subject_height * 0.148;
		double foot_saggital_ROG	 = assumed_foot_length * 0.257; 
		double foot_longitudinal_ROG = assumed_foot_length * 0.124; 
		double foot_transverse_ROG	 = assumed_foot_length * 0.245; 
		calibrate_pos[0]->inertia[0] =  foot_saggital_ROG * foot_saggital_ROG * calibrate_pos[0]->mass;
		calibrate_pos[0]->inertia[1] =  foot_longitudinal_ROG * foot_longitudinal_ROG * calibrate_pos[0]->mass;
		calibrate_pos[0]->inertia[2] =  foot_transverse_ROG * foot_transverse_ROG * calibrate_pos[0]->mass;

		// MALE SHANK
		calibrate_pos[1]->mass   = _parameters->subject_mass *0.043;
		calibrate_pos[1]->com[1] = 0.4459;
		calibrate_pos[1]->com[0] = calibrate_pos[0]->com[2] = 0;
		double assumed_shank_length	  = _parameters->subject_height * 0.249;
		double shank_saggital_ROG	  = calibrate_pos[1]->com[1] * 0.255; 
		double shank_longitudinal_ROG = calibrate_pos[1]->com[1] * 0.103; 
		double shank_transverse_ROG	  = calibrate_pos[1]->com[1] * 0.249; 
		calibrate_pos[1]->inertia[0]  =  shank_saggital_ROG * shank_saggital_ROG * calibrate_pos[1]->mass;
		calibrate_pos[1]->inertia[1]  =  shank_longitudinal_ROG * shank_longitudinal_ROG * calibrate_pos[1]->mass;
		calibrate_pos[1]->inertia[2]  =  shank_transverse_ROG * shank_transverse_ROG * calibrate_pos[1]->mass;

		//MALE THIGH
		calibrate_pos[2]->mass   = _parameters->subject_mass *0.142;
		calibrate_pos[2]->com[1] = 0.4095;
		calibrate_pos[2]->com[0] = calibrate_pos[2]->com[2] = 0;
		double assumed_thigh_length   = _parameters->subject_height * 0.243;
		double thigh_saggital_ROG	  = assumed_thigh_length * 0.329;
		double thigh_longitudinal_ROG = assumed_thigh_length * 0.149; 
		double thigh_transverse_ROG	  = assumed_thigh_length * 0.329; 
		calibrate_pos[2]->inertia[0]  =  thigh_saggital_ROG * thigh_saggital_ROG * calibrate_pos[2]->mass;
		calibrate_pos[2]->inertia[1]  =  thigh_longitudinal_ROG * thigh_longitudinal_ROG * calibrate_pos[2]->mass;
		calibrate_pos[2]->inertia[2]  =  thigh_transverse_ROG * thigh_transverse_ROG * calibrate_pos[2]->mass;

		//MALE PELVIS
		calibrate_pos[3]->mass   = _parameters->subject_mass *0.112;
		calibrate_pos[3]->com[1] = 0.6115;
		calibrate_pos[3]->com[0] = calibrate_pos[3]->com[2] = 0;
		double assumed_pelvis_length   = _parameters->subject_height * 0.084;
		double pelvis_saggital_ROG	   = assumed_pelvis_length * 0.615; 
		double pelvis_longitudinal_ROG = assumed_pelvis_length * 0.551; 
		double pelvis_transverse_ROG   = assumed_pelvis_length * 0.587; 
		calibrate_pos[3]->inertia[0] =  pelvis_saggital_ROG * pelvis_saggital_ROG * calibrate_pos[3]->mass;
		calibrate_pos[3]->inertia[1] =  pelvis_longitudinal_ROG * pelvis_longitudinal_ROG * calibrate_pos[3]->mass;
		calibrate_pos[3]->inertia[2] =  pelvis_transverse_ROG * pelvis_transverse_ROG * calibrate_pos[3]->mass;

		//MALE PATELLA
		calibrate_pos[4]->mass   = 0;
		calibrate_pos[4]->com[1] = 0;
		calibrate_pos[4]->com[0] = calibrate_pos[4]->com[2] = 0;
		calibrate_pos[4]->inertia[0] =  0;
		calibrate_pos[4]->inertia[1] =  0;
		calibrate_pos[4]->inertia[2] =  0;
	}

	cout<<calibrate_pos[1]->mass<<endl;
	cout<<calibrate_pos[1]->inertia[0] <<calibrate_pos[1]->inertia[1] <<calibrate_pos[1]->inertia[2] <<endl;



	return;
}