//----------------------------------------------------------------------------
// e_quaternion.cpp
//
// (C)Copyright 2003, Flagship Studios. All rights reserved.
//----------------------------------------------------------------------------

#include "e_pch.h"

#include "e_quaternion.h"


//namespace FSSE
//{;

//----------------------------------------------------------------------------
// GLOBALS
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// IMPLEMENTATION
//----------------------------------------------------------------------------

/* Portions Copyright (C) Alex Vlachos and John Isidoro, 2001 */

//#include <stdlib.h>
//#include <math.h>

#define QUAT_DELTA 1e-6 //error tolerance

// FUNCTIONS ================================================================
void QuatNormalize(Quaternion *quat)
{
	float dist;
	float square;

	square = (quat->x*quat->x) + (quat->y*quat->y) + (quat->z*quat->z) + (quat->w*quat->w);

	if (square > 0.0f)
		dist = 1.0f / (float)sqrt(square);
	else 
		dist = 1.0f;

	quat->x *= dist;
	quat->y *= dist;
	quat->z *= dist;
	quat->w *= dist;
}

//===========================================================================
// Converts quaternion to a matrix
//===========================================================================
void QuatToMatrix (const Quaternion *quat, MATRIX & matrix)
{
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

	x2 = quat->x + quat->x;
	y2 = quat->y + quat->y;
	z2 = quat->z + quat->z;

	xx = quat->x * x2;   xy = quat->x * y2;   xz = quat->x * z2;
	yy = quat->y * y2;   yz = quat->y * z2;   zz = quat->z * z2;
	wx = quat->w * x2;   wy = quat->w * y2;   wz = quat->w * z2;

	matrix[0] = 1.0f - (yy + zz);
	matrix[4] = xy - wz;
	matrix[8] = xz + wy;
	matrix[12] = 0.0f;

	matrix[1] = xy + wz;
	matrix[5] = 1.0f - (xx + zz);
	matrix[9] = yz - wx;
	matrix[13] = 0.0f;

	matrix[2] = xz - wy;
	matrix[6] = yz + wx;
	matrix[10] = 1.0f - (xx + yy);
	matrix[14] = 0.0f;

	matrix[3] = 0.0f;
	matrix[7] = 0.0f;
	matrix[11] = 0.0f;
	matrix[15] = 1.0f;
}

//===========================================================================
// Converts matrix representation of a rotation to a quaternion representation
//===========================================================================
void MatrixToQuat (const MATRIX & matrix, Quaternion *quat)
{
	int i;
	int j;
	int k;

	float tr;
	float s;
	float q[4];

	int nxt[3] = {1, 2, 0};

	tr = matrix[0] + matrix[5] + matrix[10];

	//check the diagonal
	if (tr > 0.0f) 
	{
		s = (float)sqrt(tr + 1.0f);

		quat->w = s / 2.0f;

		s = 0.5f / s;

		quat->x = (matrix[6] - matrix[9]) * s;
		quat->y = (matrix[8] - matrix[2]) * s;
		quat->z = (matrix[1] - matrix[4]) * s;
	}
	else
	{
		// diagonal is negative
		i = 0;

		if (matrix[5] > matrix[0]) 
			i = 1;
		if (matrix[10] > matrix[i + i*4]) 
			i = 2;
		j = nxt[i];
		k = nxt[j];


		s = (float)sqrt((matrix[i + i*4] - (matrix[j + j*4] + matrix[k + k*4])) + 1.0f);

		q[i] = s * 0.5f;

		if (s != 0.0f) 
			s = 0.5f / s;

		q[3] = (matrix[k + j*4] - matrix[j + k*4]) * s;
		q[j] = (matrix[j + i*4] + matrix[i + j*4]) * s;
		q[k] = (matrix[k + i*4] + matrix[i + k*4]) * s;
		quat->x = q[0];
		quat->y = q[1];
		quat->z = q[2];
		quat->w = q[3];
	}

	QuatNormalize(quat);
}

//===========================================================================
// Disassembles quaternion to an axis and an angle
//===========================================================================
void QuaternionToGlRotation (Quaternion *quat, float *x, float *y, float *z, float *degrees)
{
	float len;

	len = (quat->x*quat->x) + (quat->y*quat->y) + (quat->z*quat->z);

	if (len > QUAT_DELTA) 
	{
		*x = quat->x * (1.0f / len);
		*y = quat->y * (1.0f / len);
		*z = quat->z * (1.0f / len);
		*degrees = 2.0f * (float)acos(quat->w) * (180.0f/PI);
	}
	else
	{
		*x = 0.0;
		*y = 0.0;
		*z = 1.0;
		*degrees = 0.0;
	}
}

