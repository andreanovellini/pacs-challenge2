# A hierarchy of solvers for f(x)=0

This program implements few methods for computing the zero of a function in a hierarchical structure.

## Build 

To build the executable is enough to run `make` inside the folder.

## Run 

To run the solver with default parameters is enough to run the created executable, by running `./main` from inside the folder.

Example of execution:
`./main method=MethodName -f data.dat`.

To change the parameters it is possible to modify the `data.dat` file with the desired parameters. 
Another option is to create a new `.dat` file and passing it from command line thanks to GetPot usinge the `-f` or `--file` option.
The available methods are:
- Regula Falsi -> MethodName: `RegulaFalsi`
- Bisection    -> MethodName: `Bisection`
- Secant       -> MethodName: `Secant`
- Brent        -> MethodName: `Brent`
- Newton       -> MethodName: `Newton`
- Quasi Newton -> MethodName: `QuasiNewton`
