/*!	Parameter class for version 1.1 of Freebody software

	Written by Ziyun Ding, Imperial College (c) 2014,
	
	Loads specificed parameters from a named XML file.
*/

#include "StdAfx.h"
#include "XML_Parameters.h"

using namespace rapidxml;

using std::cout;
using std::endl;
using std::flush;
using std::ifstream;
using std::istreambuf_iterator;


XML_Parameters::XML_Parameters(void)
{
	anatomy = NULL;
}


XML_Parameters::~XML_Parameters(void)
{
	if (anatomy) 
		delete anatomy;
}


// convert string to int
int XML_Parameters::str_to_int( string s )
{
	stringstream ss;
	ss << s;
	int x;
	ss >> x;
	return x;
}

// convert string to bool
bool XML_Parameters::str_to_bool( string s )
{
	// check for variants of true
	if( s=="True" || s=="true" || s=="Yes" || s=="yes" || s=="Y" ||s=="y" ||s=="1")
	{ return true; }
		
	// check for variants of false
	if( s=="False" || s=="false" || s=="No" || s=="no" || s=="n" ||s=="n" ||s=="0")
	{ return false; }	
}

// convert string to sex
string XML_Parameters::str_to_sex( string s ) 
{
	// check for variants of female
	if( s=="female" || s=="FEMALE" || s=="Female" || s=="f" || s=="F")
	{ return "female"; }
		
	// check for variants of false
	if( s=="male" || s=="MALE" || s=="Male" || s=="m" || s=="M" )
	{ return "male"; }

	// if no match, raise an error
	
	cout<< "Error in Parameter File '"
	<< s << "' is not recognised as a female or male indicator. "
	<< "Aborting... Try 'female' or 'male' etc.";
	exit(0);
}

string XML_Parameters::str_to_method (string s)
{
	   if(s == "De_Leva")
	   {return "De_Leva";}

       else 
	   {
		   cout << "Error in Parameter File::"
				<< "unrecognised segment parameter "
				<< "estimation method." 
				<< "Aborbing... " 
				<< "Note the only current working option" 
				<<" is: De_Leva"<<endl;
		   exit(0);
	   }
}

// convert string to double
double XML_Parameters::str_to_dbl( string s )
{
	stringstream ss;
	ss << s;
	double d;
	ss >> d;
	return d;
}

Vec_DP XML_Parameters::node_to_Vector3d( xml_node<>* __node, double scale ) 
{   
	Vec_DP point(3);
	point[0] = str_to_dbl( __node->first_attribute("x")->value() ) *scale;
	point[1]= str_to_dbl( __node->first_attribute("y")->value() ) *scale;
	point[2] = str_to_dbl( __node->first_attribute("z")->value() ) *scale;
	return point;
}


Vec_DP XML_Parameters::str_to_ex_force_info( string s )
{
	Vec_DP ex_force_info(5);
	for (int ii = 0; ii<5; ii++)
	{
		ex_force_info[ii] = 0;
	}

	if (s=="foot")
	{ 
		ex_force_info[0] = 1;
		return ex_force_info;
	}
	if (s=="shank")
	{ 
		ex_force_info[1] = 1;
		return ex_force_info;
	}
	if (s=="thigh")
	{ 
		ex_force_info[2] = 1;
		return ex_force_info;
	}
	if (s=="pelvis")
	{ 
		ex_force_info[3] = 1;
		return ex_force_info;
	}
	else
	{
		cout<< "Error in Parameter File '"
		<< s << "' is not recognised as a foot, shank, thigh or pelvis indicator. "
		<< "Aborting... Try 'foot', 'shank', 'thigh' or 'pelvis' etc."
		<<endl;
		exit(0);
	}

}

