#include "stdafx.h"
#include "m02_05_patella_segment.h"
#include "mathematics.h"
#include "io_datainput.h"


using namespace std;

/*

Calculate the position of the patella segment (using the St Mary's knee model)

*/

void m02_05_patella_segment(Segment **segment_data[], Structure *calibrate_pos[], Muscle *patellar_tendon[], int frames, XML_Parameters* _parameters)

{
	Vec_DP tib_tub_ref1(3), tib_tub_ref2(3), quad_ten_ins_ref(3), wrap_point_ref(3), tibia_origin(3), FLE(3), FME(3);
	double pat_ten_length_ref, patella_thickness_ref, patella_radius_ref;
	double knee_flexion_angle, pat_ten_length, pat_ten_sag_ang, pat_ten_cor_ang, scale_pat_ang, scale_pat_ang_dash, sf;
	double pat_ten_strain, pat_flex, pat_tilt, pat_rot;
	double sign_x, sign_z;
	Vec_DP pat_origin(3), euler(3), q_femlcs_to_patlcs(4), q_lcs_to_fem(4), q_lcs_to_pat(4), q_fem_to_lcs(4);
	Vec_DP v0(3), v1(3), v2(3), pat_o_calib(3);
	Mat_DP patella_kinematics(6,6);
	int side_flag;

	MATHEMATICS::math_zeromatrix(patella_kinematics);
	//////////////////////////////////////////////////////////////////////////////////////////
	////////*****************************************************************/////////////////
	//Written by Ziyun Ding, based on research and code by Dan Cleather.
    /* DEFINE PATELLA TENDON KINEMATICS 
		
	Reference: 1) Kobayashi, K., Sakamoto, M., Hosseini, A., Rubash, H. E. & Li, G. 
				In-vivo patellar tendon kinematics during weight-bearing deep knee flexion. 
				J. Orthop. Res. 30, 1596–1603 (2012).
				2) Nha, K. W. et al. 
				In vivo patellar tracking: Clinical motions and patellofemoral indices. 
				J. Orthop. Res. 26, 1067–1074 (2008).
	f is the knee flexion angle
	1. Patella tendon sagittal plane angle: PTsagand = 20.4-0.26 f
	2. Patella tendon coronal plane anlge: PTcorang = 10.88-0.233f+0.00189f^2-0.00000569f^3
	3. Patella tendon strain: PTstrain = 0+0.153f-0.00186f^2+0.00000749f^3
	4. Patella flexion: PTflex= 5.59+0.6601f
	5. Patella tilt: PTtilt= 1.628+0.0667f+0.000144f^2-0.00000537f^3
	6. Patella rotation: PTrot = 1.427+0.1056f-0.00345f^2+0.000054656f^3-0.000000237568f^4		
	*/
	
	patella_kinematics[0][0] = 20.4;  patella_kinematics[0][1] = -0.26;
	patella_kinematics[1][0] = 10.88; patella_kinematics[1][1]= -0.233; patella_kinematics[1][2] = 0.00189;   patella_kinematics[1][3] =-0.00000569;
	patella_kinematics[2][0] = 0;     patella_kinematics[2][1] = 0.153; patella_kinematics[2][2] = -0.00186;  patella_kinematics[2][3] = 0.00000749;  
	patella_kinematics[3][0] = 5.59;  patella_kinematics[3][1] = 0.6601;
	patella_kinematics[4][0] = 1.628; patella_kinematics[4][1] = 0.0667;patella_kinematics[4][2] = 0.000144;  patella_kinematics[4][3]= -0.00000537;
	patella_kinematics[5][0] = 1.427; patella_kinematics[5][1] = 0.1056;patella_kinematics[5][2] =-0.00345;   patella_kinematics[5][3]= 0.000054656;  patella_kinematics[5][4] = -0.000000237568;
	
	// DEFINE PATELLA PARAMETERS 
	pat_ten_length_ref =  _parameters->anatomy->pat_ten_length;
	patella_thickness_ref = _parameters->anatomy->patella_thickness;
	patella_radius_ref = _parameters->anatomy->patella_wrapping_cylinder_radius;
	
	FLE = _parameters->anatomy->FLE_landmark_coord;
	FME = _parameters->anatomy->FME_landmark_coord;

	side_flag = _parameters->special_options;



	for (int ii=0; ii<3; ii++)
	{
		tibia_origin[ii] = (FLE[ii]+FME[ii])/2;
	}
	

	if (_parameters->anatomy->patella_origin_frame == "global" || _parameters->anatomy->patella_origin_frame == "Global")
	{ 
		for (int ii=0;ii<3;ii++)
		{tib_tub_ref1[ii]=0.0f;}
	}
	else
	{
		cout<< "\nError in Antomy Parameter File "
		<< "patella_tendon_origin_in_patella_coord must be specified in global coord system" << endl;

	}


	if(_parameters->anatomy->patella_insertion_frame == "global" || _parameters->anatomy->patella_insertion_frame == "Global")
	{ 
		for (int ii=0;ii<3;ii++) 
		{tib_tub_ref2[ii] = _parameters->anatomy->patella_insertion_coord[ii] - tibia_origin[ii];}

	}

	else {tib_tub_ref2 = _parameters->anatomy->patella_insertion_coord;}

	if(_parameters->anatomy->first_quad_tendon_insertion_frame == "global" || _parameters->anatomy->first_quad_tendon_insertion_frame == "Global")
	{
		for(int ii=0; ii<3; ii++)
		{quad_ten_ins_ref[ii] = _parameters->anatomy->first_quad_tendon_insertion_coord[ii] - _parameters->anatomy->patella_origin_coord[ii];}
	}
	else {quad_ten_ins_ref = _parameters->anatomy->first_quad_tendon_insertion_coord;}

	if(_parameters->anatomy->second_quad_tendon_insertion_frame == "global" || _parameters->anatomy->second_quad_tendon_insertion_frame == "Global")
	{
		for(int ii=0; ii<3; ii++)
		{wrap_point_ref[ii] = _parameters->anatomy->second_quad_tendon_insertion_coord[ii]-_parameters->anatomy->patella_origin_coord[ii];}
	}
	else {wrap_point_ref = _parameters->anatomy->second_quad_tendon_insertion_coord;}

	

	tib_tub_ref2 = calibrate_pos[1]->struc_rot_hors_to_lcs(tib_tub_ref2);
	quad_ten_ins_ref = calibrate_pos[4]->struc_rot_hors_to_lcs(quad_ten_ins_ref);
	wrap_point_ref = calibrate_pos[4]->struc_rot_hors_to_lcs(wrap_point_ref);

	

	for (int ii=0; ii<3; ii++) 
	{   
		tib_tub_ref2[ii]=tib_tub_ref2[ii]*calibrate_pos[1]->scaling_factors[ii];		// Position of patellar tendon insertion in tibial frame
		quad_ten_ins_ref [ii]=quad_ten_ins_ref[ii]*calibrate_pos[4]->scaling_factors[ii];	// Position of quad tendon insertion in patella frame
		wrap_point_ref[ii]=wrap_point_ref[ii]*calibrate_pos[4]->scaling_factors[ii];	// Quadriceps wrapping point in patellar frame
	}	


	
	//////////////////////////////////////////////////////////////////////////////////////////
	////////*****************************************************************/////////////////


	// Scale patellar tendon angle in anatomical position
	scale_pat_ang_dash=(patella_kinematics[0][0]/180)*MYPI;
	scale_pat_ang=(patella_kinematics[0][0]/180)*MYPI;
	scale_pat_ang=tan(scale_pat_ang);
	scale_pat_ang=scale_pat_ang*calibrate_pos[1]->scaling_factors[0]/calibrate_pos[1]->scaling_factors[1];
	scale_pat_ang=atan(scale_pat_ang);
	patella_kinematics[0][0]=(scale_pat_ang*180)/MYPI;

	sf=calibrate_pos[1]->scaling_factors[0]*(sin(scale_pat_ang_dash)/sin(scale_pat_ang));

	pat_ten_length_ref=pat_ten_length_ref*sf;

	//ZY comment: i=-1, initial state of patella, knee fully extend
	for (int i=-1; i<frames; i++) 
	{


		if (i == -1) 
		{
			knee_flexion_angle=0;
		}
		else 
		{
			knee_flexion_angle=(segment_data[i][2]->joint_angle[2]);
		}

		pat_ten_sag_ang=pat_ten_cor_ang=pat_ten_strain=0;
		pat_flex=pat_tilt=pat_rot=0;

		// Calculate patella position from regression relationships specified in patella_parameters.txt

		for (int j=0; j<6; j++) 
		{

			pat_ten_sag_ang+=patella_kinematics[0][j]*pow(knee_flexion_angle,j);
			pat_ten_cor_ang+=patella_kinematics[1][j]*pow(knee_flexion_angle,j);
//			pat_ten_strain+=patella_kinematics[2][j]*pow(knee_flexion_angle,j);			// Patella strain is switched off
			pat_flex+=patella_kinematics[3][j]*pow(knee_flexion_angle,j);
			pat_tilt+=patella_kinematics[4][j]*pow(knee_flexion_angle,j);
			pat_rot+=patella_kinematics[5][j]*pow(knee_flexion_angle,j);

		}
		
		// Calculate position of patella origin, based upon patellar tendon length and orientation
		
		pat_ten_strain=pat_ten_strain/100;
		pat_ten_length=pat_ten_length_ref*(1+pat_ten_strain);									// Calculate patellar tendon length

		pat_ten_sag_ang=(pat_ten_sag_ang/180)*MYPI;
		pat_ten_cor_ang=(pat_ten_cor_ang/180)*MYPI;

		if (pat_ten_sag_ang < 0) {sign_x=-1;} else {sign_x=1;}
		if (pat_ten_cor_ang < 0) {sign_z=1;} else {sign_z=-1;}

		pat_origin[0]=sign_x*sqrt((pow(pat_ten_length,2)*pow(tan(pat_ten_sag_ang),2))/(1+pow(tan(pat_ten_sag_ang),2)+pow(tan(pat_ten_cor_ang),2)));
		pat_origin[1]=sqrt((pow(pat_ten_length,2))/(1+pow(tan(pat_ten_sag_ang),2)+pow(tan(pat_ten_cor_ang),2)));
		pat_origin[2]=sign_z*sqrt((pow(pat_ten_length,2)*pow(tan(pat_ten_cor_ang),2))/(1+pow(tan(pat_ten_sag_ang),2)+pow(tan(pat_ten_cor_ang),2)));
		
		if (side_flag == 1)
		{
			pat_origin[0] = -pat_origin[0];
		}
		//cout<<pat_origin[0]<<endl;
		//cout<<pat_origin[1]<<endl;
		//cout<<pat_origin[2]<<endl;



		for (int j=0; j<3; j++) 
		{
			pat_origin[j]+=tib_tub_ref2[j];
		}


		//cout<<pat_origin<<endl;
		//cout<<tib_tub_ref2<<endl;
		

		if (i == -1) 
		{
			pat_o_calib=calibrate_pos[1]->struc_pos_point_in_calib(pat_origin);
			for (int j=0; j<3; j++) 
			{
				calibrate_pos[4]->origin[j]=pat_o_calib[j];
			}
		}
		else 
		{				// Specify patella origin, and geometry of the patellar tendon (for all frames)
		
			pat_origin=segment_data[i][1]->seg_pos_point_on_seg(pat_origin);
			v0=segment_data[i][1]->seg_pos_point_on_seg(tib_tub_ref2);

			//cout<<pat_origin;
			//cout<<v0;


			for (int j=0; j<3; j++) 
			{
				segment_data[i][4]->origin[j]=pat_origin[j];
				segment_data[i][4]->other_points[1][j]=v0[j];
			}


			for (int j=0; j<3; j++) 
			{
				patellar_tendon[i]->gblpnts[0][j]=pat_origin[j];
				patellar_tendon[i]->gblpnts[1][j]=v0[j];
			}

			//cout<<patellar_tendon[i]->gblpnts[0][0]<<patellar_tendon[i]->gblpnts[0][1]<<patellar_tendon[i]->gblpnts[0][2]<<endl;
			//cout<<patellar_tendon[i]->gblpnts[1][0]<<patellar_tendon[i]->gblpnts[1][1]<<patellar_tendon[i]->gblpnts[1][2]<<endl;



			patellar_tendon[i]->gblpnts[0][3]=4;
			patellar_tendon[i]->gblpnts[1][3]=1;
			patellar_tendon[i]->pnts=2;
			patellar_tendon[i]->segments_spanned=2;
			patellar_tendon[i]->frame=i;
		
		}

		// Find the orientation of the patella

		euler[0]=(pat_rot/180)*MYPI;
		euler[1]=(pat_tilt/180)*MYPI;
		euler[2]=(pat_flex/180)*MYPI;

		// Orientation of patella LCS wrt femur LCS

		KINEMATICS::kin_qcalc(euler,q_femlcs_to_patlcs,"cardan"); 

		//ZY: patella initial state, knee flexion = 0
		if (i == -1) 
		{

			for (int j=0; j<4; j++) 
			{
				q_lcs_to_fem[j]=calibrate_pos[2]->rot_lcs_to_gcs[j];
			}

			q_lcs_to_pat=MATHEMATICS::math_qmply(q_lcs_to_fem,MATHEMATICS::math_qcon(q_femlcs_to_patlcs));

			for (int j=0; j<4; j++) 
			{
				calibrate_pos[4]->rot_lcs_to_gcs[j]=q_lcs_to_pat[j];
			}

			v1=calibrate_pos[4]->struc_pos_point_in_calib(quad_ten_ins_ref);

			for (int j=0; j<3; j++) 
			{
				calibrate_pos[4]->landmarks[0][j]=v1[j];
			}

			// Calculate position of quadriceps tendon wrapping point

			calibrate_pos[4]->depth[0]=patella_thickness_ref;
			calibrate_pos[4]->depth[1]=patella_radius_ref;
	
			for (int i=0; i<6; i++) 
			{
		
				calibrate_pos[4]->landmarks_local[0][i]=0;
			}
			calibrate_pos[4]->landmarks_local[0][3]=-calibrate_pos[4]->depth[0];

			v2=calibrate_pos[4]->struc_pos_point_in_calib(wrap_point_ref);

			v2=calibrate_pos[2]->struc_pos_point_in_lcs(v2);

			v2[0]-=calibrate_pos[4]->depth[1];
		
			for (int j=0; j<3; j++) 
			{
				calibrate_pos[2]->landmarks_local[0][j]=v2[j];
			}	
		}

		else 
		{		// Find the orientation of the patella (rotation from LCS to GCS) for all frames.
	
			for (int j=0; j<4; j++) 
			{
				q_lcs_to_fem[j]=segment_data[i][2]->rot_lcs_to_gcs[j];
			}

			q_lcs_to_pat=MATHEMATICS::math_qmply(q_lcs_to_fem,MATHEMATICS::math_qcon(q_femlcs_to_patlcs));

			for (int j=0; j<4; j++) 
			{
				segment_data[i][4]->rot_lcs_to_gcs[j]=q_lcs_to_pat[j];
			}

			v1=segment_data[i][4]->seg_pos_point_on_seg(quad_ten_ins_ref);

			for (int j=0; j<3; j++) 
			{
				segment_data[i][4]->other_points[0][j]=v1[j];
			}

		}	


	}

	return;
}