//===========================================================================
// Assembles quaternion from an axis and an angle
//===========================================================================
void GlRotationToQuaternion (Quaternion *quat, float x, float y, float z, float degrees)
{
	float dist;
	float radians;
	float tmpf;

	radians = degrees * (PI/180.0f);

	//Normalize
	tmpf = (float)sqrt(x*x + y*y + z*z);

	if (tmpf != 0.0f)
	{
		dist = (float)-sin(radians * 0.5f) / tmpf;
		x *= dist;
		y *= dist;
		z *= dist;
	}

	quat->x = x;
	quat->y = y;
	quat->z = z;

	quat->w = (float)cos(radians * 0.5f);
}

//===========================================================================
// Multiplies two quaternions
//===========================================================================
void QuatMul (Quaternion *q1, Quaternion *q2, Quaternion *result)
{
	Quaternion tmpq;

	tmpq.x = (q1->w*q2->x) + (q1->x*q2->w) + (q1->y*q2->z) - (q1->z*q2->y);
	tmpq.y = (q1->w*q2->y) + (q1->y*q2->w) + (q1->z*q2->x) - (q1->x*q2->z);
	tmpq.z = (q1->w*q2->z) + (q1->z*q2->w) + (q1->x*q2->y) - (q1->y*q2->x);
	tmpq.w = (q1->w*q2->w) - (q1->x*q2->x) - (q1->y*q2->y) - (q1->z*q2->z);

	QuatCopy(result, &tmpq);

	//Make sure the resulting quaternion is normalized
	QuatNormalize(result);
}

//===========================================================================
// Copy: q1 = q2
//===========================================================================
void QuatCopy (Quaternion *q1, Quaternion *q2)
{
	q1->x = q2->x;
	q1->y = q2->y;
	q1->z = q2->z;
	q1->w = q2->w;
}

//===========================================================================
// Calculates natural logarithm of a quaternion
//===========================================================================
void QuatLog (Quaternion *q1, Quaternion *q2)
{
	float length;

	length = (float)sqrt(q1->x * q1->x + q1->y * q1->y + q1->z * q1->z);

	//make sure we do not divide by 0
	if (q1->w != 0.0) 
		length = (float)atan(length / q1->w); 
	else 
		length = PI*0.5f;

	q2->x = q1->x * length;
	q2->y = q1->y * length;
	q2->z = q1->z * length;
	q2->w = 0.0f;
}

//===========================================================================
// Computes the dot product of two unit quaternions
//===========================================================================
float QuatDot (Quaternion *q1, Quaternion *q2)
{
	return (float)(q1->x*q2->x + q1->y*q2->y + q1->z*q2->z + q1->w*q2->w);
}

//===========================================================================
void QuatInverse (Quaternion *q, Quaternion *result)
{
	float tmpf;

	tmpf = 1.0f / (q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);

	result->x = -q->x * tmpf;
	result->y = -q->y * tmpf;
	result->z = -q->z * tmpf;
	result->w =  q->w * tmpf;
}

//===========================================================================
// Calculates exponent of a quaternion
//===========================================================================
void QuatExp (Quaternion *q1, Quaternion *q2)
{
	float len1;
	float len2;

	len1 = (float) sqrt (q1->x*q1->x + q1->y*q1->y + q1->z*q1->z);
	if (len1 > 0.0) 
		len2 = (float)sin(len1) / len1; 
	else 
		len2 = 1.0;

	q2->x = q1->x * len2;
	q2->y = q1->y * len2;
	q2->z = q1->z * len2;
	q2->w = (float)cos(len1);
}

//===========================================================================
void QuatSlerpShortestPath (Quaternion *result, Quaternion *from, Quaternion *to, float t)
{
	Quaternion tmpFrom;

	if (QuatDot(from, to) < 0.0f)
	{
		tmpFrom.x = -from->x;
		tmpFrom.y = -from->y;
		tmpFrom.z = -from->z;
		tmpFrom.w = -from->w;
		QuatSlerp (result, &tmpFrom, to, t);
	}
	else
	{
		QuatSlerp (result, from, to, t);
	}

	QuatNormalize(result);
}

//===========================================================================
// Smoothly (spherically, shortest path on a quaternion sphere) 
// interpolates between two UNIT quaternion positions
//===========================================================================
// As t goes from 0 to 1, qt goes from p to q.
// slerp(p,q,t) = (p*sin((1-t)*omega) + q*sin(t*omega)) / sin(omega)
//===========================================================================
void QuatSlerp (Quaternion *result, Quaternion *from, Quaternion *to, float t)
{
	float to1[4];
	double omega;
	double cosom;
	double sinom;
	double scale0;
	double scale1;

	//Calculate cosine
	cosom = QuatDot(from, to);

	//Adjust signs (if necessary)
	if (cosom < 0.0)
	{
		cosom = -cosom;

		to1[0] = -to->x;
		to1[1] = -to->y;
		to1[2] = -to->z;
		to1[3] = -to->w;
	}
	else
	{
		to1[0] = to->x;
		to1[1] = to->y;
		to1[2] = to->z;
		to1[3] = to->w;
	}

	//Calculate coefficients
	if ((1.0-cosom) > QUAT_DELTA)
	{
		//Standard case (slerp)
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = sin((1.0f - t) * omega) / sinom;
		scale1 = sin(t * omega) / sinom;
	}
	else
	{
		//"from" and "to" quaternions are very close
		//... so we can do a linear interpolation
		scale0 = 1.0f - t;
		scale1 = t;
	}

	// calculate final values
	result->x = (float)((scale0 * from->x) + (scale1 * to1[0]));
	result->y = (float)((scale0 * from->y) + (scale1 * to1[1]));
	result->z = (float)((scale0 * from->z) + (scale1 * to1[2]));
	result->w = (float)((scale0 * from->w) + (scale1 * to1[3]));
}

