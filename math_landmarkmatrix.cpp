#include "stdafx.h"
#include "nr.h"
#include "mathematics.h"
using namespace std;

/*

Zero a given matrix

*/


void MATHEMATICS::math_landmarkmatrix(Mat_DP &m) {

	int x=m.nrows();
	int y=m.ncols();

	for (int i=0; i<x; i++) {
		for (int j=0; j<y; j++) {
			m[i][j]=-1;
		}
	}

	return;
}