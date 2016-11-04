#include "stdafx.h"
#include "m01_01_dans_data_input.h"

#include "m01_03_save_raw_positions.h"
#include "io_datainput.h"
#include "m01_01_mapcoord.h"


using namespace std;

/*

Data input scheme for data collected and formatted as described in the FreeBody user manual

*/

void m01_01_dans_data_input(Segment **segment_data[], Structure *calibrate_pos[], Mat_DP &dynamic_marker, Mat_DP &force_data,
							int dynamic_frames, double freq, int segments,  XML_Parameters* _parameters)
{
	
	int dummy=0, force_count;
	int dist_end[5] = {-1,0,1,2,2};
	double  force_scale_units = _parameters-> force_scale_units;

	
	// Input coordinate system mapping (from lab to model coordinate system)

	for (int i=0; i<dynamic_frames; i++) 
	{
		for (int j=0; j<segments; j++) 
		{
			segment_data[i][j]->dist=dist_end[j];
//			segment_data[i][segment_data[i][j]->dist]->prox=j;
		}
	}
	for (int j=0; j<segments; j++) 
	{
		calibrate_pos[j]->dist=dist_end[j];
//		calibrate_pos[calibrate_pos[j]->dist]->prox=j;
	}
	
	// Input external force info
	Vec_DP external_forces_info=_parameters->ex_force_info;	

	for (int i=0; i<dynamic_frames; i++) 
	{											// based upon the settings give in coord_map.txt
		for (int j=0; j<3; j++) 
		{
			force_data[i][j]=force_data[i][j]*force_scale_units;                         
		}
	}

	/*ofstream outfile("dynamic_data_new.txt", std::ios_base::app);

	for (int ii=0; ii<dynamic_frames; ii++)
	{
		for (int jj=0; jj<54; jj++)
		{
			outfile<<dynamic_marker[ii][jj]<<" ";
		}	
		outfile<<endl;

	}	
	outfile.close();*/

	/*ofstream outfile("force_data_new.txt", std::ios_base::app);

	for (int ii=0; ii<dynamic_frames; ii++)
	{
		for (int jj=0; jj<6; jj++)
		{
			outfile<<force_data[ii][jj]<<" ";
		}	
		outfile<<endl;

	}	
	outfile.close();*/
  
	m01_03_save_raw_positions(dynamic_marker,segment_data,dynamic_frames,_parameters);

	for (int i=0; i<dynamic_frames; i++) 
	{						// Save raw data to Segment objects
		force_count=0;
		for (int j=0; j<segments; j++) 
		{
			
			segment_data[i][j]->time=i/freq;
			segment_data[i][j]->ex_forces=(int) external_forces_info[j];


			for (int k=force_count; k<force_count+segment_data[i][j]->ex_forces; k++) 
			{
				for (int l=0; l<6; l++) 
				{
					segment_data[i][j]->external_forces[k-force_count][l]=force_data[i][6*k+l];

				}
			}
			force_count+=segment_data[i][j]->ex_forces;
		    
		}
	}

	return;
}