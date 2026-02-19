/*

  rk1210.h
  Austin Probe
  21/05/2015
  Function declarations for rk1210 numerical propagator.

*/

#ifndef _RK1210_H_
#define _RK1210_H_

#include <vector>


#define EPS  1.0e-14  //  Epsilon, a nearly zero value   (a machine dependent constant)
//  If your problem has more than 10 differential equations, then you will need to increase
//  MAXEQNS to be as great or greater than that number
#define MAXEQNS 10    //  The maximum number of differential equations   (a constant)

//  The global variables are:
extern int nfe;          //  The total number of function evaluations
extern int rstep;        //  The total number of rejected steps
extern int backprop;     //  Backwards or forward integration
extern const int neqns;       //  The number of differential equations being integrated
extern double v1[MAXEQNS], v2[MAXEQNS], v3[MAXEQNS], v4[MAXEQNS], v5[MAXEQNS], v6[MAXEQNS], v7[MAXEQNS];

//define ode function type using vectors for I/O
typedef void (*vecFunc)(std::vector<double>&, double, std::vector<double>&);
typedef void (*derivFunc)(double[], double, double[]);
// for inputs, t,u,du
// for outputs ddu


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
// nout: number of entries in the dt array (number of requested output times)
void rk1210(double t0, double tf, double * dt, int nout, double x0[], double h, double hmax,
double tol, int printout, int fixedstep, derivFunc deriv, double * output, int * fevals);

void rk1210(double t0, double tf, double *dt, int nout, std::vector<double> &x0, double h, double hmax, double tol, int printout, int fixedstep, vecFunc deriv, double *output, int *fevals);

void rk1210(double t0, double tf, double * dt, int nout, double x0[], double h, double hmax,
double tol, int printout, int fixedstep, vecFunc deriv, double * output, int *fevals);

//converts the ode function to the form needed by rk1210
void deriv_custom(double x[], double t, double f[], vecFunc odeFunc);

// t:          The value of the independent variable where you want an approximate, interpolated value
// xt:         The approximate, interpolated value of x (the order of the approximation is seven)
//             This subprogram uses the global variables hold, told, xold, v1[], v2[]... v6[], v7[];
//             It also needs the f0[], f12[], f13[], f14[], f15[], f16[], and f24[] values
// Interpolate at time t using derivatives provided by the caller instead of globals
//template interpolate to take vectors or arrays
template <typename T> int interpolate(int istep, double t, double xt[], double hold_local, double told_local, double xold_local[],
                T f0_local, T f12_local, T f13_local, T f14_local,
                T f15_local, T f16_local, T f24_local) {
// t:          The value of the independent variable where you want an approximate, interpolated value
// xt:         The approximate, interpolated value of x (the order of the approximation is seven)
//             This subprogram uses the global variables hold, told, xold, v1[], v2[]... v6[], v7[];
//             It also needs the f0[], f12[], f13[], f14[], f15[], f16[], and f24[] values
static int laststep = -1; // To keep track of last istep value
double u;
int i;
    if (backprop*(t - told_local) < 0.0 || backprop*(t - (told_local + hold_local)) > 0.0) {
        printf(" interpolate called with invalid t value: %lf , should have been in interval [%lf,%lf]\n", t, told_local, told_local + hold_local);
        return 1;  // return 1 to indicate an error was detected
    }
    if (fabs(hold_local) < EPS) {
        printf(" interpolate called when stepsize was smaller than %e\n", EPS);
        return 1;  // return 1 to indicate an error was detected
    }
    u = (t - told_local)/hold_local;    // The fraction of the stepsize
    if (istep != laststep) {  // Not same step as last step, so prepare coefficients for interpolation
            double lv1[MAXEQNS], lv2[MAXEQNS], lv3[MAXEQNS], lv4[MAXEQNS], lv5[MAXEQNS], lv6[MAXEQNS], lv7[MAXEQNS];
            for (i=0; i<neqns; i++)  {
                lv1[i] = f0_local[i];
                lv2[i] = -10.5*f0_local[i] + 14.201576602919816167081697726017431755066943*f12_local[i] -
                    5.6689852255455078785754090169022395870479816*f13_local[i] + 3.2*f14_local[i] -
                    2.0499648130767427769623871824449592454507835*f15_local[i] +
                    1.31737343570243448845609847332976707743182208*f16_local[i] - 0.5*f24_local[i];
                lv3[i] = 46.66666666666666666666666666666666666666667*f0_local[i] -
                    87.2902664004417965922778924772488513999048*f12_local[i] +
                    65.135105409067996156591318868339788120329948*f13_local[i] -
                    40.533333333333333333333333333333333333333333*f14_local[i] +
                    26.838671584600392726962022286133161994826263*f15_local[i] -
                    17.4835105932265922912754486772240987152514118*f16_local[i] +
                    6.6666666666666666666666666666666666666666667*f24_local[i];
                lv4[i] = -105.*f0_local[i] + 222.88639881611084863747680606947251151325055*f12_local[i] -
                    212.8838727288609887729485287883212825146208*f13_local[i] + 163.2*f14_local[i] -
                    116.089673317802003992125748213987805846095736*f15_local[i] +
                     77.887147230552144127597470932836576847465988*f16_local[i] - 30.*f24_local[i];
                lv5[i] = 126.*f0_local[i] - 285.34406401328592006392378481238281468669901*f12_local[i] +
                    311.11412930341036848790179624034677785608822*f13_local[i] - 276.48*f14_local[i] +
                    217.938950304820148808692037500470581529160357*f15_local[i] -
                    153.229015594944597232670048928434544698549571*f16_local[i] + 60.*f24_local[i];
                lv6[i] = -77.*f0_local[i] + 181.15100533117288056715768411727157368517256*f12_local[i] -
                    214.26102709454769670848368792659289474163563*f13_local[i] + 211.2*f14_local[i] -
                    183.202634095017623482080435013300829299326339*f15_local[i] +
                    137.11265585839243962340643882262215035578941*f16_local[i] - 55.*f24_local[i];
                lv7[i] = 18.857142857142857142857142857142857142857143*f0_local[i] -
                    45.466237312795045741509160419984817720137424*f12_local[i] +
                    56.780523027080760027223446134270532005851709*f13_local[i] -
                    60.3428571428571428571428571428571428571428571*f14_local[i] +
                    56.7805230270807600272234461342705320058517094*f15_local[i] -
                    45.4662373127950457415091604199848177201374237*f16_local[i] +
                    18.8571428571428571428571428571428571428571429*f24_local[i];
                v1[i] = lv1[i]; v2[i] = lv2[i]; v3[i] = lv3[i]; v4[i] = lv4[i]; v5[i] = lv5[i]; v6[i] = lv6[i]; v7[i] = lv7[i];
            }
        laststep = istep;  //  So on next call, we can tell if we already have the above v coefficients
        }
//  Then compute the following for any value of u where user wants an answer (of order 7)
    for (i=0; i<neqns; i++)  {
        xt[i] = xold_local[i] + hold_local*(v1[i] + (v2[i] + (v3[i] + (v4[i] + (v5[i] + (v6[i] + v7[i]*u)*u)*u)*u)*u)*u)*u;
    }
return 0;  // return 0 to indicate no error was detected
}


#endif
