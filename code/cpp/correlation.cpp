#include "correlation.h"

using namespace std;
using namespace cv;


//~ Mat correlation::corr2(float *h, int N)
//~ {
  //~ int M = 2 * N - 1;
  //~ Mat h2(1, M, CV_32FC1, Scalar::all(0));
  //~ Mat h1 = hankel(h, N);
  //~ Mat a = h1 * h1.t();
  //~ Mat b = a.row(0);
  //~ for (int i = 0; i < M; i++)
  //~ {
    //~ if (i < N)
    //~ {
      //~ h2.at<float>(0, i) = b.at<float>(0, N - 1 - i);
    //~ }
    //~ else
    //~ {
      //~ h2.at<float>(0, i) = b.at<float>(0, i - N + 1);
    //~ }
  //~ }
  //~ return h2;
//~ }

//~ Mat correlation::corr3(float *h, int N)
//~ {
  //~ int M = 2 * N - 1;
  //~ Mat h3(M, M, CV_32FC1, Scalar::all(0));
  //~ Mat h1 = hankel(h, N);
  //~ Scalar c;
  //~ /*Properties:
  //~ *   h3(m1, m2) = 
  //~ *   h3(m2, m1) = 
  //~ *   h3(m1 - m2, -m2) = 
  //~ *   h3(m2 - m1, -m1) =
  //~ *   h3(-m1, m2 - m1) = 
  //~ *   h3(-m2, m1 - m2)
  //~ * 
  //~ *    h3(m1, m2) = 0,for every (m1, m2) : | m1 - m2 | >= N 
  //~ *   We need only to calculate the elements for (m1, m2) : [0, ..., N - 1]x[0, ..., N - 1]
  //~ */
  //~ for (int i = N - 1; i < M; i++)
  //~ {
    //~ for (int j = i; j < M; j++)
    //~ {
      //~ Mat a = h1.row(0).mul(h1.row(i - N + 1));
      //~ Mat b = a.mul(h1.row(j - N + 1));
      //~ c = sum(b);
      //~ h3.at<float>(i, j) = c[0];
      //~ h3.at<float>(j, i) = c[0];
      //~ h3.at<float>(M - i - 1, N + j - i - 1) = c[0];
      //~ h3.at<float>(N + i - j - 1, M - j - 1) = c[0];
      //~ h3.at<float>(N + j - i - 1, M - i - 1) = c[0];
      //~ h3.at<float>(M - j - 1, N + i - j - 1) = c[0];
    //~ }
  //~ }
  //~ return h3;
//~ }
//~ 
//~ Mat correlation::corr22(Mat h)
//~ {
  //~ if (h.dims == 1)
  //~ {
    //~ cout << "ERROR in corr22: This function calculates the correlation of a 2D signal!!!\nUse the corr2 or the corr3 function instead!\n";
    //~ return h;
  //~ }
  //~ else
  //~ {
    //~ Scalar b;
    //~ Size sz = h.size(); 
    //~ int M1 = 2 * sz.height - 1, M2 = 2*sz.width-1;
    //~ Mat h22(M1, M2, CV_32FC1, Scalar::all(0));
    //~ ///~ Isxuei pali oti h3(m) = h3(-m). Tha upologisw to panw miso tou pinaka.
    //~ for (int i = 0; i < sz.height; i++)
    //~ {
      //~ for (int j = 0; j < M2; j++)
      //~ {
        //~ if (j < sz.width)
        //~ {
          //~ Mat h_shifted = h.rowRange(sz.height - 1 - i, sz.height).colRange(sz.width - 1 - j, sz.width);
          //~ Mat h_masked = h.rowRange(0, i + 1).colRange(0, j + 1);
          //~ Mat a = h_shifted.mul(h_masked);
          //~ b = sum(a);
          //~ h22.at<float>(i, j) = b[0];
          //~ h22.at<float>(M1 - i - 1, M2 - j - 1) = b[0];
          //~ ~h_shifted;
          //~ ~h_masked;
          //~ ~a;
        //~ }
        //~ else
        //~ {
          //~ Mat h_shifted = h.rowRange(sz.height - 1 - i, sz.height).colRange(0, 2 * sz.width - j - 1);
          //~ Mat h_masked = h.rowRange(0, i + 1).colRange(j - sz.width + 1, sz.width);
          //~ Mat a = h_shifted.mul(h_masked);
          //~ b = sum(a);
          //~ h22.at<float>(i, j) = b[0];
          //~ h22.at<float>(M1 - i - 1, M2 - j - 1) = b[0];
          //~ ~h_shifted;
          //~ ~h_masked;
          //~ ~a;
        //~ }
      //~ }
    //~ }
    //~ return h22;
  //~ }
//~ }
//~ 
//~ Mat correlation::corr23(Mat h)
//~ {
  //~ if (h.dims == 1)
  //~ {
    //~ cout << "ERROR in corr23: This function calculates the correlation of a 2D signal!!!\n\
