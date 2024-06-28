#include "Matrix.h"
#include "Vector.h"
#include "Tools.h"

const Mat3 Mat3_Identity = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};
const Mat4 Mat4_Identity = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

//-------------------------------------------------------------------------------------------------
// Fonctions à coder

// TODO
Mat3 Mat3_MulMM(Mat3 mat1, Mat3 mat2)
{
    Mat3 res = { 0 };
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            res.data[i][j] = 0;
            for (int k = 0; k < 3; k++)
                res.data[i][j] += mat1.data[i][k] * mat2.data[k][j];
        }
    }
    return res;
}

// TODO
Vec3 Mat3_MulMV(Mat3 mat, Vec3 v)
{
    Vec3 res = { 0 };
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            res.data[i] = 0;
            for (int k = 0; k < 3; k++)
                res.data[i] += mat.data[i][k] * v.data[k];
        }
    }
    return res;
}

// TODO
float Mat3_Det(Mat3 mat)
{
    float res = (mat.data[0][0] * mat.data[1][1] * mat.data[2][2]) + (mat.data[0][1] * mat.data[1][2] * mat.data[2][0]) + (mat.data[0][2] * mat.data[1][0] * mat.data[2][1]) - ((mat.data[0][2] * mat.data[1][1] * mat.data[2][0]) + (mat.data[1][2] * mat.data[2][1] * mat.data[0][0]) + (mat.data[2][2] * mat.data[0][1] * mat.data[1][0]));
    return res;
}

// TODO
Mat4 Mat4_Transpose(Mat4 matrix)
{
    Mat4 res = { 0 };
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            res.data[j][i] = matrix.data[i][j];
        }
    }
    return res;
}

// TODO
Mat4 Mat4_MulMM(Mat4 mat1, Mat4 mat2)
{
    Mat4 res = { 0 };
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            res.data[i][j] = 0;
            for (int k = 0; k < 4; k++)
                res.data[i][j] += mat1.data[i][k] * mat2.data[k][j];
        }
    }
    return res;
}

// TODO
Vec4 Mat4_MulMV(Mat4 mat, Vec4 v)
{
    Vec4 res = { 0 };
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            res.data[i] = 0;
            for (int k = 0; k < 4; k++)
                res.data[i] += mat.data[i][k] * v.data[k];
        }
    }
    return res;
}

// TODO
Mat4 Mat4_Scale(Mat4 mat, float s)
{
    Mat4 res = { 0 };
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            res.data[i][j] = mat.data[i][j] * s;
        }
    }
    return res;
}

// TODO
float Mat4_Cofactor(Mat4 mat, int i, int j)
{
    Mat3 mat3 = { 0 };

    for (int k = 0, m = 0; k < 3 && m < 4; ++k, ++m) {
        for (int l = 0, n = 0; l < 3 && n < 4; ++l, ++n) {
            if (m == i)
                m++;
            if (n == j)
                n++;
            mat3.data[k][l] = mat.data[m][n];
        }
    }
    float cofactor = pow(-1, (double)i + j) * Mat3_Det(mat3);

    return cofactor;
}

// TODO
Mat4 Mat4_CofactorMatrix(Mat4 mat)
{
    Mat4 res = { 0 };
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res.data[i][j] = Mat4_Cofactor(mat, i, j);
        }
    }
    return res;
}

// TODO
float Mat4_Det(Mat4 mat)
{
    float res = 0.f;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            res += mat.data[i][j] * Mat4_Cofactor(mat, 0, j);
        }
    }
    return res;
}

// TODO
Mat4 Mat4_Inv(Mat4 mat)
{
    Mat4 res = { 0 };
    float detM = Mat4_Det(mat);

    if (fabsf(detM - 0.f) < 1e-5f)
        return Mat4_Identity;

    res = Mat4_CofactorMatrix(mat);
    res = Mat4_Transpose(res);

    res = Mat4_Scale(res, 1 / detM);
    return res;
}

//-------------------------------------------------------------------------------------------------
// Fonctions du basecode

Mat4 Mat4_GetProjectionMatrix(float l, float r, float b, float t, float n, float f)
{
    Mat4 projectionMatrix = Mat4_Identity;

    if (r == l || t == b || f == n)
        return projectionMatrix;

    n = -n;
    f = -f;

    projectionMatrix.data[0][0] = 2.f * n / (r - l);
    projectionMatrix.data[1][1] = 2.f * n / (t - b);
    projectionMatrix.data[2][2] = (f + n) / (f - n);
    projectionMatrix.data[3][3] = 0.f;

    projectionMatrix.data[0][2] = -(r + l) / (r - l);
    projectionMatrix.data[1][2] = -(t + b) / (t - b);
    projectionMatrix.data[3][2] = 1.f;

    projectionMatrix.data[2][3] = -2.f * f * n / (f - n);

    return projectionMatrix;
}


Mat4 Mat4_GetScaleMatrix(float s)
{
    Mat4 scaleMatrix = Mat4_Identity;

    for (int i = 0; i < 3; i++)
        scaleMatrix.data[i][i] = s;

    return scaleMatrix;
}

Mat4 Mat4_GetScale3Matrix(Vec3 v)
{
    Mat4 scaleMatrix = Mat4_Identity;

    for (int i = 0; i < 3; i++)
        scaleMatrix.data[i][i] = v.data[i];

    return scaleMatrix;
}

Mat4 Mat4_GetTranslationMatrix(Vec3 v)
{
    Mat4 translationMatrix = Mat4_Identity;

    for (int i = 0; i < 3; i++)
    {
        translationMatrix.data[i][3] = v.data[i];
    }
    translationMatrix.data[3][3] = 1.f;

    return translationMatrix;
}

Mat4 Mat4_GetXRotationMatrix(float angle)
{
    Mat4 rotationMatrix = Mat4_Identity;

    angle = (float)(angle * M_PI / 180.f);
    float c = cosf(angle);
    float s = sinf(angle);

    rotationMatrix.data[1][1] = c;
    rotationMatrix.data[2][2] = c;

    rotationMatrix.data[1][2] = -s;
    rotationMatrix.data[2][1] = s;

    return rotationMatrix;
}

Mat4 Mat4_GetYRotationMatrix(float angle)
{
    Mat4 rotationMatrix = Mat4_Identity;

    angle = (float)(angle * M_PI / 180.f);
    float c = cosf(angle);
    float s = sinf(angle);

    rotationMatrix.data[0][0] = c;
    rotationMatrix.data[2][2] = c;

    rotationMatrix.data[2][0] = -s;
    rotationMatrix.data[0][2] = s;

    return rotationMatrix;
}

Mat4 Mat4_GetZRotationMatrix(float angle)
{
    Mat4 rotationMatrix = Mat4_Identity;

    angle = (float)(angle * M_PI / 180.f);
    float c = cosf(angle);
    float s = sinf(angle);

    rotationMatrix.data[0][0] = c;
    rotationMatrix.data[1][1] = c;

    rotationMatrix.data[0][1] = -s;
    rotationMatrix.data[1][0] = s;

    return rotationMatrix;
}
