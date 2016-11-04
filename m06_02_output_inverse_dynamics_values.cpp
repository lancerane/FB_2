#include "stdafx.h"
#include "m06_02_output_inverse_dynamics_values.h"
#include "io_dataoutput.h"





using namespace std;

/*
Written by Ziyun Ding
Output values for inverse dynamics analysis
Calculate the net joint forces of ankle, knee and hip joints and output them in the GCS, the adjacent distal segment system, and proximal segment system
*/

void m06_02_output_inverse_dynamics_values(Segment **segment_data[], int frames, int segments, string output_folder_for_optimisation, string study_name)
{
	Mat_DP net_force_gcs(frames, 9), net_torque_gcs(frames, 9), net_force_dis_seg(frames, 9), net_force_pro_seg(frames, 9), net_torque_dis_seg(frames, 9), net_torque_pro_seg(frames, 9);
    Vec_DP q_dis(4), q_pro(4), net_force(3), net_torque(3), force_dis(3), force_pro(3), torque_dis(3), torque_pro(3);
	
	for (int ii=0; ii<frames; ii++)
	{
		for (int jj=0; jj<segments-2; jj++)
		{
			
			for (int ll=0; ll<3; ll++)
			{
				net_force[ll] = segment_data[ii][jj]->fp[ll];
				net_torque[ll] = segment_data[ii][jj]->mp_segmental_approach[ll];
			}

			for (int kk=0; kk<4; kk++) 
			{
				q_dis[kk]=segment_data[ii][jj]->rot_lcs_to_gcs[kk];
				q_pro[kk]=segment_data[ii][jj+1]->rot_lcs_to_gcs[kk];
			}

			q_dis=MATHEMATICS::math_qcon(q_dis);
			q_pro=MATHEMATICS::math_qcon(q_pro);

			force_dis=KINEMATICS::kin_qrotate(net_force,q_dis);
			force_pro=KINEMATICS::kin_qrotate(net_force,q_pro);
			torque_dis=KINEMATICS::kin_qrotate(net_torque,q_dis);
			torque_pro=KINEMATICS::kin_qrotate(net_torque,q_pro);

			for (int ll=0; ll<3; ll++)
			{
				segment_data[ii][jj]->force_dis[ll] = force_dis[ll];
				segment_data[ii][jj]->force_pro[ll] = force_pro[ll];
				segment_data[ii][jj]->torque_dis[ll] = torque_dis[ll];
				segment_data[ii][jj]->torque_pro[ll] = torque_pro[ll];

			}
		}
	}

	for (int ii=0; ii<frames; ii++)
	{
		for (int jj=0; jj<segments-2; jj++)
		{
			for (int ll=0; ll<3; ll++)
			{
				net_force_gcs[ii][3*jj+ll] = segment_data[ii][jj]->fp[ll];
				net_torque_gcs[ii][3*jj+ll] = segment_data[ii][jj]->mp_segmental_approach[ll];
				net_force_dis_seg[ii][3*jj+ll] = segment_data[ii][jj]->force_dis[ll];
				net_force_pro_seg[ii][3*jj+ll] = segment_data[ii][jj]->force_pro[ll];
				net_torque_dis_seg[ii][3*jj+ll] = segment_data[ii][jj]->torque_dis[ll];
				net_torque_pro_seg[ii][3*jj+ll] = segment_data[ii][jj]->torque_pro[ll];
			}
		}

	}
	

	string filename_net_force_gcs=output_folder_for_optimisation +"\\" + study_name + "_"+"net_force_gcs.csv";
	string filename_net_torque_gcs=output_folder_for_optimisation +"\\" + study_name + "_"+"net_torque_gcs.csv";
	string filename_net_force_dis_seg=output_folder_for_optimisation +"\\" + study_name + "_"+"net_force_dis_seg.csv";
	string filename_net_torque_dis_seg=output_folder_for_optimisation +"\\" + study_name + "_"+"net_torque_dis_seg.csv";
	string filename_net_force_pro_seg=output_folder_for_optimisation +"\\" + study_name + "_"+"net_force_pro_seg.csv";
	string filename_net_torque_pro_seg=output_folder_for_optimisation +"\\" + study_name + "_"+"net_torque_pro_seg.csv";


	io_dataoutput(filename_net_force_gcs,net_force_gcs);
	io_dataoutput(filename_net_torque_gcs,net_torque_gcs);
	io_dataoutput(filename_net_force_dis_seg,net_force_dis_seg);
	io_dataoutput(filename_net_torque_dis_seg,net_torque_dis_seg);
	io_dataoutput(filename_net_force_pro_seg,net_force_pro_seg);
	io_dataoutput(filename_net_torque_pro_seg,net_torque_pro_seg);

}