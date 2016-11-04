#include "stdafx.h"
#include "m06_03_output_opt_values_3.h"
#include "io_dataoutput.h"





using namespace std;

/*

Output all parameters required for the optimization

*/

void m06_03_output_opt_values_3(Segment **segment_data[], Muscle **muscle_data[], Muscle *patellar_tendon[],
							  Muscle **ligament_data[], int frames, int segments, int muscles, int ligaments, string output_folder_for_optimisation, string study_name)
{

	Mat_DP m0(frames,muscles), m1(frames,muscles), m2(frames,muscles);
	Mat_DP m3(frames,muscles), m4(frames,muscles), m5(frames,muscles);
	Mat_DP l0(frames,ligaments), l1(frames,ligaments), l2(frames,ligaments);
	Mat_DP l3(frames,ligaments), l4(frames,ligaments), l5(frames,ligaments);
	Mat_DP pat_tendon(frames,9);
	Mat_DP rhs(frames,6*(segments-2)), force_ub(frames,muscles), ligaments_ub(frames,ligaments), ratio(frames,muscles);
	Mat_DP contact(frames,21);

	MATHEMATICS::math_zeromatrix(rhs);
	MATHEMATICS::math_zeromatrix(force_ub);
	MATHEMATICS::math_zeromatrix(ligaments_ub);
	MATHEMATICS::math_zeromatrix(ratio);
	MATHEMATICS::math_zeromatrix(contact);

	// Output muscular geometry values for optimization

	for (int i=0; i<segments; i++) {

		MATHEMATICS::math_zeromatrix(m0);
		MATHEMATICS::math_zeromatrix(m1);
		MATHEMATICS::math_zeromatrix(m2);
		MATHEMATICS::math_zeromatrix(m3);
		MATHEMATICS::math_zeromatrix(m4);
		MATHEMATICS::math_zeromatrix(m5);

		for (int j=0; j<frames; j++) {

			for (int k=0; k<muscles; k++) {

				for (int l=0; l<6; l++) {

					if ((l<3) && (int (muscle_data[j][k]->opt_2[l][3]) == i)) {

						m0[j][k]=muscle_data[j][k]->opt_2[l][0];
						m1[j][k]=muscle_data[j][k]->opt_2[l][1];
						m2[j][k]=muscle_data[j][k]->opt_2[l][2];
					}
					if ((l>=3) && (int (muscle_data[j][k]->opt_2[l][3]) == i)) {

						m3[j][k]=muscle_data[j][k]->opt_2[l][0];
						m4[j][k]=muscle_data[j][k]->opt_2[l][1];
						m5[j][k]=muscle_data[j][k]->opt_2[l][2];
					}

				}
			}
		}
		// muscle force is force vector
		string filename0=output_folder_for_optimisation + "\\" + study_name + "_"+"muscle_force_seg" + to_string((long long)i) + "0.csv";
		string filename1=output_folder_for_optimisation + "\\" + study_name + "_"+"muscle_force_seg" + to_string((long long)i) + "1.csv";
		string filename2=output_folder_for_optimisation + "\\" + study_name + "_"+"muscle_force_seg" + to_string((long long)i) + "2.csv";
		string filename3=output_folder_for_optimisation + "\\" + study_name + "_"+"muscle_moment_seg" + to_string((long long)i) + "0.csv";
		string filename4=output_folder_for_optimisation + "\\" + study_name + "_"+"muscle_moment_seg" + to_string((long long)i) + "1.csv";
		string filename5=output_folder_for_optimisation + "\\" + study_name + "_"+"muscle_moment_seg" + to_string((long long)i) + "2.csv";

		io_dataoutput(filename0,m0);
		io_dataoutput(filename1,m1);
		io_dataoutput(filename2,m2);
		io_dataoutput(filename3,m3);
		io_dataoutput(filename4,m4);
		io_dataoutput(filename5,m5);


	}
	for (int i=0; i<segments; i++) {

		MATHEMATICS::math_zeromatrix(l0);
		MATHEMATICS::math_zeromatrix(l1);
		MATHEMATICS::math_zeromatrix(l2);
		MATHEMATICS::math_zeromatrix(l3);
		MATHEMATICS::math_zeromatrix(l4);
		MATHEMATICS::math_zeromatrix(l5);

		for (int j=0; j<frames; j++) {

			for (int k=0; k<ligaments; k++) {

				for (int l=0; l<6; l++) {

					if ((l<3) && (int (ligament_data[j][k]->opt_2[l][3]) == i)) {

						l0[j][k]=ligament_data[j][k]->opt_2[l][0];
						l1[j][k]=ligament_data[j][k]->opt_2[l][1];
						l2[j][k]=ligament_data[j][k]->opt_2[l][2];
					}
					if ((l>=3) && (int (ligament_data[j][k]->opt_2[l][3]) == i)) {

						l3[j][k]=ligament_data[j][k]->opt_2[l][0];
						l4[j][k]=ligament_data[j][k]->opt_2[l][1];
						l5[j][k]=ligament_data[j][k]->opt_2[l][2];
					}

				}
			}
		}

		string filename_l0=output_folder_for_optimisation +"\\" + study_name + "_"+"ligament_force_seg" + to_string((long long)i) + "0.csv";
		string filename_l1=output_folder_for_optimisation +"\\" + study_name + "_"+"ligament_force_seg" + to_string((long long)i) + "1.csv";
		string filename_l2=output_folder_for_optimisation +"\\" + study_name + "_"+"ligament_force_seg" + to_string((long long)i) + "2.csv";
		string filename_l3=output_folder_for_optimisation +"\\" + study_name + "_"+"ligament_moment_seg" + to_string((long long)i) + "0.csv";
		string filename_l4=output_folder_for_optimisation +"\\" + study_name + "_"+"ligament_moment_seg" + to_string((long long)i) + "1.csv";
		string filename_l5=output_folder_for_optimisation +"\\" + study_name + "_"+"ligament_moment_seg" + to_string((long long)i) + "2.csv";

		io_dataoutput(filename_l0,l0);
		io_dataoutput(filename_l1,l1);
		io_dataoutput(filename_l2,l2);
		io_dataoutput(filename_l3,l3);
		io_dataoutput(filename_l4,l4);
		io_dataoutput(filename_l5,l5);


	}

	// Patellar tendon geometry

	MATHEMATICS::math_zeromatrix(pat_tendon);

	for (int j=0; j<frames; j++) {
		for (int l=0; l<3; l++) {

			pat_tendon[j][l]=patellar_tendon[j]->opt_2[0][l];
			pat_tendon[j][l+3]=patellar_tendon[j]->opt_2[1][l];
			pat_tendon[j][l+6]=patellar_tendon[j]->opt_2[3][l];

		}
	}
	

	string filename_pt=output_folder_for_optimisation + "\\" + study_name + "_"+"pat_tendon.csv";

	io_dataoutput(filename_pt,pat_tendon);


	// Output right hand side (known values) of equations of motion for optimization


	for (int i=0; i<frames; i++) {

		for (int j=0; j<segments-2; j++) {

			for (int l=0; l<3; l++) {

				rhs[i][3*j+l]=segment_data[i][j]->rhs_opt_approach_2[l];
				rhs[i][3*(segments-2)+3*j+l]=segment_data[i][j]->rhs_opt_approach_2[l+3];
			}
		}
	}

	string filename_rhs=output_folder_for_optimisation +"\\" + study_name + "_"+"rhs.csv";

	io_dataoutput(filename_rhs,rhs);

	// Output force upper bounds of muscle elements for optimization
	////////**********************************************************/////////////////////////////////////////////////////
	for (int i=0; i<frames; i++) 
	{

		for (int j=0; j<muscles; j++) 
		{

			force_ub[i][j]=muscle_data[i][j]->max_force;

		}
	}
	////////**********************************************************/////////////////////////////////////////////////////
	string filename_force_ub=output_folder_for_optimisation +"\\" + study_name + "_"+"force_ub.csv";

	io_dataoutput(filename_force_ub,force_ub);

	// Output force upper bounds of ligament elements for optimization

	for (int i=0; i<frames; i++) 
	{

		for (int j=0; j<ligaments; j++) 
		{

			ligaments_ub[i][j]=ligament_data[i][j]->max_force;

		}
	}

	string filename_ligament_ub=output_folder_for_optimisation +"\\" + study_name + "_"+"ligament_ub.csv";

	io_dataoutput(filename_ligament_ub,ligaments_ub);

	// Output P/Q ratio for optimization

	for (int i=0; i<frames; i++) 
	{

		for (int j=0; j<muscles; j++) 
		{

			ratio[i][j]=muscle_data[i][j]->pq_ratio;

		}
	}

	string filename_ratios=output_folder_for_optimisation +"\\" + study_name + "_"+"ratios.csv";

	io_dataoutput(filename_ratios,ratio);

	// Position of joint contacts for optimization

	for (int i=0; i<frames; i++) 
	{

		for (int j=0; j<3; j++) 
		{

			contact[i][j]=segment_data[i][2]->vec_rot_centre_to_rf[1][j];
			contact[i][j+3]=segment_data[i][1]->vec_rot_centre_to_rf[0][j];
			contact[i][j+6]=segment_data[i][1]->vec_rot_centre_to_rf[2][j];
			contact[i][j+9]=segment_data[i][1]->vec_rot_centre_to_rf[3][j];
			contact[i][j+12]=segment_data[i][2]->vec_rot_centre_to_rf[2][j];
			contact[i][j+15]=segment_data[i][2]->vec_rot_centre_to_rf[3][j];
			contact[i][j+18]=segment_data[i][1]->rot_centre[j]-segment_data[i][2]->rot_centre[j];
		}
	}

	string filename_contact=output_folder_for_optimisation +"\\" + study_name + "_"+"joint_contacts.csv";

	io_dataoutput(filename_contact,contact);

	// Output orientations of segments

	Vec_DP q(4);

	Mat_DP gcs_to_lcs(frames,4*segments);

	for (int i=0; i<frames; i++) 
	{
		for (int j=0; j<segments; j++) 
		{
			for (int k=0; k<4; k++) 
			{
				q[k]=segment_data[i][j]->rot_lcs_to_gcs[k];
			}
			q=MATHEMATICS::math_qcon(q);
			for (int k=0; k<4; k++) 
			{
				gcs_to_lcs[i][4*j+k]=q[k];
			}
		}
	}

	io_dataoutput(output_folder_for_optimisation +"\\" + study_name + "_"+"rotations.csv",gcs_to_lcs);



	return;
}