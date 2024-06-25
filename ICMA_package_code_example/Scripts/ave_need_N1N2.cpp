#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <math.h>

using namespace std;

int main(){
  int i,j,t,Npackage,N1,N2,np;
  double temp,*sum_area;

  char cwd[1024];
  char filename_in[64];
  char filename_out[64];
  char address_in[256];
  char address_out[256];

  if (getcwd(cwd, sizeof(cwd)) != NULL)

  N1 = 0;
  N2 = 50;
  Npackage = 220;

  sum_area = new double [2];

  sprintf (address_out, "%s/%s",cwd,"averaged_N1N2.txt");        //////////////////////
  ofstream writefile(address_out);         //////////////////////

  for (np=1;np<=Npackage;np++){

    sum_area[0] = sum_area[1] = 0.0;

    sprintf (filename_in, "area_evol_CCs.txt");
    sprintf (address_in, "%s/p%d/%s",cwd,np,filename_in);
    ifstream readfile(address_in);
    readfile.precision(20);

    for (i=0;i<N1*1000;i++){
      readfile>>temp;              //time
      readfile>>temp;
      readfile>>temp;              //Q_net
    }
    
    if (np != Npackage){
      for (i=0;i<(N2-N1)*1000;i++){
        readfile>>temp;
        readfile>>temp;
        sum_area[0] += temp;
        readfile>>temp;
      }
      writefile<<sum_area[0]/((N2-N1)*1000)<<endl;
    }

    if (np == Npackage){
      for (i=0;i<(N2-N1)*1000;i++){
        readfile>>temp;
        readfile>>temp;
        sum_area[0] += temp;
        readfile>>temp;
        sum_area[1] += temp;
      }
      writefile<<sum_area[0]/((N2-N1)*1000)<<endl;
      writefile<<sum_area[1]/((N2-N1)*1000)<<endl;
    }

    readfile.close();
  }

  writefile.close();
  return 78;
}

