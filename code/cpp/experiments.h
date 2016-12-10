#ifndef EXPERIMENTS
#define EXPERIMENTS


#include "imageRepresentation.h"

class experiments
{
  int theta_1, theta_2, sz[2][2], n1[2][2], n2[2][4], n3[2], rad[2],
   step;
  imageRepresentation rep;
  correlation corr;
  Mat extracted[2], padded[2], lut[2], filter[2];
  fftw_plan twoD[2], inverse[2], finalFT;
  fftw_complex *H3[2], *H[2], *h3[2], *final_ft, *h[2];
  float *h_3;
public:
  experiments();
  void begin(char* listPath, char* listName);
  Mat createGaussian(int rows, int columns, int sx, int sy);
};

#endif
