#include "imageRepresentation.h"

using namespace std;
using namespace cv;


void imageRepresentation::imgRepLut(int img_sz[2], fftw_complex* h3,
 int quantTheta1, int quantTheta2, Mat& lut, ofstream& lf, float* h_3)
{
  float m1 = 2 * img_sz[0] - 1, m2 = 2 * img_sz[1] - 1;
  int N1 = img_sz[0], N2 = img_sz[1], M1 = 2 * N1 - 1, M2 = 2 * N2 - 1,
   mid1 = ceil(m1 / 2), mid2 = ceil(m2 / 2), i, j, k, l, theta1, theta2;
  int h_3_sz = (quantTheta1 + 1) * (quantTheta2 + 1);

  ///~ Set h_3 to zero
  for (int i = 0; i < h_3_sz; i++)
  {
    h_3[i] = 0;
  }

  for (int cnt = 0; cnt < lut.rows; cnt++)
  {
    ///~ Filling h_3 matrix
    ///~ h_3((quantTheta1 + 1),(quantTheta2 + 1))
    i = (N1 - 1 - lut.at<int>(cnt,1) + mid1) % M1; ///~ We didn't sift the
    j = (lut.at<int>(cnt,0) + N2 - 1 + mid2) % M2; ///~ correlation. We do it
    k = (N1 - 1 - lut.at<int>(cnt,3) + mid1) % M1; ///~ now when taking the
    l = (lut.at<int>(cnt,2) + N2 - 1 + mid2) % M2; ///~ elements.
    theta1 = lut.at<int>(cnt,4);
    theta2 = lut.at<int>(cnt,5);

    h_3[theta2 + (quantTheta2 + 1) * theta1] +=
     h3[l + M2 * (k + M1 * (j + M2 * i))][0] / (M1 * M2 * M1 * M2);
     ///~ Due to the inverse Fourier transform we need to divide with
     ///~ (M1 * M2 * M1 * M2)
  }

  //~ for (int i = 0; i < (quantTheta1 + 1) * (quantTheta2 + 1); i++)
  //~ {
    //~ lf << h_3[i] << " ";
  //~ }
    lf.write ((char*)&h_3[0],
     (quantTheta1 + 1) * (quantTheta2 + 1) * sizeof(float));
}


void imageRepresentation::createLut(int N1, int N2, int quantTheta1,
 int quantTheta2)
{
  int t1LengthSquare, t2LengthSquare, t2Minust1LengthSquare, t1Minust2x,
   t1Minust2y, t2Minust1x, t2Minust1y, theta1, theta2;
  Mat t1(1, 2, CV_32SC1), t2(1, 2, CV_32SC1), t2Rot(1, 2, CV_32SC1);
  float theta1float, theta2float;

  ///~ Step 2.3
  int count = 0;
  ///~ Create File
  ostringstream convert;
  convert << "config_files/lut" << N1 << "x" << N2 <<
   ",theta1=" << quantTheta1 << "theta2=" << quantTheta2 << ".bin";
  ofstream lut;
  lut.open(convert.str().c_str(), ios::out | ios::binary);
  int lutLine[6];
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

          ///~ Testing for S''(τ1)
          if ((t1Minust2x <= N2 - 1)
           && (t1Minust2x >= -N2 + 1)
           && (t1Minust2y <= N1 - 1)
           && (t1Minust2y >= -N1 + 1)
           && (t1x * t2x + t1y * t2y >= 0) && (t1x * (-t2y) + t1y * t2x >= 0)
           && (t1x * t2x + t1y * t2y <= t1LengthSquare))
          {
            ///~ S''(τ1) guarantees that θ1 & θ2 are less than 90 degrees.
            ///~ τ2 E S''(τ1)

            t2LengthSquare = t2x * t2x + t2y * t2y;
            t2Minust1x = t2x - t1x;
            t2Minust1y = t2y - t1y;
            t2Minust1LengthSquare =
             t2Minust1x * t2Minust1x + t2Minust1y * t2Minust1y;
             
            if (t1LengthSquare * t2LengthSquare * t2Minust1LengthSquare != 0)
            {
              ///~ A triangle is formed
              ///~ Calculate θ1 & θ2
              theta1float = (t1x * t2x + t1y * t2y) /
               sqrt(t1LengthSquare * t2LengthSquare);
               
              theta2float = (t2Minust1x * (-t1x) + t2Minust1y * (-t1y)) /
               sqrt(t1LengthSquare * t2Minust1LengthSquare);

              theta2float = theta2float > 1.?1.:theta2float;
              theta2float = theta2float < -1.?-1.:theta2float;
              
              theta1float = theta1float > 1.?1.:theta1float;
              theta1float = theta1float < -1.?-1.:theta1float;

              theta1float = acos(theta1float) * 180 / CV_PI; ///~ theta1 in degrees
              theta2float = acos(theta2float) * 180 / CV_PI; ///~ theta2 in degrees

              ///~ Quantization of θ1 & θ2
              theta1 = round((theta1float / 90) * quantTheta1);
              theta2 = round((theta2float / 90) * quantTheta2);

              ///~ Write to File
              count += 1;
              lutLine[0] = t1x;
              lutLine[1] = t1y;
              lutLine[2] = t2x;
              lutLine[3] = t2y;
              lutLine[4] = theta1;
              lutLine[5] = theta2;
              lut.write ((char*)&lutLine[0], 6 * sizeof(int));
            }
          }
        }
      }
    }
  }
  ///~ Close File
  lut.close();
  cout << "File count = " << count << "\n";
}



Mat imageRepresentation::readLut(int N, int theta1, int theta2)
{
  ostringstream convert;
  convert << "config_files/lut" << N << "x" << N <<
   ",theta1=" << theta1 << "theta2=" << theta2 << ".bin";
  ifstream lutIn(convert.str().c_str(), ios::in | ios::binary);
  ///~ Check if file exists
  if (!lutIn.is_open())
  {
    createLut(N, N, theta1, theta2);
    lutIn.open(convert.str().c_str(), ios::in | ios::binary);
  }
  ///~ Get number of lut bytes
  streampos begin = lutIn.tellg();
  lutIn.seekg (0, ios::end);
  streampos end = lutIn.tellg();
  int bytes = end - begin;
  int rows = bytes / (sizeof(int) * 6);
  ///~ Create LUT Mat
  Mat lut(rows, 6, CV_32SC1);
  lutIn.seekg (0);
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < 6; j++)
    {
      lutIn.read((char*) &lut.at<int>(i,j), sizeof(int));
      //~ cout << lut.at<int>(i,j) << "\t";
    }
    //~ cout << "\n";
  }
  lutIn.close();
  return lut;
}
