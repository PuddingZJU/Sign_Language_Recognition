// Copyright (c) 2006-2009 Wataru Kasai
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENCE file.

#pragma once

#include "State.h"
#include "k_mean.h"
#include <fstream>

#define UNDER_DET 0.001  //For lower bound of the determinant
#define THRESHOLD 1.0e-3 //Convergence threshold 
#define LOG_O -1.0e300   //Approximation of log(0.0)
#define ST 1             //If user don't want to change the structure, set "True"

class HMM{
    //privat: If not experimental useage, this comment out should be removed.
 public:
    int N; //state number
    int Dim; //dimension of the observation
    Double1D pi; //initial state probabilities
    Double2D a; //state transition probabilities
    State *states; //states
    ColumnVector pi_const; //constraints of the initial state probability
    Matrix Trans_const; //structure of the state transition
    Matrix s_alpha; //scaled Forward-variables (state num * time)
    Matrix s_beta; //scaled Backward-variables (state num * time)
    ColumnVector c; //scaling constants
    
    /*threshold for preventing update of covariance
      (Practical setting is [0.01 * c] where c is the determinant calculated by the real data)*/
    double TH;
    
    void init();
    void init(int n, int dim, int* m);
    
    /*set the scaling constant, or scaled Forward-Backward valiables*/
    void set_abc(const Matrix& obs);
    void set_ac(const Matrix& obs, int t);
    void set_b(const Matrix& obs, int t);
    
    /*recursive processing of the Viterbi algorithm*/
    void re_viterbi(Double2D delta,Int2D psi,const Matrix& obs,int t);
    
    //public: If not experimental useage, this comment out should be removed.
    
    HMM();
    HMM(int n, int dim);
    HMM(int n, int dim, int m);
    HMM(int n, int dim, int* m);
    /*Initialization with the structure*/
    HMM(int n,int dim,int m,const ColumnVector& P,const Matrix& T);
    HMM(int n,int dim,Int1D m,const ColumnVector& P,const Matrix& T);
    /*Initialization with all member valiables*/
    HMM(int n,int dim,int m,const ColumnVector& P,const Matrix& T,const Matrix& obs);
    HMM(int n,int dim,Int1D m,const ColumnVector& P,const Matrix& T,const Matrix& obs);
    ~HMM();
    
    /*This function randomly sets the initial-state probabilities, transition probabilities and 
      coefficients of the mixtures*/
    void set_HMM();
    /*This function sets valiables listed above by calculating "average" and "variable" 
      from one sequential data*/
    void set_HMM(const Matrix& obs);
    
    /*initialization by using k-means*/
    void init_k_mean(const Matrix* obs,int number_ob);
    /*initialization by using segmental k-means*/
    void init_segmental(const Matrix* obs,int number_ob);
    
    /*return the log-likelihood of the observed sequences(feature*time)*/
    double get_log_p(const Matrix& obs);
    
    /*model estimation by the EM(Baum-Welch) algorithm*/
    void estimate(const Matrix* obs, int number_ob);
    
    /*Main function of the model estimation -- return the iteration number
      "display": If true, it can be possible to check the averaged log-likelihoods on console*/
    int estimations(const Matrix* obs, int number_ob ,bool display);    
    
    /*return the approx-probability by use of the Viterbi algorithm*/
    double viterbi_p(const Matrix& obs);
    /*return the optimal-path as Int1D by use of the Viterbi algorithm*/
    void viterbi_path(const Matrix& obs, Int1D q);
    
    /*for graphviz-dot output n: true(differ the initial state showing from other states)*/
    void graph_out(char* str, int n);
    
    /*memory clean-up*/
    void CleanUp();
    
    /*load or save of the model
      (caution: These are undefined for models not given the structure yet.)*/
    void Load(char* str);
    void Save(char* str);
    
    /*operator-overloads*/
    HMM &operator=(HMM &hmm);
    /*model In-Out
      (caution: These are undefined for models not given the structure yet.)*/
    friend ostream &operator<<(ostream &stream,HMM &hmm);
    friend istream &operator>>(istream &stream,HMM &hmm);
};
