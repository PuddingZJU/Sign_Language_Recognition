// Copyright (c) 2006-2009 Wataru Kasai
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENCE file.

// This is now under construction.

#include "VBHMM.h"

void VBHMM::init(){
  N = 0;
  M = 0;
  Dim = 0;
  B = NULL;
  MM = NULL; //heyper parameters of Normal distribution (N*M*Dim)
  pre_B = NULL;
  pre_MM = NULL;
  pi = NULL;
  a = NULL;
  c = NULL;
}

void VBHMM::init(int n){
  init();
  N = n;
  u_pi.ReSize(N);
  u_a.ReSize(N,N);
}

void VBHMM::init(int n,int m){
  init(n);
  M = m;
  u_c.ReSize(M,N);
  A.ReSize(N,M);
  lambda.ReSize(N,M);
}


void VBHMM::init(int n,int m,int dim){
  init(n,m);
  Dim = dim;
  B = new (Matrix*) [N];
  MM = new (ColumnVector*) [N];
  for(int i=0;i<N;i++){
    B[i] = new Matrix [M];
    MM[i] = new ColumnVector [M];
    for(int j=0;j<M;j++){
      B[i][j].ReSize(Dim,Dim);
      MM[i][j].ReSize(Dim);
    }
  }
}

VBHMM::VBHMM(){
  init();
}

VBHMM::VBHMM(int n){
  init(n);
}

VBHMM::VBHMM(int n, int m){
  init(n,m);
}

VBHMM::VBHMM(int n, int m, int dim){
  init(n,m,dim);
}

void VBHMM::CleanUp(){
  u_pi.CleanUp();
  u_a.CleanUp();
  u_c.CleanUp();
  A.CleanUp();
  lambda.CleanUp();
  if(B&&MM){
    for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
	B[i][j].CleanUp();
	MM[i][j].CleanUp();
      }
    }
    exit(1);
    for(int i=0;i<N;i++){
      delete [] B[i];
      delete [] MM[i];
    }
    delete [] B;
    delete [] MM;
  }
  s_alpha.CleanUp();
  s_beta.CleanUp();
  s_c.CleanUp();
  if(pi) FreeDouble_1D(pi);
  if(a) FreeDouble_2D(a,N,N);
  if(c) FreeDouble_2D(c,N,M);
  N = M = Dim = 0;
}

VBHMM::~VBHMM(){
  CleanUp();
}

/* The digamma function is the derivative of gammaln.

   Reference:
    J Bernardo,
    Psi ( Digamma ) Function,
    Algorithm AS 103,
    Applied Statistics,
    Volume 25, Number 3, pages 315-317, 1976.

    From http://www.psc.edu/~burkardt/src/dirichlet/dirichlet.f
    (with modifications for negative numbers and extra precision)
*/
double VBHMM::digamma(double x)
{
  double neginf = -INFINITY;
  static const double c = 12,
    d1 = -0.57721566490153286,
    d2 = 1.6449340668482264365, /* pi^2/6 */
    s = 1e-6,
    s3 = 1./12,
    s4 = 1./120,
    s5 = 1./252,
    s6 = 1./240,
    s7 = 1./132;
    /*s8 = 691/32760,*/
    /*s9 = 1/12,*/
    /*s10 = 3617/8160;*/
  double result;
  /* Illegal arguments */
  if((x == neginf) || isnan(x)) {
    return NAN;
  }
  /* Singularities */
  if((x <= 0) && (floor(x) == x)) {
    return neginf;
  }
  /* Negative values */
  /* Use the reflection formula (Jeffrey 11.1.6):
   * digamma(-x) = digamma(x+1) + pi*cot(pi*x)
   *
   * This is related to the identity
   * digamma(-x) = digamma(x+1) - digamma(z) + digamma(1-z)
   * where z is the fractional part of x
   * For example:
   * digamma(-3.1) = 1/3.1 + 1/2.1 + 1/1.1 + 1/0.1 + digamma(1-0.1)
   *               = digamma(4.1) - digamma(0.1) + digamma(1-0.1)
   * Then we use
   * digamma(1-z) - digamma(z) = pi*cot(pi*z)
   */
  if(x < 0) {
    return digamma(1-x) + M_PI/tan(-M_PI*x);
  }
  /* Use Taylor series if argument <= S */
  if(x <= s) return d1 - 1/x + d2*x;
  /* Reduce to digamma(X + N) where (X + N) >= C */
  result = 0;
  while(x < c) {
    result -= 1/x;
    x++;
  }
  /* Use de Moivre's expansion if argument >= C */
  /* This expansion can be computed in Maple via asympt(Psi(x),x) */
  if(x >= c) {
    double r = 1/x, t;
    result += log(x) - 0.5*r;
    r *= r;
#if 0
    result -= r * (s3 - r * (s4 - r * (s5 - r * (s6 - r * s7))));
#else
    /* this version for lame compilers */
    t = (s5 - r * (s6 - r * s7));
    result -= r * (s3 - r * (s4 - r * t));
#endif
  }
  return result;
}


