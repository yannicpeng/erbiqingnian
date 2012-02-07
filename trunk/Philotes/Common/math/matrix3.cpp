

#include "matrix3.h"


// Adapted from Matrix math by Wild Magic http://www.geometrictools.com/

namespace Math
{
    const scalar Matrix3::EPSILON = (scalar)1e-06;
    const Matrix3 Matrix3::ZERO(0,0,0,0,0,0,0,0,0);
    const Matrix3 Matrix3::IDENTITY(1,0,0,0,1,0,0,0,1);
    const scalar Matrix3::ms_fSvdEpsilon = (scalar)1e-04;
    const unsigned int Matrix3::ms_iSvdMaxIterations = 32;

    //-----------------------------------------------------------------------
    Vector3 Matrix3::GetColumn (size_t iCol) const
    {
        ph_assert( 0 <= iCol && iCol < 3 );
        return Vector3(m[0][iCol],m[1][iCol],
            m[2][iCol]);
    }
    //-----------------------------------------------------------------------
    void Matrix3::SetColumn(size_t iCol, const Vector3& vec)
    {
        ph_assert( 0 <= iCol && iCol < 3 );
        m[0][iCol] = vec.x;
        m[1][iCol] = vec.y;
        m[2][iCol] = vec.z;

    }
    //-----------------------------------------------------------------------
    void Matrix3::FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
    {
        SetColumn(0,xAxis);
        SetColumn(1,yAxis);
        SetColumn(2,zAxis);

    }

