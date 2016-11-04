#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "rapidxml.hpp"

#include "XML_anatomy_dataset_parameters.h"

struct marker_per_segment 
{
	    int marker_per_segment_num;
		int bony_landmark_flag[10];
		bool marker_used_in_dynamic[10];
		int marker_column[10];
};

class XML_Parameters
{
public:
	XML_Parameters(void);
	~XML_Parameters(void);

	void load_parameters_from_file( string _parameter_filename );

	//anatomy 
	XML_Anatomy_Dataset_Parameters *anatomy;

	// first root 
	string study_name;
	string responsible_person; 
	string input_file_root;
	string output_file_root_for_visualisation;
	string output_file_root_for_optimisation;
	int special_options;
	
	// universal physical parameters
	int frames_per_second;
	double marker_radius;
	string segment_method;
	bool thigh_cluster_used;
	bool shank_cluster_used;
	Vec_DP gravity_vector;


	// universal marker file parameters
	double marker_scale_units;
	int mk_col_RPSIS;
	int mk_col_LPSIS;
	int mk_col_RASIS;
	int mk_col_LASIS;
	int mk_col_FLE;
	int mk_col_FME;
	int mk_col_TAM;
	int mk_col_FAM;
	int mk_col_FM2;
	marker_per_segment marker_per_segment[4];

	
	// universal ex force file parameters
	double force_scale_units;
	string ex_force_directory_name;
	string ex_force_file_name;
	Vec_DP ex_force_info;

	// subject
	string subject_sex;
	double subject_height;
	double subject_mass;
	string subject_anatomy_data_path;
	string subject_anatomy_data_name;
	// first root of subject: static_trial_parameters
	int static_start_frame_number; 
	int static_end_frame_number; 
	string static_trial_directory_name;
	string static_marker_file_name; 
	// first root of subject: dynamic_trial_parameters
	int dynamic_start_frame_number; 
	int dynamic_end_frame_number; 
	string dynamic_trial_directory_name; 
	string dynamic_marker_file_name; 

private:
	double str_to_dbl( string s );
	int str_to_int( string s );
	bool str_to_bool( string s );
	string str_to_sex( string s );
	string str_to_method( string s);
	Vec_DP node_to_Vector3d( rapidxml::xml_node<>* __node, double scale); 
	Vec_DP str_to_ex_force_info( string s );


};

