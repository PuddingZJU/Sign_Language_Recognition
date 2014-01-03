// Copyright (c) 2006-2009 Wataru Kasai
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENCE file.

#pragma once

#include <stdio.h>
#include <cmath>
#include <iostream>

/*
  #include "./newmat10/newmat.h"
  #include "./newmat10/newmatap.h"
  #include "./newmat10/newmatrm.h"
  #include "./newmat10/precisio.h"
*/
#include "newmat/newmat.h"
#include "newmat/newmatap.h"
#include "newmat/newmatrm.h"
#include "newmat/precisio.h"

#include "mem.h"
using namespace std;

#define UNDER_PRO 1.0e-300 //lower bound of the probability
#define INF       300      //canstant for avoiding "Inf" in Gaussian calculation

class State{
 private:
  void init();
  
  /*calculate the multivaliate Gaussian distribution*/
  double gaussian(const ColumnVector& ob,const ColumnVector& m,const Matrix& u);
  
 public:
  int ID; //state ID
  int M; //mixture num
  int Dim; //dimension
  Double1D c; //mixture coefficients
  ColumnVector* mu; //average vector
  Matrix* U; //covariance matrix
  
  State();
  State(int m,int dim);
  ~State();
  
  void set_State(int m,int dim);
  
  /*return the probability of the obsevation vector "ob" from the model*/
  double get_p(const ColumnVector& ob);

  /*return the probability of the obsevation vector "ob" from each mixture components*/
  double get_p(int k,const ColumnVector& ob);

  /*memory clean-up*/
  void CleanUp();

  /*operator-overloads*/
  State &operator=(State &s);
  friend ostream &operator<<(ostream &stream,State &s);
  friend istream &operator>>(istream &stream,State &s);
};

