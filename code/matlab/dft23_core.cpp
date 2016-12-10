#include <mex.h> /* Always include this */
#include <math.h>
#include <complex>

void mexFunction(int nlhs, mxArray *plhs[], /* Output Array */
 int nrhs, const mxArray *prhs[]) /* Input Array */
{
  /* Macros for the output and input arguments */

  int M1,M2,xDim,yDim;
  int indx[4], m31, m32; // indices
  double *Hr,*Hi;  // 2d - input
  double *H3r,*H3i; // output
  double firstr,firsti,secondr,secondi,thirdr,thirdi; // dummies

  /* check for the proper number of arguments */
  if(nrhs != 3)
   mexErrMsgIdAndTxt( "dft23_core:invalidNumInputs",
   "Three inputs required. (M1,M2,H)");
  if(nlhs > 1)
   mexErrMsgIdAndTxt( "dft23_core:maxlhs",
   "Too many output arguments.");
  /*Check that both inputs are row vectors*/
  if( mxGetM(prhs[2]) != 1 )
   mexErrMsgIdAndTxt( "dft23_core:inputsNotVectors",
   "Third inputs must be row vector.");
  /* Check that both inputs are complex*/
  if( !mxIsComplex(prhs[2]) )
   mexErrMsgIdAndTxt( "dft23_core:inputsNotComplex",
   "Third input must be complex.\n");


  // get values from input pointers
  M1 = (int) mxGetScalar(prhs[0]);
  M2 = (int) mxGetScalar(prhs[1]);
  Hr = mxGetPr(prhs[2]); //get real part
  Hi = mxGetPi(prhs[2]); //get imaginary part

  // allocate memory for output matrix
  plhs[0] = mxCreateDoubleMatrix(1, M1*M2*M1*M2, mxCOMPLEX);
  H3r = (double*) mxGetPr(plhs[0]);
  H3i = (double*) mxGetPi(plhs[0]);

  for (indx[0] = 0; indx[0] < M1; indx[0]++){
    for (indx[1] = 0; indx[1] < M2; indx[1]++){
      firstr = Hr[indx[1] + M2 * indx[0]];
      firsti = Hi[indx[1] + M2 * indx[0]];
      for (indx[2] = 0; indx[2] < M1; indx[2]++){
        for (indx[3] = 0; indx[3] < M2; indx[3]++){
          m31 = (indx[0] + indx[2]) % M1;
          m32 = (indx[1] + indx[3]) % M2;

          secondr = Hr[indx[3] + M2 * indx[2]];
          secondi = Hi[indx[3] + M2 * indx[2]];

          thirdr = Hr[m32 + M2 * m31];
          thirdi = Hi[m32 + M2 * m31];

          //H3(m11, m12, m21, m22) = H(m11, m12) * H(m21, m22) * conj(H(m31, m32));
          H3r[indx[3] + M2 * (indx[2] + M1 * (indx[1] +M2 * indx[0]))] =
           firstr * secondr * thirdr + firstr * secondi * thirdi +
           firsti * secondr * thirdi - firsti * secondi * thirdr;
          //mexPrintf("%d\n",H3r[m11+m12*M1+m21*M1*M2+m22*M1*M2*M1]);
          H3i[indx[3] + M2 * (indx[2] + M1 * (indx[1] +M2 * indx[0]))] = 
           firsti * secondi * thirdi + firsti * secondr * thirdr +
           firstr * secondi * thirdr - firstr * secondr * thirdi;
        }
      }
    }
  }
}
