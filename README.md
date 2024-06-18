---
author: Pavel Perina
date: 2024-06-06
---

## Motivation

Experiments with [Ceres Solver](http://ceres-solver.org) and [Downhill simplex](https://github.com/botaojia/simplex) and basic evaluation.

## Compilation

Compilation of this project was done in Microsoft Visual Studio 2022 Community

`CMakeLists.txt` should download all dependencies from git repositories and compile them.
Good luck downloading them manually and specifying paths. Hopefully, I found ~~half~~
all of dependencies could be removed. All repos are relatively small, so it takes few seconds.

On my PC requires to copy
`c:\Program Files (x86)\Intel\oneAPI\mkl\2023.0.0\redist\intel64\mkl_intel_thread.2.dll`
into
`c:\dev-c\optim\out\build\x64-Clang-Debug\mkl_intel_thread.2.dll`
or similar directory, unless binary is run from MSVC.

Compilation works with both Microsoft C++ Compliler (`x64-Debug`) and Clang (`x64-Clang-Debug`) giving different warning messages
when something in templated code is wrong and it's higher chance that one of output can be parsed by us, humans.


## Example 1 

It tries to find function in form `y=A1*exp(-lambda1*x) + A2*exp(-lambda2*x)` that approximates predefined data (generated in Python).

### Output

```
c:\dev-c\optim\out\build\x64-Clang-Release>ceres_example_1.exe
```

```
iter      cost      cost_change  |gradient|   |step|    tr_ratio  tr_radius  ls_iter  iter_time  total_time
   0  1.723603e+01    0.00e+00    2.88e+01   0.00e+00   0.00e+00  1.00e+04        0    5.49e-05    2.00e-04
   1  9.108901e+00    8.13e+00    1.90e+01   0.00e+00   1.00e+00  3.00e+04        1    1.15e-04    6.94e-04
   2  3.094324e+00    6.01e+00    4.22e+00   6.56e-01   1.18e+00  9.00e+04        1    3.11e-05    8.06e-04
   3  1.320764e+00    1.77e+00    7.95e-01   5.97e-01   1.16e+00  2.70e+05        1    2.61e-05    8.82e-04
   4  1.168544e+00    1.52e-01    4.56e-01   2.04e-01   1.26e+00  8.10e+05        1    3.09e-05    9.98e-04
   5  1.064969e+00    1.04e-01    3.59e+00   3.12e-01   1.07e-01  5.45e+05        1    2.22e-04    1.28e-03
   6  1.026700e+00    3.83e-02    3.98e+00   1.70e-01   3.94e-02  3.06e+05        1    3.81e-05    1.37e-03
   7  9.550805e-01    7.16e-02    4.95e+00   3.22e-01   7.55e-02  1.90e+05        1    3.63e-05    1.47e-03
   8  8.337797e-01    1.21e-01    1.17e+01   2.17e+00   1.35e-01  1.37e+05        1    2.56e-05    1.54e-03
   9  5.450557e-01    2.89e-01    5.07e+00   1.80e+00   3.70e-01  1.34e+05        1    3.58e-05    1.62e-03
  10  5.388057e-02    4.91e-01    2.08e-01   7.08e-01   9.93e-01  4.03e+05        1    2.56e-05    1.69e-03
  11  5.213641e-02    1.74e-03    4.81e-03   1.20e-01   9.44e-01  1.21e+06        1    2.56e-05    1.76e-03
  12  5.211361e-02    2.28e-05    2.12e-04   1.49e-02   9.25e-01  3.12e+06        1    2.51e-05    1.83e-03
  13  5.211347e-02    1.38e-07    2.58e-05   1.28e-03   9.14e-01  7.20e+06        1    2.51e-05    1.90e-03
C:\dev-c\optim\out\build\x64-Clang-Release\_deps\ceres-src\internal\ceres\trust_region_minimizer.cc:758 Terminating: Function tolerance reached. |cost_change|/cost: 2.017492e-08 <= 1.000000e-06
Ceres Solver Report: Iterations: 14, Initial cost: 1.723603e+01, Final cost: 5.211347e-02, Termination: CONVERGENCE
Final solution:
    y = 0.959354*exp(-0.195071*x) + 2.23905*exp(-1.89224*x)
```

Note: starting condition was changed later to match example 3.

## Example 2

It tries to find minimum of a function that is (squared,) positive distance from a point, starting at zero.

Problems:
* Without limiting A>0, lambda<0 there are more steps, some of them return infinity
* Function was intentionally done with double arguments and result. Automatic differentiation does not work, numeric one used. This is the case with all black-box functions embedded in some library.

### Output
```
c:\dev-c\optim\out\build\x64-Release>ceres_example_2.exe
```

```
   0: f: 1.580642e+06 d: 0.00e+00 g: 1.49e+05 h: 0.00e+00 s: 0.00e+00 e:  0 it: 3.49e-05 tt: 1.35e-04
   1: f: 5.330761e+05 d: 1.05e+06 g: 6.61e+04 h: 1.00e+01 s: 6.70e-05 e:  2 it: 6.51e-05 tt: 5.38e-04
   2: f: 1.707265e+05 d: 3.62e+05 g: 2.81e+04 h: 7.97e+00 s: 1.00e+00 e:  1 it: 1.48e-05 tt: 6.22e-04
   3: f: 5.583508e+04 d: 1.15e+05 g: 1.22e+04 h: 5.91e+00 s: 1.00e+00 e:  1 it: 5.30e-06 tt: 6.86e-04
...
  22: f: 5.000174e-01 d: 9.92e-06 g: 7.81e-03 h: 1.26e-03 s: 1.00e+00 e:  1 it: 4.60e-06 tt: 1.78e-03
  23: f: 5.000034e-01 d: 1.40e-05 g: 3.18e-03 h: 2.41e-03 s: 6.39e-01 e:  1 it: 5.00e-06 tt: 1.84e-03
  24: f: 5.000000e-01 d: 3.37e-06 g: 2.83e-04 h: 1.79e-03 s: 1.00e+00 e:  1 it: 4.40e-06 tt: 1.89e-03
C:\dev-c\optim\out\build\x64-Release\_deps\ceres-src\internal\ceres\line_search_minimizer.cc:468 Terminating: Function tolerance reached. |cost_change|/cost: 4.559134e-08 <= 1.000000e-06
Ceres Solver Report: Iterations: 25, Initial cost: 1.580642e+06, Final cost: 5.000000e-01, Termination: CONVERGENCE
Final solution:
    -1.99996
  2.9999
      42
```

## Example 3

The same task as example 1, but with numeric differentiation. 
Problems:
* Limits did not work
* Unclear why trust region does not work
* ~~`ceres::LineSearchDirectionType::NONLINEAR_CONJUGATE_GRADIENT;` is actually only method that seem to work~~

### Output

```
   0: f: 6.831329e+04 d: 0.00e+00 g: 1.74e+06 h: 0.00e+00 s: 0.00e+00 e:  0 it: 4.69e-05 tt: 1.43e-04
   1: f: 1.353429e+01 d: 6.83e+04 g: 1.50e+02 h: 1.00e+00 s: 5.76e-07 e:  1 it: 2.29e-05 tt: 5.10e-04
   2: f: 5.827367e+00 d: 7.71e+00 g: 2.70e+01 h: 7.57e-01 s: 4.91e-03 e:  5 it: 8.17e-05 tt: 6.47e-04
   3: f: 1.639450e+00 d: 4.19e+00 g: 2.13e+01 h: 6.06e-01 s: 4.08e-02 e:  2 it: 1.77e-05 tt: 7.12e-04
...
  39: f: 5.431639e-03 d: 6.10e-08 g: 1.27e-04 h: 9.25e-04 s: 1.00e+00 e:  1 it: 9.70e-06 tt: 5.36e-03
  40: f: 5.431628e-03 d: 1.14e-08 g: 2.51e-05 h: 2.70e-04 s: 1.00e+00 e:  1 it: 8.90e-06 tt: 5.68e-03
C:\dev-c\optim\out\build\x64-Release\_deps\ceres-src\internal\ceres\line_search_minimizer.cc:468 Terminating: Function tolerance reached. |cost_change|/cost: 1.255704e-07 <= 1.000000e-06
Ceres Solver Report: Iterations: 41, Initial cost: 6.831329e+04, Final cost: 5.431627e-03, Termination: CONVERGENCE
Final solution:
    y = 2.23903*exp(-1.89234*x) + 0.959393*exp(-0.195078*x)
Number of calls: 468
```

## Example 4

Using good, old [simplex method](https://github.com/botaojia/simplex) which has roughly 150 lines of code.
This took 377 iterations (with really tight tolerance), but it took 9.4e-04s.
For some reason, cost reported by ceres solver is this cost squared and divided by two. `0.104227^2/2=0.00543163`

```
Initial cost = 369.63
Duration = 0.0009437s
Final cost = 0.104227
Final solution:
    y = 2.23902*exp(-1.89233*x) + 0.959393*exp(-0.195078*x)
Number of calls: 2517
```

## Summary

Pros:

* It should be fast, state-of-the-art library
* Reference guide with few examples
* Dependencies on gtest, google-benchmark and glog are optional

Cons:

* Not many examples
* Heavily templated code -> in short, blackbox
* Takes about minute to compile (and Ryzen 5900X is not slow)
* Hopefully optional dependencies on google tests, google benchmark, glog (google logging)
* For unknown reason, loads `mkl_intel_thread.2.dll` from Intel oneAPI toolkit (math kernel library which is installed on my PC(s)

Compared to simplex:

In this case, simplex method is faster, but it calls evaluation function more often. If evaluation is more expensive, using ceres solver could be much faster.


