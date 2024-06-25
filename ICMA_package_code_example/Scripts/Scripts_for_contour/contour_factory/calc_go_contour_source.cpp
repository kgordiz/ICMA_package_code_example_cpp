#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <math.h>
#include <fftw3.h>

using namespace std;

int main()
{
  int i,j,t,np,N1,N2,N,Nbins,Nv,Nmodes,Noutput,Nmodes_eachfile,total_modes,ihi1,jhi1,Neq;
  double sum,aveHeat,*freq,**Q,temp,*in_Q1,*in_Q2,*out_cor12,*out_cor21,ps2s,A2m,eV2J,lat_cnst,AI,evps2Js_pwr2,kB,temperature,dt,prefactor,Area_cor12,Area_cor21,**cor_map;
  fftw_complex *out_Q1,*out_Q2,*sp12,*sp21;
  fftw_plan my_plan1,my_plan2,my_plan_i1,my_plan_i2;

  char cwd[1024];
  char filename_in[64];
  char filename_in_freq[64];
  char filename_out[64];
  char filename_out2[64];
  char address_in[256];
  char address_in_freq[256];
  char address_out[256];
  char address_out2[256];

  if (getcwd(cwd, sizeof(cwd)) != NULL)

  Neq = 240000;
  N = 2240010-Neq;
  Nbins = 220;
  //Noutput = 100001;

  N1 = 0;
  N2 = 50;
/////////////////////////////////////
  eV2J = 1.60217646e-19;
  A2m = 1e-10;
  ps2s = 1e-12;
  evps2Js_pwr2 = ((eV2J/A2m)*A2m/ps2s)*((eV2J/A2m)*A2m/ps2s);
  kB = 1.3806 * pow(10,-23);
  AI = pow(10.86,2)*pow(10,-20);
  temperature = 320;
  dt = 2.5*pow(10,-15);
  prefactor = evps2Js_pwr2*dt*1./(kB*temperature*temperature*AI);
/////////////////////////////////////

  Q = (double**) malloc(N*sizeof(double*));  
  for (int t = 0; t < N; t++)
    Q[t] = (double*) malloc(Nbins*sizeof(double));

  cor_map = (double**) malloc(Nbins*sizeof(double*));  
  for (int t = 0; t < Nbins; t++)  
    cor_map[t] = (double*) malloc(Nbins*sizeof(double));

  freq = (double*) malloc(Nbins*sizeof(double));

  in_Q1 = (double*) malloc(N*sizeof(double));
  in_Q2 = (double*) malloc(N*sizeof(double));

  out_cor12 = (double*) malloc(N*sizeof(double));
  out_cor21 = (double*) malloc(N*sizeof(double));

  out_Q1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);	
  out_Q2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);

  sp12 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
  sp21 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);

  my_plan1 = fftw_plan_dft_r2c_1d(N, in_Q1, out_Q1, FFTW_ESTIMATE);
  my_plan2 = fftw_plan_dft_r2c_1d(N, in_Q2, out_Q2, FFTW_ESTIMATE);

  my_plan_i1 = fftw_plan_dft_c2r_1d(N, sp12, out_cor12, FFTW_ESTIMATE);
  my_plan_i2 = fftw_plan_dft_c2r_1d(N, sp21, out_cor21, FFTW_ESTIMATE);

  sprintf (filename_in_freq, "w.txt");
  sprintf (address_in_freq, "%s/%s",cwd,filename_in_freq);
  ifstream readfile_freq(address_in_freq);
  readfile_freq.precision(20);

  for (np=0;np<Nbins;np++){
    readfile_freq>>freq[np];
    //freq[np] *= 0.03;    //convert cm^-1 to THz
  }

  readfile_freq.close();

//////readfile
  sprintf (filename_in, "../Qaveraged_bins.txt");
  sprintf (address_in, "%s/%s",cwd,filename_in);
  ifstream readfile(address_in);
  readfile.precision(20);

  for (t=0;t<Neq;t++)
    for (np=0;np<=Nbins;np++)
      readfile>>temp;

  for (t=0;t<N;t++){
    for (np=0;np<Nbins;np++)
      readfile>>Q[t][np];
    readfile>>temp;
  }

  readfile.close();

