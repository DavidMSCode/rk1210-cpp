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
  double dt;    // Output time step
  double h;     // An initial guess for the stepsize to try
  double hmax;  // A maximum stepsize (in case error estimate is near zero)
  double tol;   // A tolerance for the size of the local truncation error
  double error; // The global error
  int    printout = 2;  // To control when outputSolution() is called from within rk1210
  int    fixedstep = 0; // To control whether fixed step integration is desired (0 for variable step)
  double * output;
  int states,ii,jj,numPoints;


  t0 = 0.0;
  // tf = 605;
  dt = 60.0;

  tf = 1.0*7.121081577578024e+03;
  /* Initialize the extra sentinel element to a value that won't confuse the
     rk1210 logic (set equal to tf). This prevents uninitialized memory (often
     zero) from being read and interpreted as a valid output time. */
  dtVec[numPoints] = tf;

  // tf = -3600;
  // dt = -6;
  h = 60;      //  An initial guess or setting for the stepsize, h
  hmax = 100;
  tol = 1.0e-14;
  states = 6;

  numPoints = (int)ceil((tf-t0)/dt)+1;
  /* Allocate one extra sentinel slot in dtVec and a little extra in output
     to ensure rk1210 cannot read/write past the provided buffers. */
  dtVec = (double*)malloc ((numPoints + 1) * sizeof(double));

  dtVec[0] = t0;
  for (ii = 1; ii < numPoints - 1; ii++){
    dtVec[ii] = dtVec[ii-1] + dt;
  }
  dtVec[numPoints-1] = tf;
  /* sentinel: set equal to tf so rk1210 won't see uninitialized 0 or garbage */
  dtVec[numPoints] = tf;

  output = (double*)malloc ((states * (numPoints + 1)) * sizeof(double));

  clock_t startTime = clock();
  rk1210(t0, tf, dtVec, numPoints, x0, h, hmax, tol, printout, fixedstep, output);
  clock_t endTime = clock();
  float elapsedTime = ((float) (endTime - startTime))/CLOCKS_PER_SEC;
  printf("Elapsed time: %f s\n",elapsedTime);
  // rk1210(t0, tf, dtVec, x0, -h, -hmax, tol, printout, fixedstep, output);

  if (FILEOUT == 1){
    fID = fopen((const char*)"./rk1210Out.txt",(const char*)"w");
    double nextTime = t0;
    for (ii = 0; ii<numPoints; ii++){
      fprintf(fID,"%1.16E\t", dtVec[ii]);
      for ( jj=0; jj<states; jj++ ) {
        fprintf(fID,"%1.16E\t", output[states*ii+jj]);
      }
      fprintf(fID,"\n");
    }

    fclose(fID);
  }

  // Hamiltonian
  double H = 0.0;
  double H0 = 0.0;
  double solN[6] = {0.0};
  for (ii = 0; ii<numPoints; ii++){
    for ( jj=0; jj<states; jj++ ) {
      solN[jj] = output[states*ii+jj];
    }
    jacobiIntegral(dtVec[ii],solN,&H,70.0);
    if (ii == 0){
      H0 = H;
    }
    printf("H %1.16E\n",fabs(H-H0));
  }


  return 0;
}
