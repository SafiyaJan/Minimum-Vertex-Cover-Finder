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

## Usage 

### Building the Executable

Before, the program can be run, it first needs to be built (aka compiled). Run the following command to build the program on you CLI:
```
mkdir build && cd build && cmake ../ && make