double VBHMM::f(const ColumnVector& ob, int n){
  double x = 0.0;
  for(int k=0;k<M;k++) x += c[n][k]*f(ob,n,k);

  if(x<UNDER_PRO) return UNDER_PRO;
  return x;
}

double VBHMM::f(const ColumnVector& ob, int n, int k){
  double x = 0.0;
  ColumnVector v;
  for(int i=0;i<Dim;i++) x += digamma(0.5*(A.element(n,k)+1.0-i));
  v = ob - MM[n][k];

  x = sqrt( pow(M_PI,-Dim) * 1.0 / B[n][k].Determinant()
	    * exp(x-A.element(n,k)*(v.t()*B[n][k].i()*v).AsScalar()-Dim/lambda.element(n,k)) );
  
  v.CleanUp();
  
  if(x<UNDER_PRO) return UNDER_PRO;
  return x;
}


void VBHMM::set_opt(){
  double tmp = 0.0;
  double tmp2 = 0.0;
  for(int i=0;i<N;i++) tmp += u_pi.element(i);
  for(int i=0;i<N;i++){
    pi[i] = exp(digamma(u_pi.element(i))-digamma(tmp));
    tmp2 = 0.0;
    for(int j=0;j<N;j++) tmp2 += u_a.element(i,j);
    for(int j=0;j<N;j++) a[i][j] = exp(digamma(u_a.element(i,j))-digamma(tmp2));
    tmp2 = 0.0;
    for(int k=0;k<M;k++) tmp2 += u_c.element(k,i);
    for(int k=0;k<M;k++) c[i][k] = exp(digamma(u_c.element(k,i))-digamma(tmp2));
  }
}



void VBHMM::set_abc(const Matrix& obs){
  double tmp,tmp2,tmp3;
  tmp = tmp2 = tmp3 = 0.0;
  int T = obs.Ncols();
  s_alpha.ReSize(N,T);
  s_beta.ReSize(N,T);
  s_c.ReSize(T);
  if(pi) FreeDouble_1D(pi);
  if(a) FreeDouble_2D(a,N,N);
  if(c) FreeDouble_2D(c,N,M);
  pi = AllocDouble_1D(N);
  a = AllocDouble_2D(N,N);
  c = AllocDouble_2D(N,M);
  
  set_opt();
  set_ac(obs,T-1);
  set_b(obs,0);
}


void VBHMM::set_ac(const Matrix& obs, int t){
  if(!t){
    s_c.element(t) = 0.0;
    for(int i=0;i<N;i++){
      s_c.element(t) += pi[i]*f(obs.Column(t+1),i);
    }
    s_c.element(t) = pow(s_c.element(t),-1.0);
    if(s_c.element(t)<0.0) s_c.element(t) = 1.0;
    for(int i=0;i<N;i++){
      s_alpha.element(i,t) = s_c.element(t) * pi[i] * f(obs.Column(t+1),i);
    }
  }
  else{
    set_ac(obs,t-1);
    s_c.element(t)=0.0;
    double tmp = 0.0;
    for(int j=0;j<N;j++){
      s_alpha.element(j,t) = 0.0;
      for(int i=0;i<N;i++){
	tmp += s_alpha.element(i,t-1) * a[i][j];
      }
      s_alpha.element(j,t) = tmp * f(obs.Column(t+1),j);
      s_c.element(t) += s_alpha.element(j,t);
      tmp = 0.0;
    }
    s_c.element(t) = pow(s_c.element(t),-1.0);
    for(int j=0;j<N;j++) s_alpha.element(j,t) *= s_c.element(t);
  }
}


