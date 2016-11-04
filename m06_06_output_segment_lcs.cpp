#include "stdafx.h"
#include "m06_06_output_segment_lcs.h"
#include "io_dataoutput.h"


using namespace std;

/*



*/

void m06_06_output_segment_lcs(Segment **segment_data[], int frames, int segments, string output_folder_for_optimisation, string study_name)
{


	Mat_DP quaternion(frames,4*segments);
	
	for (int i=0; i<frames; i++) 
	{
		for (int j=0; j<segments; j++) 
		{
			for (int k=0; k<4; k++) 
			{
				quaternion[i][4*j+k]=segment_data[i][j]->rot_lcs_to_gcs[k];

			}
		}
	}

	io_dataoutput(output_folder_for_optimisation + "\\" + study_name + "_"+ "lcs_quaternion.csv",quaternion);

	

	return;
}