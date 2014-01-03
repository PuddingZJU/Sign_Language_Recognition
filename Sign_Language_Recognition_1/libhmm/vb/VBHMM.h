// Copyright (c) 2006-2009 Wataru Kasai
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENCE file.

// This is now under construction.

#define _USE_MATH_DEFINES 1
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <cmath>
#include <math.h>
#include <iostream>
#include <fstream>
/*
#include "./newmat10/newmat.h"
#include "./newmat10/newmatap.h"
#include "./newmat10/newmatrm.h"
#include "./newmat10/precisio.h"
*/
#include <newmat/newmat.h>
#include <newmat/newmatap.h>
#include <newmat/newmatrm.h>
#include <newmat/precisio.h>
#include "mem.h"
using namespace std;

#ifdef _MSC_VER
#define finite _finite
#define isnan _isnan
#endif

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifdef	 __USE_ISOC99
/* INFINITY and NAN are defined by the ISO C99 standard */
#else
double my_infinity(void) {
  double zero = 0;
  return 1.0/zero;
}
double my_nan(void) {
  double zero = 0;
  return zero/zero;
}
#define INFINITY my_infinity()
#define NAN my_nan()
#endif

#define UNDER_PRO 1.0e-300
#define THRESHOLD 0.001

class VBHMM{
 private:
  int N; //Number of States
  int M; //Number of Mixtures
  int Dim; //Dimension of Observations
  ColumnVector u_pi; //hyper parameters of initial state distribution (N)
  Matrix u_a; //hyper parameters of state transition distributions (N*N)
  Matrix u_c; //hyper parameters of mixture coefficient distributions (M*N)
  Matrix A;   //hyper parameters of Wishart distribution      (N*M)
  Matrix **B;   //hyper parameters of Wishart distribution     (N*M*Dim*Dim)
  ColumnVector **MM; //heyper parameters of Normal distribution (N*M*Dim)
  Matrix lambda;       //heyper parameters of Normal distribution (N*M)
  ColumnVector pre_u_pi;
  Matrix pre_u_a;
  Matrix pre_u_c;
  Matrix pre_A;
  Matrix **pre_B;
  ColumnVector **pre_MM;
  Matrix pre_lambda;
  
  /*optimized value in the E-step*/
  Double1D pi;
  Double2D a;
  Double2D c;
  Matrix s_alpha; //scaled forward probability (N*T)
  Matrix s_beta; //scaled backward probability (N*T)
  ColumnVector s_c; //scaling variable

  
  /*Forward-Backward Algorithm*/
  void set_opt();
  void set_abc(const Matrix& obs);
  void set_ac(const Matrix& obs, int t);
  void set_b(const Matrix& obs, int t);
  /*Forward-Backward Algorithm for prediction*/
  void set_abc2(const Matrix& obs);
  void set_ac2(const Matrix& obs, int t);
  void set_b2(const Matrix& obs, int t);

  /*randomize operation for initializarion*/
  double random_d(int n);

  /*refer to source code of Lightspeed toolbox*/
  double digamma(double x); 

  /*updating gaussian density*/
  double f(const ColumnVector& ob, int n);
  double f(const ColumnVector& ob, int n, int k);
  /*Expected gaussian density*/
  double E_f(const ColumnVector& ob, int n);
  double E_f(const ColumnVector& ob, int n, int k);

  /*M-step: optimization of the variational posterior on model parameters*/
  void M_step(const Matrix* obs, int number_ob);

  /*log variational posterior on hidden variables*/
  double get_log_q(const Matrix& obs);
  
  /*negative free enagy*/
  double F(const Matrix* obs, int number_ob);

  /*Kullback-Leibler divergence of Dirichlet distribution*/
  double KL_dirichlet(const ColumnVector& u1, const ColumnVector& u2);
  /*Kullback-Leibler divergence of Wishart distribution*/
  double KL_wishart(double a1, const Matrix& b1, double lambda1, const ColumnVector& mm1,
		    double a2, const Matrix& b2, double lambda2, const ColumnVector& mm2);
  /*Kullback-Leibler divergence of Normal-Wishart distribution*/
  double KL_NW(double a1, const Matrix& b1, double lambda1, const ColumnVector& mm1,
	       double a2, const Matrix& b2, double lambda2, const ColumnVector& mm2);

 public:
  void init();
  void init(int n);
  void init(int n, int m);
  void init(int n, int m, int dim);
  VBHMM();
  VBHMM(int n);
  VBHMM(int n, int m);
  VBHMM(int n, int m, int dim);
  ~VBHMM();
  void CleanUp();
  
  /*estimation process: if display==true then show enagy value*/
  double estimation(const Matrix* obs, int number_ob, int display);
  
  /*Predictive log-Likelihood*/
  double prediction(const Matrix& obs);
};
