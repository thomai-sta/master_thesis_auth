#ifndef CORRELATION
#define CORRELATION

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <fstream>
#include <complex>
#include <ctime>

#include <fftw3.h>


using namespace cv;
using namespace std;

class correlation
{
public:
  //~ Mat corr2(float *h, int N);
  //~ Mat corr3(float *h, int N);
  //~ Mat corr22(Mat h);
  //~ Mat corr23(Mat h);
  void print(Mat h23,int M1, int M2); 
  void corr23DFT(fftw_complex* h3, int image_sz[2], fftw_complex* H,
   fftw_complex* product, fftw_plan inverse);
};

#endif
