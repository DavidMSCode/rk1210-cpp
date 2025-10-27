/**
 * Test file for RK1210 library
 */


#include <iostream>
#include <vector>
#include "rk1210.hpp"
#include <chrono>

//same as deriv but using std::vector and no globals
void custom_ode(std::vector<double>& u, double t, std::vector<double>& ddu) {
    double r = sqrt(u[0]*u[0] + u[1]*u[1] + u[2]*u[2]);
    double mu = 3.986004418e5; // Gravitational parameter for Earth in km^3/s^2
    ddu[3] = -mu * u[0] / (r*r*r);
    ddu[4] = -mu * u[1] / (r*r*r);
    ddu[5] = -mu * u[2] / (r*r*r);
    //add J2 perturbation
    double J2 = 1.08263e-3;
    double Re = 6378.137; // Earth's radius in km
    double z2 = u[2]*u[2];
    double r2 = r*r;
    double factor = 1.5 * J2 * mu * (Re*Re) / (r2*r2*r);
    ddu[3] += factor * u[0] * (5*z2/r2 - 1);
    ddu[4] += factor * u[1] * (5*z2/r2 - 1);
    ddu[5] += factor * u[2] * (5*z2/r2 - 3);

    ddu[0] = u[3];
    ddu[1] = u[4];
    ddu[2] = u[5];
    return;
}

//write the custom ode in 2nd order form t,u,du.ddu
std::vector<double> cutom_ode_2nd(double t, std::vector<double>& u, std::vector<double>& du) {
    std::vector<double> ddu(3);
    double r = sqrt(u[0]*u[0] + u[1]*u[1] + u[2]*u[2]);
    double mu = 3.986004418e5; // Gravitational parameter for Earth in km^3/s^2
    ddu[0] = -mu * u[0] / (r*r*r);
    ddu[1] = -mu * u[1] / (r*r*r);
    ddu[2] = -mu * u[2] / (r*r*r);
    //add J2 perturbation
    double J2 = 1.08263e-3;
    double Re = 6378.137; // Earth's radius in km
    double z2 = u[2]*u[2];
    double r2 = r*r;
    double factor = 1.5 * J2 * mu * (Re*Re) / (r2*r2*r);
    ddu[0] += factor * u[0] * (5*z2/r2 - 1);
    ddu[1] += factor * u[1] * (5*z2/r2 - 1);
    ddu[2] += factor * u[2] * (5*z2/r2 - 3);
    nfe++;

    return ddu;
}

//write a wrapper to convert to the 1st order vector form needed by rk1210
void custom_ode_wrapper(std::vector<double>& x, double t, std::vector<double>& dx) {
    std::vector<double> u = {x[0], x[1], x[2]};
    std::vector<double> du = {x[3], x[4], x[5]};
    std::vector<double> ddu = cutom_ode_2nd( t, u, du);
    dx = {du[0], du[1], du[2], ddu[0], ddu[1], ddu[2]};
    return;
}

void deriv(double x[], double t, double f[]) {
//  The derivatives are defined in this procedure (see "rk1210" procedure below for more information)
//  The code below is for the 3D two-body problem with J2 perturbation

    double u[3], du[3], ddu[3];
    for (int i = 0; i < 3; ++i) {
        u[i] = x[i];
        du[i] = x[i + 3];
    }
    double r = sqrt(u[0]*u[0] + u[1]*u[1] + u[2]*u[2]);
    double mu = 3.986004418e5; // Gravitational parameter for Earth in km^3/s^2
    ddu[0] = -mu * u[0] / (r*r*r);
    ddu[1] = -mu * u[1] / (r*r*r);
    ddu[2] = -mu * u[2] / (r*r*r);

    //add J2 perturbation
    double J2 = 1.08263e-3;
    double Re = 6378.137; // Earth's radius in km
    double z2 = u[2]*u[2];
    double r2 = r*r;
    double factor = 1.5 * J2 * mu * (Re*Re) / (r2*r2*r);
    ddu[0] += factor * u[0] * (5*z2/r2 - 1);
    ddu[1] += factor * u[1] * (5*z2/r2 - 1);
    ddu[2] += factor * u[2] * (5*z2/r2 - 3);

    f[0] = x[3];
    f[1] = x[4];
    f[2] = x[5];
    f[3] = ddu[0];
    f[4] = ddu[1];
    f[5] = ddu[2];

    nfe++;//  Increment the total number of derivative function evaluations
    return;
}

int main(int argc, char* argv[]) {



    double t0 = 0.0;          // Initial time
    double tf = 24*3600;         // Final time
    double h = 0.1;           // Initial step size
    double hmax = 0.5;        // Maximum step size
    double tol = 1e-6;        // Tolerance for local truncation error
    int printout = 0;         // No intermediate output
    int fixedstep = 0;        // Variable step size

    double x0[6] = {7000, 0, 0, 0, 7.5, 1.0}; // Initial state: position (km) and velocity (km/s)
    std::vector<double> x0_vec = {7000, 0, 0, 0, 7.5, 1.0}; // Initial state: position (km) and velocity (km/s)
    const int neqns = 6;
    const int maxSteps = 10000;
    double dt[maxSteps];
    for (int i = 0; i < maxSteps; ++i) {
        dt[i] = t0 + i * (tf - t0) / (maxSteps - 1);
    }
    double output1[maxSteps * neqns];
    double output[maxSteps * neqns];

    const int nout = maxSteps;
    auto start = std::chrono::high_resolution_clock::now();
    rk1210(t0, tf, dt, nout, x0, h, hmax, tol, printout, fixedstep, deriv, output1);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Integration with deriv completed in " << elapsed.count() << " seconds.\n";
    auto start2 = std::chrono::high_resolution_clock::now();
    rk1210(t0, tf, dt, nout, x0_vec, h, hmax, tol, printout, fixedstep,
               custom_ode_wrapper, output);
    auto end2 = std::chrono::high_resolution_clock::now();
    elapsed = end2 - start2;
    std::cout << "Integration with odeFunc completed in " << elapsed.count() << " seconds.\n";
    // Print final state
    std::cout << "Final state at t = " << tf << ":\n";
    for (int i = 0; i < neqns; ++i) {
        std::cout << "x[" << i << "] = " << x0[i] << "\n";
    }

    //write to rk1210Out.txt' at max precision, t,x,y,z,vx,vy,vz
    FILE *file = fopen("rk1210Out.txt", "w");
    if (file == NULL) {
        std::cerr << "Error opening file for writing\n";
        return 1;
    }
    for (int i = 0; i < maxSteps; ++i) {
        fprintf(file, "%.15e %.15e %.15e %.15e %.15e %.15e %.15e\n", dt[i], output[i*neqns], output[i*neqns+1], output[i*neqns+2], output[i*neqns+3], output[i*neqns+4], output[i*neqns+5]);
    }
    fclose(file);
    //
    std::cout << "Output written to rk1210Out.txt\n";

    //print nfe
    std::cout << "Number of function evaluations: " << nfe << "\n";
    return 0;
}