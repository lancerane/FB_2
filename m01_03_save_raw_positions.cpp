// Input Data.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "m01_03_save_raw_positions.h"

#include "io_outop.h"

using namespace std;


/*
  Save marker data collected in the dynamic process into segment_data[frame][segment]->marker_pos frame by frame
  Re-written by Ziyun Ding
*/
void m01_03_save_raw_positions(Mat_DP &raw_data, Segment **segment_data[], int dynamic_frames, XML_Parameters* _parameters)
{

	int column_num, dynamic_marker_used, marker_count;

	for (int i=0; i<dynamic_frames; i++) 	     // Save raw data to Segment objects
	{						
	
		for (int j=0; j<4; j++) 
		{
			
			dynamic_marker_used = 0;

			marker_count = _parameters->marker_per_segment[j].marker_per_segment_num;

			for (int kk=0; kk<marker_count; kk++) 
			{
				if (_parameters->marker_per_segment[j].marker_used_in_dynamic[kk])
				{
					column_num = _parameters->marker_per_segment[j].marker_column[kk]-1;

					for (int l=0; l<3; l++) 
					{
						segment_data[i][j]->marker_pos[dynamic_marker_used][l]=raw_data[i][column_num+l];
					}

					dynamic_marker_used = dynamic_marker_used+1;
				}
			}
			segment_data[i][j]->frame=i;
			segment_data[i][j]->segnum=j;
			segment_data[i][j]->markers=dynamic_marker_used;
				
		}
	}

	
	// initialize patella segment_data frame by frame
	for (int ii=0; ii<dynamic_frames; ii++)
	{
		segment_data[ii][4]->frame=ii;
		segment_data[ii][4]->segnum=4;
	}


	/*ofstream outfile("dynamic_data.txt", std::ios_base::app);
	for (int ii=0; ii<dynamic_frames; ii++)
	{
		for (int kk=0; kk<4; kk++)
		{
			int dynamic_marker = segment_data[ii][kk]->markers;

			for (int jj=0; jj<dynamic_marker; jj++)
			{
				for (int ll=0; ll<3; ll++)
				{
					outfile<<segment_data[ii][kk]->marker_pos[jj][ll]<<"";
				}
			}
		}
		
		outfile<<endl;
	}
	outfile.close();*/

	
	return;
}


 /* Save bony landmark data collected in the calibration process into calibrate_pos[segment]->landmarks 
    Save marker calibration data in to calibrate_pos[segment]->marker_pos. 
	If bony landmark is off in the dynamic process, discard its calibration data. 
	Re-written by Ziyun Ding
 */

void m01_03_save_raw_positions(Mat_DP &raw_data, Structure *calibrate_pos[], XML_Parameters* _parameters)
{
	// Detemine the bony landmark for each segment from the static data
	int FM2_column, FAM_column, TAM_column, FLE_column, FME_column, RASIS_column, LASIS_column, RPSIS_column, LPSIS_column;
	int column_num, dynamic_marker_used, marker_count;


	FM2_column = _parameters->marker_per_segment[0].marker_column[0]-1;
	FAM_column = _parameters->marker_per_segment[1].marker_column[0]-1;
	TAM_column = _parameters->marker_per_segment[1].marker_column[1]-1;
	FLE_column = _parameters->marker_per_segment[2].marker_column[0]-1;
	FME_column = _parameters->marker_per_segment[2].marker_column[1]-1;
	RASIS_column = _parameters->marker_per_segment[3].marker_column[0]-1;
	LASIS_column = _parameters->marker_per_segment[3].marker_column[1]-1;
	RPSIS_column = _parameters->marker_per_segment[3].marker_column[2]-1;
	LPSIS_column = _parameters->marker_per_segment[3].marker_column[3]-1;
	
	for (int kk=0; kk<3; kk++)
	{
		// Markers used to define foot coordinate system
		calibrate_pos[0]->landmarks[1][kk+3] = raw_data[0][FM2_column+kk];			// static marker FM2
		calibrate_pos[0]->landmarks[2][kk] = raw_data[0][FAM_column+kk];			// static marker FAM
		calibrate_pos[0]->landmarks[2][kk+3] = raw_data[0][TAM_column+kk];			// static marker TAM

		// Markers used to define shank coordinate system
		calibrate_pos[1]->landmarks[2][kk] = raw_data[0][FLE_column+kk];			// static marker FLE
		calibrate_pos[1]->landmarks[2][kk+3] = raw_data[0][FME_column+kk];			// static marker FME

		// Markers used to define thigh coordinate system
		calibrate_pos[2]->landmarks[2][kk] = raw_data[0][FLE_column+kk];			// static marker FLE
		calibrate_pos[2]->landmarks[2][kk+3] = raw_data[0][FME_column+kk];			// static marker FME

		// Markers used to define pelvis coordinate system
		calibrate_pos[3]->landmarks[0][kk] = raw_data[0][RASIS_column+kk];			// static marker RASIS
		calibrate_pos[3]->landmarks[0][kk+3] = raw_data[0][RPSIS_column+kk];		// static marker RPSIS
		calibrate_pos[3]->landmarks[2][kk] = raw_data[0][RASIS_column+kk];			// static marker RASIS
		calibrate_pos[3]->landmarks[2][kk+3] = raw_data[0][LASIS_column+kk];		// static marker LASIS
	}

	/*ofstream outfile("landmark.txt", std::ios_base::app);
	for (int ii=0; ii<5; ii++)
	{
		for (int jj=0; jj<3; jj++)
		{
			for (int kk=0; kk<6; kk++)
			{
				outfile<<calibrate_pos[ii]->landmarks[jj][kk]<<"";
			}
			outfile<<endl;
		}
		
	}
	outfile.close();*/


	// Determined which markers would be used in dynamic process and save their static data into calibrate_pos[segment]->marker_pos
	for (int j=0; j<4; j++) 
	{
		dynamic_marker_used = 0;

		marker_count = _parameters->marker_per_segment[j].marker_per_segment_num;

		for (int kk=0; kk<marker_count; kk++)
		{
			if (_parameters->marker_per_segment[j].marker_used_in_dynamic[kk])
			{
				column_num = _parameters->marker_per_segment[j].marker_column[kk]-1;

				for(int ll=0; ll<3; ll++)
				{
					calibrate_pos[j]->marker_pos[dynamic_marker_used][ll]=raw_data[0][column_num+ll];
				}

				dynamic_marker_used = dynamic_marker_used+1;
			}
		}
		
		calibrate_pos[j]->frame=0;
		calibrate_pos[j]->segnum=j;
		calibrate_pos[j]->markers=dynamic_marker_used;
				
	}


	for (int j=0; j<4; j++) 
	{
		
		marker_count = _parameters->marker_per_segment[j].marker_per_segment_num;

		for (int kk=0; kk<marker_count; kk++)
		{
			column_num = _parameters->marker_per_segment[j].marker_column[kk]-1;

			for(int ll=0; ll<3; ll++)
			{
				calibrate_pos[j]->static_marker[kk][ll]=raw_data[0][column_num+ll];
			}

		}

		calibrate_pos[j]->calib_marker = marker_count;
	}
		
	

	/*ofstream outfile("static_data_bony_off.txt", std::ios_base::app);
	for (int ii=0; ii<5; ii++)
	{
		for (int jj=0; jj<10; jj++)
		{
			for (int kk=0; kk<3; kk++)
			{
				outfile<<calibrate_pos[ii]->marker_pos[jj][kk]<<"";
			}
			outfile<<endl;
		}
		
	}
	outfile.close();*/


	return;
}