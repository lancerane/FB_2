#include "StdAfx.h"
#include "XML_anatomy_dataset_parameters.h"

using namespace rapidxml;

using std::cout;
using std::endl;
using std::flush;
using std::ifstream;
using std::istreambuf_iterator;


XML_Anatomy_Dataset_Parameters::XML_Anatomy_Dataset_Parameters(void)
{
}


XML_Anatomy_Dataset_Parameters::~XML_Anatomy_Dataset_Parameters(void)
{
}

// convert string to int
int XML_Anatomy_Dataset_Parameters::str_to_int( string s )
{
	stringstream ss;
	ss << s;
	int x;
	ss >> x;
	return x;
}

// convert string to double
double XML_Anatomy_Dataset_Parameters::str_to_dbl( string s )
{
	stringstream ss;
	ss << s;
	double d;
	ss >> d;
	return d;
}

Vec_DP XML_Anatomy_Dataset_Parameters::node_to_Vector3d( xml_node<>* __node, double scale ) 
{   
	Vec_DP point(3);
	point[0] = str_to_dbl( __node->first_attribute("x")->value() ) *scale;
	point[1]= str_to_dbl( __node->first_attribute("y")->value() ) *scale;
	point[2] = str_to_dbl( __node->first_attribute("z")->value() ) *scale;
	return point;
}

string XML_Anatomy_Dataset_Parameters::str_to_sex( string s ) 
{
	// check for variants of female
	if( s=="female" || s=="FEMALE" || s=="Female" || s=="f" || s=="F")
	{ return "female"; }
		
	// check for variants of false
	if( s=="male" || s=="MALE" || s=="Male" || s=="m" || s=="M" )
	{ return "male"; }

	// if no match, raise an error
	
	cout<< "Error in Parameter File "
	<< s << "' is not recognised as a female or male indicator. "
	<< "Aborting... Try 'female' or 'male' etc.";
	exit(0);
}

string XML_Anatomy_Dataset_Parameters::str_to_wrapping_method(string s)
{
	   if(s == "cylinder")
	   {return "cylinder";}

       else 
	   {
		   cout << "Error in Parameter File::"
				<< "unrecognised wrapping object "
				<< "shape." 
				<< "Aborbing... " 
				<< "Note the only current wrapping object" 
				<<" shape is: cylinder"<<endl;
		   exit(0);
	   }
}

int XML_Anatomy_Dataset_Parameters::str_to_wrapping_direction(string s)
{
	   int flag;

	   if(s == "anti-clockwise")
	   {
		   flag = -1;
		   return flag;
	   }
        if(s == "clockwise")
	   {
		   flag = 1;
		   return flag;
	   }	   

       else 
	   {
		   cout << "Error in Parameter File::"
				<< "unrecognised wrapping direction "
				<< "Aborbing... " 
				<<"Try 'anti-clockwise' or 'clockwise' etc."
				<<endl;
		   exit(0);
	   }
}

int XML_Anatomy_Dataset_Parameters::str_to_flag (string s)
{
	int flag;

	   if(s == "pelvis" || s == "Pelvis")
	   {
			flag = 3;
			return flag;
	   }
	   if(s == "thigh"  || s == "Thigh" )
	   {
		   flag = 2;
		   return flag;
	   }
	   if( s == "shank"  || s == "Shank" )
	   {
		   flag = 1;
		   return flag;
	   } 
	   if(s == "foot"   || s == "Foot")
	   {
		   flag = 0;
		   return flag;
	   }

	   if ( s == "patella"|| s == "Patella")
	   {
		   flag = 4;
		   return flag;
	   }
       else 
	   {
		   cout << "Error in Parameter File::"
				<< "unrecognised wrapping object "
				<< "located segment." 
				<< "Aborbing... " 
				<< "Try 'foot', 'shank', 'thigh' or 'pelvis' or 'patella' etc."
				<<endl; 
		   exit(0);
	   }
	   
}

