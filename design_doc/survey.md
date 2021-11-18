Survey of Software

## Writing a Measurment Set

Done with casacore. This would use casacore arrays. The arrays here can take a pointer and an allocator with *[this constructor](https://casacore.github.io/casacore/classcasacore_1_1Array.html#ad772865a89355fef5c1b1a6c54052271)*

Output would be a measurement set written to disk.

## Flagging

Currently written in Python and uses numpy arrays; can be rewritten in C++ fairly easily.
In addition, calculations currently in Python might change depending on further science research.

Input and output are numpy arrays.

## Calibration Solve

Currently using a multitude of software. Most widely used is CASA: takes a measurement set (in disk) as an input and reads into casacore array. Output is also written to a measurement set.

Solving does not happen with the real-time pipeline, and there are calibration solvers that do not need to be run at all times of the day. This can be rewritten in C++ (with some hassle).

## Calibration Apply

Currently using CASA for this. It also takes measurement sets as an input and writes to measurement sets for output. Intermediate steps likely to use casacore arrays.

These calculations are well-defined and unchanging. Can be rewritten in C++.

## Gridding

The IDG software will call the following function to grid.

```
void gridding(const Plan& plan, const Array1D<float>& frequencies,
                const Array4D<std::complex<float>>& visibilities,
                const Array2D<UVW<float>>& uvw,
                const Array1D<std::pair<unsigned int, unsigned int>>& baselines,
                const Array4D<Matrix2x2<std::complex<float>>>& aterms,
                const Array1D<unsigned int>& aterms_offsets,
                const Array2D<float>& spheroidal);
```

The Object references that are passed in are defined in *[this header file](https://gitlab.com/astron-idg/idg/-/blob/master/idg-lib/src/common/ArrayTypes.h)*.

The output of this function is the spheroidal reference in the gridding function. Intermediate steps of this function go into the GPU.