void XML_Parameters::load_parameters_from_file( string _parameter_filename )
{
	/////////***************************************************
	cout << "\n***************************************" << endl;
	cout << "     LOAD PARAMETERS FROM XML FILE" << endl;
	cout << "***************************************" << endl;

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
	xml_node<>* __study = doc.first_node("study_level_parameters");

	// **********Get study attributes**********//
	
	this->study_name = __study->first_attribute("study_name")->value();
	this->responsible_person = __study->first_attribute("responsible_person")->value(); 
	this->input_file_root = __study->first_attribute("input_directory_path")->value();
	this->output_file_root_for_visualisation = __study->first_attribute("output_directory_path_for_visualisation")->value();
	this->output_file_root_for_optimisation = __study->first_attribute("output_directory_path_for_optimisation")->value();
	this->special_options = str_to_int( __study->first_attribute("special_options")->value() );


	// **********Get universal physical parameters attributes**********//
	xml_node<>* __physical_parameters = __study->first_node("universal_physical_parameters");
	this->frames_per_second = str_to_int(__physical_parameters->first_attribute("frames_per_second")->value());
	this->marker_radius = str_to_dbl(__physical_parameters->first_attribute("radius_per_marker_metres")->value());
	this->segment_method = str_to_method(__physical_parameters->first_attribute("segment_parameter_estamation_method")->value());
	gravity_vector = node_to_Vector3d (__physical_parameters->first_node("gravity_vector_in_GCS"),1.0);


	// **********Get subject attributes**********//
	xml_node<>* __subject = __study->first_node( "subject" );
	this->subject_sex = str_to_sex(__subject->first_attribute("subject_sex")->value() );
	this->subject_height = str_to_dbl( __subject->first_attribute("subject_height_metres")->value() );
	this->subject_mass = str_to_dbl( __subject->first_attribute( "subject_mass_kg" )->value() );
	this->subject_anatomy_data_path = __subject->first_attribute( "subject_anatomy_dataset_path" )->value() ;
	this->subject_anatomy_data_name =__subject->first_attribute("subject_anatomy_dataset_file_name")->value();

	anatomy = new XML_Anatomy_Dataset_Parameters();

	anatomy->load_parameters_from_file(subject_anatomy_data_path, subject_anatomy_data_name);


	// Get subject first root static_trial_parameters
	xml_node<>* __static_trial = __subject->first_node("static_trial_parameters");
	// Get subject static trial attributes
	this->static_trial_directory_name = __static_trial->first_attribute("static_trial_directory_name")->value(); 
	this->static_marker_file_name = __static_trial->first_attribute("static_marker_file_name")->value(); 
	this->static_start_frame_number = str_to_int( __static_trial->first_attribute("start_frame_number")->value() ); 
	this->static_end_frame_number = str_to_int (__static_trial->first_attribute("end_frame_number")->value() ); 
	// Get subject first root dynamic trial parameters
	xml_node<>* __dynamic_trial = __subject->first_node("dynamic_trial_parameters");
	// Get subject dynamic trial attributes
	this->dynamic_trial_directory_name = __dynamic_trial->first_attribute("dynamic_trial_directory_name")->value(); 
	this->dynamic_marker_file_name = __dynamic_trial->first_attribute("dynamic_marker_file_name")->value(); 
	this->dynamic_start_frame_number = str_to_int( __dynamic_trial->first_attribute("start_frame_number")->value() ); 
	this->dynamic_end_frame_number = str_to_int (__dynamic_trial->first_attribute("end_frame_number")->value() ); 


	// Get universal marker file parameters attributes
	xml_node<>* __marker_file_parameters = __study->first_node("universal_marker_file_parameters");
	marker_scale_units = str_to_dbl(__marker_file_parameters->first_attribute("scale_to_SI_units")->value());


	// Get first node column number in marker file 
	int marker_num=0;
    xml_node<>* __marker_parameter = __marker_file_parameters->first_node("marker_parameters_per_segment");

	/* marker parameters for foot segment */
	xml_node<>* __foot_segment_marker = __marker_parameter->first_node("foot_segment");
	xml_node<>* __foot_bony_landmarks = __foot_segment_marker->first_node("bony_landmark_list");

	for (xml_node<> * __foot_bony_landmark_attributes = __foot_bony_landmarks->first_node( "bony_landmark_attributes"); 
		__foot_bony_landmark_attributes; 
		__foot_bony_landmark_attributes = __foot_bony_landmark_attributes->next_sibling())
	{

		string foot_bony_landmarks_ID = __foot_bony_landmark_attributes->first_attribute("bony_landmark_ID")->value();
		
		if (foot_bony_landmarks_ID == "FM2")
		{
			
			marker_per_segment[0].bony_landmark_flag[marker_num] = 1;

			marker_per_segment[0].marker_column[marker_num] = str_to_int(__foot_bony_landmark_attributes->first_attribute("bony_landmark_column_number")->value());

			marker_per_segment[0].marker_used_in_dynamic[marker_num] = str_to_bool(__foot_bony_landmark_attributes->first_attribute("bony_landmark_used_in_dynamic_trial")->value());
		   
			marker_num = marker_num+1;
		}
		else 
		{
			cout<< "Error in Foot Segment Bony Landmark Attributes::"
			<< "Bony landmark FM2 is required in static trial"
			<< "Aborbing... " 
			<< "Try bony_landmarke_ID 'FM2'." ;
			exit(0);
		}

	}

    xml_node<>* __foot_other_markers = __foot_segment_marker->first_node("other_marker_list");
	for (xml_node<> * __foot_other_marker_attributes = __foot_other_markers->first_node( "other_marker_attributes"); 
		__foot_other_marker_attributes; 
		__foot_other_marker_attributes = __foot_other_marker_attributes->next_sibling())
	{
		string other_marker_ID = __foot_other_marker_attributes->first_attribute("marker_ID")->value();
		if (other_marker_ID != "")
		{
			
			marker_per_segment[0].bony_landmark_flag[marker_num] = 0;

			marker_per_segment[0].marker_column[marker_num] = str_to_int(__foot_other_marker_attributes->first_attribute("marker_column_number")->value());

			marker_per_segment[0].marker_used_in_dynamic[marker_num] = true;
			
			marker_num = marker_num+1;

		}
	}

	marker_per_segment[0].marker_per_segment_num = marker_num;


	/* marker parameter for shank segment */
	xml_node<>* __shank_segment_marker = __marker_parameter->first_node("shank_segment");
	xml_node<>* __shank_bony_landmarks = __shank_segment_marker->first_node("bony_landmark_list");

	marker_num = 0;

	for (xml_node<> * __shank_bony_landmark_attributes = __shank_bony_landmarks->first_node( "bony_landmark_attributes"); 
		__shank_bony_landmark_attributes; 
		__shank_bony_landmark_attributes = __shank_bony_landmark_attributes->next_sibling())
	{
		
		string shank_bony_landmark_ID = __shank_bony_landmark_attributes->first_attribute("bony_landmark_ID")->value();

		if (shank_bony_landmark_ID != "")
		{
			
			if (shank_bony_landmark_ID == "FAM")
			{
				
				marker_per_segment[1].bony_landmark_flag[marker_num] = 1;

				marker_per_segment[1].marker_column[marker_num] = str_to_int(__shank_bony_landmark_attributes->first_attribute("bony_landmark_column_number")->value());

				marker_per_segment[1].marker_used_in_dynamic[marker_num] = str_to_bool(__shank_bony_landmark_attributes->first_attribute("bony_landmark_used_in_dynamic_trial")->value());
			
			}

			if (shank_bony_landmark_ID == "TAM")
			{

				marker_per_segment[1].bony_landmark_flag[marker_num] = 1;

				marker_per_segment[1].marker_column[marker_num] = str_to_int(__shank_bony_landmark_attributes->first_attribute("bony_landmark_column_number")->value());

				marker_per_segment[1].marker_used_in_dynamic[marker_num] = str_to_bool(__shank_bony_landmark_attributes->first_attribute("bony_landmark_used_in_dynamic_trial")->value());
			
			}

		    marker_num = marker_num+1;
		}
		
		else if (shank_bony_landmark_ID != "TAM" || shank_bony_landmark_ID != "FAM") 
		{
			cout<< "Error in Shank Segment Bony Landmark Attributes::"
			<< "Bony landmark FAM and TAM are required in static trial"
			<< "Aborbing... " 
			<< "Try bony_landmarke_ID 'FAM' or 'TAM'." ;
			exit(0);
		}
	
	}

    xml_node<>* __shank_other_markers = __shank_segment_marker->first_node("other_marker_list");
	for (xml_node<> * __shank_other_marker_attributes = __shank_other_markers->first_node( "other_marker_attributes"); 
		__shank_other_marker_attributes; 
		__shank_other_marker_attributes = __shank_other_marker_attributes->next_sibling())
	{
		string shank_other_marker_ID = __shank_other_marker_attributes->first_attribute("marker_ID")->value();


		if (shank_other_marker_ID !=  "")
		{
			marker_per_segment[1].bony_landmark_flag[marker_num] = 0;

			marker_per_segment[1].marker_column[marker_num] = str_to_int(__shank_other_marker_attributes->first_attribute("marker_column_number")->value());

			marker_per_segment[1].marker_used_in_dynamic[marker_num] = true;
		    
			marker_num = marker_num+1;
			
		}

	}

	marker_per_segment[1].marker_per_segment_num = marker_num;


	/* marker parameter for thigh segment */
	xml_node<>* __thigh_segment_marker = __marker_parameter->first_node("thigh_segment");
    xml_node<>* __thigh_bony_landmark = __thigh_segment_marker->first_node("bony_landmark_list");

	marker_num = 0;

	for (xml_node<> * __thigh_bony_landmark_attributes = __thigh_bony_landmark->first_node( "bony_landmark_attributes"); 
		__thigh_bony_landmark_attributes; 
		__thigh_bony_landmark_attributes = __thigh_bony_landmark_attributes->next_sibling())
	{
		
		string thigh_bony_landmark_ID = __thigh_bony_landmark_attributes->first_attribute("bony_landmark_ID")->value();

		if (thigh_bony_landmark_ID != "")
		{
			if (thigh_bony_landmark_ID == "FLE")
			{
				marker_per_segment[2].bony_landmark_flag[marker_num] = 1;

				marker_per_segment[2].marker_column[marker_num] = str_to_int(__thigh_bony_landmark_attributes->first_attribute("bony_landmark_column_number")->value());

				marker_per_segment[2].marker_used_in_dynamic[marker_num] = str_to_bool(__thigh_bony_landmark_attributes->first_attribute("bony_landmark_used_in_dynamic_trial")->value());
			
			}

		    if (thigh_bony_landmark_ID == "FME")
			{
				marker_per_segment[2].bony_landmark_flag[marker_num] = 1;

				marker_per_segment[2].marker_column[marker_num] = str_to_int(__thigh_bony_landmark_attributes->first_attribute("bony_landmark_column_number")->value());

				marker_per_segment[2].marker_used_in_dynamic[marker_num] = str_to_bool(__thigh_bony_landmark_attributes->first_attribute("bony_landmark_used_in_dynamic_trial")->value());	
				
			}

			marker_num = marker_num+1;
		}
		
		
		else if (thigh_bony_landmark_ID != "FME" || thigh_bony_landmark_ID != "FLE")
		{
			cout<< "Error in Thigh Segment Bony Landmark Attributes::"
			<< "Bony landmark FLE and FME are required in static trial"
			<< "Aborbing... " 
			<< "Try bony_landmarke_ID 'FLE' or 'FME'." ;
			exit(0);
		}
	}


    xml_node<>* __thigh_other_markers = __thigh_segment_marker->first_node("other_marker_list");
	for (xml_node<> * __thigh_other_marker_attributes = __thigh_other_markers->first_node( "other_marker_attributes"); 
		__thigh_other_marker_attributes; 
		__thigh_other_marker_attributes = __thigh_other_marker_attributes->next_sibling())
	{
		string thigh_other_marker_ID = __thigh_other_marker_attributes->first_attribute("marker_ID")->value();
		
		if (thigh_other_marker_ID != "")
		{
			marker_per_segment[2].bony_landmark_flag[marker_num] = 0;

			marker_per_segment[2].marker_column[marker_num] = str_to_int(__thigh_other_marker_attributes->first_attribute("marker_column_number")->value());

			marker_per_segment[2].marker_used_in_dynamic[marker_num] = true;
			
			marker_num = marker_num+1;
			
		}
	}

	marker_per_segment[2].marker_per_segment_num = marker_num;


	/* marker parameter for pelvis segment */
	xml_node<>* __pelvis_segment_marker = __marker_parameter->first_node("pelvis_segment");
	xml_node<>* __pelvis_bony_landmark = __pelvis_segment_marker->first_node("bony_landmark_list");

	marker_num = 0;

	for (xml_node<> * __pelvis_bony_landmark_attributes = __pelvis_bony_landmark->first_node( "bony_landmark_attributes"); 
		__pelvis_bony_landmark_attributes; 
		__pelvis_bony_landmark_attributes = __pelvis_bony_landmark_attributes->next_sibling())
	{

		string pelvis_bony_landmark_ID = __pelvis_bony_landmark_attributes->first_attribute("bony_landmark_ID")->value();

		if (pelvis_bony_landmark_ID != "")
		{

			if (pelvis_bony_landmark_ID == "RASIS")
			{
				marker_per_segment[3].bony_landmark_flag[marker_num] = 1;

				marker_per_segment[3].marker_column[marker_num] = str_to_int(__pelvis_bony_landmark_attributes->first_attribute("bony_landmark_column_number")->value());

				marker_per_segment[3].marker_used_in_dynamic[marker_num] = str_to_bool(__pelvis_bony_landmark_attributes->first_attribute("bony_landmark_used_in_dynamic_trial")->value());

			}
			
			if (pelvis_bony_landmark_ID == "LASIS")
			{
				marker_per_segment[3].bony_landmark_flag[marker_num] = 1;

				marker_per_segment[3].marker_column[marker_num] = str_to_int(__pelvis_bony_landmark_attributes->first_attribute("bony_landmark_column_number")->value());

				marker_per_segment[3].marker_used_in_dynamic[marker_num] = str_to_bool(__pelvis_bony_landmark_attributes->first_attribute("bony_landmark_used_in_dynamic_trial")->value());

			}

			if (pelvis_bony_landmark_ID == "RPSIS")
			{
				marker_per_segment[3].bony_landmark_flag[marker_num] = 1;

				marker_per_segment[3].marker_column[marker_num] = str_to_int(__pelvis_bony_landmark_attributes->first_attribute("bony_landmark_column_number")->value());

				marker_per_segment[3].marker_used_in_dynamic[marker_num] = str_to_bool(__pelvis_bony_landmark_attributes->first_attribute("bony_landmark_used_in_dynamic_trial")->value());
		
			}

			if (pelvis_bony_landmark_ID == "LPSIS")
			{
				marker_per_segment[3].bony_landmark_flag[marker_num] = 1;

				marker_per_segment[3].marker_column[marker_num] = str_to_int(__pelvis_bony_landmark_attributes->first_attribute("bony_landmark_column_number")->value());

				marker_per_segment[3].marker_used_in_dynamic[marker_num] = str_to_bool(__pelvis_bony_landmark_attributes->first_attribute("bony_landmark_used_in_dynamic_trial")->value());
		
			}
			marker_num = marker_num+1;
		}
		
			
		else if (pelvis_bony_landmark_ID != "RASIS" || pelvis_bony_landmark_ID != "LASIS" || pelvis_bony_landmark_ID != "RPSIS" || pelvis_bony_landmark_ID != "LPSIS")
		{
			cout<< "Error in Pelvis Segment Bony Landmark Attributes::"
			<< "Bony landmark RASIS, LASIS, RPSIS and LPSIS are required in static trial"
			<< "Aborbing... " 
			<< "Try bony_landmarke_ID 'RASIS', 'LASIS', 'RPSIS' or 'LPSIS'." ;
			exit(0);
		}

	}

    xml_node<>* __pelvis_other_markers = __pelvis_segment_marker->first_node("other_marker_list");
	for (xml_node<> * __pelvis_other_marker_attributes = __pelvis_other_markers->first_node( "other_marker_attributes"); 
		__pelvis_other_marker_attributes; 
		__pelvis_other_marker_attributes = __pelvis_other_marker_attributes->next_sibling())
	{
		
		string pelvis_other_marker_ID = __pelvis_other_marker_attributes->first_attribute("marker_ID")->value();

		if (pelvis_other_marker_ID != "")
		{
			marker_per_segment[3].bony_landmark_flag[marker_num] = 0;

			marker_per_segment[3].marker_column[marker_num] = str_to_int(__pelvis_other_marker_attributes->first_attribute("marker_column_number")->value());

			marker_per_segment[3].marker_used_in_dynamic[marker_num] = true;
			
			marker_num = marker_num+1;
	
		}
	}

	marker_per_segment[3].marker_per_segment_num = marker_num;

	



	// Get universal ex force file parameters attributes
	xml_node<>* __ex_force_file_parameters = __study->first_node("universal_ex_force_file_parameters");
	force_scale_units = str_to_dbl(__ex_force_file_parameters->first_attribute("scale_to_SI_units")->value());
	ex_force_directory_name = __ex_force_file_parameters->first_attribute("ex_force_directory_name")->value();
	ex_force_file_name = __ex_force_file_parameters->first_attribute("ex_force_file_name")->value();
	ex_force_info = str_to_ex_force_info(__ex_force_file_parameters->first_attribute("ex_force_segment_name")->value());
	

}