void VBHMM::set_b(const Matrix& obs ,int t){
  if(t == obs.Ncols()-1){
    for(int i=0;i<N;i++){
      s_beta.element(i,t) = 1.0;
    }
  }
  else{
    set_b(obs,t+1);
    for(int i=0;i<N;i++){
      s_beta.element(i,t) = 0.0;
      for(int j=0;j<N;j++){
	s_beta.element(i,t) +=
	  a[i][j] * s_beta.element(j,t+1) * f(obs.Column(t+2),j);
      }
    }
    for(int i=0;i<N;i++){
      s_beta.element(i,t) *= s_c.element(t+1);
      if(s_beta.element(i,t)>1.0e300) s_beta.element(i,t) = 1.0e300;
    }
  }
}

void VBHMM::M_step(const Matrix* obs, int number_ob){
  double tmp = 0.0;
  double tmp2 = 0.0;
  double tmp3 = 0.0;
  ColumnVector tmp_vector(Dim);
  Double4D xi = new Double3D [number_ob];
  for(int i=0;i<number_ob;i++) xi[i] = new Double2D [obs[i].Ncols()];
  Double2D tmp_w = AllocDouble_2D(N,M);
  ColumnVector** x_bar = new ColumnVector* [N];
  Matrix** S = new Matrix* [N];
  for(int i=0;i<N;i++){
    x_bar[i] = new ColumnVector [M];
    S[i] = new Matrix [M];
    for(int j=0;j<M;j++) x_bar[i][j].ReSize(Dim);
  }

  for(int seq=0;seq<number_ob;seq++){
    set_abc(obs[seq]);
    
    for(int t=0;t<obs[seq].Ncols();t++){
      xi[seq][t] = new Double1D [N];
      tmp = 0.0;
      for(int i=0;i<N;i++) tmp += s_alpha.element(i,t)*s_beta.element(i,t);
      for(int j=0;j<N;j++){
	xi[seq][t][j] = AllocDouble_1D(M);
	for(int k=0;k<M;k++){
	  xi[seq][t][j][k] = s_alpha.element(j,t)*s_beta.element(j,t) / tmp 
	    * f(obs[seq].Column(t+1),j,k) / f(obs[seq].Column(t+1),j);
	}
      }
    }
  }
  
  
  for(int i=0;i<N;i++){
    tmp = 0.0;
    for(int seq=0;seq<number_ob;seq++)
      for(int k=0;k<M;k++) tmp += xi[seq][0][i][k];
    tmp /= (double) number_ob;
    u_pi.element(i) += tmp;
  }
  
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
      tmp3 = 0.0;
      for(int seq=0;seq<number_ob;seq++){
	for(int t=0;t<obs[seq].Ncols()-1;t++){
	  tmp = tmp2 = 0.0;
	  for(int k=0;k<M;k++){
	    tmp += xi[seq][t][i][k];
	    tmp2 += xi[seq][t+1][j][k];
	  }
	}
	if(tmp2) tmp3 += tmp*tmp2;
      }
      tmp3 /= number_ob;
      u_a.element(i,j) += tmp3;
    }
  }
  
  for(int i=0;i<N;i++){
    for(int k=0;k<M;k++){
      tmp_w[i][k] = 0.0;
      for(int seq=0;seq<number_ob;seq++){
	for(int t=0;t<obs[seq].Ncols();t++){
	  tmp_w[i][k] += xi[seq][t][i][k];
	}
      }
      tmp_w[i][k] /= number_ob;
      u_c.element(k,i) += tmp_w[i][k];
    }
  }
  
  
  for(int i=0;i<N;i++){
    for(int k=0;k<M;k++){
      A.element(i,k) += tmp_w[i][k];
      
      x_bar[i][k] = 0.0;
      for(int seq=0;seq<number_ob;seq++){
	for(int t=0;t<obs[seq].Ncols();t++) x_bar[i][k] += xi[seq][t][i][k]*obs[seq].Column(t+1);
      }
      x_bar[i][k] /= number_ob;
      x_bar[i][k] /= tmp_w[i][k];
      
      S[i][k].ReSize(Dim,Dim);
      S[i][k] = 0.0;
      for(int seq=0;seq<number_ob;seq++){
	for(int t=0;t<obs[seq].Ncols();t++){
	  tmp_vector = obs[seq].Column(t+1)-x_bar[i][k];
	  S[i][k] +=xi[seq][t][i][k]*(tmp_vector*tmp_vector.t());
	}
      }
      S[i][k] /= number_ob;
      
      tmp_vector = MM[i][k]-x_bar[i][k];
      
      B[i][k] += S[i][k]+lambda.element(i,k)*tmp_w[i][k]/(lambda.element(i,k)+tmp_w[i][k])
	*(tmp_vector*tmp_vector.t());
      cout << B[i][k].Determinant() << "  ";
      
      
      MM[i][k] = (lambda.element(i,k)*MM[i][k]+tmp_w[i][k]*x_bar[i][k]) / 
	(lambda.element(i,k)+tmp_w[i][k]);
      
      lambda.element(i,k) += tmp_w[i][k];
    }
  }

  tmp_vector.CleanUp();
  for(int seq=0;seq<number_ob;seq++) FreeDouble_3D(xi[seq],obs[seq].Ncols(),N,M);
  delete [] xi;
  FreeDouble_2D(tmp_w,N,M);
  for(int i=0;i<N;i++){
    for(int k=0;k<M;k++){
      x_bar[i][k].CleanUp();
      S[i][k].CleanUp();
    }
    delete [] x_bar[i];
    delete [] S[i];
  }
  delete [] x_bar;
  delete [] S;
  s_alpha.CleanUp();
  s_beta.CleanUp();
  s_c.CleanUp();
}

