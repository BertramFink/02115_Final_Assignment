# RISC-V instruction simulator: Group 66

## Build
To build the simulator and tester use:
```
make
```

## Run
To run the simulator with desired RISC-V binary file use:
```
./simulator <path to file>
```

## Output
The binary dump of the registers will be outputted in "output.res"

## Running the tester
The tester can be used to run all RISC-V binaries in directory tests/
To run the tester use:
```
./tester
```

## Adding tests
Tests can be added in the tests/ directory. If a corresponding result file with extension .res is present, the simulator will automatically compare its results with the content of this file. 