    //-----------------------------------------------------------------------
    bool Matrix3::operator== (const Matrix3& rkMatrix) const
    {
        for (size_t iRow = 0; iRow < 3; iRow++)
        {
            for (size_t iCol = 0; iCol < 3; iCol++)
            {
                if ( m[iRow][iCol] != rkMatrix.m[iRow][iCol] )
                    return false;
            }
        }

        return true;
    }
    //-----------------------------------------------------------------------
    Matrix3 Matrix3::operator+ (const Matrix3& rkMatrix) const
    {
        Matrix3 kSum;
        for (size_t iRow = 0; iRow < 3; iRow++)
        {
            for (size_t iCol = 0; iCol < 3; iCol++)
            {
                kSum.m[iRow][iCol] = m[iRow][iCol] +
                    rkMatrix.m[iRow][iCol];
            }
        }
        return kSum;
    }
    //-----------------------------------------------------------------------
    Matrix3 Matrix3::operator- (const Matrix3& rkMatrix) const
    {
        Matrix3 kDiff;
        for (size_t iRow = 0; iRow < 3; iRow++)
        {
            for (size_t iCol = 0; iCol < 3; iCol++)
            {
                kDiff.m[iRow][iCol] = m[iRow][iCol] -
                    rkMatrix.m[iRow][iCol];
            }
        }
        return kDiff;
    }
    //-----------------------------------------------------------------------
    Matrix3 Matrix3::operator* (const Matrix3& rkMatrix) const
    {
        Matrix3 kProd;
        for (size_t iRow = 0; iRow < 3; iRow++)
        {
            for (size_t iCol = 0; iCol < 3; iCol++)
            {
                kProd.m[iRow][iCol] =
                    m[iRow][0]*rkMatrix.m[0][iCol] +
                    m[iRow][1]*rkMatrix.m[1][iCol] +
                    m[iRow][2]*rkMatrix.m[2][iCol];
            }
        }
        return kProd;
    }
    //-----------------------------------------------------------------------
    Vector3 Matrix3::operator* (const Vector3& rkPoint) const
    {
        Vector3 kProd;
        for (size_t iRow = 0; iRow < 3; iRow++)
        {
            kProd[iRow] =
                m[iRow][0]*rkPoint[0] +
                m[iRow][1]*rkPoint[1] +
                m[iRow][2]*rkPoint[2];
        }
        return kProd;
    }
    //-----------------------------------------------------------------------
    Vector3 operator* (const Vector3& rkPoint, const Matrix3& rkMatrix)
    {
        Vector3 kProd;
        for (size_t iRow = 0; iRow < 3; iRow++)
        {
            kProd[iRow] =
                rkPoint[0]*rkMatrix.m[0][iRow] +
                rkPoint[1]*rkMatrix.m[1][iRow] +
                rkPoint[2]*rkMatrix.m[2][iRow];
        }
        return kProd;
    }
    //-----------------------------------------------------------------------
    Matrix3 Matrix3::operator- () const
    {
        Matrix3 kNeg;
        for (size_t iRow = 0; iRow < 3; iRow++)
        {
            for (size_t iCol = 0; iCol < 3; iCol++)
                kNeg[iRow][iCol] = -m[iRow][iCol];
        }
        return kNeg;
    }
    //-----------------------------------------------------------------------
    Matrix3 Matrix3::operator* (scalar fScalar) const
    {
        Matrix3 kProd;
        for (size_t iRow = 0; iRow < 3; iRow++)
        {
            for (size_t iCol = 0; iCol < 3; iCol++)
                kProd[iRow][iCol] = fScalar*m[iRow][iCol];
        }
        return kProd;
    }
    //-----------------------------------------------------------------------
    Matrix3 operator* (scalar fScalar, const Matrix3& rkMatrix)
    {
        Matrix3 kProd;
        for (size_t iRow = 0; iRow < 3; iRow++)
        {
            for (size_t iCol = 0; iCol < 3; iCol++)
                kProd[iRow][iCol] = fScalar*rkMatrix.m[iRow][iCol];
        }
        return kProd;
    }
    //-----------------------------------------------------------------------
    Matrix3 Matrix3::Transpose () const
    {
        Matrix3 kTranspose;
        for (size_t iRow = 0; iRow < 3; iRow++)
        {
            for (size_t iCol = 0; iCol < 3; iCol++)
                kTranspose[iRow][iCol] = m[iCol][iRow];
        }
        return kTranspose;
    }
    //-----------------------------------------------------------------------
    bool Matrix3::Inverse (Matrix3& rkInverse, scalar fTolerance) const
    {
        // Invert a 3x3 using cofactors.  This is about 8 times faster than
        // the Numerical Recipes code which uses Gaussian elimination.

        rkInverse[0][0] = m[1][1]*m[2][2] -
            m[1][2]*m[2][1];
        rkInverse[0][1] = m[0][2]*m[2][1] -
            m[0][1]*m[2][2];
        rkInverse[0][2] = m[0][1]*m[1][2] -
            m[0][2]*m[1][1];
        rkInverse[1][0] = m[1][2]*m[2][0] -
            m[1][0]*m[2][2];
        rkInverse[1][1] = m[0][0]*m[2][2] -
            m[0][2]*m[2][0];
        rkInverse[1][2] = m[0][2]*m[1][0] -
            m[0][0]*m[1][2];
        rkInverse[2][0] = m[1][0]*m[2][1] -
            m[1][1]*m[2][0];
        rkInverse[2][1] = m[0][1]*m[2][0] -
            m[0][0]*m[2][1];
        rkInverse[2][2] = m[0][0]*m[1][1] -
            m[0][1]*m[1][0];

        scalar fDet =
            m[0][0]*rkInverse[0][0] +
            m[0][1]*rkInverse[1][0]+
            m[0][2]*rkInverse[2][0];

        if ( fabs(fDet) <= fTolerance )
            return false;

        scalar fInvDet = 1.0f/fDet;
        for (size_t iRow = 0; iRow < 3; iRow++)
        {
            for (size_t iCol = 0; iCol < 3; iCol++)
                rkInverse[iRow][iCol] *= fInvDet;
        }

        return true;
    }
    //-----------------------------------------------------------------------
    Matrix3 Matrix3::Inverse (scalar fTolerance) const
    {
        Matrix3 kInverse = Matrix3::ZERO;
        Inverse(kInverse,fTolerance);
        return kInverse;
    }
    //-----------------------------------------------------------------------
    scalar Matrix3::Determinant () const
    {
        scalar fCofactor00 = m[1][1]*m[2][2] -
            m[1][2]*m[2][1];
        scalar fCofactor10 = m[1][2]*m[2][0] -
            m[1][0]*m[2][2];
        scalar fCofactor20 = m[1][0]*m[2][1] -
            m[1][1]*m[2][0];

        scalar fDet =
            m[0][0]*fCofactor00 +
            m[0][1]*fCofactor10 +
            m[0][2]*fCofactor20;

        return fDet;
    }
    //-----------------------------------------------------------------------
    void Matrix3::Bidiagonalize (Matrix3& kA, Matrix3& kL,
        Matrix3& kR)
    {
        scalar afV[3], afW[3];
        scalar fLength, fSign, fT1, fInvT1, fT2;
        bool bIdentity;

        // map first column to (*,0,0)
        fLength = MathMisc::Sqrt(kA[0][0]*kA[0][0] + kA[1][0]*kA[1][0] +
            kA[2][0]*kA[2][0]);
        if ( fLength > 0.0 )
        {
            fSign = (kA[0][0] > 0.0f ? 1.0f : -1.0f);
            fT1 = kA[0][0] + fSign*fLength;
            fInvT1 = 1.0f/fT1;
            afV[1] = kA[1][0]*fInvT1;
            afV[2] = kA[2][0]*fInvT1;

            fT2 = -2.0f/(1.0f+afV[1]*afV[1]+afV[2]*afV[2]);
            afW[0] = fT2*(kA[0][0]+kA[1][0]*afV[1]+kA[2][0]*afV[2]);
            afW[1] = fT2*(kA[0][1]+kA[1][1]*afV[1]+kA[2][1]*afV[2]);
            afW[2] = fT2*(kA[0][2]+kA[1][2]*afV[1]+kA[2][2]*afV[2]);
            kA[0][0] += afW[0];
            kA[0][1] += afW[1];
            kA[0][2] += afW[2];
            kA[1][1] += afV[1]*afW[1];
            kA[1][2] += afV[1]*afW[2];
            kA[2][1] += afV[2]*afW[1];
            kA[2][2] += afV[2]*afW[2];

            kL[0][0] = 1.0f+fT2;
            kL[0][1] = kL[1][0] = fT2*afV[1];
            kL[0][2] = kL[2][0] = fT2*afV[2];
            kL[1][1] = 1.0f+fT2*afV[1]*afV[1];
            kL[1][2] = kL[2][1] = fT2*afV[1]*afV[2];
            kL[2][2] = 1.0f+fT2*afV[2]*afV[2];
            bIdentity = false;
        }
        else
        {
            kL = Matrix3::IDENTITY;
            bIdentity = true;
        }

        // map first row to (*,*,0)
        fLength = MathMisc::Sqrt(kA[0][1]*kA[0][1]+kA[0][2]*kA[0][2]);
        if ( fLength > 0.0 )
        {
            fSign = (kA[0][1] > 0.0f ? 1.0f : -1.0f);
            fT1 = kA[0][1] + fSign*fLength;
            afV[2] = kA[0][2]/fT1;

            fT2 = -2.0f/(1.0f+afV[2]*afV[2]);
            afW[0] = fT2*(kA[0][1]+kA[0][2]*afV[2]);
            afW[1] = fT2*(kA[1][1]+kA[1][2]*afV[2]);
            afW[2] = fT2*(kA[2][1]+kA[2][2]*afV[2]);
            kA[0][1] += afW[0];
            kA[1][1] += afW[1];
            kA[1][2] += afW[1]*afV[2];
            kA[2][1] += afW[2];
            kA[2][2] += afW[2]*afV[2];

            kR[0][0] = 1.0;
            kR[0][1] = kR[1][0] = 0.0;
            kR[0][2] = kR[2][0] = 0.0;
            kR[1][1] = 1.0f+fT2;
            kR[1][2] = kR[2][1] = fT2*afV[2];
            kR[2][2] = 1.0f+fT2*afV[2]*afV[2];
        }
        else
        {
            kR = Matrix3::IDENTITY;
        }

        // map second column to (*,*,0)
        fLength = MathMisc::Sqrt(kA[1][1]*kA[1][1]+kA[2][1]*kA[2][1]);
        if ( fLength > 0.0 )
        {
            fSign = (kA[1][1] > 0.0f ? 1.0f : -1.0f);
            fT1 = kA[1][1] + fSign*fLength;
            afV[2] = kA[2][1]/fT1;

            fT2 = -2.0f/(1.0f+afV[2]*afV[2]);
            afW[1] = fT2*(kA[1][1]+kA[2][1]*afV[2]);
            afW[2] = fT2*(kA[1][2]+kA[2][2]*afV[2]);
            kA[1][1] += afW[1];
            kA[1][2] += afW[2];
            kA[2][2] += afV[2]*afW[2];

            scalar fA = 1.0f+fT2;
            scalar fB = fT2*afV[2];
            scalar fC = 1.0f+fB*afV[2];

            if ( bIdentity )
            {
                kL[0][0] = 1.0;
                kL[0][1] = kL[1][0] = 0.0;
                kL[0][2] = kL[2][0] = 0.0;
                kL[1][1] = fA;
                kL[1][2] = kL[2][1] = fB;
                kL[2][2] = fC;
            }
            else
            {
                for (int iRow = 0; iRow < 3; iRow++)
                {
                    scalar fTmp0 = kL[iRow][1];
                    scalar fTmp1 = kL[iRow][2];
                    kL[iRow][1] = fA*fTmp0+fB*fTmp1;
                    kL[iRow][2] = fB*fTmp0+fC*fTmp1;
                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void Matrix3::GolubKahanStep (Matrix3& kA, Matrix3& kL,
        Matrix3& kR)
    {
        scalar fT11 = kA[0][1]*kA[0][1]+kA[1][1]*kA[1][1];
        scalar fT22 = kA[1][2]*kA[1][2]+kA[2][2]*kA[2][2];
        scalar fT12 = kA[1][1]*kA[1][2];
        scalar fTrace = fT11+fT22;
        scalar fDiff = fT11-fT22;
        scalar fDiscr = MathMisc::Sqrt(fDiff*fDiff+4.0f*fT12*fT12);
        scalar fRoot1 = 0.5f*(fTrace+fDiscr);
        scalar fRoot2 = 0.5f*(fTrace-fDiscr);

        // adjust right
        scalar fY = kA[0][0] - (fabs(fRoot1-fT22) <=
            fabs(fRoot2-fT22) ? fRoot1 : fRoot2);
        scalar fZ = kA[0][1];
		scalar fInvLength = MathMisc::InvSqrt(fY*fY+fZ*fZ);
        scalar fSin = fZ*fInvLength;
        scalar fCos = -fY*fInvLength;

        scalar fTmp0 = kA[0][0];
        scalar fTmp1 = kA[0][1];
        kA[0][0] = fCos*fTmp0-fSin*fTmp1;
        kA[0][1] = fSin*fTmp0+fCos*fTmp1;
        kA[1][0] = -fSin*kA[1][1];
        kA[1][1] *= fCos;

        size_t iRow;
        for (iRow = 0; iRow < 3; iRow++)
        {
            fTmp0 = kR[0][iRow];
            fTmp1 = kR[1][iRow];
            kR[0][iRow] = fCos*fTmp0-fSin*fTmp1;
            kR[1][iRow] = fSin*fTmp0+fCos*fTmp1;
        }

        // adjust left
        fY = kA[0][0];
        fZ = kA[1][0];
        fInvLength = MathMisc::InvSqrt(fY*fY+fZ*fZ);
        fSin = fZ*fInvLength;
        fCos = -fY*fInvLength;

        kA[0][0] = fCos*kA[0][0]-fSin*kA[1][0];
        fTmp0 = kA[0][1];
        fTmp1 = kA[1][1];
        kA[0][1] = fCos*fTmp0-fSin*fTmp1;
        kA[1][1] = fSin*fTmp0+fCos*fTmp1;
        kA[0][2] = -fSin*kA[1][2];
        kA[1][2] *= fCos;

        size_t iCol;
        for (iCol = 0; iCol < 3; iCol++)
        {
            fTmp0 = kL[iCol][0];
            fTmp1 = kL[iCol][1];
            kL[iCol][0] = fCos*fTmp0-fSin*fTmp1;
            kL[iCol][1] = fSin*fTmp0+fCos*fTmp1;
        }

        // adjust right
        fY = kA[0][1];
        fZ = kA[0][2];
        fInvLength = MathMisc::InvSqrt(fY*fY+fZ*fZ);
        fSin = fZ*fInvLength;
        fCos = -fY*fInvLength;

        kA[0][1] = fCos*kA[0][1]-fSin*kA[0][2];
        fTmp0 = kA[1][1];
        fTmp1 = kA[1][2];
        kA[1][1] = fCos*fTmp0-fSin*fTmp1;
        kA[1][2] = fSin*fTmp0+fCos*fTmp1;
        kA[2][1] = -fSin*kA[2][2];
        kA[2][2] *= fCos;

        for (iRow = 0; iRow < 3; iRow++)
        {
            fTmp0 = kR[1][iRow];
            fTmp1 = kR[2][iRow];
            kR[1][iRow] = fCos*fTmp0-fSin*fTmp1;
            kR[2][iRow] = fSin*fTmp0+fCos*fTmp1;
        }

        // adjust left
        fY = kA[1][1];
        fZ = kA[2][1];
        fInvLength = MathMisc::InvSqrt(fY*fY+fZ*fZ);
        fSin = fZ*fInvLength;
        fCos = -fY*fInvLength;

        kA[1][1] = fCos*kA[1][1]-fSin*kA[2][1];
        fTmp0 = kA[1][2];
        fTmp1 = kA[2][2];
        kA[1][2] = fCos*fTmp0-fSin*fTmp1;
        kA[2][2] = fSin*fTmp0+fCos*fTmp1;

        for (iCol = 0; iCol < 3; iCol++)
        {
            fTmp0 = kL[iCol][1];
            fTmp1 = kL[iCol][2];
            kL[iCol][1] = fCos*fTmp0-fSin*fTmp1;
            kL[iCol][2] = fSin*fTmp0+fCos*fTmp1;
        }
    }
    //-----------------------------------------------------------------------
    void Matrix3::SingularValueDecomposition (Matrix3& kL, Vector3& kS,
        Matrix3& kR) const
    {
        // temas: currently unused
        //const int iMax = 16;
		size_t iRow, iCol;

        Matrix3 kA = *this;
        Bidiagonalize(kA,kL,kR);

        for (unsigned int i = 0; i < ms_iSvdMaxIterations; i++)
        {
            scalar fTmp, fTmp0, fTmp1;
            scalar fSin0, fCos0, fTan0;
            scalar fSin1, fCos1, fTan1;

            bool bTest1 = (fabs(kA[0][1]) <=
                ms_fSvdEpsilon*(fabs(kA[0][0])+fabs(kA[1][1])));
            bool bTest2 = (fabs(kA[1][2]) <=
                ms_fSvdEpsilon*(fabs(kA[1][1])+fabs(kA[2][2])));
            if ( bTest1 )
            {
                if ( bTest2 )
                {
                    kS[0] = kA[0][0];
                    kS[1] = kA[1][1];
                    kS[2] = kA[2][2];
                    break;
                }
                else
                {
                    // 2x2 closed form factorization
                    fTmp = (kA[1][1]*kA[1][1] - kA[2][2]*kA[2][2] +
                        kA[1][2]*kA[1][2])/(kA[1][2]*kA[2][2]);
                    fTan0 = 0.5f*(fTmp+MathMisc::Sqrt(fTmp*fTmp + 4.0f));
                    fCos0 = MathMisc::InvSqrt(1.0f+fTan0*fTan0);
                    fSin0 = fTan0*fCos0;

                    for (iCol = 0; iCol < 3; iCol++)
                    {
                        fTmp0 = kL[iCol][1];
                        fTmp1 = kL[iCol][2];
                        kL[iCol][1] = fCos0*fTmp0-fSin0*fTmp1;
                        kL[iCol][2] = fSin0*fTmp0+fCos0*fTmp1;
                    }

                    fTan1 = (kA[1][2]-kA[2][2]*fTan0)/kA[1][1];
                    fCos1 = MathMisc::InvSqrt(1.0f+fTan1*fTan1);
                    fSin1 = -fTan1*fCos1;

                    for (iRow = 0; iRow < 3; iRow++)
                    {
                        fTmp0 = kR[1][iRow];
                        fTmp1 = kR[2][iRow];
                        kR[1][iRow] = fCos1*fTmp0-fSin1*fTmp1;
                        kR[2][iRow] = fSin1*fTmp0+fCos1*fTmp1;
                    }

                    kS[0] = kA[0][0];
                    kS[1] = fCos0*fCos1*kA[1][1] -
                        fSin1*(fCos0*kA[1][2]-fSin0*kA[2][2]);
                    kS[2] = fSin0*fSin1*kA[1][1] +
                        fCos1*(fSin0*kA[1][2]+fCos0*kA[2][2]);
                    break;
                }
            }
            else
            {
                if ( bTest2 )
                {
                    // 2x2 closed form factorization
                    fTmp = (kA[0][0]*kA[0][0] + kA[1][1]*kA[1][1] -
                        kA[0][1]*kA[0][1])/(kA[0][1]*kA[1][1]);
                    fTan0 = 0.5f*(-fTmp+MathMisc::Sqrt(fTmp*fTmp + 4.0f));
                    fCos0 = MathMisc::InvSqrt(1.0f+fTan0*fTan0);
                    fSin0 = fTan0*fCos0;

                    for (iCol = 0; iCol < 3; iCol++)
                    {
                        fTmp0 = kL[iCol][0];
                        fTmp1 = kL[iCol][1];
                        kL[iCol][0] = fCos0*fTmp0-fSin0*fTmp1;
                        kL[iCol][1] = fSin0*fTmp0+fCos0*fTmp1;
                    }

                    fTan1 = (kA[0][1]-kA[1][1]*fTan0)/kA[0][0];
                    fCos1 = MathMisc::InvSqrt(1.0f+fTan1*fTan1);
                    fSin1 = -fTan1*fCos1;

                    for (iRow = 0; iRow < 3; iRow++)
                    {
                        fTmp0 = kR[0][iRow];
                        fTmp1 = kR[1][iRow];
                        kR[0][iRow] = fCos1*fTmp0-fSin1*fTmp1;
                        kR[1][iRow] = fSin1*fTmp0+fCos1*fTmp1;
                    }

                    kS[0] = fCos0*fCos1*kA[0][0] -
                        fSin1*(fCos0*kA[0][1]-fSin0*kA[1][1]);
                    kS[1] = fSin0*fSin1*kA[0][0] +
                        fCos1*(fSin0*kA[0][1]+fCos0*kA[1][1]);
                    kS[2] = kA[2][2];
                    break;
                }
                else
                {
                    GolubKahanStep(kA,kL,kR);
                }
            }
        }

        // positize diagonal
        for (iRow = 0; iRow < 3; iRow++)
        {
            if ( kS[iRow] < 0.0 )
            {
                kS[iRow] = -kS[iRow];
                for (iCol = 0; iCol < 3; iCol++)
                    kR[iRow][iCol] = -kR[iRow][iCol];
            }
        }
    }
    //-----------------------------------------------------------------------
    void Matrix3::SingularValueComposition (const Matrix3& kL,
        const Vector3& kS, const Matrix3& kR)
    {
        size_t iRow, iCol;
        Matrix3 kTmp;

        // product S*R
        for (iRow = 0; iRow < 3; iRow++)
        {
            for (iCol = 0; iCol < 3; iCol++)
                kTmp[iRow][iCol] = kS[iRow]*kR[iRow][iCol];
        }

        // product L*S*R
        for (iRow = 0; iRow < 3; iRow++)
        {
            for (iCol = 0; iCol < 3; iCol++)
            {
                m[iRow][iCol] = 0.0;
                for (int iMid = 0; iMid < 3; iMid++)
                    m[iRow][iCol] += kL[iRow][iMid]*kTmp[iMid][iCol];
            }
        }
    }
    //-----------------------------------------------------------------------
    void Matrix3::Orthonormalize ()
    {
        // Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
        // M = [m0|m1|m2], then orthonormal output matrix is Q = [q0|q1|q2],
        //
        //   q0 = m0/|m0|
        //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
        //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
        //
        // where |V| indicates length of vector V and A*B indicates dot
        // product of vectors A and B.

        // compute q0
        scalar fInvLength = MathMisc::InvSqrt(m[0][0]*m[0][0]
            + m[1][0]*m[1][0] +
            m[2][0]*m[2][0]);

        m[0][0] *= fInvLength;
        m[1][0] *= fInvLength;
        m[2][0] *= fInvLength;

        // compute q1
        scalar fDot0 =
            m[0][0]*m[0][1] +
            m[1][0]*m[1][1] +
            m[2][0]*m[2][1];

        m[0][1] -= fDot0*m[0][0];
        m[1][1] -= fDot0*m[1][0];
        m[2][1] -= fDot0*m[2][0];

        fInvLength = MathMisc::InvSqrt(m[0][1]*m[0][1] +
            m[1][1]*m[1][1] +
            m[2][1]*m[2][1]);

        m[0][1] *= fInvLength;
        m[1][1] *= fInvLength;
        m[2][1] *= fInvLength;

        // compute q2
        scalar fDot1 =
            m[0][1]*m[0][2] +
            m[1][1]*m[1][2] +
            m[2][1]*m[2][2];

        fDot0 =
            m[0][0]*m[0][2] +
            m[1][0]*m[1][2] +
            m[2][0]*m[2][2];

        m[0][2] -= fDot0*m[0][0] + fDot1*m[0][1];
        m[1][2] -= fDot0*m[1][0] + fDot1*m[1][1];
        m[2][2] -= fDot0*m[2][0] + fDot1*m[2][1];

        fInvLength = MathMisc::InvSqrt(m[0][2]*m[0][2] +
            m[1][2]*m[1][2] +
            m[2][2]*m[2][2]);

        m[0][2] *= fInvLength;
        m[1][2] *= fInvLength;
        m[2][2] *= fInvLength;
    }
    //-----------------------------------------------------------------------
    void Matrix3::QDUDecomposition (Matrix3& kQ,
        Vector3& kD, Vector3& kU) const
    {
        // Factor M = QR = QDU where Q is orthogonal, D is diagonal,
        // and U is upper triangular with ones on its diagonal.  Algorithm uses
        // Gram-Schmidt orthogonalization (the QR algorithm).
        //
        // If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
        //
        //   q0 = m0/|m0|
        //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
        //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
        //
        // where |V| indicates length of vector V and A*B indicates dot
        // product of vectors A and B.  The matrix R has entries
        //
        //   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
        //   r10 = 0      r11 = q1*m1  r12 = q1*m2
        //   r20 = 0      r21 = 0      r22 = q2*m2
        //
        // so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
        // u02 = r02/r00, and u12 = r12/r11.

        // Q = rotation
        // D = scaling
        // U = shear

        // D stores the three diagonal entries r00, r11, r22
        // U stores the entries U[0] = u01, U[1] = u02, U[2] = u12

        // build orthogonal matrix Q
        scalar fInvLength = MathMisc::InvSqrt(m[0][0]*m[0][0]
            + m[1][0]*m[1][0] +
            m[2][0]*m[2][0]);
        kQ[0][0] = m[0][0]*fInvLength;
        kQ[1][0] = m[1][0]*fInvLength;
        kQ[2][0] = m[2][0]*fInvLength;

        scalar fDot = kQ[0][0]*m[0][1] + kQ[1][0]*m[1][1] +
            kQ[2][0]*m[2][1];
        kQ[0][1] = m[0][1]-fDot*kQ[0][0];
        kQ[1][1] = m[1][1]-fDot*kQ[1][0];
        kQ[2][1] = m[2][1]-fDot*kQ[2][0];
        fInvLength = MathMisc::InvSqrt(kQ[0][1]*kQ[0][1] + kQ[1][1]*kQ[1][1] +
            kQ[2][1]*kQ[2][1]);
        kQ[0][1] *= fInvLength;
        kQ[1][1] *= fInvLength;
        kQ[2][1] *= fInvLength;

        fDot = kQ[0][0]*m[0][2] + kQ[1][0]*m[1][2] +
            kQ[2][0]*m[2][2];
        kQ[0][2] = m[0][2]-fDot*kQ[0][0];
        kQ[1][2] = m[1][2]-fDot*kQ[1][0];
        kQ[2][2] = m[2][2]-fDot*kQ[2][0];
        fDot = kQ[0][1]*m[0][2] + kQ[1][1]*m[1][2] +
            kQ[2][1]*m[2][2];
        kQ[0][2] -= fDot*kQ[0][1];
        kQ[1][2] -= fDot*kQ[1][1];
        kQ[2][2] -= fDot*kQ[2][1];
        fInvLength = MathMisc::InvSqrt(kQ[0][2]*kQ[0][2] + kQ[1][2]*kQ[1][2] +
            kQ[2][2]*kQ[2][2]);
        kQ[0][2] *= fInvLength;
        kQ[1][2] *= fInvLength;
        kQ[2][2] *= fInvLength;

        // guarantee that orthogonal matrix has determinant 1 (no reflections)
        scalar fDet = kQ[0][0]*kQ[1][1]*kQ[2][2] + kQ[0][1]*kQ[1][2]*kQ[2][0] +
            kQ[0][2]*kQ[1][0]*kQ[2][1] - kQ[0][2]*kQ[1][1]*kQ[2][0] -
            kQ[0][1]*kQ[1][0]*kQ[2][2] - kQ[0][0]*kQ[1][2]*kQ[2][1];

        if ( fDet < 0.0 )
        {
            for (size_t iRow = 0; iRow < 3; iRow++)
                for (size_t iCol = 0; iCol < 3; iCol++)
                    kQ[iRow][iCol] = -kQ[iRow][iCol];
        }

        // build "right" matrix R
        Matrix3 kR;
        kR[0][0] = kQ[0][0]*m[0][0] + kQ[1][0]*m[1][0] +
            kQ[2][0]*m[2][0];
        kR[0][1] = kQ[0][0]*m[0][1] + kQ[1][0]*m[1][1] +
            kQ[2][0]*m[2][1];
        kR[1][1] = kQ[0][1]*m[0][1] + kQ[1][1]*m[1][1] +
            kQ[2][1]*m[2][1];
        kR[0][2] = kQ[0][0]*m[0][2] + kQ[1][0]*m[1][2] +
            kQ[2][0]*m[2][2];
        kR[1][2] = kQ[0][1]*m[0][2] + kQ[1][1]*m[1][2] +
            kQ[2][1]*m[2][2];
        kR[2][2] = kQ[0][2]*m[0][2] + kQ[1][2]*m[1][2] +
            kQ[2][2]*m[2][2];

        // the scaling component
        kD[0] = kR[0][0];
        kD[1] = kR[1][1];
        kD[2] = kR[2][2];

        // the shear component
        scalar fInvD0 = 1.0f/kD[0];
        kU[0] = kR[0][1]*fInvD0;
        kU[1] = kR[0][2]*fInvD0;
        kU[2] = kR[1][2]/kD[1];
    }
    //-----------------------------------------------------------------------
    scalar Matrix3::MaxCubicRoot (scalar afCoeff[3])
    {
        // Spectral norm is for A^T*A, so characteristic polynomial
        // P(x) = c[0]+c[1]*x+c[2]*x^2+x^3 has three positive real roots.
        // This yields the assertions c[0] < 0 and c[2]*c[2] >= 3*c[1].

        // quick out for uniform scale (triple root)
        const scalar fOneThird = 1.0f/3.0f;
        const scalar fEpsilon = (scalar)1e-06;
        scalar fDiscr = afCoeff[2]*afCoeff[2] - 3.0f*afCoeff[1];
        if ( fDiscr <= fEpsilon )
            return -fOneThird*afCoeff[2];

        // Compute an upper bound on roots of P(x).  This assumes that A^T*A
        // has been scaled by its largest entry.
        scalar fX = 1.0;
        scalar fPoly = afCoeff[0]+fX*(afCoeff[1]+fX*(afCoeff[2]+fX));
        if ( fPoly < 0.0 )
        {
            // uses a matrix norm to find an upper bound on maximum root
            fX = fabs(afCoeff[0]);
            scalar fTmp = 1.0f+fabs(afCoeff[1]);
            if ( fTmp > fX )
                fX = fTmp;
            fTmp = 1.0f+fabs(afCoeff[2]);
            if ( fTmp > fX )
                fX = fTmp;
        }

        // Newton's method to find root
        scalar fTwoC2 = 2.0f*afCoeff[2];
        for (int i = 0; i < 16; i++)
        {
            fPoly = afCoeff[0]+fX*(afCoeff[1]+fX*(afCoeff[2]+fX));
            if ( fabs(fPoly) <= fEpsilon )
                return fX;

            scalar fDeriv = afCoeff[1]+fX*(fTwoC2+3.0f*fX);
            fX -= fPoly/fDeriv;
        }

        return fX;
    }
    //-----------------------------------------------------------------------
    scalar Matrix3::SpectralNorm () const
    {
        Matrix3 kP;
        size_t iRow, iCol;
        scalar fPmax = 0.0;
        for (iRow = 0; iRow < 3; iRow++)
        {
            for (iCol = 0; iCol < 3; iCol++)
            {
                kP[iRow][iCol] = 0.0;
                for (int iMid = 0; iMid < 3; iMid++)
                {
                    kP[iRow][iCol] +=
                        m[iMid][iRow]*m[iMid][iCol];
                }
                if ( kP[iRow][iCol] > fPmax )
                    fPmax = kP[iRow][iCol];
            }
        }

        scalar fInvPmax = 1.0f/fPmax;
        for (iRow = 0; iRow < 3; iRow++)
        {
            for (iCol = 0; iCol < 3; iCol++)
                kP[iRow][iCol] *= fInvPmax;
        }

        scalar afCoeff[3];
        afCoeff[0] = -(kP[0][0]*(kP[1][1]*kP[2][2]-kP[1][2]*kP[2][1]) +
            kP[0][1]*(kP[2][0]*kP[1][2]-kP[1][0]*kP[2][2]) +
            kP[0][2]*(kP[1][0]*kP[2][1]-kP[2][0]*kP[1][1]));
        afCoeff[1] = kP[0][0]*kP[1][1]-kP[0][1]*kP[1][0] +
            kP[0][0]*kP[2][2]-kP[0][2]*kP[2][0] +
            kP[1][1]*kP[2][2]-kP[1][2]*kP[2][1];
        afCoeff[2] = -(kP[0][0]+kP[1][1]+kP[2][2]);

        scalar fRoot = MaxCubicRoot(afCoeff);
        scalar fNorm = MathMisc::Sqrt(fPmax*fRoot);
        return fNorm;
    }
    //-----------------------------------------------------------------------
    void Matrix3::ToAxisAngle (Vector3& rkAxis, Radian& rfRadians) const
    {
        // Let (x,y,z) be the unit-length axis and let A be an angle of rotation.
        // The rotation matrix is R = I + sin(A)*P + (1-cos(A))*P^2 where
        // I is the identity and
        //
        //       +-        -+
        //   P = |  0 -z +y |
        //       | +z  0 -x |
        //       | -y +x  0 |
        //       +-        -+
        //
        // If A > 0, R represents a counterclockwise rotation about the axis in
        // the sense of looking from the tip of the axis vector towards the
        // origin.  Some algebra will show that
        //
        //   cos(A) = (trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
        //
        // In the event that A = pi, R-R^t = 0 which prevents us from extracting
        // the axis through P.  Instead note that R = I+2*P^2 when A = pi, so
        // P^2 = (R-I)/2.  The diagonal entries of P^2 are x^2-1, y^2-1, and
        // z^2-1.  We can solve these for axis (x,y,z).  Because the angle is pi,
        // it does not matter which sign you choose on the square roots.

        scalar fTrace = m[0][0] + m[1][1] + m[2][2];
        scalar fCos = 0.5f*(fTrace-1.0f);
        rfRadians = MathMisc::ACos(fCos);  // in [0,PI]

        if ( rfRadians > Radian(0.0) )
        {
            if ( rfRadians < Radian(N_PI) )
            {
                rkAxis.x = m[2][1]-m[1][2];
                rkAxis.y = m[0][2]-m[2][0];
                rkAxis.z = m[1][0]-m[0][1];
                rkAxis.normalise();
            }
            else
            {
                // angle is PI
                float fHalfInverse;
                if ( m[0][0] >= m[1][1] )
                {
                    // r00 >= r11
                    if ( m[0][0] >= m[2][2] )
                    {
                        // r00 is maximum diagonal term
                        rkAxis.x = 0.5f*MathMisc::Sqrt(m[0][0] -
                            m[1][1] - m[2][2] + 1.0f);
                        fHalfInverse = 0.5f/rkAxis.x;
                        rkAxis.y = fHalfInverse*m[0][1];
                        rkAxis.z = fHalfInverse*m[0][2];
                    }
                    else
                    {
                        // r22 is maximum diagonal term
                        rkAxis.z = 0.5f*MathMisc::Sqrt(m[2][2] -
                            m[0][0] - m[1][1] + 1.0f);
                        fHalfInverse = 0.5f/rkAxis.z;
                        rkAxis.x = fHalfInverse*m[0][2];
                        rkAxis.y = fHalfInverse*m[1][2];
                    }
                }
                else
                {
                    // r11 > r00
                    if ( m[1][1] >= m[2][2] )
                    {
                        // r11 is maximum diagonal term
                        rkAxis.y = 0.5f*MathMisc::Sqrt(m[1][1] -
                            m[0][0] - m[2][2] + 1.0f);
                        fHalfInverse  = 0.5f/rkAxis.y;
                        rkAxis.x = fHalfInverse*m[0][1];
                        rkAxis.z = fHalfInverse*m[1][2];
                    }
                    else
                    {
                        // r22 is maximum diagonal term
                        rkAxis.z = 0.5f*MathMisc::Sqrt(m[2][2] -
                            m[0][0] - m[1][1] + 1.0f);
                        fHalfInverse = 0.5f/rkAxis.z;
                        rkAxis.x = fHalfInverse*m[0][2];
                        rkAxis.y = fHalfInverse*m[1][2];
                    }
                }
            }
        }
        else
        {
            // The angle is 0 and the matrix is the identity.  Any axis will
            // work, so just use the x-axis.
            rkAxis.x = 1.0;
            rkAxis.y = 0.0;
            rkAxis.z = 0.0;
        }
    }
    //-----------------------------------------------------------------------
	void Matrix3::FromAngleAxis (const Vector3& rkAxis, const Radian& fRadians)
	{
		scalar fCos =MathMisc::Cos(fRadians);
		scalar fSin = MathMisc::Sin(fRadians);
		scalar fOneMinusCos = 1.0f-fCos;
		scalar fX2 = rkAxis.x*rkAxis.x;
		scalar fY2 = rkAxis.y*rkAxis.y;
		scalar fZ2 = rkAxis.z*rkAxis.z;
		scalar fXYM = rkAxis.x*rkAxis.y*fOneMinusCos;
		scalar fXZM = rkAxis.x*rkAxis.z*fOneMinusCos;
		scalar fYZM = rkAxis.y*rkAxis.z*fOneMinusCos;
		scalar fXSin = rkAxis.x*fSin;
		scalar fYSin = rkAxis.y*fSin;
		scalar fZSin = rkAxis.z*fSin;

		m[0][0] = fX2*fOneMinusCos+fCos;
		m[0][1] = fXYM-fZSin;
		m[0][2] = fXZM+fYSin;
		m[1][0] = fXYM+fZSin;
		m[1][1] = fY2*fOneMinusCos+fCos;
		m[1][2] = fYZM-fXSin;
		m[2][0] = fXZM-fYSin;
		m[2][1] = fYZM+fXSin;
		m[2][2] = fZ2*fOneMinusCos+fCos;
	}
    //-----------------------------------------------------------------------
    bool Matrix3::ToEulerAnglesXYZ (Radian& rfYAngle, Radian& rfPAngle,
        Radian& rfRAngle) const
    {
        // rot =  cy*cz          -cy*sz           sy
        //        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
        //       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

        rfPAngle = Radian(MathMisc::ASin(m[0][2]));
        if ( rfPAngle < Radian(N_PI_HALF) )
        {
            if ( rfPAngle > Radian(-N_PI_HALF) )
            {
                rfYAngle = MathMisc::ATan2(-m[1][2],m[2][2]);
                rfRAngle = MathMisc::ATan2(-m[0][1],m[0][0]);
                return true;
            }
            else
            {
                // WARNING.  Not a unique solution.
				Radian fRmY = MathMisc::ATan2(m[1][0],m[1][1]);
                rfRAngle = Radian(0.0);  // any angle works
                rfYAngle = rfRAngle - fRmY;
                return false;
            }
        }
        else
        {
            // WARNING.  Not a unique solution.
            Radian fRpY = MathMisc::ATan2(m[1][0],m[1][1]);
            rfRAngle = Radian(0.0);  // any angle works
            rfYAngle = fRpY - rfRAngle;
            return false;
        }
    }
    //-----------------------------------------------------------------------
    bool Matrix3::ToEulerAnglesXZY (Radian& rfYAngle, Radian& rfPAngle,
        Radian& rfRAngle) const
    {
        // rot =  cy*cz          -sz              cz*sy
        //        sx*sy+cx*cy*sz  cx*cz          -cy*sx+cx*sy*sz
        //       -cx*sy+cy*sx*sz  cz*sx           cx*cy+sx*sy*sz

        rfPAngle = MathMisc::ASin(-m[0][1]);
        if ( rfPAngle < Radian(N_PI_HALF) )
        {
            if ( rfPAngle > Radian(-N_PI_HALF) )
            {
                rfYAngle = MathMisc::ATan2(m[2][1],m[1][1]);
                rfRAngle = MathMisc::ATan2(m[0][2],m[0][0]);
                return true;
            }
            else
            {
                // WARNING.  Not a unique solution.
                Radian fRmY = MathMisc::ATan2(-m[2][0],m[2][2]);
                rfRAngle = Radian(0.0);  // any angle works
                rfYAngle = rfRAngle - fRmY;
                return false;
            }
        }
        else
        {
            // WARNING.  Not a unique solution.
            Radian fRpY = MathMisc::ATan2(-m[2][0],m[2][2]);
            rfRAngle = Radian(0.0);  // any angle works
            rfYAngle = fRpY - rfRAngle;
            return false;
        }
    }
    //-----------------------------------------------------------------------
    bool Matrix3::ToEulerAnglesYXZ (Radian& rfYAngle, Radian& rfPAngle,
        Radian& rfRAngle) const
    {
        // rot =  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz  cx*sy
        //        cx*sz           cx*cz          -sx
        //       -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz  cx*cy

        rfPAngle = MathMisc::ASin(-m[1][2]);
        if ( rfPAngle < Radian(N_PI_HALF) )
        {
            if ( rfPAngle > Radian(-N_PI_HALF) )
            {
                rfYAngle = MathMisc::ATan2(m[0][2],m[2][2]);
                rfRAngle = MathMisc::ATan2(m[1][0],m[1][1]);
                return true;
            }
            else
            {
                // WARNING.  Not a unique solution.
                Radian fRmY = MathMisc::ATan2(-m[0][1],m[0][0]);
                rfRAngle = Radian(0.0);  // any angle works
                rfYAngle = rfRAngle - fRmY;
                return false;
            }
        }
        else
        {
            // WARNING.  Not a unique solution.
            Radian fRpY = MathMisc::ATan2(-m[0][1],m[0][0]);
            rfRAngle = Radian(0.0);  // any angle works
            rfYAngle = fRpY - rfRAngle;
            return false;
        }
    }
    //-----------------------------------------------------------------------
    bool Matrix3::ToEulerAnglesYZX (Radian& rfYAngle, Radian& rfPAngle,
        Radian& rfRAngle) const
    {
        // rot =  cy*cz           sx*sy-cx*cy*sz  cx*sy+cy*sx*sz
        //        sz              cx*cz          -cz*sx
        //       -cz*sy           cy*sx+cx*sy*sz  cx*cy-sx*sy*sz

        rfPAngle = MathMisc::ASin(m[1][0]);
        if ( rfPAngle < Radian(N_PI_HALF) )
        {
            if ( rfPAngle > Radian(-N_PI_HALF) )
            {
                rfYAngle = MathMisc::ATan2(-m[2][0],m[0][0]);
                rfRAngle = MathMisc::ATan2(-m[1][2],m[1][1]);
                return true;
            }
            else
            {
                // WARNING.  Not a unique solution.
                Radian fRmY = MathMisc::ATan2(m[2][1],m[2][2]);
                rfRAngle = Radian(0.0);  // any angle works
                rfYAngle = rfRAngle - fRmY;
                return false;
            }
        }
        else
        {
            // WARNING.  Not a unique solution.
            Radian fRpY = MathMisc::ATan2(m[2][1],m[2][2]);
            rfRAngle = Radian(0.0);  // any angle works
            rfYAngle = fRpY - rfRAngle;
            return false;
        }
    }
    //-----------------------------------------------------------------------
    bool Matrix3::ToEulerAnglesZXY (Radian& rfYAngle, Radian& rfPAngle,
        Radian& rfRAngle) const
    {
        // rot =  cy*cz-sx*sy*sz -cx*sz           cz*sy+cy*sx*sz
        //        cz*sx*sy+cy*sz  cx*cz          -cy*cz*sx+sy*sz
        //       -cx*sy           sx              cx*cy

        rfPAngle = MathMisc::ASin(m[2][1]);
        if ( rfPAngle < Radian(N_PI_HALF) )
        {
            if ( rfPAngle > Radian(-N_PI_HALF) )
            {
                rfYAngle = MathMisc::ATan2(-m[0][1],m[1][1]);
                rfRAngle = MathMisc::ATan2(-m[2][0],m[2][2]);
                return true;
            }
            else
            {
                // WARNING.  Not a unique solution.
                Radian fRmY = MathMisc::ATan2(m[0][2],m[0][0]);
                rfRAngle = Radian(0.0);  // any angle works
                rfYAngle = rfRAngle - fRmY;
                return false;
            }
        }
        else
        {
            // WARNING.  Not a unique solution.
            Radian fRpY = MathMisc::ATan2(m[0][2],m[0][0]);
            rfRAngle = Radian(0.0);  // any angle works
            rfYAngle = fRpY - rfRAngle;
            return false;
        }
    }
    //-----------------------------------------------------------------------
    bool Matrix3::ToEulerAnglesZYX (Radian& rfYAngle, Radian& rfPAngle,
        Radian& rfRAngle) const
    {
        // rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
        //        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
        //       -sy              cy*sx           cx*cy

        rfPAngle = MathMisc::ASin(-m[2][0]);
        if ( rfPAngle < Radian(N_PI_HALF) )
        {
            if ( rfPAngle > Radian(-N_PI_HALF) )
            {
                rfYAngle = MathMisc::ATan2(m[1][0],m[0][0]);
                rfRAngle = MathMisc::ATan2(m[2][1],m[2][2]);
                return true;
            }
            else
            {
                // WARNING.  Not a unique solution.
                Radian fRmY = MathMisc::ATan2(-m[0][1],m[0][2]);
                rfRAngle = Radian(0.0);  // any angle works
                rfYAngle = rfRAngle - fRmY;
                return false;
            }
        }
        else
        {
            // WARNING.  Not a unique solution.
            Radian fRpY = MathMisc::ATan2(-m[0][1],m[0][2]);
            rfRAngle = Radian(0.0);  // any angle works
            rfYAngle = fRpY - rfRAngle;
            return false;
        }
    }
    //-----------------------------------------------------------------------
    void Matrix3::FromEulerAnglesXYZ (const Radian& fYAngle, const Radian& fPAngle,
        const Radian& fRAngle)
    {
        scalar fCos, fSin;

        fCos = MathMisc::Cos(fYAngle);
        fSin = MathMisc::Sin(fYAngle);
        Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

        fCos = MathMisc::Cos(fPAngle);
        fSin = MathMisc::Sin(fPAngle);
        Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

        fCos = MathMisc::Cos(fRAngle);
        fSin = MathMisc::Sin(fRAngle);
        Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

        *this = kXMat*(kYMat*kZMat);
    }
    //-----------------------------------------------------------------------
    void Matrix3::FromEulerAnglesXZY (const Radian& fYAngle, const Radian& fPAngle,
        const Radian& fRAngle)
    {
        scalar fCos, fSin;

        fCos = MathMisc::Cos(fYAngle);
        fSin = MathMisc::Sin(fYAngle);
        Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

        fCos = MathMisc::Cos(fPAngle);
        fSin = MathMisc::Sin(fPAngle);
        Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

        fCos = MathMisc::Cos(fRAngle);
        fSin = MathMisc::Sin(fRAngle);
        Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

        *this = kXMat*(kZMat*kYMat);
    }
    //-----------------------------------------------------------------------
    void Matrix3::FromEulerAnglesYXZ (const Radian& fYAngle, const Radian& fPAngle,
        const Radian& fRAngle)
    {
        scalar fCos, fSin;

        fCos = MathMisc::Cos(fYAngle);
        fSin = MathMisc::Sin(fYAngle);
        Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

        fCos = MathMisc::Cos(fPAngle);
        fSin = MathMisc::Sin(fPAngle);
        Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

        fCos = MathMisc::Cos(fRAngle);
        fSin = MathMisc::Sin(fRAngle);
        Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

        *this = kYMat*(kXMat*kZMat);
    }
    //-----------------------------------------------------------------------
    void Matrix3::FromEulerAnglesYZX (const Radian& fYAngle, const Radian& fPAngle,
        const Radian& fRAngle)
    {
        scalar fCos, fSin;

        fCos = MathMisc::Cos(fYAngle);
        fSin = MathMisc::Sin(fYAngle);
        Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

        fCos = MathMisc::Cos(fPAngle);
        fSin = MathMisc::Sin(fPAngle);
        Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

        fCos = MathMisc::Cos(fRAngle);
        fSin = MathMisc::Sin(fRAngle);
        Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

        *this = kYMat*(kZMat*kXMat);
    }
    //-----------------------------------------------------------------------
    void Matrix3::FromEulerAnglesZXY (const Radian& fYAngle, const Radian& fPAngle,
        const Radian& fRAngle)
    {
        scalar fCos, fSin;

        fCos = MathMisc::Cos(fYAngle);
        fSin = MathMisc::Sin(fYAngle);
        Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

        fCos = MathMisc::Cos(fPAngle);
        fSin = MathMisc::Sin(fPAngle);
        Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

        fCos = MathMisc::Cos(fRAngle);
        fSin = MathMisc::Sin(fRAngle);
        Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

        *this = kZMat*(kXMat*kYMat);
    }
    //-----------------------------------------------------------------------
    void Matrix3::FromEulerAnglesZYX (const Radian& fYAngle, const Radian& fPAngle,
        const Radian& fRAngle)
    {
        scalar fCos, fSin;

        fCos = MathMisc::Cos(fYAngle);
        fSin = MathMisc::Sin(fYAngle);
        Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

        fCos = MathMisc::Cos(fPAngle);
        fSin = MathMisc::Sin(fPAngle);
        Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

        fCos = MathMisc::Cos(fRAngle);
        fSin = MathMisc::Sin(fRAngle);
        Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

        *this = kZMat*(kYMat*kXMat);
    }
    //-----------------------------------------------------------------------
    void Matrix3::Tridiagonal (scalar afDiag[3], scalar afSubDiag[3])
    {
        // Householder reduction T = Q^t M Q
        //   Input:
        //     mat, symmetric 3x3 matrix M
        //   Output:
        //     mat, orthogonal matrix Q
        //     diag, diagonal entries of T
        //     subd, subdiagonal entries of T (T is symmetric)

        scalar fA = m[0][0];
        scalar fB = m[0][1];
        scalar fC = m[0][2];
        scalar fD = m[1][1];
        scalar fE = m[1][2];
        scalar fF = m[2][2];

        afDiag[0] = fA;
        afSubDiag[2] = 0.0;
        if ( fabs(fC) >= EPSILON )
        {
            scalar fLength = MathMisc::Sqrt(fB*fB+fC*fC);
            scalar fInvLength = 1.0f/fLength;
            fB *= fInvLength;
            fC *= fInvLength;
            scalar fQ = 2.0f*fB*fE+fC*(fF-fD);
            afDiag[1] = fD+fC*fQ;
            afDiag[2] = fF-fC*fQ;
            afSubDiag[0] = fLength;
            afSubDiag[1] = fE-fB*fQ;
            m[0][0] = 1.0;
            m[0][1] = 0.0;
            m[0][2] = 0.0;
            m[1][0] = 0.0;
            m[1][1] = fB;
            m[1][2] = fC;
            m[2][0] = 0.0;
            m[2][1] = fC;
            m[2][2] = -fB;
        }
        else
        {
            afDiag[1] = fD;
            afDiag[2] = fF;
            afSubDiag[0] = fB;
            afSubDiag[1] = fE;
            m[0][0] = 1.0;
            m[0][1] = 0.0;
            m[0][2] = 0.0;
            m[1][0] = 0.0;
            m[1][1] = 1.0;
            m[1][2] = 0.0;
            m[2][0] = 0.0;
            m[2][1] = 0.0;
            m[2][2] = 1.0;
        }
    }
    //-----------------------------------------------------------------------
    bool Matrix3::QLAlgorithm (scalar afDiag[3], scalar afSubDiag[3])
    {
        // QL iteration with implicit shifting to reduce matrix from tridiagonal
        // to diagonal

        for (int i0 = 0; i0 < 3; i0++)
        {
            const unsigned int iMaxIter = 32;
            unsigned int iIter;
            for (iIter = 0; iIter < iMaxIter; iIter++)
            {
                int i1;
                for (i1 = i0; i1 <= 1; i1++)
                {
                    scalar fSum = fabs(afDiag[i1]) +
                        fabs(afDiag[i1+1]);
                    if ( fabs(afSubDiag[i1]) + fSum == fSum )
                        break;
                }
                if ( i1 == i0 )
                    break;

                scalar fTmp0 = (afDiag[i0+1]-afDiag[i0])/(2.0f*afSubDiag[i0]);
                scalar fTmp1 = MathMisc::Sqrt(fTmp0*fTmp0+1.0f);
                if ( fTmp0 < 0.0 )
                    fTmp0 = afDiag[i1]-afDiag[i0]+afSubDiag[i0]/(fTmp0-fTmp1);
                else
                    fTmp0 = afDiag[i1]-afDiag[i0]+afSubDiag[i0]/(fTmp0+fTmp1);
                scalar fSin = 1.0;
                scalar fCos = 1.0;
                scalar fTmp2 = 0.0;
                for (int i2 = i1-1; i2 >= i0; i2--)
                {
                    scalar fTmp3 = fSin*afSubDiag[i2];
                    scalar fTmp4 = fCos*afSubDiag[i2];
                    if ( fabs(fTmp3) >= fabs(fTmp0) )
                    {
                        fCos = fTmp0/fTmp3;
                        fTmp1 = MathMisc::Sqrt(fCos*fCos+1.0f);
                        afSubDiag[i2+1] = fTmp3*fTmp1;
                        fSin = 1.0f/fTmp1;
                        fCos *= fSin;
                    }
                    else
                    {
                        fSin = fTmp3/fTmp0;
                        fTmp1 = MathMisc::Sqrt(fSin*fSin+1.0f);
                        afSubDiag[i2+1] = fTmp0*fTmp1;
                        fCos = 1.0f/fTmp1;
                        fSin *= fCos;
                    }
                    fTmp0 = afDiag[i2+1]-fTmp2;
                    fTmp1 = (afDiag[i2]-fTmp0)*fSin+2.0f*fTmp4*fCos;
                    fTmp2 = fSin*fTmp1;
                    afDiag[i2+1] = fTmp0+fTmp2;
                    fTmp0 = fCos*fTmp1-fTmp4;

                    for (int iRow = 0; iRow < 3; iRow++)
                    {
                        fTmp3 = m[iRow][i2+1];
                        m[iRow][i2+1] = fSin*m[iRow][i2] +
                            fCos*fTmp3;
                        m[iRow][i2] = fCos*m[iRow][i2] -
                            fSin*fTmp3;
                    }
                }
                afDiag[i0] -= fTmp2;
                afSubDiag[i0] = fTmp0;
                afSubDiag[i1] = 0.0;
            }

            if ( iIter == iMaxIter )
            {
                // should not get here under normal circumstances
                return false;
            }
        }

        return true;
    }
    //-----------------------------------------------------------------------
    void Matrix3::EigenSolveSymmetric (scalar afEigenvalue[3],
        Vector3 akEigenvector[3]) const
    {
        Matrix3 kMatrix = *this;
        scalar afSubDiag[3];
        kMatrix.Tridiagonal(afEigenvalue,afSubDiag);
        kMatrix.QLAlgorithm(afEigenvalue,afSubDiag);

        for (size_t i = 0; i < 3; i++)
        {
            akEigenvector[i][0] = kMatrix[0][i];
            akEigenvector[i][1] = kMatrix[1][i];
            akEigenvector[i][2] = kMatrix[2][i];
        }

        // make eigenvectors form a right--handed system
        Vector3 kCross = akEigenvector[1].crossProduct(akEigenvector[2]);
        scalar fDet = akEigenvector[0].dotProduct(kCross);
        if ( fDet < 0.0 )
        {
            akEigenvector[2][0] = - akEigenvector[2][0];
            akEigenvector[2][1] = - akEigenvector[2][1];
            akEigenvector[2][2] = - akEigenvector[2][2];
        }
    }
    //-----------------------------------------------------------------------
    void Matrix3::TensorProduct (const Vector3& rkU, const Vector3& rkV,
        Matrix3& rkProduct)
    {
        for (size_t iRow = 0; iRow < 3; iRow++)
        {
            for (size_t iCol = 0; iCol < 3; iCol++)
                rkProduct[iRow][iCol] = rkU[iRow]*rkV[iCol];
        }
    }
    //-----------------------------------------------------------------------
}