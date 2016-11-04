// Input Data.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "kinematics.h"
#include "m01_00_data_input.h"
#include "m01_01_dans_data_input.h"
#include "m01_02_dans_calibration_data.h"
#include "m01_01_mapcoord.h"
#include "m01_04_cal_orientation.h"
#include "m01_05_cal_direction.h"
#include "mathematics.h"

using namespace std;

/*

Select the data input scheme to use

This routine is included to increase the extensibility of the model.  Should other data input routines
be required, they can be called here, and the existing data input routines switched off.

*/

void m01_00_data_input(Segment **segment_data[], Structure *calibrate_pos[], int dynamic_frames, double freq,
					   int segments, XML_Parameters* _parameters)
{
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int static_frames, total_marker_number, marker_column, RASIS_column, LASIS_column, RPSIS_column, FLE_column, FAM_column;
	Vec_DP mean_pelvis_width_vector(3), mean_shank_length_vector(3);
	Vec_DP RASIS_static(3), LASIS_static(3),RPSIS_static(3), FLE_static(3), FAM_static(3); 
	Vec_DP RASIS_dynamic(3), LASIS_dynamic(3), RPSIS_dynamic(3), FLE_dynamic(3), FAM_dynamic(3);
	int  *static_orientation = new int[3];
	int  *dynamic_orientation = new int[3];
	int  *static_sign = new int[3];
	int  *dynamic_sign = new int[3];
	double marker_scale;

	marker_scale = _parameters->marker_scale_units;

	string calibration_full_name, dynamic_full_name, ex_force_full_name;

	calibration_full_name = _parameters->static_trial_directory_name + "\\" +_parameters->static_marker_file_name;

	dynamic_full_name = _parameters->dynamic_trial_directory_name + "\\" +_parameters->dynamic_marker_file_name;

	ex_force_full_name = _parameters->ex_force_directory_name+"\\"+_parameters->ex_force_file_name;

	RASIS_column = _parameters->marker_per_segment[3].marker_column[0]-1;
	LASIS_column = _parameters->marker_per_segment[3].marker_column[1]-1;
	RPSIS_column =_parameters->marker_per_segment[3].marker_column[2]-1;
	FAM_column = _parameters->marker_per_segment[1].marker_column[0]-1;
	FLE_column = _parameters->marker_per_segment[2].marker_column[0]-1;

	total_marker_number = 0;
	for (int ii=0; ii<4; ii++)
	{
		total_marker_number = total_marker_number+ _parameters->marker_per_segment[ii].marker_per_segment_num;
	}

	marker_column = total_marker_number*3;

	static_frames=_parameters->static_end_frame_number - _parameters->static_start_frame_number + 1;

	Mat_DP mean_calibration(1,marker_column), calibration_data(static_frames, marker_column), dynamic_data(dynamic_frames,marker_column), ex_force_data(dynamic_frames,6);

	calibration_data = io_datainput(static_frames, marker_column, calibration_full_name);
	dynamic_data = io_datainput(dynamic_frames, marker_column, dynamic_full_name);
	ex_force_data = io_datainput(dynamic_frames, 6, ex_force_full_name);


	MATHEMATICS::math_zeromatrix(mean_calibration);

	for (int i=0; i<static_frames; i++)
	{
		for (int j=0; j<marker_column; j++)
		{
			mean_calibration[0][j] = mean_calibration[0][j]+calibration_data[i][j];
		}
	}

	for (int ii=0; ii<marker_column; ii++) 
	{
		mean_calibration[0][ii] = mean_calibration[0][ii]/static_frames; 
	} 

	for (int ii=0; ii<3; ii++)
	{
		mean_pelvis_width_vector[ii] = mean_calibration[0][RASIS_column+ii]-mean_calibration[0][LASIS_column+ii];
		mean_shank_length_vector[ii] = mean_calibration[0][FLE_column+ii]-mean_calibration[0][FAM_column+ii];
		
	}

	double mean_pelvis_width = MATHEMATICS::math_vecmag(mean_pelvis_width_vector);
	double mean_shank_length =  MATHEMATICS::math_vecmag(mean_shank_length_vector);



	for(int ii=0; ii<3; ii++)
	{
		RASIS_static[ii] = mean_calibration[0][RASIS_column+ii];
		LASIS_static[ii] = mean_calibration[0][LASIS_column+ii];
		FLE_static[ii] = mean_calibration[0][FLE_column+ii];
		FAM_static[ii] = mean_calibration[0][FAM_column+ii];
		RASIS_dynamic[ii] = dynamic_data[0][RASIS_column+ii];
		LASIS_dynamic[ii] = dynamic_data[0][LASIS_column+ii];
		FLE_dynamic[ii] = dynamic_data[0][FLE_column+ii];
		FAM_dynamic[ii] = dynamic_data[0][FAM_column+ii];

	}



	m01_04_cal_orientation(mean_pelvis_width,  mean_shank_length, RASIS_static, LASIS_static, FLE_static, FAM_static, static_orientation);
	m01_04_cal_orientation(mean_pelvis_width,  mean_shank_length, RASIS_dynamic, LASIS_dynamic, static_orientation[1], dynamic_orientation);
	m01_01_mapcoord(mean_calibration, static_orientation);
	m01_01_mapcoord(dynamic_data, dynamic_orientation);
	m01_01_mapcoord(ex_force_data, dynamic_orientation);


	for(int ii=0; ii<3; ii++)
	{
		RASIS_static[ii] = mean_calibration[0][RASIS_column+ii];
		LASIS_static[ii] = mean_calibration[0][LASIS_column+ii];
		RPSIS_static[ii] = mean_calibration[0][RPSIS_column+ii];
		FAM_static[ii] = mean_calibration[0][FAM_column+ii];
		RASIS_dynamic[ii] = dynamic_data[0][RASIS_column+ii];
		LASIS_dynamic[ii] = dynamic_data[0][LASIS_column+ii];
		RPSIS_dynamic[ii] = dynamic_data[0][RPSIS_column+ii];
		FAM_dynamic[ii] = dynamic_data[0][FAM_column+ii];

	}

	m01_05_cal_direction(RASIS_static, LASIS_static, RPSIS_static, FAM_static, static_sign);
	m01_05_cal_direction(RASIS_dynamic, LASIS_dynamic, RPSIS_dynamic, static_sign[1], dynamic_sign);



	m01_01_mapcoord(mean_calibration, static_sign, marker_scale);
	m01_01_mapcoord(dynamic_data, dynamic_sign, marker_scale);
	m01_01_mapcoord(ex_force_data, dynamic_sign, 1);

	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m01_01_dans_data_input(segment_data, calibrate_pos, dynamic_data, ex_force_data, dynamic_frames, freq, segments,  _parameters);



	// Input calibration data

	// The existing model is calibrated based upon the consideration of one static frame.  Should an alternative
	// method be desired (for instance based upon the consideration of multiple static frames and average data) it 
	// can be called here.

	m01_02_dans_calibration_data(calibrate_pos,segments, _parameters, mean_calibration);


	cout << "1.";

	return;
}

/*	Auditing

m01_00_data_input.cpp					DC - checked 18/12/13
m01_01_dans_data_input.cpp				DC - checked 20/12/13
m01_01_mapcoord.cpp						DC - checked 18/12/13
m01_02_dans_calibration_data.cpp		DC - checked 20/12/13
m01_03_save_raw_positions				DC - checked 20/12/13

*/