double VBHMM::get_log_q(const Matrix& obs){
  double x=0.0;
  set_abc(obs);

  for(int t=0;t<obs.Ncols();t++) x -= log(s_c.element(t));
  return x;
}

double VBHMM::F(const Matrix* obs, int number_ob){
  double x=0.0;
  for(int seq=0;seq<number_ob;seq++) x += get_log_q(obs[seq]);
  x /= (double) number_ob;

  x -= KL_dirichlet(u_pi,pre_u_pi);
  for(int i=0;i<N;i++) x -= KL_dirichlet((u_a.Row(i+1)).t(), (pre_u_a.Row(i+1)).t());
  for(int i=0;i<N;i++) x -= KL_dirichlet(u_c.Column(i+1), pre_u_c.Column(i+1));  

  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      x -= KL_NW(A.element(i,j), B[i][j], lambda.element(i,j), MM[i][j], 
		 pre_A.element(i,j), pre_B[i][j], pre_lambda.element(i,j), pre_MM[i][j]);
    }
  }
  
  return x;
}


double VBHMM::KL_dirichlet(const ColumnVector& u1, const ColumnVector& u2){
  int n = u1.Nrows();
  double x1,x2,y;
  x1 = x2 = y = 0.0;
  for(int i=0;i<n;i++){
    x1 += u1.element(i);
    x2 += u2.element(i);
  }
  
  y += lgammaf(x1) - lgammaf(x2);
  
  for(int i=0;i<n;i++){
    y += lgammaf(u2.element(i)) -  lgammaf(u1.element(i)) + 
      (u1.element(i)-u2.element(i)) * (digamma(u1.element(i))-digamma(x1));
  }

  if(y<0.0) y=0.0;
  return y;
}

double VBHMM::KL_wishart(double a1, const Matrix& b1, double lambda1, const ColumnVector& mm1,
			 double a2, const Matrix& b2, double lambda2, const ColumnVector& mm2){
  double x,y;
  double dim = b1.Ncols();
  double tmp;
  x = y = tmp = 0.0;
  for(int i=0;i<dim;i++) y += digamma(0.5*(a1+1-i));
  
  tmp += (Dim*(Dim-1)/4.0)*log(M_PI) - a2/2.0*log((b2/2.0).Determinant())
    -(Dim*(Dim-1)/4.0)*log(M_PI) + a1/2.0*log((b1/2.0).Determinant());
  
  for(int d=0;d<Dim;d++) tmp += lgamma((a2+1-d)/2.0) - lgamma((a1+1-d)/2.0);
  
  x += (a1-a2)/2.0 * (-log((0.5*b1).Determinant()) + y)
    - a1*Dim/2.0 + a1/2.0*(b2*b1.i()).Trace() + tmp;
  
  return x;
}

