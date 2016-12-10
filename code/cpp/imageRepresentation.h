#ifndef IMAGE_REPRESENTATION
#define IMAGE_REPRESENTATION



#include "correlation.h"



class imageRepresentation
{
  public:
  void createLut(int N1, int N2, int theta1, int theta2);

  //~ Mat imgRep(Mat h, Mat h3,int quantR, int quantF, int quantTheta1,
   //~ int quantTheta2);

  void imgRepLut(int img_sz[2], fftw_complex* h3, int quantTheta1,
   int quantTheta2, Mat& lut, ofstream& lf, float* h_3);

  Mat readLut(int N, int theta1, int theta2);
};

#endif
