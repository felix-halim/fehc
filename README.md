FEHC - Fast and Effective Histogram Construction
------------------------------------------------

FEHC is a program to evaluate the efficiency and effectiveness of several 
histogram construction (or sequence segmentation) algorithms
as mentioned in these two papers:

(1). Felix Halim, Panagiotis Karras, Roland H. C. Yap: 
     Fast and effective histogram construction. CIKM 2009.

(2). Felix Halim, Panagiotis Karras, Roland H. C. Yap: 
     Local Search in Histogram Construction. AAAI 2010.



How to run the program
----------------------

1. Compile all the source codes
   Go to the folder where the "makefile" resides and type:

   make

   It will compile all the source codes and put in the executables
   in the bin folder

2. Run the algorithm by feeding the input to its standard input
   and specify the input parameter for the program.
   For example, to run the GDY algorithm on "winding" dataset, type:

   ./gdy_dp -B 16 -I 8 < data/winding1.in

   The available parameters to the program are:
   -B to specify how many buckets to generate (mandatory)
   -e to specify the epsilon for the AHistL program
   -I to specify the number of iterations for GDY_DP and GDY_BDP



LICENSE
-------

Copyright (C) 2009-2012 Felix Halim

FEHC is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 3 of the License.

FEHC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License <http://www.gnu.org/licenses/> for more details.
