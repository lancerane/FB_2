#include "stdafx.h"

#include "m06_00_output_data.h"
#include "m06_01_output_kinematics.h"
#include "m06_02_output_inverse_dynamics_values.h"
#include "m06_02_output_trad_ida.h"
#include "m06_03_output_opt_values.h"
#include "m06_03_output_opt_values_2.h"
#include "m06_03_output_opt_values_3.h"
#include "m06_03_output_opt_values_4.h"
#include "m06_04_output_muscle_geometry.h"
#include "m06_05_output_bony_geometry.h"
#include "m06_06_output_segment_lcs.h"
#include "m06_07_output_marker.h"
#include "m06_08_output_ref_coord.h"


using namespace std;

/*



*/

void m06_00_output_data(Segment **segment_data[], Structure *calibrate_pos[], Muscle **muscle_data[], Muscle *patellar_tendon[], Muscle **ligament_data[], 
						int frames, int segments, int muscles, int ligaments, XML_Parameters* _parameters)
{
	string output_folder_for_visualisation = _parameters->output_file_root_for_visualisation;
	string output_folder_for_optimisation = _parameters->output_file_root_for_optimisation;
	string study_name = _parameters->study_name;

	m06_01_output_kinematics(segment_data,frames,segments, output_folder_for_optimisation, study_name);
	
    m06_02_output_inverse_dynamics_values(segment_data, frames, segments, output_folder_for_optimisation, study_name);


//	m06_03_output_opt_values(segment_data,muscle_data,patellar_tendon,ligament_data,frames,segments,muscles,ligaments);
//	m06_03_output_opt_values_2(segment_data,muscle_data,patellar_tendon,ligament_data,frames,segments,muscles,ligaments);
	m06_03_output_opt_values_3(segment_data,muscle_data,patellar_tendon,ligament_data,frames,segments,muscles,ligaments, output_folder_for_optimisation, study_name);
//	m06_03_output_opt_values_4(segment_data,muscle_data,patellar_tendon,ligament_data,frames,segments,muscles,ligaments);

	for (int i=0; i<muscles; i++) 
	{
		m06_04_output_muscle_geometry(muscle_data,frames,i, output_folder_for_visualisation, study_name);
	}

	m06_05_output_bony_geometry(segment_data,patellar_tendon,calibrate_pos,frames,segments, output_folder_for_visualisation, study_name);

	m06_06_output_segment_lcs(segment_data,frames,segments, output_folder_for_optimisation, study_name);

	m06_07_output_marker(segment_data, calibrate_pos, frames, segments,output_folder_for_visualisation, study_name);

	m06_08_output_ref_coord(calibrate_pos, _parameters, segments,output_folder_for_visualisation, study_name);


	cout << "6.";

	return;
}