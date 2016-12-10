#include <mex.h> /* Always include this */
#include <math.h>

#define M_PI           3.14159265358979323846  /* pi */

float roundf(float x);
void createLut(int N1, int N2, int quantTheta1,int quantTheta2,double* LUT);
int findDim(int N1, int N2, int quantTheta1,int quantTheta2);

void mexFunction(int nlhs, mxArray *plhs[], /* Output Array */
 int nrhs, const mxArray *prhs[]) /* Input Array */
{
  /* Macros for the output and input arguments */

  int N,Q1,Q2;
  double *LUT = NULL;

  if(nrhs <3 || nrhs >3)
  {
    mexErrMsgTxt("The correct number of inputs is 3 (image_size,theta_1_quant,theta_2_quant)");
  }
  else if (nlhs > 1)
  {
    mexErrMsgTxt("There is only one output");
  }

  // get values from input pointers
  N = (int) mxGetScalar(prhs[0]); 
  Q1 = (int) mxGetScalar(prhs[1]);
  Q2 = (int) mxGetScalar(prhs[2]);

  // allocate memory for output matrix
  plhs[0] = mxCreateDoubleMatrix(1, findDim(N, N, Q1, Q2)*6, mxREAL);
  LUT = (double*) mxGetData(plhs[0]);

  // calculate output matrix
  createLut(N, N, Q1, Q2, LUT);
}

float roundf(float x)
{
  return x >= 0.0f ? floorf(x + 0.5f) : ceilf(x - 0.5f);
}

void createLut(int N1, int N2, int quantTheta1,int quantTheta2,double* LUT)
{
  int t1LengthSquare, t2LengthSquare, t2Minust1LengthSquare, t1Minust2x,
   t1Minust2y, t2Minust1x, t2Minust1y, theta1, theta2;
  float theta1float, theta2float;

  ///~ Step 2.3
  int count = 0;
  for (int t1x = -N2 + 1; t1x <= N2 - 1; t1x++)
  {
    for (int t1y = -N1 + 1; t1y <= N1 - 1; t1y++)
    {
      t1LengthSquare = t1x * t1x + t1y * t1y;

      for (int t2x = -N2 + 1; t2x <= N2 - 1; t2x++)
      {
        for (int t2y = -N1 + 1; t2y <= N1 - 1; t2y++)
        {

          t1Minust2x = t1x - t2x;
          t1Minust2y = t1y - t2y;

          ///~ Testing for S''(?1)
          if ((t1Minust2x <= N2 - 1)
           && (t1Minust2x >= -N2 + 1)
           && (t1Minust2y <= N1 - 1)
           && (t1Minust2y >= -N1 + 1)
           && (t1x * t2x + t1y * t2y >= 0) && (t1x * (-t2y) + t1y * t2x >= 0)
           && (t1x * t2x + t1y * t2y <= t1LengthSquare))
          {
            ///~ S''(?1) guarantees that ?1 & ?2 are less than 90 degrees.
            ///~ ?2 E S''(?1)

            t2LengthSquare = t2x * t2x + t2y * t2y;
            t2Minust1x = t2x - t1x;
            t2Minust1y = t2y - t1y;
            t2Minust1LengthSquare =
             t2Minust1x * t2Minust1x + t2Minust1y * t2Minust1y;

            if (t1LengthSquare * t2LengthSquare * t2Minust1LengthSquare != 0)
            {
              ///~ A triangle is formed
              ///~ Calculate ?1 & ?2
              theta1float = (t1x * t2x + t1y * t2y) /
              sqrt((double) (t1LengthSquare * t2LengthSquare));

              theta2float = (t2Minust1x * (-t1x) + t2Minust1y * (-t1y)) /
              sqrt((double) (t1LengthSquare * t2Minust1LengthSquare));

              theta2float = theta2float > 1.?1.:theta2float;
              theta2float = theta2float < -1.?-1.:theta2float;

              theta1float = theta1float > 1.?1.:theta1float;
              theta1float = theta1float < -1.?-1.:theta1float;

              theta1float = acos(theta1float) * 180 / M_PI; ///~ theta1 in degrees
              theta2float = acos(theta2float) * 180 / M_PI; ///~ theta2 in degrees

              ///~ Quantization of ?1 & ?2
              theta1 = roundf((theta1float / 90) * quantTheta1);
              theta2 = roundf((theta2float / 90) * quantTheta2);

              count += 1;
              LUT = (double*) mxRealloc(LUT, count * 6 * sizeof(double));
              //mexPrintf("%d ", count);

              LUT[(count - 1) * 6] = t1x;
              LUT[(count - 1) * 6 + 1] = t1y;
              LUT[(count - 1) * 6 + 2] = t2x;
              LUT[(count - 1) * 6 + 3] = t2y;
              LUT[(count - 1) * 6 + 4] = theta1;
              LUT[(count - 1) * 6 + 5] = theta2;
            }
          }
        }
      }
    }
  }
}

// dummy function - couldnt find an other way
int findDim(int N1, int N2, int quantTheta1,int quantTheta2) {


int t1LengthSquare, t2LengthSquare, t2Minust1LengthSquare, t1Minust2x,
t1Minust2y, t2Minust1x, t2Minust1y, theta1, theta2;
float theta1float, theta2float;

///~ Step 2.3
int count = 0;
for (int t1x = -N2 + 1; t1x <= N2 - 1; t1x++)
{
for (int t1y = -N1 + 1; t1y <= N1 - 1; t1y++)
{
t1LengthSquare = t1x * t1x + t1y * t1y;

for (int t2x = -N2 + 1; t2x <= N2 - 1; t2x++)
{
for (int t2y = -N1 + 1; t2y <= N1 - 1; t2y++)
{

t1Minust2x = t1x - t2x;
t1Minust2y = t1y - t2y;

///~ Testing for S''(?1)
if ((t1Minust2x <= N2 - 1)
&& (t1Minust2x >= -N2 + 1)
&& (t1Minust2y <= N1 - 1)
&& (t1Minust2y >= -N1 + 1)
&& (t1x * t2x + t1y * t2y >= 0) && (t1x * (-t2y) + t1y * t2x >= 0)
&& (t1x * t2x + t1y * t2y <= t1LengthSquare))
{
///~ S''(?1) guarantees that ?1 & ?2 are less than 90 degrees.
///~ ?2 E S''(?1)

t2LengthSquare = t2x * t2x + t2y * t2y;
t2Minust1x = t2x - t1x;
t2Minust1y = t2y - t1y;
t2Minust1LengthSquare = t2Minust1x * t2Minust1x + t2Minust1y * t2Minust1y;

if (t1LengthSquare * t2LengthSquare * t2Minust1LengthSquare != 0)
{              
count += 1;
}
}
}
}
}
}

return count;

}
