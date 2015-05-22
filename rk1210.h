/*

  rk1210.h
  Austin Probe
  21/05/2015
  Function declarations for rk1210 numerical propagator.

*/

#ifndef _RK1210_H_
#define _RK1210_H_

// Project include files
#include "../egm2008_c/EGM2008.h"



// Define maximum sizes of MCPI arrays
#ifndef M_PI
  #define M_PI 3.14159265359 // Pi
#endif
#define MaxN 150          // Maximum order of Chebyshev approximation
#define MaxM 151          // Used to allocate matrices - order increases on integration
#define Nstates 6        // Num states - hack for now until STM is added
#define MaxOutPts 300000 // Max number of ephemeris output points
#define f_num 3          // Mandate that we split orbits into "f_num" segements
#define MaxNR 100        // Max number of radius sample points in gravity surface
#define MaxNT 15         // Max number of tolerance sample point in grav surface
#define NumN 36          // Length of Nvec - number of orders of approximation to be used
#define LG 13            // Length of gravity vector in Napprox surface
#define LP 50            // Length of perigee vector in Napprox surface
#define LE 37            // Length of eccentricity vector in Napprox surface
#define ERad 6378.137    // Radius of the Earth in your choice of coordinates
#define CTU 1 // 13.446735833 // TU Conversion Factor
#define MU 3.9857440509E5 // Gravitational Constant
#define DEBUG 0

//  The derivatives are defined in this procedure (see "rk1210" procedure below for more information)
void deriv(double x[], double t, double f[]);


// This procedure performs an integration of the differential equations given in procedure "deriv"
//             using a variable-step Runge-Kutta method known as RK12(10) developed by T. Feagin in 2006
//             The input and output variables are described below:
// t0:         The initial value of t for the integration
// tf:         The final value of t for the integration
// x0[]:       The array of initial values of x and upon return, the final values of x
// h:          An initial, guessed or given stepsize
// hmax:       The largest stepsize to be permitted
// tol:        The tolerance (or desired upper bound) on the local truncation error
// printout:   Set to one or two if call to outputSolution() from within rk1210 is desired, otherwise zero
//             If printout is two, outputSolution() will be able to call interpolate() to get intermediate values
// fixedstep:  Set to one if fixed step integration is desired, otherwise zero
void rk1210(double t0, double tf, double * dt, double x0[], double h, double hmax,
double tol, int printout, int fixedstep, double * output);


// t:          The value of the independent variable where you want an approximate, interpolated value
// xt:         The approximate, interpolated value of x (the order of the approximation is seven)
//             This subprogram uses the global variables hold, told, xold, v1[], v2[]... v6[], v7[];
//             It also needs the f0[], f12[], f13[], f14[], f15[], f16[], and f24[] values
int interpolate(int istep, double t, double xt[]);




#endif