void XML_Anatomy_Dataset_Parameters::load_parameters_from_file( string _parameter_filepath, string _parameter_XML_name )
{

	
	string _parameter_filename;


	_parameter_filename = _parameter_filepath + "\\" + _parameter_XML_name;

	// Open parameter file
	ifstream file( _parameter_filename.c_str() );

	if( !file.is_open() )
	{
		cout<< "\nError in Parameter File "
		<< "file named '" << _parameter_filename
		<< "' failed to open successfully. Aborting." << endl;
		getchar();
		exit(0);
		
	}

	cout << "\nParameter file '" << _parameter_filename
		 << "' opened successfully." << endl;

	// create xml document object
	xml_document<> doc;

	// declare a buffer and load XML the file into it
	vector<char> buffer((istreambuf_iterator<char>(file)), 
		istreambuf_iterator<char>());
	buffer.push_back('\0');
	
	// Parse the buffer into the documnent (doc) 
	doc.parse<0>(&buffer[0]);
	
	// Note: double underscore at start of variable name denotes an XML node, here

	// Find root node
	xml_node<>* __anatomy_dataset = doc.first_node("anatomy_dataset_parameters");
	
	this->responsible_person = __anatomy_dataset->first_attribute("responsible_person")->value(); 


	// **********Get universal anatomy parameters attributes**********//
	xml_node<>* __anthropometry_parameters = __anatomy_dataset->first_node("universal_anthropometry_parameters");
	this->anatomy_sex= str_to_sex(__anthropometry_parameters->first_attribute("Sex")->value());
	this->anatomy_height = str_to_dbl(__anthropometry_parameters->first_attribute("Height_metres")->value());
	this->anatomy_mass = str_to_dbl(__anthropometry_parameters->first_attribute("Mass_kg")->value());


	// Get bony landmarks parameters attributes
	xml_node<>* __bony_landmarks_parameters = __anatomy_dataset->first_node("universal_bony_landmarks_parameters");
	this->landmark_scale_units = str_to_dbl (__bony_landmarks_parameters->first_attribute("scale_to_SI_units")->value());
	// Get bony landmarks values
	RASIS_landmark_coord = node_to_Vector3d (__bony_landmarks_parameters->first_node("RASIS_landmark_coord"),landmark_scale_units);
	LASIS_landmark_coord = node_to_Vector3d (__bony_landmarks_parameters->first_node("LASIS_landmark_coord"),landmark_scale_units);
	RPSIS_landmark_coord = node_to_Vector3d (__bony_landmarks_parameters->first_node("RPSIS_landmark_coord"),landmark_scale_units);
    LPSIS_landmark_coord = node_to_Vector3d (__bony_landmarks_parameters->first_node("LPSIS_landmark_coord"),landmark_scale_units);
	FLE_landmark_coord = node_to_Vector3d (__bony_landmarks_parameters->first_node("FLE_landmark_coord"),landmark_scale_units);
	FME_landmark_coord = node_to_Vector3d (__bony_landmarks_parameters->first_node("FME_landmark_coord"),landmark_scale_units);
	FAM_landmark_coord = node_to_Vector3d (__bony_landmarks_parameters->first_node("FAM_landmark_coord"),landmark_scale_units);
	TAM_landmark_coord = node_to_Vector3d (__bony_landmarks_parameters->first_node("TAM_landmark_coord"),landmark_scale_units);
	FM2_landmark_coord = node_to_Vector3d (__bony_landmarks_parameters->first_node("FM2_landmark_coord"),landmark_scale_units);

	// Get universal joint rotation centres parameters attributes
	xml_node<>* __joint_centres_parameters = __anatomy_dataset->first_node("universal_joint_rotation_centres_parameters");
	this->joint_centres_scale_units = str_to_dbl (__joint_centres_parameters->first_attribute("scale_to_SI_units")->value());
	hip_rotation_centre_coord = node_to_Vector3d (__joint_centres_parameters->first_node("hip_rotation_centre_coord"),joint_centres_scale_units);
	knee_rotation_centre_coord = node_to_Vector3d (__joint_centres_parameters->first_node("knee_rotation_centre_coord"),joint_centres_scale_units);
	ankle_rotation_centre_coord = node_to_Vector3d (__joint_centres_parameters->first_node("ankle_rotation_centre_coord"),joint_centres_scale_units);


	//cout<<knee_rotation_centre_coord;

	// Get universal knee contact model parameters attributes
	xml_node<>* __knee_contact_parameters = __anatomy_dataset->first_node("universal_knee_contact_model_parameters");
	this->contact_point_scale_units = str_to_dbl (__knee_contact_parameters->first_attribute("scale_to_SI_units")->value());
	xml_node<>* __lateral_contact_point = __knee_contact_parameters->first_node("lateral_contact_point_coord");
	tf_lat_contact_coord = node_to_Vector3d (__lateral_contact_point,contact_point_scale_units);
	tf_lat_frame = __lateral_contact_point->first_attribute("coord_frame")->value();
	xml_node<>* __medial_contact_point = __knee_contact_parameters->first_node("medial_contact_point_coord");
	tf_med_contact_coord = node_to_Vector3d (__medial_contact_point,contact_point_scale_units);
	tf_med_frame = __medial_contact_point->first_attribute("coord_frame")->value();


	// Get universal patella parameters attributes
	xml_node<>* __patella_parameters = __anatomy_dataset->first_node("universal_patella_parameters");
	this->patella_scale_units = str_to_dbl (__patella_parameters->first_attribute("scale_to_SI_units")->value());
	pat_ten_length = str_to_dbl (__patella_parameters->first_attribute("patella_tendon_length_metres")->value());
	patella_thickness = str_to_dbl (__patella_parameters->first_attribute("patella_thickness_metres")->value());
	patella_wrapping_cylinder_radius = str_to_dbl (__patella_parameters->first_attribute("patella_wrapping_cylinder_radius_metres")->value());
	xml_node<>* __patella_origin = __patella_parameters->first_node("patella_tendon_origin_in_patella_coord");
	patella_origin_coord = node_to_Vector3d (__patella_origin,patella_scale_units);
	patella_origin_frame = __patella_origin->first_attribute("coord_frame")->value();
    if (patella_origin_frame == "local" || patella_origin_frame == "Local")
	{
		cout<< "Error in Parameter File::"
			<< "patella tendon origin coord should be given "
			<< "in global coord system."
			<< "Aborbing... " 
			<< "Try global value." ;
		exit(0);
		
	}
	xml_node<>* __patella_insertion = __patella_parameters->first_node("patella_tendon_insertion_in_tibia_coord");
	patella_insertion_coord = node_to_Vector3d (__patella_insertion,patella_scale_units);
	patella_insertion_frame = __patella_insertion->first_attribute("coord_frame")->value();
	xml_node<>* __first_quad_tendon_insertion = __patella_parameters->first_node("first_quadriceps_tendon_insertion_in_patella_coord");
	first_quad_tendon_insertion_coord = node_to_Vector3d (__first_quad_tendon_insertion,patella_scale_units);
	first_quad_tendon_insertion_frame = __first_quad_tendon_insertion->first_attribute("coord_frame")->value();
	xml_node<>* __second_quad_tendon_insertion = __patella_parameters->first_node("second_quadriceps_tendon_insertion_in_patella_coord");
	second_quad_tendon_insertion_coord = node_to_Vector3d (__second_quad_tendon_insertion,patella_scale_units);
	second_quad_tendon_insertion_frame = __second_quad_tendon_insertion->first_attribute("coord_frame")->value();

	// Get universal muscle geometry parameters attributes
	xml_node<>* __muscle_parameters = __anatomy_dataset->first_node("universal_muscle_geometry_parameters");
	musl_pt_scale_units = str_to_dbl (__muscle_parameters->first_attribute("scale_to_SI_units")->value());
	num_muscle_elements = str_to_int(__muscle_parameters->first_attribute("number_of_muscle_elements")->value());
	muscle_geometry_file_name = __muscle_parameters->first_attribute("muscle_geometry_file_name")->value();
	num_ligament_elements = str_to_int(__muscle_parameters->first_attribute("number_of_ligament_elements")->value());
	ligament_geometry_file_name = __muscle_parameters->first_attribute("ligament_geometry_file_name")->value();
	xml_node<>* __wrapping_object_list = __muscle_parameters->first_node("wrapping_object_list");
	wrapping_object_num = 0;
	for (xml_node<> * __wrapping_object = __wrapping_object_list->first_node( "wrapping_object_parameters" ); 
		__wrapping_object; 
		__wrapping_object = __wrapping_object->next_sibling() )
		{
			wrapping_object[wrapping_object_num].wrapping_object_id = str_to_int (__wrapping_object->first_attribute("wrapping_object_ID")->value());
			double scale = str_to_dbl (__wrapping_object->first_attribute("scale_to_SI_units")->value());
			wrapping_object[wrapping_object_num].wrapping_object_shape = str_to_wrapping_method(__wrapping_object->first_attribute("wrapping_object_shape")->value());
			wrapping_object[wrapping_object_num].radius = str_to_dbl(__wrapping_object->first_attribute("wrapping_object_radius_metres")->value());
			wrapping_object[wrapping_object_num].wrapping_segment_flag = str_to_flag(__wrapping_object->first_attribute("wrapping_object_attached_segment")->value());
			xml_node<>* __axis_vector = __wrapping_object->first_node("wrapping_object_central_axis_direction_coord");
			wrapping_object[wrapping_object_num].central_axis_coord= node_to_Vector3d(__axis_vector,1.0);
			wrapping_object[wrapping_object_num].wrapping_direction_flag = str_to_wrapping_direction(__axis_vector->first_attribute("wrapping_direction")->value()); 
			xml_node<>* __point_vector = __wrapping_object->first_node("a_point_on_wrapping_object_central_axis_coord");
			wrapping_object[wrapping_object_num].point_on_cnetral_axis_coord = node_to_Vector3d(__point_vector,scale);
			wrapping_object[wrapping_object_num].coord_flag = __point_vector->first_attribute("coord_frame")->value();

			wrapping_object_num = wrapping_object_num+1;
		}

	
}