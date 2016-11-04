#include "stdafx.h"
#include "m06_04_output_muscle_geometry.h"
#include "io_dataoutput.h"





using namespace std;

/*



*/

void m06_04_output_muscle_geometry(Muscle **muscle_data[], int frames, int muscle, string output_folder_for_visualisation, string study_name)
{

	int pnts=muscle_data[0][muscle]->pnts;
	Mat_DP out(frames,(pnts)*3);
	Mat_DP out_2(frames,1);
	Mat_DP out_3(frames, 1);
	Mat_DP out_4(frames, 1);
	string name_test;
	

	for (int i=0; i<frames; i++) 
	{

		for (int j=0; j<pnts; j++) 
		{

			for (int k=0; k<3; k++) 
			{

				out[i][3*j+k]=muscle_data[i][muscle]->gblpnts[j][k];
				out_2[i][0] = muscle_data[i][muscle]->length_change; //gives length change at each frame
				out_3[i][0] = muscle_data[i][muscle]->length; //length at each frame
				name_test = muscle_data[i][muscle]->name; //muscle name
				out_4[i][0] = muscle_data[i][muscle]->l_v_factor;
			}

		}
	}


	string filename0=output_folder_for_visualisation +"\\" + study_name + "_" + "muscle_path" + to_string((long long)muscle) + ".csv";
	io_dataoutput(filename0, out);

	string filename1 = output_folder_for_visualisation + "\\" + study_name + "_" + "muscle_velocity" + to_string((long long)muscle) + ".csv";
	io_dataoutput(filename1, out_2);

	//string filename2 = output_folder_for_visualisation + "\\" + study_name + "_" + "muscle_length" + to_string((long long)muscle) + ".csv";
	//io_dataoutput(filename2, out_3);

	string filename2 = output_folder_for_visualisation + "\\" + study_name + "_" + "muscle_length" + "_" + name_test  + ".csv";
	io_dataoutput(filename2, out_3);

	string filename3 = output_folder_for_visualisation + "\\" + study_name + "_" + "factors" +  "_" + name_test + ".csv";
	io_dataoutput(filename3, out_4);

	return;
}