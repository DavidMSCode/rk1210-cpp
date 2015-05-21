/*
   Copyright (c) TAMU LASR Lab, Austin Probe 2014
   Texas A&M University - AEROSPACE department
   File name     : mcpiTest.c
   Description   : Main script to test the MCPI C lib
   21/05/2015
*/

#include "./rk1210.h"

#include <time.h>
#include <math.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// defines for PRINTOUTging and file output
#define FILEOUT 1

FILE *fID;

int main() {                                 //  For most other C compilers
//  The main procedure
//  Set the initial conditions and stepsize
double t0;    // Initial t value
double tf;    // Final   t value
double x0[6] = { -464.856 , 7667.880 , 574.231 , 2.8381186 , -0.7871898 , 7.0830275 };; // Initial x values
double h;     // An initial guess for the stepsize to try
double hmax;  // A maximum stepsize (in case error estimate is near zero)
double tol;   // A tolerance for the size of the local truncation error
double error; // The global error
int    printout = 2;  // To control when outputSolution() is called from within rk1210
int    fixedstep = 0; // To control whether fixed step integration is desired (0 for variable step)

    t0 = 0.0;
    tf = 2 * 6.283185307179586476925286766559005768394;  // tf is set to 4 Pi
    h = 0.25;      //  An initial guess or setting for the stepsize, h
    hmax = 1.0;
    tol = 1.0e-10;
    rk1210(t0, tf, x0, h, hmax, tol, printout, fixedstep);

    return 0;
}


// // Main function test
// int main(){
//
//     // Initial State Vector
//     double Xinit[6] = { -464.856 , 7667.880 , 574.231 , 2.8381186 , -0.7871898 , 7.0830275 };
//
//
//     const double t0 = 0.0;
//     const double tf = 24*3600;
//     const double dt = 30.0;
//     int numPoints = (int)((tf-t0)/dt)+1;
//     double *output;
//
//     output = (double*)malloc ((6*numPoints)*sizeof(double));
//
//     mcpiDriver(Xinit, t0, tf, dt, output);
//
//     int jj,ii;
//     if (FILEOUT == 1){
//
//       fID = fopen((const char*)"./output/mcpiOut.txt",(const char*)"w");
//       double nextTime = t0;
//       for (ii = 0; ii<numPoints; ii++){
//         fprintf(fID,"%1.16E\t", nextTime);
//         for ( jj=0; jj<Nstates; jj++ ) {
//             fprintf(fID,"%1.16E\t", output[6*ii+jj]);
//         }
//         fprintf(fID,"\n");
//         nextTime = nextTime + dt;
//       }
//
//       fclose(fID);
//     }
//
//     return 0;
// }