double VBHMM::KL_NW(double a1, const Matrix& b1, double lambda1, const ColumnVector& mm1,
		    double a2, const Matrix& b2, double lambda2, const ColumnVector& mm2){
  double x = 0.0;
  x += KL_wishart(a1, b1, lambda1, mm1, a2, b2, lambda2, mm2);
  x += 0.5*Dim*log(lambda1/lambda2)+0.5*Dim*(lambda2/lambda1)-0.5*Dim
    +0.5*lambda2*((mm1-mm2).t()*(a1*b1.i())*(mm1-mm2)).AsScalar();
  
  return x;
}
      

double VBHMM::estimation(const Matrix* obs, int number_ob, int display){
  bool flag = true;
  int ite = 0;
  double free_enagy = 0.0;
  double pre_enagy = 0.0;
  pre_B = new Matrix* [N];
  pre_MM = new ColumnVector* [N];
  for(int i=0;i<N;i++){
    pre_B[i] = new Matrix [M];
    pre_MM[i] = new ColumnVector [M];
  }

  
  for(int i=0;i<N;i++){
    u_pi.element(i) = random_d(N);
    for(int j=0;j<N;j++) u_a.element(i,j) = random_d(N);
    for(int k=0;k<M;k++) u_c.element(k,i) = random_d(M);
  }
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      A.element(i,j) = (double) Dim+1;
      lambda.element(i,j) = random_d(M);
      MM[i][j] = random_d(M);
      for(int k=0;k<Dim;k++){
	for(int l=0;l<Dim;l++){
	  if(k==l)B [i][j].element(k,l) = 1.0;
	  else B[i][j].element(k,l) = 0.0;
	}
      }
    }
  }
  
  
  pre_u_pi = u_pi;
  pre_u_a = u_a;
  pre_u_c = u_c;
  pre_A.ReSize(N,M);
  pre_lambda = lambda;
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      pre_A.element(i,j) = A.element(i,j);
      pre_B[i][j] = B[i][j];
      pre_MM[i][j] = MM[i][j];
    }
  }
  
  ite++;
  free_enagy = F(obs,number_ob);
  if(display) cout << ite << ": F=" << free_enagy << "\n";
  
  do{
    pre_enagy = free_enagy;
   
    pre_u_pi = u_pi;
    pre_u_a = u_a;
    pre_u_c = u_c;
    pre_A = A;
    pre_lambda = lambda;
    for(int i=0;i<N;i++){
      for(int j=0;j<M;j++){
	pre_B[i][j] = B[i][j];
	pre_MM[i][j] = MM[i][j];
      }
    }
    
    M_step(obs,number_ob);
    
    free_enagy = F(obs,number_ob);
    
    if(free_enagy>0) flag = ((1.0 - pre_enagy/free_enagy) > THRESHOLD);
    else flag = ((1.0 - pre_enagy/free_enagy) < THRESHOLD);
    ite++;
    if(display==true&&flag) 
      cout << ite << ": F=" << free_enagy << "\n";
  }while(flag);
  ite--;
  free_enagy = pre_enagy;

  
  u_pi = pre_u_pi;
  u_a = pre_u_a;
  u_c = pre_u_c;
  A = pre_A;
  lambda = pre_lambda;
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      B[i][j] = pre_B[i][j];
      MM[i][j] = pre_MM[i][j];
    }
  }
  
  pre_u_pi.CleanUp();
  pre_u_a.CleanUp();
  pre_u_c.CleanUp();
  pre_A.CleanUp();
  pre_lambda.CleanUp();
  for(int i=0;i<N;i++){
    for(int j=0;j<M;j++){
      pre_B[i][j].CleanUp();
      pre_MM[i][j].CleanUp();
    }
  }
  for(int i=0;i<N;i++){
    delete [] pre_B[i];
    delete [] pre_MM[i];
  }
  delete [] pre_B;
  delete [] pre_MM;

  return free_enagy;
}


void VBHMM::set_abc2(const Matrix& obs){
  int T = obs.Ncols();
  double tmp1,tmp2,tmp3;
  tmp1 = tmp2 = tmp3 = 0.0;
  s_alpha.ReSize(N,T);
  s_beta.ReSize(N,T);
  s_c.ReSize(T);
  if(pi) FreeDouble_1D(pi);
  if(a) FreeDouble_2D(a,N,N);
  if(c) FreeDouble_2D(c,N,M);
  pi = AllocDouble_1D(N);
  a = AllocDouble_2D(N,N);
  c = AllocDouble_2D(N,M);
  
  set_opt();
  set_abc(obs);  
  
  tmp1 = 0.0;
  for(int i=0;i<N;i++){
    tmp1 += pi[i];
    tmp2 = tmp3 = 0.0;
    for(int j=0;j<N;j++) tmp2 += a[i][j];
    for(int j=0;j<N;j++) a[i][j] /= tmp2;
    for(int k=0;k<M;k++) tmp3 += c[i][k];
    for(int k=0;k<M;k++) c[i][k] /= tmp3;
  }
  for(int i=0;i<N;i++) pi[i] /= tmp1;


  set_ac2(obs,T-1);
  set_b2(obs,0);
}