//~ Use the corr2 or the corr3 function instead!\n";
    //~ return h;
  //~ }
  //~ else
  //~ {
    //~ int N2 = h.size().width, N1 = h.size().height, M1 = 2 * N1 - 1,
     //~ M2 = 2 * N2 - 1, sz[4] = {M1, M2, M1, M2};
    //~ Mat h23(4, sz, CV_32FC1, Scalar::all(0));
    //~ Scalar b;
    //~ int indx[4];
    //~ /*
    //~ * Properties:
    //~ *  h3(m1,m2) = 
    //~ *  h3(m2,m1) =
    //~ *  h3(m1-m2,-m2) =
    //~ *  h3(m2-m1,-m1) =
    //~ *  h3(-m1,m2-m1) =
    //~ *  h3(-m2,m1-m2)         
    //~ */
    //~ for (int m11 = 0; m11 < M1; m11++)
    //~ {
      //~ int y1 = N1 - 1 - m11;
      //~ for (int m12 = 0; m12 < M2; m12++)
      //~ {
        //~ int x1 = m12 - N2 + 1;
         //~ ///===========================  temp1  =====================================           
        //~ Mat temp1(N1, N2, CV_32FC1, Scalar::all(0));
        //~ if (m11 < N1 && m12 < N2)
        //~ {
          //~ h.rowRange(0, m11 + 1).colRange(0, m12 + 1).copyTo
           //~ (temp1.rowRange(N1 - m11 - 1, N1).colRange(N2 - m12 - 1, N2));
        //~ }
        //~ else if (m11 < N1 && m12 >= N2)
        //~ {
          //~ h.rowRange(0, m11 + 1).colRange(m12 - N2 + 1, N2).copyTo
           //~ (temp1.rowRange(N1 - m11 - 1, N1).colRange(0, 2 * N2 - m12 - 1));
        //~ }
        //~ else if (m11 >= N1 && m12 < N2)
        //~ {
          //~ h.rowRange(m11 - N1 + 1, N1).colRange(0, m12 + 1).copyTo
           //~ (temp1.rowRange(0, 2 * N1 - m11 - 1).colRange(N2 - m12 - 1, N2));
        //~ }
        //~ else if (m11 >= N1 && m12 >= N2)
        //~ {
          //~ h.rowRange(m11 - N1 + 1, N1).colRange(m12 - N2 + 1, N2).copyTo
           //~ (temp1.rowRange(0, 2 * N1 - m11 - 1).colRange(0, 2 * N2 - m12 - 1));
        //~ }
        ///====================================================================
        //~ int absm1 = pow(x1, 2) + pow(y1, 2);
        //~ for (int m21 = 0; m21 < M1; m21++)
        //~ {
          //~ int y2 = N1 - 1 - m21;
          //~ for (int m22 = 0; m22 < M2; m22++)
          //~ {
            //~ int x2 = m22 - N2 + 1;
            //~ ///~ Checking for S''(m1)
            //~ if ((x1 * x2 + y1 * y2 >= 0) && (x1 * x2 + y1 * y2 <= absm1) &&
             //~ (x1 - x2 >= -N2 + 1) && (x1 - x2 <= N2 - 1) &&
             //~ (y1 - y2 >= -N1 + 1) && (y1 - y2 <= N1 - 1) &&
             //~ (x1 * y2 - y1 * x2 >= 0))
            //~ {
              //~ ========================  temp2  ===============================
              //~ Mat temp2(N1, N2, CV_32FC1, Scalar::all(0));
              //~ if (m21 < N1 && m22 < N2)
              //~ {
                //~ h.rowRange(0, m21 + 1).colRange(0, m22 + 1).copyTo
                 //~ (temp2.rowRange(N1 - m21 - 1, N1).colRange(N2 - m22 - 1, N2));
              //~ }
              //~ else if (m21 < N1 && m22 >= N2)
              //~ {
                //~ h.rowRange(0, m21 + 1).colRange(m22 - N2 + 1, N2).copyTo
                 //~ (temp2.rowRange(N1 - m21 - 1, N1).colRange(0, 2 * N2 - m22 - 1));
              //~ }
              //~ else if (m21 >= N1 && m22 < N2)
              //~ {
                //~ h.rowRange(m21 - N1 + 1, N1).colRange(0, m22 + 1).copyTo
                 //~ (temp2.rowRange(0, 2 * N1 - m21 - 1).colRange(N2 - m22 - 1, N2));
              //~ }
              //~ else if (m21 >= N1 && m22 >= N2)
              //~ {
                //~ h.rowRange(m21 - N1 + 1, N1).colRange(m22 - N2 + 1, N2).copyTo
                 //~ (temp2.rowRange(0, 2 * N1 - m21 - 1).
                 //~ colRange(0, 2 * N2 - m22 - 1));
              //~ }
              //~ ================================================================
              //~ b = sum(h.mul(temp1.mul(temp2)));
              //~ indx[0] = m11;
              //~ indx[1] = m12;
              //~ indx[2] = m21;
              //~ indx[3] = m22;
              //~ h23.at<float>(indx) = b[0];
              //~ ==============================  Symmetries  ====================
              //~ indx[0] = m21;
              //~ indx[1] = m22;
              //~ indx[2] = m11;
              //~ indx[3] = m12;
              //~ h23.at<float>(indx) = b[0];
              //~ if ((N1 + m21 - m11 - 1 >= 0) && (N2 + m22 - m12 - 1 >= 0) &&
               //~ (N1 + m21 - m11 - 1 < M1) && (N2 + m22 - m12 - 1 < M2))
              //~ {
                //~ indx[0] = M1 - m11 - 1;
                //~ indx[1] = M2 - m12 - 1;
                //~ indx[2] = N1 + m21 - m11 - 1;
                //~ indx[3] = N2 + m22 - m12 - 1;
                //~ h23.at<float>(indx) = b[0];
                //~ indx[0] = N1 + m21 - m11 - 1;
                //~ indx[1] = N2 + m22 - m12 - 1;
                //~ indx[2] = M1 - m11 - 1;
                //~ indx[3] = M2 - m12 - 1;
                //~ h23.at<float>(indx) = b[0];
              //~ }
              //~ if ((N1 + m11 - m21 - 1 >= 0) && (N2 + m12 - m22 - 1 >= 0) &&
               //~ (N1 + m11 - m21 - 1 < M1) && (N2 + m12 - m22 - 1 < M2))
              //~ {
                //~ indx[0] = N1 + m11 - m21 - 1;
                //~ indx[1] = N2 + m12 - m22 - 1;
                //~ indx[2] = M1 - m21 - 1;
                //~ indx[3] = M2 - m22 - 1;
                //~ h23.at<float>(indx) = b[0];
                //~ indx[0] = M1 - m21 - 1;
                //~ indx[1] = M2 - m22 - 1;
                //~ indx[2] = N1 + m11 - m21 - 1;
                //~ indx[3] = N2 + m12 - m22 - 1;
                //~ h23.at<float>(indx) = b[0];
              //~ }
              //~ ~temp2;
            //~ }
          //~ }
        //~ }
        //~ ~temp1;
      //~ }
    //~ }
    //~ return h23;
    //~ return h;
  //~ }