cout<<"Kia!!!!"<<endl;

////////////////////////////////////////
  for (np=0;np<Nbins;np++){
    sum = 0;
    for (t=0;t<N;t++) sum += Q[t][np];
    aveHeat = sum/(N*1.);
    for (t=0;t<N;t++) Q[t][np] -= aveHeat;
  }
////////////////////////////////////////

  sprintf (filename_out, "contour.txt");
  sprintf (address_out, "%s/%s",cwd,filename_out);
  ofstream outfile(address_out);
  outfile.precision(20);

  for (i=0;i<Nbins;i++){
    for (j=0;j<Nbins;j++){
      /////////////for Q1->i & Q2->j
      for (t=0;t<N;t++){
        in_Q1[t] = Q[t][i];
        in_Q2[t] = Q[t][j];
      }

      fftw_execute(my_plan1);
      fftw_execute(my_plan2);

      for (t=0;t<N;t++){
        sp12[t][0] = out_Q1[t][0]*out_Q2[t][0] + out_Q1[t][1]*out_Q2[t][1];
        sp12[t][1] = out_Q1[t][0]*out_Q2[t][1] - out_Q1[t][1]*out_Q2[t][0];

        sp21[t][0] = out_Q2[t][0]*out_Q1[t][0] + out_Q2[t][1]*out_Q1[t][1];
        sp21[t][1] = out_Q2[t][0]*out_Q1[t][1] - out_Q2[t][1]*out_Q1[t][0];
      }

      fftw_execute(my_plan_i1);
      fftw_execute(my_plan_i2);

      Area_cor12 = 0;
      Area_cor21 = 0;
      for (t=N1*1000;t<N2*1000;t++){
        Area_cor12 += out_cor12[t]/(1.*N*N);
        Area_cor21 += out_cor21[t]/(1.*N*N);
      }
      cor_map[i][j]=prefactor*(Area_cor12+Area_cor21)/2;

      /////////////for Q2->i & Q1->j
      for (t=0;t<N;t++){
        in_Q1[t] = Q[t][j];
        in_Q2[t] = Q[t][i];
      }

      fftw_execute(my_plan1);
      fftw_execute(my_plan2);

      for (t=0;t<N;t++){
        sp12[t][0] = out_Q1[t][0]*out_Q2[t][0] + out_Q1[t][1]*out_Q2[t][1];
        sp12[t][1] = out_Q1[t][0]*out_Q2[t][1] - out_Q1[t][1]*out_Q2[t][0];

        sp21[t][0] = out_Q2[t][0]*out_Q1[t][0] + out_Q2[t][1]*out_Q1[t][1];
        sp21[t][1] = out_Q2[t][0]*out_Q1[t][1] - out_Q2[t][1]*out_Q1[t][0];
      }

      fftw_execute(my_plan_i1);
      fftw_execute(my_plan_i2);

      Area_cor12 = 0;
      Area_cor21 = 0;
      for (t=N1*1000;t<N2*1000;t++){
        Area_cor12 += out_cor12[t]/(1.*N*N);
        Area_cor21 += out_cor21[t]/(1.*N*N);
      }
      cor_map[j][i]=prefactor*(Area_cor12+Area_cor21)/2;      
    }  //end i
  }  //end j

  for (i=0;i<Nbins;i++){
    for (j=0;j<Nbins;j++){
      outfile<<freq[j]<<" "<<freq[i]<<" "<<(cor_map[i][j]+cor_map[j][i])/2<<endl;
    }
  }

  fftw_destroy_plan(my_plan1);
  fftw_destroy_plan(my_plan2);
  fftw_destroy_plan(my_plan_i1);
  fftw_destroy_plan(my_plan_i2);

  fftw_free(in_Q1);
  fftw_free(in_Q2);
  fftw_free(out_Q1);
  fftw_free(out_Q2);
  fftw_free(sp12);
  fftw_free(sp21);
  fftw_free(out_cor12);
  fftw_free(out_cor21);

  outfile.close();
  return 0;
}

