#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "rapidxml.hpp"

struct wrapping_object 
{
	    int wrapping_object_id;
		string wrapping_object_shape;
		int wrapping_segment_flag;
		double radius;
		Vec_DP central_axis_coord;
		int wrapping_direction_flag;
		Vec_DP point_on_cnetral_axis_coord;
		string coord_flag;
};

class XML_Anatomy_Dataset_Parameters
{
public:
	XML_Anatomy_Dataset_Parameters(void);
	
	~XML_Anatomy_Dataset_Parameters(void);

	void load_parameters_from_file( string _parameter_filepath, string _parameter_name );

	// first root 
	string responsible_person; 

	
	//********** universal anthropometry parameters **********//
	string anatomy_sex;
	double anatomy_height;
	double anatomy_mass;
	

	//********** universal bony landmarks parameters **********//
	double landmark_scale_units;
	Vec_DP RPSIS_landmark_coord;
	Vec_DP LPSIS_landmark_coord;
	Vec_DP RASIS_landmark_coord;
	Vec_DP LASIS_landmark_coord;
	Vec_DP FLE_landmark_coord;
	Vec_DP FME_landmark_coord;
	Vec_DP FAM_landmark_coord;
	Vec_DP TAM_landmark_coord;
	Vec_DP FM2_landmark_coord;

	//********** universal joint rotation centres parameters **********//
	double joint_centres_scale_units;
	Vec_DP hip_rotation_centre_coord;
	Vec_DP knee_rotation_centre_coord;
	Vec_DP ankle_rotation_centre_coord;

	//********** universal knee contact model parameters **********//
	double contact_point_scale_units;
	Vec_DP tf_lat_contact_coord;
	Vec_DP tf_med_contact_coord;
	string tf_lat_frame;
	string tf_med_frame;

	//********** universal patella parameters **********//
	double patella_scale_units;
	double patella_thickness;
	double pat_ten_length;
	double patella_wrapping_cylinder_radius;
	Vec_DP patella_origin_coord;
	string patella_origin_frame;
	Vec_DP patella_insertion_coord;
	string patella_insertion_frame;
	Vec_DP first_quad_tendon_insertion_coord;
	string first_quad_tendon_insertion_frame;
	Vec_DP second_quad_tendon_insertion_coord;
	string second_quad_tendon_insertion_frame;

	//********** Get universal muscle geometry parameters attributes **********//
	string muscle_geometry_file_name;
	string ligament_geometry_file_name;
	double musl_pt_scale_units;
	int num_muscle_elements;
	int num_ligament_elements;
	wrapping_object wrapping_object[20];
	string wrapping_segment;
	int wrapping_object_num;


private:
	string str_to_wrapping_method (string s);
	int str_to_wrapping_direction(string s);
	string str_to_sex( string s );
	int str_to_int( string s );
	double str_to_dbl( string s );
	Vec_DP node_to_Vector3d(rapidxml::xml_node<>* __node, double scale );
	int str_to_flag (string s);
	
};

