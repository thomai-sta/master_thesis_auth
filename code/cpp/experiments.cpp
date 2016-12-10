#include "experiments.h"
using namespace std;
using namespace cv;

experiments::experiments()
{
  theta_1 = 8;
  theta_2 = 8;
  
  step = 5;

  rad[0] = 8;
  rad[1] = 4;

  for (int i = 0; i < 2; i++)
  {
    ///~ Extracted window from image
    extracted[i] = Mat(2 * rad[i] + 1, 2 * rad[i] + 1, CV_32FC1);

    ///~ Zeropad it to calculate FT
    padded[i] = Mat(2 * (2 * rad[i] + 1) - 1, 2 * (2 * rad[i] + 1) - 1,
     CV_32FC1);

    ///~ FFTW needs these numbers
    sz[i][0] = extracted[i].rows;
    sz[i][1] = extracted[i].cols;

    n1[i][0] = 2 * (2 * rad[i] + 1) - 1;
    n1[i][1] = 2 * (2 * rad[i] + 1) - 1;

    n2[i][0] = 2 * (2 * rad[i] + 1) - 1;
    n2[i][1] = 2 * (2 * rad[i] + 1) - 1;
    n2[i][2] = 2 * (2 * rad[i] + 1) - 1;
    n2[i][3] = 2 * (2 * rad[i] + 1) - 1;

    ///~ We need a LUT for each radius =========================================
    lut[i] = rep.readLut(2 * rad[i] + 1, theta_1, theta_2);

    ///~ We need a Gaussian matrix for each radius =============================
    filter[i] = createGaussian(2 * rad[i] + 1, 2 * rad[i] + 1,
     rad[i] / 2, rad[i] / 2);

    ///~ FFTW PLANS
    ///~ N = 2 * radius[i] + 1, M = 2 * N - 1 ===========================

    h[i] = (fftw_complex*) ///~ Zeropadded extracted window
     fftw_malloc((2 * (2 * rad[i] + 1) - 1) * (2 * (2 * rad[i] + 1) - 1)
     * sizeof(fftw_complex));

    H[i] = (fftw_complex*) ///~ FT of extracted window
     fftw_malloc((2 * (2 * rad[i] + 1) - 1) * (2 * (2 * rad[i] + 1) - 1)
     * sizeof(fftw_complex));

    ///~ FT of thid order correlation
    H3[i] = (fftw_complex*) fftw_malloc((2 * (2 * rad[i] + 1) - 1)
     * (2 * (2 * rad[i] + 1) - 1) *(2 * (2 * rad[i] + 1) - 1) *
     (2 * (2 * rad[i] + 1) - 1) * sizeof(fftw_complex));

    ///~ Third order correlation
    h3[i] = (fftw_complex*) fftw_malloc((2 * (2 * rad[i] + 1) - 1)
     * (2 * (2 * rad[i] + 1) - 1) *(2 * (2 * rad[i] + 1) - 1)
     * (2 * (2 * rad[i] + 1) - 1) * sizeof(fftw_complex));

    twoD[i] = fftw_plan_dft(2, n1[i], h[i], H[i], -1, FFTW_EXHAUSTIVE);
    inverse[i] = fftw_plan_dft(4, n2[i], H3[i], h3[i], 1, FFTW_EXHAUSTIVE);
  }
  ///~ Initialize h_3 and final ================================================
  h_3 = new float[(theta_1 + 1) * (theta_2 + 1)];
}


void experiments::begin(char* listPath, char* listName)
{
  ostringstream listLocation;
  listLocation << listPath << listName;
  fstream list(listLocation.str().c_str(), ios::in);
  if (!list.is_open())
  {
    cout << "ERROR opening list\n";
    return;
  }
  string line;

  ///~ Create output file for local features ===================================
  ostringstream file, filetxt;
  ofstream lf, names;
  file << "/home/thomai/experiments/features/" << listName << ".bin";
  lf.open(file.str().c_str(), ios::out | ios::binary);
  filetxt << "/home/thomai/experiments/features/" << listName << ".txt";
  names.open(filetxt.str().c_str(), ios::out);

  while(!list.eof())
  {
    getline(list, line);
    if (!line.empty())
    {
      ///~ Get Image ===========================================================
      ostringstream imgName;
      imgName << line;
      Mat image = imread(imgName.str().c_str(), CV_LOAD_IMAGE_COLOR);
      ///~ Check if empty
      if (image.empty())
      {
        cout << "Error loading the image\n";
        return;
      }
      ///~ Image type = 16 --> CV_8UC3
      if (image.type() != 21)
      {
        image.convertTo(image, CV_32F, 1);
      }
      ///~ Seperate channels
      Mat image_split[image.channels()];
      split(image, image_split);

      ///~ Start proccess ======================================================
      cout << "Creating LFs for " << line << "\n";

      names << line << " ";
      int local_feature = 0;

      for (int img_row = rad[0]; img_row < image.rows - rad[0];
       img_row += step)
      {
        for (int img_col = rad[0]; img_col < image.cols - rad[0];
         img_col += step)
        {
          for (int radius = 0; radius < 2; radius++)
          {
            ///~ Extract window from image =====================================
            for (int chan = 0; chan < image.channels(); chan++)
            {
              image_split[chan]
               .rowRange(img_row - rad[radius], img_row + rad[radius] + 1)
               .colRange(img_col - rad[radius], img_col + rad[radius] + 1)
               .copyTo(extracted[radius]);
              ///~ Multiply with Gaussian Function
              extracted[radius] = filter[radius].mul(extracted[radius]);
      
              ///~ Zeropad extracted
              copyMakeBorder(extracted[radius], padded[radius],
               0, 2 * rad[radius], 0, 2 * rad[radius],
               BORDER_CONSTANT, Scalar::all(0));
      
              ///~ Fill h
              for (int i = 0; i < 2 * (2 * rad[radius] + 1) - 1; i++)
              {
                for (int j = 0; j < 2 * (2 * rad[radius] + 1) - 1; j++)
                {
                  h[radius][j + (2 * (2 * rad[radius] + 1) - 1) * i][0] =
                   padded[radius].at<float>(i,j);
                  h[radius][j + (2 * (2 * rad[radius] + 1) - 1) * i][1] = 0;
                }
              }
              ///~ Calculate fft2 of extracted
              fftw_execute(twoD[radius]); ///~ H contains the FT of h
              ///~ Calculate correlation of extracted
              corr.corr23DFT(h3[radius], sz[radius], H[radius], H3[radius],
               inverse[radius]);
      
              ///~ Calculate representation of extracted
              rep.imgRepLut(sz[radius], h3[radius], theta_1, theta_2,
               lut[radius], lf, h_3);
            }
          }
          local_feature++;
        }
      }
      names << (theta_1 + 1) * (theta_2 + 1) * 2 * 3 * local_feature << "\n";
    }
  }
  lf.close();
  names.close();
}

Mat experiments::createGaussian(int rows, int columns , int sx, int sy)
{
  ///~ Center of Gaussian Function
  int i0 = floor(double(rows / 2));
  int j0 = floor(double(columns / 2));
  ///~ Create Gaussian Function
  Mat filter(rows, columns, CV_32FC1, Scalar::all(0));
  for (int x = 0; x < columns; x++)
  {
    for (int y = 0; y < rows; y++)
    {
      filter.at<float>(y, x) =
       exp(-(
       pow(y - i0, 2) / (2 * pow(sy, 2)) +
       pow(x - j0, 2) / (2 * pow(sx, 2))));
    }
  }
  return filter;
}
