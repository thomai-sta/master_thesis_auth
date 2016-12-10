#include "demo.h"

using namespace std;
using namespace cv;

Mat points;


void demo::checkLut(int N,int quantR, int quantF, int quantTheta1,
 int quantTheta2)
{
  //~ correlation corr;
  //~ imageRepresentation rep;
  //~ ///~ Initialize h_3 and final
  //~ Mat** h_3;
  //~ h_3 = new Mat*[quantTheta1 + 1];
  //~ for (int i = 0; i < quantTheta1 + 1; i++)
  //~ {
    //~ h_3[i] = new Mat[quantTheta2 + 1];
    //~ for (int j = 0; j < quantTheta2 + 1; j++)
    //~ {
      //~ h_3[i][j] = Mat::zeros(quantR + 1, quantF + 1, CV_32FC1);
    //~ }
  //~ }
  //~ float* final;
  //~ final = new float[(quantTheta1 + 1) * (quantTheta2 + 1)];
  //~ 
  //~ for (int i = 0; i < 6; i++)
  //~ {
    //~ ///~ Create output file for local feature
    //~ ostringstream file;
    //~ file << "test" << N << ".bin";
    //~ ofstream lf;
    //~ lf.open(file.str().c_str(), ios::out | ios::binary | ios::app);
    //~ cout << "N = " << N << "\n";
    //~ Mat h3;
    //~ Mat h(N, N, CV_32FC1, Scalar::all(1));
    //~ h3 = Mat::zeros(2 * N - 1, 2 * N - 1, CV_32FC1);
    //~ corr.corr23DFT(h, h3);
    //~ 
    //~ Mat lut = rep.readLut(N, quantR, quantF, quantTheta1, quantTheta2);
    //~ rep.imgRepLut(h, h3, quantR, quantF, quantTheta1, quantTheta2, lut, lf, h_3, final);
    //~ N++;
    //~ lf.close();
//~ 
    //~ ///~ read test file
    //~ ifstream lutIn(file.str().c_str(), ios::in | ios::binary);
    //~ ///~ Get number of lut bytes
    //~ streampos begin = lutIn.tellg();
    //~ lutIn.seekg (0, ios::end);
    //~ streampos end = lutIn.tellg();
    //~ int bytes = end - begin;
    //~ int rows = bytes / (sizeof(float)*(quantTheta2 + 1));
    //~ cout << "bytes = " << bytes << "\n";
    //~ cout << "rows = " << rows << "\n";
    //~ lutIn.seekg (0);
    //~ Mat test(rows, quantTheta2 + 1, CV_32FC1);
    //~ for (int i = 0; i < rows; i++)
    //~ {
      //~ for (int j = 0; j < quantTheta2 + 1; j++)
      //~ {
        //~ lutIn.read((char*) &test.at<float>(i,j), sizeof(float));
        //~ cout << test.at<float>(i,j) << "\t";
      //~ }
      //~ cout << "\n";
    //~ }
    //~ lutIn.close();
  //~ }
  //~ ///~ Destroy h_3 and final
  //~ for(int i = 0; i < quantTheta1 + 1; i++)
  //~ {
    //~ delete []h_3[i];
  //~ }
  //~ delete []h_3;
  //~ delete []final;
}


void demo::checkFFTW()
{
  //~ fftw_plan plan;
  //~ int n3[2];
  //~ fftw_complex *h_3, *final_ft;
  //~ n3[0] = 15 + 1;
  //~ n3[1] = 15 + 1;
  //~ h_3 = new fftw_complex[(5 + 1) * (5 + 1) * (15 + 1) * (15 + 1)];
  //~ final_ft =
   //~ (fftw_complex*) fftw_malloc((5 + 1) * (5 + 1) * (15 + 1) * (15 + 1)
   //~ * sizeof(fftw_complex));
//~ 
//~ 
  //~ cout << "h_3 and final initialized\n";
//~ 
  //~ plan = fftw_plan_many_dft(2, n3, (5 + 1) * (5 + 1), h_3,
   //~ n3, 1, n3[0] * n3[1], final_ft, n3, 1, n3[0] * n3[1], -1, FFTW_PATIENT);
//~ 
//~ 
  //~ for (int i = 0; i < 14904; i++)
  //~ {
    //~ fftw_execute(plan);
  //~ }
  int theta_1 = 5, theta_2 = 5, N = 5, M = 2 * N - 1;
  imageRepresentation rep;
  correlation corr;
  fftw_complex *H3, *H, *h3, *h;
  int sz[2] = {N, N}, n1[2], n2[4];
  float *h_3;
  n1[0] = M;
  n1[1] = M;

  n2[0] = M;
  n2[1] = M;
  n2[2] = M;
  n2[3] = M;

  ///~ We need a LUT =========================================================
  Mat lut;
  lut = rep.readLut(N, theta_1, theta_2);
  cout << "LUTs OK\n";


  ///~ FFTW PLANS
  
  h = (fftw_complex*) fftw_malloc(M * M * sizeof(fftw_complex));

  H = (fftw_complex*) fftw_malloc(M * M * sizeof(fftw_complex));

  H3 = (fftw_complex*) fftw_malloc(M * M * M * M * sizeof(fftw_complex));

  h3 = (fftw_complex*) fftw_malloc(M * M * M * M * sizeof(fftw_complex));

  fftw_plan twoD, inverse;


  twoD = fftw_plan_dft(2, n1, h, H, -1, FFTW_EXHAUSTIVE);
  inverse = fftw_plan_dft(4, n2, H3, h3, 1, FFTW_EXHAUSTIVE);


  ///~ Initialize h_3 and final ==============================================
  
  h_3 = new float[(theta_1 + 1) * (theta_2 + 1)];
  cout << "h_3 initialized\n";

  ///~ Create folder to store Local Features =================================
  ostringstream folder, command, all;
  command << "mkdir ";
  folder << "test/radius=4,theta1=" << theta_1 << ",theta2=" << theta_2;
  all << command.str().c_str() << folder.str().c_str();
  system(all.str().c_str());
  cout << "Folder for LFs created\n";


  ///~ Start proccess ========================================================
  cout << "\nCreating LFs...\n";

  ///~ Create output file for local feature ==================================
  ostringstream file;
  file << folder.str().c_str() << "/localFeatureNo1.bin";
  ofstream lf;
  lf.open(file.str().c_str(), ios::out | ios::binary);

  ///~ Fill h
  int cnt = 0;
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < M; j++)
    {
      if ((j < N) && (i < N))
      {
        h[j + M * i][0] = cnt++;
      }
      else
      {
        h[j + M * i][0] = 0;
      }
      h[j + M * i][1] = 0;
    }
  }
  ///~ Display h
  cout << "h = \n";
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < M; j++)
    {
      cout << h[j + M * i][0] << "\t";
    }
    cout << "\n";
  }
  ///~ Calculate fft2 of h
  fftw_execute(twoD); ///~ H contains the FT of h

  ///~ Calculate correlation of extracted
  corr.corr23DFT(h3, sz, H, H3, inverse);
  ///~ Calculate representation of extracted
  rep.imgRepLut(sz, h3,theta_1, theta_2, lut, lf, h_3);
  lf.close();
  ///~ Check representations
  ///~ Read file
  ifstream lfIn(file.str().c_str(), ios::in | ios::binary);

  cout << "\n";
  float in;
  for (int i = 0; i < theta_1 + 1; i++)
  {
    for (int j = 0; j < theta_2 + 1; j++)
    {
      lfIn.read((char*) &in, sizeof(float));
      cout << in << "\t";
    }
    cout << "\n";
  }
  lfIn.close();
}