void VBHMM::set_ac2(const Matrix& obs, int t){
  if(!t){
    s_c.element(t) = 0.0;
    for(int i=0;i<N;i++){
      s_c.element(t) += pi[i]*E_f(obs.Column(t+1),i);
    }
    s_c.element(t) = pow(s_c.element(t),-1.0);
    for(int i=0;i<N;i++){
      s_alpha.element(i,t) = s_c.element(t) * pi[i] * E_f(obs.Column(t+1),i);
    }
  }
  else{
    set_ac2(obs,t-1);
    s_c.element(t)=0.0;
    double tmp = 0.0;
    for(int j=0;j<N;j++){
      s_alpha.element(j,t) = 0.0;
      for(int i=0;i<N;i++){
	tmp += s_alpha.element(i,t-1) * a[i][j];
      }
      s_alpha.element(j,t) = tmp * E_f(obs.Column(t+1),j);
      s_c.element(t) += s_alpha.element(j,t);
      tmp = 0.0;
    }
    s_c.element(t) = pow(s_c.element(t),-1.0);
    for(int j=0;j<N;j++) s_alpha.element(j,t) *= s_c.element(t);
  }
}

void VBHMM::set_b2(const Matrix& obs ,int t){
  if(t == obs.Ncols()-1){
    for(int i=0;i<N;i++){
      s_beta.element(i,t) = 1.0;
    }
  }
  else{
    set_b2(obs,t+1);
    for(int i=0;i<N;i++){
      s_beta.element(i,t) = 0.0;
      for(int j=0;j<N;j++){
	s_beta.element(i,t) +=
	  a[i][j] * s_beta.element(j,t+1) * E_f(obs.Column(t+2),j);
      }
    }
    for(int i=0;i<N;i++){
      s_beta.element(i,t) *= s_c.element(t+1);
      if(s_beta.element(i,t)>1.0e300) s_beta.element(i,t) = 1.0e300;
    }
  }
}

double VBHMM::E_f(const ColumnVector& ob,int n, int k){
  double x = 0.0;
  double tmp,tmp2;
  tmp = tmp2 = 0.0;
  ColumnVector tmp_vec(Dim);
  Matrix tmp_Mat(Dim,Dim);
  tmp_vec = MM[n][k]-ob;
  tmp_Mat = B[n][k] + (lambda.element(n,k)/(lambda.element(n,k)+1.0))*(tmp_vec*tmp_vec.t());
  
  x = pow(lambda.element(n,k)/(M_PI*(lambda.element(n,k)+1.0)), (double) Dim/2.0);

  tmp = B[n][k].Determinant();
  tmp2 = tmp_Mat.Determinant();
  
  x *= exp(A.element(n,k)*log(tmp)/2.0-(A.element(n,k)+1.0)*log(tmp2)/2.0);
  
  tmp = lgamma((A.element(n,k)+1.0)/2.0)-lgamma((A.element(n,k)+1.0-Dim)/2.0);
  
  x *= exp(tmp);
  
  tmp_vec.CleanUp();
  tmp_Mat.CleanUp();
  
  if(x<UNDER_PRO) return UNDER_PRO;
  
  return x;
}

double VBHMM::E_f(const ColumnVector& ob,int n){
  double x = 0.0;
  for(int k=0;k<M;k++) x += c[n][k] * E_f(ob,n,k);
  
  return x;
}

double VBHMM::prediction(const Matrix& obs){
  double x=0.0;
  set_abc2(obs);

  for(int t=0;t<obs.Ncols();t++) x -= log(s_c.element(t));
  
  return x;
}
      
double VBHMM::random_d(int n){
  double r1;

  r1=(double)rand(); 
  return (r1/RAND_MAX * (double)n);
}
