# Minimum-Vertex-Cover-Finder

In the realm graph theory, the Vertex Cover problem is a defined as find the smalled number of vertices that cover all the edges in any particular graph. The Vertex Cover problem is often described as a NP-Hard optimization problem, where only a solution to the instance of the problem can be verified in polynomial time. The program in this repository aims to solve the Vertex Cover using a SAT Solver and 2 Approximate Algorithms. An analysis was also conducted on the efficacy of these algorithms which can also be found in the written report, ```VC_Analysis.pdf```, in the repository.

## Overview 
As mentioned before, the program aims to solve the Vertex Cover problem in 2 ways, outlined as follows:
- Approximate Algorithms
  - Two approximate algorithms, Approx-VC-1 and Approx-VC-2, are used to approximate solutions for instances of the Vertex Cover problem within polynomial time. However, as evident by the word ”approximation”, they only provide estimated solutions to each instance. 
- Reduction from Vertex Cover to CNF-SAT
  - A polynomial time reduction is performed on one graphical instance, G, and transformed into a propositional logic formula, F, in Conjunctive Normal Form (CNF). If the formula is satisfiable, the minimum vertex cover can be re-constructed from the satisfiability assignment using a SAT Solver. The reduction is as follows: 
    <img src="CNFSAT Reduction.png" width=600>

Implementation details of these algorithms can be found in the ```solve_vertex_cover.cpp``` file.

## Getting Started

In order to run the program, firstly, you must clone the repository on your local machine. You can do this by running the following line on your machine's CLI:

```git clone https://github.com/SafiyaJan/Minimum-Vertex-Cover-Finder.git```

### Prerequisites

- You must run the program within a Linux environment, as the program uses libraries that could potentially not be supported by other operating system
- Ensure that you have the latest version of CMake installed on your local machine. You can checkout the following link to download CMake for your specific machine: https://cmake.org/download/
- You will also need a C++ compiler in order to compile the program. GCC and Clang are 2 such compilers and either can be used to compile the program
  - Installing Clang - https://clang.llvm.org/get_started.html
  - Installing GCC - https://gcc.gnu.org/wiki/InstallingGCC
- You also need to clone the MiniSat SAT Solver into your working directory. This can be done by running the following command under the same project directory:

  ``` git clone https://github.com/agurfinkel/minisat.git ```


## Usage 

### Building the Executable

Before, the program can be run, it first needs to be built (aka compiled). Run the following command to build the program on you CLI:
```
mkdir build && cd build && cmake ../ && make
```

### Running the Executable
After running the build sequence, the program can be run as follows under the ```/build``` folder:
```
./vertex_cover

# user input
V 10
E {<4,8>,<7,0>,<7,5>,<0,9>,<5,6>,<0,1>,<1,7>,<9,2>,<0,2>,<5,1>,<2,5>,<4,3>,<3,5>,<5,4>,<6,9>}

# program output that shows the Vertex Cover produced by the various algorithms
CNF-SAT-VC: 0,1,4,5,9         
APPROX-VC-1: 0,1,4,5,9
APPROX-VC-2: 0,1,2,3,4,5,7,9
CNF_SAT_VC Runtime - 37108.7 microsecs
Approx_VC1 Runtime - 165.024 microsecs
Approx_VC2 Runtime - 213.865 microsecs

```
Note - Ensure that you run the executable and the enter the user input in the same format as above.



