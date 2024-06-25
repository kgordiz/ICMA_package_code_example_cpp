#include "Q_average_over_ensembles.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <math.h>

using namespace std;

int main()
{
  int t,nv,np,N,Nbins,Nv;
  double **Q,temp;

  char cwd[1024];
  char filename_in[64];
  //char filename_out[64];
  char address_in[256];
  char address_out[256];

  if (getcwd(cwd, sizeof(cwd)) != NULL)

  N = 2240010;
  Nbins = 220;
  Nv = 1;

/////////////////////////////////////

  Q = (double**) malloc(N*sizeof(double*));  
  for (int t = 0; t < N; t++)  
    Q[t] = (double*) malloc((Nbins+1)*sizeof(double));

  for (t=0;t<N;t++)
    for (np=0;np<=Nbins;np++)
      Q[t][np] = 0.0;

  for (np=1;np<=Nbins;np++){
    for (nv=1;nv<=Nv;nv++){
      sprintf (filename_in, "Q.txt");
      sprintf (address_in, "%s/p%d/v%d/%s",cwd,np,nv,filename_in);
      ifstream readfile(address_in);
      readfile.precision(20);
      for (t=0;t<N;t++){
        readfile>>temp;
        Q[t][np-1] += temp;
        if (np==1){
          readfile>>temp;
          Q[t][Nbins] += temp;
        }
        else readfile>>temp;
      }
      readfile.close();
    }
  }

  for (t=0;t<N;t++)
    for (np=0;np<=Nbins;np++)
      Q[t][np] *= 1./Nv;

  sprintf (address_out, "%s/Qaveraged_bins.txt",cwd);
  ofstream outfile(address_out);
  outfile.precision(8);

  for (t=0;t<N;t++){
    for (np=0;np<=Nbins;np++){
      outfile<<Q[t][np]<<" ";
    }
    outfile<<endl;
  }
      
  outfile.close();
  outfile.clear();

  return 0;
}

