/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxAppUtils for documentation
 *
 */
#pragma once

//
// 3x3 matrix manipulation routines for
// projection warping from a modified Theo example on the OF forums:
// http://threeblindmiceandamonkey.com/?p=31
//

// multiply matrix: c = a * b
void multiplyMatrix(double a[3][3], double b[3][3], double c[3][3]) {
	c[0][0] = a[0][0]*b[0][0] + a[0][1]*b[1][0] + a[0][2]*b[2][0];
	c[0][1] = a[0][0]*b[0][1] + a[0][1]*b[1][1] + a[0][2]*b[2][1];
	c[0][2] = a[0][0]*b[0][2] + a[0][1]*b[1][2] + a[0][2]*b[2][2];
	c[1][0] = a[1][0]*b[0][0] + a[1][1]*b[1][0] + a[1][2]*b[2][0];
	c[1][1] = a[1][0]*b[0][1] + a[1][1]*b[1][1] + a[1][2]*b[2][1];
	c[1][2] = a[1][0]*b[0][2] + a[1][1]*b[1][2] + a[1][2]*b[2][2];
	c[2][0] = a[2][0]*b[0][0] + a[2][1]*b[1][0] + a[2][2]*b[2][0];
	c[2][1] = a[2][0]*b[0][1] + a[2][1]*b[1][1] + a[2][2]*b[2][1];
	c[2][2] = a[2][0]*b[0][2] + a[2][1]*b[1][2] + a[2][2]*b[2][2];
}

// determinant of a 2x2 matrix
double det2(double a, double b, double c, double d) {
	return( a*d - b*c);
}

// adjoint matrix: b = adjoint(a); returns determinant(a)
double adjointMatrix(double a[3][3], double b[3][3]) {
	b[0][0] = det2(a[1][1], a[1][2], a[2][1], a[2][2]);
	b[1][0] = det2(a[1][2], a[1][0], a[2][2], a[2][0]);
	b[2][0] = det2(a[1][0], a[1][1], a[2][0], a[2][1]);
	b[0][1] = det2(a[2][1], a[2][2], a[0][1], a[0][2]);
	b[1][1] = det2(a[2][2], a[2][0], a[0][2], a[0][0]);
	b[2][1] = det2(a[2][0], a[2][1], a[0][0], a[0][1]);
	b[0][2] = det2(a[0][1], a[0][2], a[1][1], a[1][2]);
	b[1][2] = det2(a[0][2], a[0][0], a[1][2], a[1][0]);
	b[2][2] = det2(a[0][0], a[0][1], a[1][0], a[1][1]);
	return a[0][0]*b[0][0] + a[0][1]*b[0][1] + a[0][2]*b[0][2];
}

#define MATRIX_TOLERANCE 1e-13
#define MATRIX_ZERO(x) ((x)<MATRIX_TOLERANCE && (x)>-MATRIX_TOLERANCE)

// calculate matrix for unit square to quad mapping
void mapSquareToQuad(double quad[4][2],    // vertices of quadrilateral
                     double   SQ[3][3]) {  // square->quad transform
	double px, py;

	px = quad[0][0]-quad[1][0]+quad[2][0]-quad[3][0];
	py = quad[0][1]-quad[1][1]+quad[2][1]-quad[3][1];

	if (MATRIX_ZERO(px) && MATRIX_ZERO(py)) {
		SQ[0][0] = quad[1][0]-quad[0][0];
		SQ[1][0] = quad[2][0]-quad[1][0];
		SQ[2][0] = quad[0][0];
		SQ[0][1] = quad[1][1]-quad[0][1];
		SQ[1][1] = quad[2][1]-quad[1][1];
		SQ[2][1] = quad[0][1];
		SQ[0][2] = 0.;
		SQ[1][2] = 0.;
		SQ[2][2] = 1.;
		return;
	}
	else {
		double dx1, dx2, dy1, dy2, del;

		dx1 = quad[1][0]-quad[2][0];
		dx2 = quad[3][0]-quad[2][0];
		dy1 = quad[1][1]-quad[2][1];
		dy2 = quad[3][1]-quad[2][1];
		del = det2(dx1,dx2, dy1,dy2);

		SQ[0][2] = det2(px,dx2, py,dy2)/del;
		SQ[1][2] = det2(dx1,px, dy1,py)/del;
		SQ[2][2] = 1.;
		SQ[0][0] = quad[1][0]-quad[0][0]+SQ[0][2]*quad[1][0];
		SQ[1][0] = quad[3][0]-quad[0][0]+SQ[1][2]*quad[3][0];
		SQ[2][0] = quad[0][0];
		SQ[0][1] = quad[1][1]-quad[0][1]+SQ[0][2]*quad[1][1];
		SQ[1][1] = quad[3][1]-quad[0][1]+SQ[1][2]*quad[3][1];
		SQ[2][1] = quad[0][1];
	}
}

// calculate matrix for general quad to quad mapping
void mapQuadToQuad(double  in[4][2],    // starting quad
                   double out[4][2],    // target quad
                   double  ST[3][3]) {  // the matrix (returned)
					
	double quad[4][2], MS[3][3];
	double SM[3][3], MT[3][3];

	quad[0][0] = in[0][0]; quad[0][1] = in[0][1];
	quad[1][0] = in[1][0]; quad[1][1] = in[1][1];
	quad[2][0] = in[2][0]; quad[2][1] = in[2][1];
	quad[3][0] = in[3][0]; quad[3][1] = in[3][1];
	mapSquareToQuad(quad, MS);
	adjointMatrix(MS, SM);

	quad[0][0] = out[0][0] ; quad[0][1] = out[0][1] ;
	quad[1][0] = out[1][0] ; quad[1][1] = out[1][1] ;
	quad[2][0] = out[2][0] ; quad[2][1] = out[2][1] ;
	quad[3][0] = out[3][0] ; quad[3][1] = out[3][1] ;
	mapSquareToQuad(quad, MT);

	multiplyMatrix(SM, MT, ST);
}
