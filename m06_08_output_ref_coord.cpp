#include "stdafx.h"
#include "m06_08_output_ref_coord.h"
#include "io_dataoutput.h"


using namespace std;

/*



*/



void m06_08_output_ref_coord(Structure *calibrate_pos[], XML_Parameters* _parameters, int segments, string output_folder_for_visualisation, string study_name)
{
	Mat_DP ref_origin(1,3*segments), ref_orientation(1,4*segments), ref_bony_landmark(1, 27);

	Vec_DP RASIS(3), LASIS(3), RPSIS(3), LPSIS(3), hip_COR(3), FLE(3), FME(3), FAM(3), TAM(3), FM2(3), patella_ten_origin(3); 

	RASIS = _parameters->anatomy->RASIS_landmark_coord;
	LASIS = _parameters->anatomy->LASIS_landmark_coord;
	RPSIS = _parameters->anatomy->RPSIS_landmark_coord;
	LPSIS = _parameters->anatomy->LPSIS_landmark_coord;
	hip_COR = _parameters->anatomy->hip_rotation_centre_coord;
	FLE = _parameters->anatomy->FLE_landmark_coord;
	FME = _parameters->anatomy->FME_landmark_coord;
	TAM = _parameters->anatomy->TAM_landmark_coord;
	FAM = _parameters->anatomy->FAM_landmark_coord;
	FM2 = _parameters->anatomy->FM2_landmark_coord;
	patella_ten_origin = _parameters->anatomy->patella_origin_coord;

	for (int ii = 0;ii<3; ii++)
	{
		ref_bony_landmark[0][ii] = RASIS[ii];
		ref_bony_landmark[0][ii+3] = LASIS[ii];
		ref_bony_landmark[0][ii+6] = RPSIS[ii];
		ref_bony_landmark[0][ii+9]= LPSIS[ii];
		ref_bony_landmark[0][ii+12] = FLE[ii];
		ref_bony_landmark[0][ii+15] = FME[ii];
		ref_bony_landmark[0][ii+18] = FAM[ii];
		ref_bony_landmark[0][ii+21] = TAM[ii];
		ref_bony_landmark[0][ii+24] = FM2[ii];

	}

	for (int ss=0; ss<segments; ss++)
	{
		for (int ll=0;ll<3; ll++)
		{
			ref_origin[0][ll] = (FAM[ll]+TAM[ll])/2;
			ref_origin[0][ll+3] = (FLE[ll]+FME[ll])/2;
			ref_origin[0][ll+6] = hip_COR[ll];
			ref_origin[0][ll+9] = (RASIS[ll]+LASIS[ll])/2;
			ref_origin[0][ll+12] = patella_ten_origin[ll];
		}
				  
		
		for (int jj=0; jj<4; jj++)
		{
			ref_orientation[0][ss*4+jj] = calibrate_pos[ss]->rot_lcs_to_horsman[jj];
		}
	}
	
	io_dataoutput(output_folder_for_visualisation + "\\" + study_name + "_"+ "anatomy_model_bony_landmark.csv",ref_bony_landmark);
	io_dataoutput(output_folder_for_visualisation + "\\" + study_name + "_"+ "anatomy_model_origin.csv",ref_origin);
	io_dataoutput(output_folder_for_visualisation + "\\" + study_name + "_"+ "anatomy_model_orientation.csv",ref_orientation);

	return;
}




