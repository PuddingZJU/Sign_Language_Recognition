// Copyright (c) 2006-2009 Wataru Kasai
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENCE file.

#include <iomanip>
#include <vector>
#include "State.h"

#define LIBHMM_PI 3.14159265359

State::State(){
  init();
}

State::State(int m,int dim){
  init(); 
  M=m;
  Dim=dim;
  c = AllocDouble_1D(m);
  mu = new ColumnVector[m];
  U = new Matrix[m];
  for(int i=0;i<m;i++){
    mu[i].ReSize(dim);
    U[i].ReSize(dim,dim);
  }
  for(int i=0;i<M;i++){
    for(int j=0;j<Dim;j++){
      mu[i].element(j) = 0.0;
      for(int k=0;k<Dim;k++){
	U[i].element(j,k) = 0.0;
      }
    }
  }
}

State::~State(){
  CleanUp();
}

void State::CleanUp(){
  FreeDouble_1D(c);
  c = NULL;
  for(int i=0;i<M;i++){
    mu[i].CleanUp();
    U[i].CleanUp();
  }
  delete [] mu;
  mu = NULL;
  delete [] U;
  U = NULL;
}

void State::init(){
  ID = 0;
  M = 0;
  Dim = 0;
  c = NULL;
  mu = NULL;
  U = NULL;
}

void State::set_State(int m,int dim){
  init();
  M=m;
  Dim=dim;
  c = AllocDouble_1D(m);
  mu = new ColumnVector[m];
  U = new Matrix[m];
  for(int i=0;i<m;i++){
    mu[i].ReSize(dim);
    U[i].ReSize(dim,dim);
  }
}

double State::get_p(const ColumnVector& ob){
  double x=0.0;
  if(!mu||!U) nrerror("State-ID:%d can't output likelihood");
  for(int i=0;i<M;i++){
    x += c[i] * gaussian(ob, mu[i], U[i]);
  }
  
  if(x<UNDER_PRO||!(x==x)) return UNDER_PRO;
  else if(x>1.0e300) return 1.0e300;
  else return x;
}


double State::get_p(int k,const ColumnVector& ob){
  if((k<0)||(M<=k)) return 0;
  else return c[k]*gaussian(ob,mu[k],U[k]);
}

double State::gaussian(const ColumnVector& ob,const ColumnVector& m,const Matrix& A){
  
	std::vector<double> vob, vm, vA;
	for (int i=1; i<=ob.Nrows(); i++)
	{
		vob.push_back(ob(i));
	}
	for (int i=1; i<=m.Nrows(); i++)
	{
		vm.push_back(m(i));
	}
	for (int i=1; i<=A.Nrows(); i++)
	{
		for (int j=1; j<=A.Ncols(); j++)
		{
			vA.push_back(A(i,j));
		}
	}
	

	ColumnVector tmp(Dim);
  tmp = ob-m;
  double x = (tmp.t() * A.i() * tmp).AsScalar();
  if(x<0.0) x = 0.0;
  tmp.CleanUp();

  return (exp(x*(-0.5)) / (pow(2.0*LIBHMM_PI,ob.Nrows()*0.5)*pow(A.Determinant(),0.5)));
}


State& State::operator=(State &s){
  if(!(M==s.M&&Dim==s.Dim)){
    if(c) FreeDouble_1D(c);
    for(int i=0;i<M;i++){
      mu[i].CleanUp();
      U[i].CleanUp();
    }
    delete [] mu;
    delete [] U;
     
    M=s.M;
    Dim=s.Dim;
    c = AllocDouble_1D(M);
    mu = new ColumnVector[M];
    U = new Matrix[M];
    for(int i=0;i<M;i++){
      mu[i].ReSize(Dim);
      U[i].ReSize(Dim,Dim);
    }
  }

  ID = s.ID;
  for(int i=0;i<M;i++){
    c[i] = s.c[i];
    for(int j=0;j<Dim;j++){
      mu[i].element(j) = s.mu[i].element(j);
      for(int k=0;k<Dim;k++){
	U[i].element(j,k) = s.U[i].element(j,k);
      }
    }
  }

  return *this;
}


ostream &operator<<(ostream &stream,State &s){
  stream << s.ID << "\n" << s.M << "\n" << s.Dim << "\n";
  for(int i=0;i<s.M;i++){
    stream << s.c[i] << " ";
  }
  stream <<  "\n";  
  for(int i=0;i<s.M;i++){
    for(int j=0;j<s.Dim;j++){
      stream << setprecision(20) << s.mu[i].element(j) << " ";
    }
  }
  stream <<  "\n";
  for(int i=0;i<s.M;i++){
    for(int j=0;j<s.Dim;j++){
      for(int k=0;k<s.Dim;k++){
	stream << setprecision(20) << s.U[i].element(j,k) << " ";
      }
    }
  }
  stream << "\n";
  return stream;
}


istream &operator>>(istream &stream,State &s){
  s.CleanUp();
  stream >> s.ID >> s.M >> s.Dim;
  
  s.c = AllocDouble_1D(s.M);
  for(int i=0;i<s.M;i++){
    stream >> s.c[i];
  }
  
  s.mu = new ColumnVector[s.M];
  for(int i=0;i<s.M;i++){
    s.mu[i].ReSize(s.Dim);
    for(int j=0;j<s.Dim;j++){
      stream >> s.mu[i].element(j);
    }
  }
  
  s.U = new Matrix[s.M];
  for(int i=0;i<s.M;i++){
    s.U[i].ReSize(s.Dim,s.Dim);
    for(int j=0;j<s.Dim;j++){
      for(int k=0;k<s.Dim;k++){
	stream >> s.U[i].element(j,k);
      }
    }
  }
  return stream;
}