//~ }

void correlation::print(Mat h23, int M1, int M2)
{
  if (h23.dims != 4)
  {
    cout << "No need for this function!\n";
  }
  else
  {
    int indx[4];
    for (indx[3] = 0; indx[3] < M2; indx[3]++)
    {
      for (indx[2] = 0; indx[2] < M1; indx[2]++)
      {
        cout << "Matrix (:,:," << indx[2] + 1 << "," << indx[3] + 1 << "):\n";
        for (indx[0] = 0; indx[0] < M1; indx[0]++)
        {
          for (indx[1] = 0; indx[1] < M2; indx[1]++)
          {
            cout << h23.at<float>(indx) << "\t";
          }
          cout << "\n";
        }
      }
    }
  }
}

void correlation::corr23DFT(fftw_complex* h3, int image_sz[2],
 fftw_complex* H, fftw_complex* product, fftw_plan inverse)
{
  /**
   * IMPORTANT:
   * ----------
   * h and h3 are in row-major format!!!!
   **/

  ///~ Declarations
  int M1 = 2 * image_sz[0] - 1, M2 = 2 * image_sz[1] - 1, indx[4],
   m31, m32;

  ///~ Calculate FT of h3 using the following property:
  ///~ Property:H3(w1,w2) = H(w1)H(w2)H*(w1 + w2)
  for (indx[0] = 0; indx[0] < M1; indx[0]++)
  {
    for (indx[1] = 0; indx[1] < M2; indx[1]++)
    {

      complex<float> first(H[indx[1] + M2 * indx[0]][0],
       H[indx[1] + M2 * indx[0]][1]);

      for (indx[2] = 0; indx[2] < M1; indx[2]++)
      {
        for (indx[3] = 0; indx[3] < M2; indx[3]++)
        {
          m31 = (indx[0] + indx[2]) % M1;
          m32 = (indx[1] + indx[3]) % M2;
          
          complex<float> second(H[indx[3] + M2 * indx[2]][0],
           H[indx[3] + M2 * indx[2]][1]);
           
          complex<float> third(H[m32 + M2 * m31][0], H[m32 + M2 * m31][1]);

          product[indx[3] + M2 * (indx[2] + M1 * (indx[1] +M2 * indx[0]))][0]
           = real(first * second * conj(third));

          product[indx[3] + M2 * (indx[2] + M1 * (indx[1] +M2 * indx[0]))][1]
           = imag(first * second * conj(third));
        }
      }
    }
  }
  fftw_execute(inverse); ///~ ifftn(product) = h3!!
  /**
   * IMPORTANT:
   * ----------
   * The result of the function needs to be fftshifted
   * in order to be equal with the third order correlation.
   **/
}
