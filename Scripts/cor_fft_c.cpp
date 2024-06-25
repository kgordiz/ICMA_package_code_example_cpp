#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <math.h>
#include <fftw3.h>

using namespace std;

int main()
{
  int N,t,Noutput,i,j,Neq,L;
  double sum,aveHeat,**corArray,temp;
  double *in_Q1,*in_Q4,*out_cor1,*out_cor4;
  fftw_complex *out_Q1,*out_Q4,*sp14,*sp44;
  fftw_plan my_plan1,my_plan4,my_plan_i1,my_plan_i4;

  char cwd[1024];
  char filename_in[64];
  char filename_out[64];
  char address_in[256];
  char address_out[256];

  if (getcwd(cwd, sizeof(cwd)) != NULL)

  Neq = 240000;
  L= Replacethis;
  N = L-Neq;
  Noutput = 100001;

  corArray = (double**) malloc(Noutput*sizeof(double*));  
  for (int t = 0; t < Noutput; t++)  
     corArray[t] = (double*) malloc(2*sizeof(double)); 

  in_Q1 = (double*) malloc(N*sizeof(double));
  in_Q4 = (double*) malloc(N*sizeof(double));

  out_cor1 = (double*) malloc(N*sizeof(double));
  out_cor4 = (double*) malloc(N*sizeof(double));
  
  out_Q1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
  out_Q4 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);

  sp14 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
  sp44 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);

  my_plan1 = fftw_plan_dft_r2c_1d(N, in_Q1, out_Q1, FFTW_ESTIMATE);
  my_plan4 = fftw_plan_dft_r2c_1d(N, in_Q4, out_Q4, FFTW_ESTIMATE);

  my_plan_i1 = fftw_plan_dft_c2r_1d(N, sp14, out_cor1, FFTW_ESTIMATE);
  my_plan_i4 = fftw_plan_dft_c2r_1d(N, sp44, out_cor4, FFTW_ESTIMATE);

  sprintf (filename_in, "Q.txt");
  sprintf (address_in, "%s/%s",cwd,filename_in);
  ifstream readfile(address_in);
  readfile.precision(20);

  for (t=0;t<Neq;t++){  
    readfile>>temp;
    readfile>>temp;
  }

  for (t=0;t<N;t++){  
    readfile>>in_Q1[t];
    readfile>>in_Q4[t];
  }

  sum = 0;
  for (t = 0 ; t < N ; t++) sum += in_Q1[t];
  aveHeat = sum/(N*1.);
  for (t = 0 ; t < N ; t++) in_Q1[t] -= aveHeat;

  sum = 0;
  for (t = 0 ; t < N ; t++) sum += in_Q4[t];
  aveHeat = sum/(N*1.);
  for (t = 0 ; t < N ; t++) in_Q4[t] -= aveHeat;

  fftw_execute(my_plan1); 
  fftw_execute(my_plan4);

  for (t=0;t<N;t++){
    sp14[t][0] = out_Q1[t][0]*out_Q4[t][0] + out_Q1[t][1]*out_Q4[t][1];
    sp14[t][1] = out_Q1[t][0]*out_Q4[t][1] - out_Q1[t][1]*out_Q4[t][0];

    sp44[t][0] = out_Q4[t][0]*out_Q4[t][0] + out_Q4[t][1]*out_Q4[t][1];
    sp44[t][1] = -out_Q4[t][0]*out_Q4[t][1] + out_Q4[t][1]*out_Q4[t][0];
  }  
    
  fftw_execute(my_plan_i1); 
  fftw_execute(my_plan_i4);

  for (t=0;t<Noutput;t++){  
    corArray[t][0] = out_cor1[t];
    corArray[t][1] = out_cor4[t];
  }
  readfile.close();

  fftw_destroy_plan(my_plan1);
  fftw_destroy_plan(my_plan4);
  fftw_destroy_plan(my_plan_i1);
  fftw_destroy_plan(my_plan_i4);

  fftw_free(in_Q1);
  fftw_free(in_Q4);
  fftw_free(out_Q1);
  fftw_free(out_Q4);
  fftw_free(sp14);
  fftw_free(sp44);
  fftw_free(out_cor1);
  fftw_free(out_cor4);

  sprintf (filename_out, "cordata.txt");
  sprintf (address_out, "%s/%s",cwd,filename_out);
  ofstream outfile(address_out);
  outfile.precision(20);

  for (t=0;t<Noutput;t++){
    for (j=0;j<=1;j++){
      outfile<<corArray[t][j]/(1.*N*N)<<" ";}
    outfile<<endl;
  }

  outfile.close();
  return 1;
}
