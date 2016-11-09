// Model v2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "m01_00_data_input.h"
#include "m02_00_create_segments.h"
#include "m03_00_calculate_kinematics.h"
#include "m04_00_inverse_dynamics.h"
#include "m05_00_muscle_model.h"
#include "m06_00_output_data.h"

#include "XML_Parameters.h"

int _tmain(int argc, _TCHAR* argv[]) // FOR PREDEFINED FILE
//int main( int argc, char *argv[] )        // for drag and drop  
{
	// Define all variables for tmain
	const int max_segments=5, max_frames=4000, max_muscles=163, max_ligaments=14;		

	string parameter_filename;

	XML_Parameters* xml_leg_parameters = new XML_Parameters();



	/*////for drag and drop 
	if (argc < 2)
	{
		cout<< "\nError in Reading XML Parameter File "
		<< "failed to open successfully. Aborting." << endl;	
		getchar();
		return 0;
	}
	else
		parameter_filename = argv[1];
	

    xml_leg_parameters->load_parameters_from_file(parameter_filename);
	//// for drag and drop */


	xml_leg_parameters->load_parameters_from_file("\\\\icnas2.cc.ic.ac.uk\\lr2214\\Desktop\\FESstudy0607\\LR\\running_04.xml");



	//////***************************************************************************///////////////////////////////////////////////////////////
                     
	int segments=5;
	int muscles=xml_leg_parameters->anatomy->num_muscle_elements;
	int ligaments=xml_leg_parameters->anatomy->num_ligament_elements;

	int dynamic_frames = xml_leg_parameters->dynamic_end_frame_number - xml_leg_parameters->dynamic_start_frame_number + 1;
	double freq=xml_leg_parameters->frames_per_second;	


	Segment **segment_data[max_frames];
	Structure *calibrate_pos[max_segments];
	Muscle **muscle_data[max_frames];
	Muscle *muscle_model[max_muscles];
	Muscle **ligament_data[max_frames];
	Muscle *ligament_model[max_ligaments];
	Muscle *patellar_tendon[max_frames];
	Muscle *patella_ratio_av[max_frames];

	for (int i=0; i<dynamic_frames; i++)
	{
		segment_data[i]=new Segment *[max_segments];
		muscle_data[i]=new Muscle *[max_muscles];
		ligament_data[i]=new Muscle *[max_ligaments];
		patellar_tendon[i]=new Muscle;
		patella_ratio_av[i]=new Muscle;
		for (int j=0; j<segments; j++) 
		{
			segment_data[i][j]=new Segment;
		}
		for (int j=0; j<muscles; j++) 
		{
			muscle_data[i][j]=new Muscle;
		}
		for (int j=0; j<ligaments; j++) 
		{
			ligament_data[i][j]=new Muscle;
		}

	}
	for (int i=0; i<segments; i++) 
	{
		calibrate_pos[i]=new Structure;
	}
	for (int i=0; i<muscles; i++) 
	{
		muscle_model[i]=new Muscle;
	}
	for (int i=0; i<ligaments; i++) 
	{
		ligament_model[i]=new Muscle;
	}

	// ****************************************************************************************

	m01_00_data_input(segment_data,calibrate_pos,dynamic_frames,freq,segments, xml_leg_parameters);							// Input raw data from text files

	m02_00_create_segments(segment_data,calibrate_pos,dynamic_frames,segments,patellar_tendon,patella_ratio_av,xml_leg_parameters);

	m03_00_calculate_kinematics(segment_data,calibrate_pos,dynamic_frames,segments,xml_leg_parameters);

	m04_00_inverse_dynamics(segment_data,dynamic_frames,segments,xml_leg_parameters);

    m05_00_muscle_model(segment_data,calibrate_pos,muscle_data,muscle_model,patellar_tendon,ligament_data,ligament_model,
		dynamic_frames,freq,segments,muscles,ligaments,patella_ratio_av,xml_leg_parameters);

	m06_00_output_data(segment_data,calibrate_pos,muscle_data,patellar_tendon,ligament_data,dynamic_frames,segments,muscles,ligaments,xml_leg_parameters);

	// ****************************************************************************************


	delete xml_leg_parameters;
	
	return 0;
}

