import math 
import shutil
import os

def copyFile(src, dest):
    try:
        shutil.copy(src, dest)
    except shutil.Error as e:
        print('Error: %s' % e)
    except IOError as e:
        print('Error: %s' % e.strerror)


here_root=os.getcwd()


for i in range(1, 221):   #bayad ta 121 beri baraye 2 barabar.

  strng_dofolder="%s%.2f" %(here_root+'/dofolder',i)
  os.makedirs(strng_dofolder)
  os.chdir(strng_dofolder)

  here2=os.getcwd()

  add1 = here_root+'/w.txt'
  add2 = here2
  copyFile(add1,add2)

  add1 = here_root+'/calc_go_contour_source.cpp'
  add2 = here2+'/calc_go_contour.cpp'
  copyFile(add1,add2)

  add1 = here_root+'/job_aout.pbs'
  add2 = here2+'/job_aout.pbs'
  copyFile(add1,add2)

  with open(here2+'/calc_go_contour.cpp', 'r') as file:
      data = file.readlines()
      data[116] = '  sprintf (filename_out, "contourkia{}.txt");\n'.format(i)
      data[121] = '  for (i={};i<{};i++)'.format(i-1,i)+'{\n'
      data[181] = '  for (i={};i<{};i++)'.format(i-1,i)+'{\n'

  with open(here2+'/calc_go_contour.cpp', 'w') as file:
      file.writelines( data )

  with open(here2+'/job_aout.pbs', 'r') as file:
      data = file.readlines()
      data[0] = '#PBS -N corcalc{}\n'.format(i)

  with open(here2+'/job_aout.pbs', 'w') as file:
      file.writelines( data )

  os.system("g++ calc_go_contour.cpp -lfftw3")
  os.system("qsub job_aout.pbs")


