# Robust Critical Point Detection v1.0.1

Copyright (c) 2016, Lawrence Livermore National Security, LLC.

Produced at the Lawrence Livermore National Laboratory.

Written by Harsh Bhatia (bhatia4@llnl.gov).

CODE-701040.

All rights reserved.

### Purpose & Scope

`Robust Critical Point Detection` is a command line utility to compute critical points in a 2D or 3D vector field robustly. The technique was published in 2014 [1], and uses a numerically robust approach for detecting whether a singularity exists in a simplex. The code assumes piecewise-linear interpolation in a simplex. Built upon Simulation of Simplicity [2], the code naturally resolves degenerate cases.

[1] H. Bhatia, A. Gyulassy, H. Wang, P.-T. Bremer, and V. Pascucci. Robust Detection of Singularities in Vector Fields. In Topological Methods in Data Analysis and Visualization III - Theory, Algorithms, and Applications, Mathematics and Visualization, pp. 3–18, P-T Bremer, I Hotz, V Pascucci, R Peikert (Eds.), Springer Berlin Heidelberg, 2014.

[2] H. Edelsbrunner, E.P. Mücke, Simulation of simplicity: a technique to cope with degenerate
cases in geometric algorithms. ACM Trans. Graph. 9, (1990), pp. 66–104.

### Dependencies

The software depends upon `Simulation of Simplicity`. The code for SoS was provided by Ernst Mücke (<http://www.geom.uiuc.edu/software/cglist/GeomDir/>) as a sub-module for a different software. The original SoS code was written in C in the 1990's, and requires some modifications for compilation with modern C++ compilers. A **patch** has been provided to fix the code and build it (`patch_SOS.txt`)

The SoS and the patch can be automatically installed using the `install_deps.sh` script.

### Installation & Execution

The CP detection tool can be installed by first building `SimulationOfSimplicity` as a static library and then using the cmake system.

```
$ pwd
RobustCriticalPointDetection

$ sh install_deps.sh
$ mkdir build && cd build
$ cmake ../
$ make
```

The tool can be run in one of the following two ways.

```
$ ./CriticalPointDetection file1 X Y Z

 where,
   file1 is a text file where each line is: x y z vx vy vz (coordinates of points and corresponding vectors)
   X Y Z are the dimensions of the regular grid (program creates tets automatically)
```

or 

```
$ ./CriticalPointDetection file1 file2

 where,
   file1 is a text file where each line is: x y z vx vy vz (coordinates of points and corresponding vectors)
   file2 is a text file where each line is: i1 i2 i3 i4 (indices of the 4 corners of a tet)
```

The code is easy to extend for a variety of data formats. The code only needs the vetor field and tetrahedra. Please see the main function to write custom input/output formats.