//===========================================================================
// Compute a spherical quadratic interpolation
//===========================================================================
void QuatSQuad (float t, Quaternion *q0, Quaternion *q1, Quaternion *q2, Quaternion *q3, Quaternion *result)
{
	Quaternion tmp1;
	Quaternion tmp2;

	QuatSlerp(&tmp1, q0, q3, t);
	QuatSlerp(&tmp2, q1, q2, t);
	QuatSlerp(result, &tmp1, &tmp2, (2.0f*t*(1.0f-t)));

	//QuatSlerp(q0, q3, t, result); //Linear interpolation

	QuatNormalize(result);
}

//===========================================================================
// Cross product of 3 quaternions: result = (q1 X q2 X q3)
//
// This is done by creating the matrix:
//      i   j   k   m
//      x1  x2  x3  x4
//      y1  y2  y3  y4
//      z1  z2  z3  z4
//      w1  w2  w3  w4
// and solving for i, j, k, and m. These 4 values compose the resulting
// Quaternion: result = (i, j, k, m)
//===========================================================================
void QuatAxBxC (Quaternion *q1, Quaternion *q2, Quaternion *q3, Quaternion *result)
{
	float det[3][3]; //Determinant to pass to function

	/*************/
	/* result->x */
	/*************/
	det[0][0] = q1->y;
	det[0][1] = q1->z;
	det[0][2] = q1->w;

	det[1][0] = q2->y;
	det[1][1] = q2->z;
	det[1][2] = q2->w;

	det[2][0] = q3->y;
	det[2][1] = q3->z;
	det[2][2] = q3->w;

	result->x = Determinant3x3(det);

	/*************/
	/* result->y */
	/*************/
	det[0][0] = q1->x;
	det[0][1] = q1->z;
	det[0][2] = q1->w;

	det[1][0] = q2->x;
	det[1][1] = q2->z;
	det[1][2] = q2->w;

	det[2][0] = q3->x;
	det[2][1] = q3->z;
	det[2][2] = q3->w;

	result->y = -Determinant3x3(det);

	/*************/
	/* result->z */
	/*************/
	det[0][0] = q1->x;
	det[0][1] = q1->y;
	det[0][2] = q1->w;

	det[1][0] = q2->x;
	det[1][1] = q2->y;
	det[1][2] = q2->w;

	det[2][0] = q3->x;
	det[2][1] = q3->y;
	det[2][2] = q3->w;

	result->z = Determinant3x3(det);

	/*************/
	/* result->w */
	/*************/
	det[0][0] = q1->x;
	det[0][1] = q1->y;
	det[0][2] = q1->z;

	det[1][0] = q2->x;
	det[1][1] = q2->y;
	det[1][2] = q2->z;

	det[2][0] = q3->x;
	det[2][1] = q3->y;
	det[2][2] = q3->z;

	result->w = -Determinant3x3(det);
}

//===========================================================================
// Determinant of 3x3 matrix
//===========================================================================
float Determinant3x3 (float det[3][3])
{
	float tmpf1;
	float tmpf2;
	float tmpf3;

	tmpf1 = det[0][0] * (det[1][1]*det[2][2] - det[1][2]*det[2][1]);
	tmpf2 = det[0][1] * (det[1][0]*det[2][2] - det[1][2]*det[2][0]);
	tmpf3 = det[0][2] * (det[1][0]*det[2][1] - det[1][1]*det[2][0]);

	return (tmpf1 - tmpf2 + tmpf3);
}


void QuatFromDirectionVector( Quaternion & tQuat, const VECTOR & vDirection )
{
	tQuat.Set( 1,0,0,0 );

	// build a facing matrix
	MATRIX mFacing;
	VECTOR vPos(0,0,0);
	VECTOR vUp(0,0,1);
	MatrixFromVectors( mFacing, vPos, vUp, vDirection );
	MatrixToQuat( mFacing, &tQuat );
	QuatNormalize( &tQuat );
}

#undef QUAT_DELTA


//}; // FSSE
