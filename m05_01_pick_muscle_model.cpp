#include "stdafx.h"
#include "m05_01_pick_muscle_model.h"
#include "m05_02_horsman_muscle_model.h"





using namespace std;

/*

This routine is here to allow the option to use other muscle models (they can be selected here)

*/

void m05_01_pick_muscle_model(Muscle *muscle_model[], Structure *calibrate_pos[], int muscles, string filename_model, XML_Parameters* _parameters)
{

	m05_02_horsman_muscle_model(muscle_model,calibrate_pos,muscles, filename_model, _parameters);

	return;
}