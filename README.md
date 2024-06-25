# ICMA_package_code_example_cpp


This folder contains the required files for a sample interface conductance modal analysis (ICMA) calculation. The provided example is for ICMA calculation across the interface between crystalline Si and amorphous Ge. In the ICMA_lammps_related folder you will find:

1) The tersoff.cpp and tersoff.h are intended for LAMMPS. Recompile LAMMPS using these files. As explained in the manuscript, because of periodic boundary conditions we have two interfaces. We only include the middle interface in the calculations. To do so, 4 types of atoms are introduced in the system: 2 types belonging to Si and 2 types belonging to Ge. The interface that we want to do the calculations for is between types 2 (Si) and 3 (Ge). This is the reason that you see a condition in the tersoff.cpp code that only allows the interactions between types 2 and 3 to be accounted for in the calculations.

2) There is a file called init.txt that includes some information to be input to the program, like how frequent you want the modal analysis be calculated and output, or the number of atoms in the system.

3) ev_real.txt includes the eigen vectors for the modes that you want to do the calculations for. For this example, there are 4 modes included in the calculations. Each column is an eigen vector. N atoms in the system, hence there are 3*N numbers in each column. Usually, we submit multiple simulation jobs (in different folders) and each job deals with a different group of modes. Based on our experience, if you put all the modes in one folder, the calculations can get a very long time.

Performing
```make```
in the "post_processing_code_cpp" folder will compile the code. to run you can execute:
```./my_program```
