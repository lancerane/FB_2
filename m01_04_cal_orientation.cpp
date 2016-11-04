#include "stdafx.h"
#include "m01_04_cal_orientation.h"

#include "io_outop.h"

using namespace std;
/*
Calculate subject's orientation in the calibiration process and in the dynamic process with respect to the orientation defined by ISB coordinate system
Written by Ziyun Ding
ISB coordinate system's direction:
X: anterior-posterior;
Y: superior-inferior;
Z: lateral-medial.

*/

void m01_04_cal_orientation(double mean_pelvis_width, double mean_shank_length, Vec_DP &RASIS, Vec_DP &LASIS, Vec_DP &FLE, Vec_DP &FAM, int orientation[3])
{


	Vec_DP current_pelvis_vector(3), current_shank_vector(3);


	for (int ii=0; ii<3; ii++)
	{
		current_pelvis_vector[ii] = RASIS[ii]-LASIS[ii];
		current_shank_vector[ii] = FLE[ii]-FAM[ii];
	}


	double width_current_min = abs(abs(current_pelvis_vector[0])-mean_pelvis_width);

    int width_index_min = 0;
	for (int ii = 1; ii<3; ii++)
	{
      if (abs(abs(current_pelvis_vector[ii])-mean_pelvis_width) < width_current_min)

      {
               width_current_min = abs(abs(current_pelvis_vector[ii]) - mean_pelvis_width);
               width_index_min = ii;
      }
	}

	double length_current_min = abs(abs(current_shank_vector[0])-mean_shank_length);
    int length_index_min = 0;
	for (int ii = 1; ii<3; ii++)
	{
      if (abs(abs(current_shank_vector[ii])-mean_shank_length) < length_current_min)

      {
               length_current_min = abs(abs(current_shank_vector[ii]) - mean_shank_length);
               length_index_min = ii;
      }
	}

	orientation[1] = length_index_min;
	orientation[2] = width_index_min;
	orientation[0] = 3-orientation[1]-orientation[2];

	//cout<<orientation[0]<<endl;
	//cout<<orientation[1]<<endl;
	//cout<<orientation[2]<<endl;


	return ;
}


void m01_04_cal_orientation(double mean_pelvis_width, double mean_shank_length, Vec_DP &RASIS, Vec_DP &LASIS, int y_index, int orientation[3])
{


	Vec_DP current_pelvis_vector(3), current_shank_vector(3);


	for (int ii=0; ii<3; ii++)
	{
		current_pelvis_vector[ii] = RASIS[ii]-LASIS[ii];
		
	}


	double width_current_min = abs(abs(current_pelvis_vector[0])-mean_pelvis_width);

    int width_index_min = 0;
	for (int ii = 1; ii<3; ii++)
	{
      if (abs(abs(current_pelvis_vector[ii])-mean_pelvis_width) < width_current_min)

      {
               width_current_min = abs(abs(current_pelvis_vector[ii]) - mean_pelvis_width);
               width_index_min = ii;
      }
	}



	orientation[1] = y_index;
	orientation[2] = width_index_min;
	orientation[0] = 3-orientation[1]-orientation[2];

	//cout<<orientation[0]<<endl;
	//cout<<orientation[1]<<endl;
	//cout<<orientation[2]<<endl;





	return ;
}