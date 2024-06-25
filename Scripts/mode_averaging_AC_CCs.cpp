#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <math.h>

using namespace std;

int main(){
  int i,j,t,Natoms,Nv,L;
  double AI,kB,temperature,evps2Js_pwr2,temp,**cor_data_ave,*Area_Q_cor,dt,prefactor;

  char cwd[1024];
  char filename_in[64];
  char filename_out[64];
  char address_in[256];
  char address_out[256];

  double eV2J = 1.60217646e-19;
  double A2m = 1e-10;
  double ps2s = 1e-12;

  if (getcwd(cwd, sizeof(cwd)) != NULL)

  AI = pow(10.86 ,2)*pow(10,-20);
  evps2Js_pwr2 = ((eV2J/A2m)*A2m/ps2s)*((eV2J/A2m)*A2m/ps2s);	// eV2J/A2m : -dU/dr = Force & A2m/ps2s : velocity
  kB = 1.3806 * pow(10,-23);					// J/K
  temperature = 320;						// K

  Nv = 1;

  dt = 2.5*pow(10,-15);     //s
  L = 100001;

  prefactor = evps2Js_pwr2*dt*1./(kB*temperature*temperature*AI);

  cor_data_ave = new double* [L];
  for (i=0;i<L;i++) cor_data_ave[i] = new double [2];

  Area_Q_cor = new double [2];

 for (t=0;t<L;t++) {
   for (i=0;i<=1;i++){
      cor_data_ave[t][i] = 0.0;
    }
  }

  for (i=1;i<=Nv;i++){
    sprintf (filename_in, "cordata.txt");
    sprintf (address_in, "%s/v%d/%s",cwd,i,filename_in);
    ifstream readfile(address_in);
    readfile.precision(20);

    for (t=0;t<L;t++){
      for (j=0;j<=1;j++){
        readfile>>temp;
        cor_data_ave[t][j] += temp;
      }
    }
    readfile.close();
  }

  for (t=0;t<L;t++){
    for (j=0;j<=1;j++){
    cor_data_ave[t][j] *= 1./Nv;
    }
  }

  sprintf (address_out, "%s/%s",cwd,"cordata_CCs.txt");         //////////////////////
  ofstream writefile_cor_ave(address_out);         //////////////////////

  sprintf (address_out, "%s/%s",cwd,"area_evol_CCs.txt");         //////////////////////
  ofstream writefile_area_ave(address_out);         //////////////////////
  
  for (j=0;j<=1;j++){
    Area_Q_cor[j] = 0.0;
  }

  for (t=0;t<L;t++){
    for (j=0;j<=1;j++){ 
      Area_Q_cor[j] += cor_data_ave[t][j];
    }
    writefile_cor_ave<<t*dt*pow(10,12);
    for (j=0;j<=1;j++){
      writefile_cor_ave<<" "<<cor_data_ave[t][j];
    }
    writefile_cor_ave<<endl;

    writefile_area_ave<<t*dt*pow(10,12);
    for (j=0;j<=1;j++){
      writefile_area_ave<<" "<<prefactor*Area_Q_cor[j];
    }
    writefile_area_ave<<endl;
  }

  writefile_cor_ave.close();
  writefile_area_ave.close();
  return 78;
}

