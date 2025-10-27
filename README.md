# RK12(10) Integrator (CMake/C++)

C++ implementation of the Feagin RK12(10) adaptive integrator with optional C API compatibility. The project is configured with CMake and provides:

- C arrays API via `rk1210(double t0, double tf, double* dt, int nout, double x0[], double h, double hmax, double tol, int printout, int fixedstep, derivFunc deriv, double* output)` declared in `include/rk1210.hpp`.
- Vector-based overloads for C++ clients (wrapped internally).
- Robust interpolation without relying on globals and bounds checks using `nout`.

## Build

```bash
# from repository root
mkdir -p build
cd build
cmake ..
cmake --build . -j
```

## Run

```bash
# from build directory
./rk1210_test
```

This writes `rk1210Out.txt` to the repo root by default.

## Notes
- Prefer including `include/rk1210.hpp` in C++ sources; avoid including the legacy C header.
- `.gitignore` excludes build artifacts and generated outputs.
