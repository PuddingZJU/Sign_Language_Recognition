// Copyright (c) 2006-2009 Wataru Kasai
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENCE file.

#include <iomanip>
#include "HMM.h"


HMM::HMM(){
  init();
}

HMM::HMM(int n,int dim){
  init();
  N = n;
  Dim = dim;
  pi = AllocDouble_1D(n);
  a = AllocDouble_2D(n,n);
  states = new State [n];
  for(int i=0;i<N;i++){
    states[i].ID = i;
    states[i].Dim = dim;
  }
}

HMM::HMM(int n,int dim,int m){
  init();
  int *tmp = new int[n];
  for(int i=0;i<n;i++) tmp[i] = m;
  init(n,dim,tmp);
  delete []tmp;
}

HMM::HMM(int n,int dim,Int1D m){
  init();
  init(n,dim,m);
}

HMM::HMM(int n,int dim,int m,const ColumnVector& P,const Matrix& T){
  init();
  int *tmp = new int[n];
  for(int i=0;i<n;i++) tmp[i] = m;
  init(n,dim,tmp);
  pi_const = P;
  Trans_const = T;
  delete []tmp;
}
  

HMM::HMM(int n,int dim,Int1D m,const ColumnVector& P,const Matrix& T){
  init();
  init(n,dim,m);
  pi_const = P;
  Trans_const = T;
}

HMM::HMM(int n,int dim,int m,const ColumnVector& P,const Matrix& T, const Matrix& obs){
  HMM(n,dim,m,P,T);
  set_HMM(obs);
}

HMM::HMM(int n,int dim,Int1D m,const ColumnVector& P,const Matrix& T,const Matrix& obs){
  HMM(n,dim,m,P,T);
  set_HMM(obs);
}


HMM::~HMM(){
  CleanUp();
}

void HMM::CleanUp(){
  if(pi) FreeDouble_1D(pi);
  pi = NULL;
  if(a) FreeDouble_2D(a,N,N);
  a = NULL;
  if(states) delete [] states;
  states = NULL;
  pi_const.CleanUp();
  Trans_const.CleanUp();
  s_alpha.CleanUp();
  s_beta.CleanUp();
  c.CleanUp();
}


void HMM::init(){
  N = 0;
  Dim = 0;
  pi = NULL;
  a = NULL;
  states = NULL;
}

void HMM::init(int n,int dim,Int1D m){ 
  N = n;
  Dim = dim;
  pi = AllocDouble_1D(n);
  a = AllocDouble_2D(n,n);
  states = new State [n];
  for(int i=0;i<N;i++){
    states[i].set_State(m[i],dim);
    states[i].ID = i;
  }
  pi_const.ReSize(n);
  Trans_const.ReSize(n,n); 
}

void HMM::set_HMM(){
  double tmp = 0.0;
  int num1 = 0,num2 = 0;

  for(int i=0;i<N;i++){
    num1=0;
    num2=0;
    tmp = 1.0 / states[i].M;
    for(int j=0;j<states[i].M;j++){
	states[i].c[j] = tmp; //initialization of the coefficients
    }
    
    for(int j=0;j<N;j++){
      if(pi_const.element(j)) num1++;
      if(Trans_const.element(i,j)) num2++;
    }
    tmp = 1.0 / num1;
    if(pi_const.element(i)) pi[i] = tmp;
    else pi[i] = 0.0; //initialization of the init-probabilities
    tmp = 1.0 / num2;
    for(int j=0;j<N;j++){
      if(Trans_const.element(i,j)) a[i][j] = tmp;
      else a[i][j] = 0.0;//initialization of the transition-probabilities
    }
  }
}


void HMM::set_HMM(const Matrix& obs){
  set_HMM();
  double tmp=0.0;
  for(int n=0;n<N;n++){
    for(int m=0;m<states[n].M;m++){
      for(int i=0;i<Dim;i++){
	tmp = 0.0;
	for(int j=0;j<obs.Ncols();j++){
	  tmp += obs.element(i,j);
	}
	states[n].mu[m].element(i) = tmp / obs.Ncols();
      }
    }
  }
  
  for(int i=0;i<Dim;i++){
    for(int j=Dim-1;i<=j;j--){
      tmp = 0.0;
      for(int n=0;n<obs.Ncols();n++){
	tmp += (obs.element(i,n)-states[0].mu[0].element(i)) * 
	  (obs.element(j,n)-states[0].mu[0].element(j));
      }
      tmp /= obs.Ncols();
      for(int n=0;n<N;n++){
	for(int m=0;m<states[n].M;m++){
	  if(i==j) states[n].U[m].element(i,i) = tmp;
	  else{
	    states[n].U[m].element(i,j) = tmp;
	    states[n].U[m].element(j,i) = tmp;
	  }
	}
      }
    }
  }

  TH = UNDER_DET * states[0].U[0].Determinant();
}


void HMM::init_k_mean(const Matrix* obs,int number_ob){
  int length=0, tmp=0, n=0, roop=0;
  double tmp2 = 0.0;
  for(int i=0;i<number_ob;i++){
    length += obs[i].Ncols();
  }
  Double2D data = AllocDouble_2D(Dim,length);
  int *class_data = new int[length];
  Double2D each_data = AllocDouble_2D(Dim,length);
  Int1D state_class_data = AllocInt_1D(length);
  Double2D tmp_vectors = AllocDouble_2D(length,Dim);

  set_HMM();

  for(int i=0;i<number_ob;i++){
    for(int j=0;j<obs[i].Ncols();j++){
      for(int k=0;k<Dim;k++){
	data[k][tmp+j] = obs[i].element(k,j);
      }
      class_data[tmp+j] = 0;
    }
    tmp += obs[i].Ncols();
  }


  /*for state initialization*/
  k_Mean(N,data,class_data,Dim,length);

  for(int i=0;i<N;i++){
    tmp=0;   
    for(int j=0;j<length;j++){
      if(class_data[j]==i){
	for(int k=0;k<Dim;k++){
	  each_data[k][tmp] = data[k][j];
	}
	state_class_data[tmp]=0;
	tmp++;
      }
    }
    
    /*for initialization of the mixtures of each state*/
    k_Mean(states[i].M, each_data, state_class_data, Dim, tmp);
    
    /*calculation of init-value of each Gaussians(average & covariance)*/
    for(int j=0;j<states[i].M;j++){
      n=0;
      for(int k=0;k<Dim;k++){
	states[i].mu[j].element(k) = 0.0;
	for(int l=0;l<Dim;l++) states[i].U[j].element(k,l) = 0.0;
      }
      
      for(int k=0;k<tmp;k++){
	if(state_class_data[k] == j){
	  for(int l=0;l<Dim;l++){
	    states[i].mu[j].element(l) += each_data[l][k];
	    tmp_vectors[n][l] = each_data[l][k];
	  }
	  n++;
	}
      }
      /*routine for avoiding singular value of the covariance, in case of [data num <= dimension]*/ 
      roop = 0;
      while(n <= Dim){
	n++;
	for(int check=1; roop<tmp || check; roop++){
	  if(state_class_data[roop]!=j){
	    for(int l=0;l<Dim;l++){
	      states[i].mu[j].element(l) += each_data[l][roop];
	      tmp_vectors[n][l] = each_data[l][roop];
	      check = 0;
	    }
	  }
	}
      }
      
      states[i].c[j] = (double) n / tmp;
      states[i].mu[j] /= n;
      for(int k=0;k<Dim;k++){
	  for(int l=0;l<Dim;l++){
	      for(int m=0;m<n;m++){ states[i].U[j].element(k,l) += (tmp_vectors[m][k] - states[i].mu[j].element(k)) * (tmp_vectors[m][l] - states[i].mu[j].element(l));
	      }
	  states[i].U[j].element(k,l) /= n;
	  }
      }
      /*previous routine is not perfectly safe, so final check of the positive definition*/
      tmp2 = states[i].U[j].Determinant();
      if(tmp2<=0||!(tmp2==tmp2)){
	for(int k=0;k<Dim;k++){
	  for(int l=0;l<Dim;l++){
	    if(k==l) states[i].U[j].element(k,l) = 1.0;
	    else states[i].U[j].element(k,l) = 0.0;
	  }
	}
      }
    }
  }
 
  FreeDouble_2D(tmp_vectors,length,Dim);
  FreeDouble_2D(data,Dim,length);
  FreeDouble_2D(each_data,Dim,length);
  FreeInt_1D(state_class_data);

  delete []class_data;
  
  TH = UNDER_DET * states[0].U[0].Determinant();
}

void HMM::init_segmental(const Matrix* obs,int number_ob){
  int length=0,t,tmp,flag,roop;
  double tmp2;
  for(int i=0;i<number_ob;i++) length += obs[i].Ncols();
  Int2D segment_t = AllocInt_2D(number_ob,N);
  Int2D segment_tmp = AllocInt_2D(number_ob,N);
  Matrix* DP_cost = new Matrix [number_ob];
  Int3D DP_keiro = new Int2D [number_ob];
  ColumnVector* Average = new ColumnVector [N];
  Matrix* tmp_Data = new Matrix [N];
 
  set_HMM();

  for(int i=0;i<number_ob;i++){
    DP_cost[i].ReSize(obs[i].Ncols(),N);
    DP_keiro[i] = AllocInt_2D(obs[i].Ncols(),N);
  }
  for(int i=0;i<N;i++){
    Average[i].ReSize(Dim);
    for(int j=0;j<Dim;j++){
      Average[i].element(j) = 0.0;
    }
  }

  /*set the init boundary*/
  for(int n=0;n<number_ob;n++){
    for(int i=0;i<N;i++) segment_t[n][i] = (int) obs[n].Ncols()*(i+1) / N;
  }

  do{
    for(int i=0;i<N;i++){
      t=0;
      tmp=0;
      for(int n=0;n<number_ob;n++){
	if(!i) t += segment_t[n][i]; 
	else t += (segment_t[n][i]-segment_t[n][i-1]);
	tmp_Data[i].ReSize(Dim,t);
      }
      for(int n=0;n<number_ob;n++){
	if(!i){
	  for(int k=0;k<segment_t[n][i];k++){
	    tmp_Data[i].Column(tmp+1) = obs[n].Column(k+1);
	    tmp++;
	  }
	}
	else{
	  for(int k=segment_t[n][i-1];k<segment_t[n][i];k++){
	    tmp_Data[i].Column(tmp+1) = obs[n].Column(k+1);
	    tmp++;
	  }
	}
      }

      /*calculate the init value of the sample average*/
      for(int j=0;j<tmp;j++) Average[i] += tmp_Data[i].Column(j+1);
      Average[i] /= (double) tmp;
    }

    /*calculate the min-path on the Euclid distance by DP matching*/
    for(int n=0;n<number_ob;n++){
      for(int i=0;i<obs[n].Ncols();i++){
	for(int j=0;j<N;j++){
	  tmp2 = 0.0;
	  if(!i){
	    if(!j){
	      DP_cost[n].element(i,j) = sqrt(((obs[n].Column(i+1) - Average[j]).t() * 
					      (obs[n].Column(i+1) - Average[j])).AsScalar());
	      DP_keiro[n][i][j] = j;
	    }
	    else{
	      DP_cost[n].element(i,j) = sqrt(((obs[n].Column(i+1) - Average[j]).t() * 
					      (obs[n].Column(i+1) - Average[j])).AsScalar()) + DP_cost[n].element(i,j-1);
	      DP_keiro[n][i][j] = j;
	    }
	  }
	  else{
	    if(!j){
	      DP_cost[n].element(i,j) = sqrt(((obs[n].Column(i+1) - Average[j]).t() * 
					      (obs[n].Column(i+1) - Average[j])).AsScalar()) + DP_cost[n].element(i-1,j);
	      DP_keiro[n][i][j] = j;
	    }
	    else{
	      tmp2 = min(DP_cost[n].element(i-1,j),DP_cost[n].element(i-1,j-1));
	      DP_cost[n].element(i,j) = sqrt(((obs[n].Column(i+1) - Average[j]).t() * 
					      (obs[n].Column(i+1) - Average[j])).AsScalar()) + tmp2;
	      if(DP_cost[n].element(i-1,j) < DP_cost[n].element(i-1,j-1)) DP_keiro[n][i][j] = j;
	      else DP_keiro[n][i][j] = j-1;
	    }
	  }
	}
      }
    }
    /*calculate the boundary means from min-path in reverse*/
    for(int n=0;n<number_ob;n++){
      tmp = N-1;
      segment_tmp[n][tmp] = obs[n].Ncols();
      for(int i=obs[n].Ncols()-1;tmp&&i>=0;i--){
	if(DP_keiro[n][i][tmp]==tmp-1){
	  segment_tmp[n][tmp-1] = i;
	  tmp--;
	}
      }
    }
    flag = 0;
    for(int n=0;n<number_ob;n++){
      for(int i=0;i<N;i++){
	flag += abs(segment_t[n][i]-segment_tmp[n][i]);
	segment_t[n][i] = segment_tmp[n][i];
      }
    }
  }while(flag);
  
  FreeInt_2D(segment_t,number_ob,N);
  FreeInt_2D(segment_tmp,number_ob,N);
  for(int n=0;n<number_ob;n++){
    DP_cost[n].CleanUp();
    FreeInt_2D(DP_keiro[n], obs[n].Ncols(),N);
  }
  delete [] DP_cost;
  delete [] DP_keiro;
  for(int i=0;i<N;i++) Average[i].CleanUp();
  delete [] Average;
  
  Double2D each_data = AllocDouble_2D(Dim,length);
  Int1D state_class_data = AllocInt_1D(length);
  Double2D tmp_vectors = AllocDouble_2D(length,Dim);
  
  for(int i=0;i<N;i++){
    tmp=0;
    for(int j=0;j<tmp_Data[i].Ncols();j++){
      for(int k=0;k<Dim;k++){
	each_data[k][j] = tmp_Data[i].element(k,j);
      }
    }
    for(int j=0;j<length;j++) state_class_data[j] = 0;

    k_Mean(states[i].M, each_data, state_class_data, Dim, tmp_Data[i].Ncols());

    for(int j=0;j<states[i].M;j++){
      int n=0;
      for(int k=0;k<Dim;k++){
	states[i].mu[j].element(k) = 0.0;
	for(int l=0;l<Dim;l++) states[i].U[j].element(k,l) = 0.0;
      }

      for(int k=0;k<tmp_Data[i].Ncols();k++){
	if(state_class_data[k] == j){
	  for(int l=0;l<Dim;l++){
	    states[i].mu[j].element(l) += each_data[l][k];
	    tmp_vectors[n][l] = each_data[l][k];
	  }
	  n++;
	}
      }
      /*routine for avoiding singular value of the covariance, in case of [data num <= dimension]*/ 
      roop = 0;
      while(n <= Dim){
	n++;
	for(int check=1; roop<tmp_Data[i].Ncols() || check; roop++){
	  if(state_class_data[roop]!=j){
	    for(int l=0;l<Dim;l++){
	      states[i].mu[j].element(l) += each_data[l][roop];
	      tmp_vectors[n][l] = each_data[l][roop];
	      check = 0;
	    }
	  }
	}
      }
      states[i].c[j] = (double) n / tmp_Data[i].Ncols();
      states[i].mu[j] /= n;
      for(int k=0;k<Dim;k++){
	for(int l=0;l<Dim;l++){
	  for(int m=0;m<n;m++) states[i].U[j].element(k,l) += (tmp_vectors[m][k] - states[i].mu[j].element(k)) * (tmp_vectors[m][l] - states[i].mu[j].element(l));
	  states[i].U[j].element(k,l) /= n;
	}
      }
      /*previous routine is not perfectly safe, so final check of the positive definition*/
      tmp2 = states[i].U[j].Determinant();
      if(tmp2<=0||!(tmp2==tmp2)){
	for(int k=0;k<Dim;k++){
	  for(int l=0;l<Dim;l++){
	    if(k==l) states[i].U[j].element(k,l) = 1.0;
	    else states[i].U[j].element(k,l) = 0.0;
	  }
	}
      }
    }
  }  

  TH = UNDER_DET * states[0].U[0].Determinant();

  FreeDouble_2D(each_data,Dim,length);
  FreeInt_1D(state_class_data);
  FreeDouble_2D(tmp_vectors,length,Dim);
  for(int i=0;i<N;i++) tmp_Data[i].CleanUp();
  delete [] tmp_Data;
}

void HMM::set_abc(const Matrix& obs){
  int T = obs.Ncols();
  s_alpha.ReSize(N,T);
  s_beta.ReSize(N,T);
  c.ReSize(T);

  set_ac(obs,T-1);
  set_b(obs,0);
}

void HMM::set_ac(const Matrix& obs, int t){
  if(!t){
    c.element(t)=0.0;
    for(int i=0;i<N;i++){
      c.element(t) += pi[i]*states[i].get_p(obs.Column(t+1));
    }
    c.element(t) = pow(c.element(t),-1.0);
    for(int i=0;i<N;i++){
      s_alpha.element(i,t) = c.element(t) * pi[i] * states[i].get_p(obs.Column(t+1));
    }
  }
  else{
    set_ac(obs,t-1);
    c.element(t)=0.0;
    double tmp = 0.0;
    for(int j=0;j<N;j++){
      s_alpha.element(j,t) = 0.0;
      for(int i=0;i<N;i++){
	tmp += s_alpha.element(i,t-1) * a[i][j];
      }
      s_alpha.element(j,t) = tmp * states[j].get_p(obs.Column(t+1));
      c.element(t) += s_alpha.element(j,t);
      tmp = 0.0;
    }
    c.element(t) = pow(c.element(t),-1.0);
    for(int j=0;j<N;j++){
      s_alpha.element(j,t) *= c.element(t);
    }
  }
}

void HMM::set_b(const Matrix& obs ,int t){
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
	  a[i][j] * s_beta.element(j,t+1) * states[j].get_p(obs.Column(t+2));
      }
    }
    for(int i=0;i<N;i++){
      s_beta.element(i,t) *= c.element(t+1);
      if(s_beta.element(i,t)>1.0e300) s_beta.element(i,t) = 1.0e300;
    } 
  }
}



void HMM::estimate(const Matrix* obs, int number_ob){
  double tmp1 = 0.0, tmp2 = 0.0, tmp3 = 0.0;
  int num = 0, flag = 0;
  ColumnVector tmp_mu(Dim);
  Matrix tmp_U(Dim,Dim);
  Matrix *Alpha = new Matrix[number_ob];
  Matrix *Beta = new Matrix[number_ob];
  ColumnVector *C = new ColumnVector[number_ob];
  Double4D xi = new Double3D[number_ob];
  for(int i=0;i<number_ob;i++) xi[i] = new Double2D[obs[i].Ncols()];

  for(int seq=0;seq<number_ob;seq++){
    /*calculate Forward & Backward valiables*/
    set_abc(obs[seq]);

    Alpha[seq] = s_alpha;
    Beta[seq] = s_beta;
    C[seq] = c;
    
    /*memory allocation & calculation of the valiable "xi"*/
    for(int t=0;t<obs[seq].Ncols();t++){
      xi[seq][t] = new Double1D [N];
      tmp1 = 0.0;
      for(int i=0;i<N;i++) tmp1 += s_alpha.element(i,t)*s_beta.element(i,t);
      for(int j=0;j<N;j++){
	xi[seq][t][j] = AllocDouble_1D(states[j].M);
	for(int k=0;k<states[j].M;k++){
	  xi[seq][t][j][k] = s_alpha.element(j,t)*s_beta.element(j,t) / tmp1 
	    * states[j].get_p(k,obs[seq].Column(t+1)) 
	    / states[j].get_p(obs[seq].Column(t+1));
	}
      }
    }
  }

  /*update "pi"*/
  tmp1 = tmp2 = 0.0;
  num = N;
  for(int i=0;i<N;i++){
    pi[i] = 0.0;
    for(int seq=0;seq<number_ob;seq++) pi[i] += Alpha[seq].element(i,0)*Beta[seq].element(i,0);
    pi[i] /= (double) number_ob;

    tmp1 += pi[i];
    if(!pi_const.element(i)){
      num--;
      tmp2 += pi[i];
      pi[i] = 0.0;
    }
  }
  if(num) tmp2 /= num;
  for(int j=0;j<N;j++){
    if(pi_const.element(j)){
      pi[j] += tmp2;
      pi[j] /= tmp1;
    }
  }


  /*update init-state probability "a"*/
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
      tmp1 = tmp2 = tmp3 = 0.0;
      for(int seq=0;seq<number_ob;seq++){
	for(int t=0;t<obs[seq].Ncols()-1;t++){
	  tmp3 = tmp1;
	  tmp1 += Alpha[seq].element(i,t)*states[j].get_p(obs[seq].Column(t+2))
	    * Beta[seq].element(j,t+1) * C[seq].element(t+1);
	  if(tmp1>1.0e300||!(tmp1==tmp1)) tmp1 = tmp3;
	  tmp2 += Alpha[seq].element(i,t)*Beta[seq].element(i,t);
	}
      }
      tmp1 /= (double) number_ob;
      tmp2 /= (double) number_ob;
      tmp1 *= a[i][j];
      if(tmp1&&tmp2) a[i][j] = tmp1 / tmp2;
      else a[i][j] = 0;
    }
  }

  for(int i=0;i<N;i++){
    tmp1 = tmp2 = 0.0;
    flag = 0;
    num = N;
    for(int j=0;j<N;j++){
      tmp1 += a[i][j];
      if(!Trans_const.element(i,j)){
	tmp2 += a[i][j];
	a[i][j] = 0.0;
	num--;
      }
    }
    if(num) tmp2 /= (double) num;
    tmp3 = 1.0;
    for(int j=0;j<N;j++){
      if(Trans_const.element(i,j)){
	tmp3 = (a[i][j]&&(a[i][j]<tmp3)) ? a[i][j] : tmp3;
	a[i][j] += tmp2;
	if(!tmp1) a[i][j] = 1.0 / num;
	else a[i][j] /= tmp1;
      }
    }

    if(ST){
      flag = 0;
      for(int j=0;j<N;j++){
	if(Trans_const.element(i,j)&&(a[i][j]<1.0e-10)){
	  a[i][j] = 1.0e-10;
	  if(!flag){
	    flag++;
	  }
	}
      }
      if(flag) i--;
    }
  }

  /*update mixtures*/
  for(int j=0;j<N;j++){
    tmp2 = 0.0;
    for(int seq=0;seq<number_ob;seq++)
      for(int t=0;t<obs[seq].Ncols();t++)
	for(int m=0;m<states[j].M;m++) tmp2 += xi[seq][t][j][m];
    
    for(int k=0;k<states[j].M;k++){
      tmp1 = 0.0;
      tmp_mu = states[j].mu[k];
      tmp_U = states[j].U[k];
      states[j].U[k] -= states[j].U[k];
      for(int seq=0;seq<number_ob;seq++){
	for(int t=0;t<obs[seq].Ncols();t++){
	  states[j].U[k] += xi[seq][t][j][k]*
	    (obs[seq].Column(t+1)-states[j].mu[k])*(obs[seq].Column(t+1)-states[j].mu[k]).t();
	  tmp1 += xi[seq][t][j][k];
	}
      }
      states[j].mu[k] -= states[j].mu[k];
      for(int seq=0;seq<number_ob;seq++){
	for(int t=0;t<obs[seq].Ncols();t++){ 
	  states[j].mu[k] += xi[seq][t][j][k]*obs[seq].Column(t+1);
	}
      }
      

      if(tmp2&&tmp1>1.0e-300){
	states[j].c[k] = tmp1 / tmp2;
	states[j].mu[k] /= tmp1;
	states[j].U[k] /= tmp1;
	
	if(states[j].U[k].Determinant()<TH*0.01){
	  states[j].U[k] = tmp_U;
	}
	
      }
      else{
	states[j].mu[k] = tmp_mu;
	states[j].U[k] = tmp_U;
      }
    }
  }
  
  /*regulalization*/
  for(int j=0;j<N;j++){
    tmp1 = 0.0;
    for(int k=0;k<states[j].M;k++) tmp1 += states[j].c[k];
    for(int k=0;k<states[j].M;k++) states[j].c[k] /= tmp1;
  }
  
  /*memory deallocation*/
  for(int seq=0;seq<number_ob;seq++){
    FreeDouble_3D(xi[seq],obs[seq].Ncols(),N,0);
    Alpha[seq].CleanUp();
    Beta[seq].CleanUp();
    C[seq].CleanUp();
  }
  delete[] Alpha;
  delete[] Beta;
  delete[] C;
  delete[] xi;
  tmp_mu.CleanUp();
  tmp_U.CleanUp();
}

double HMM::get_log_p(const Matrix& obs){
  double x=0.0;
  set_abc(obs);

  for(int t=0;t<obs.Ncols();t++) x -= log(c.element(t));
  return x;
}


int HMM::estimations(const Matrix* obs, int number_ob, bool display){
  int ite=0;
  double likelihood = 0.0, previous = 0.0;
  Double1D pre_pi = AllocDouble_1D(N);
  Double2D pre_a = AllocDouble_2D(N,N);
  State *pre_states = new State [N];
  
  for(int i=0;i<number_ob;i++){
    likelihood += get_log_p(obs[i]);
  }
  likelihood /= (double) number_ob;
  

  do{
    previous = likelihood;
    likelihood = 0.0;
    ite++;
    
    for(int i=0;i<N;i++){
      pre_pi[i] = pi[i];
      pre_states[i] = states[i];
      for(int j=0;j<N;j++){
	pre_a[i][j] = a[i][j];
      }
    }

    estimate(obs,number_ob);

    for(int i=0;i<number_ob;i++){
      likelihood += get_log_p(obs[i]);
    }
    likelihood /= (double) number_ob;

    if(display&&(((1.0 - previous/likelihood) > THRESHOLD)||(ite==1))) 
      cout << ite << ": L=" << likelihood << "\n";
  }while(((1.0 - previous/likelihood) > THRESHOLD)||(ite==1));

  for(int i=0;i<N;i++){
    pi[i] = pre_pi[i];
    states[i] = pre_states[i];
    for(int j=0;j<N;j++){
      a[i][j] = pre_a[i][j];
    }
  }
  
  FreeDouble_1D(pre_pi);
  FreeDouble_2D(pre_a,N,N);
  delete [] pre_states;

  return ite-1;
}



double HMM::viterbi_p(const Matrix& obs){
  Double2D delta = AllocDouble_2D(obs.Ncols(),N);
  Int2D psi = AllocInt_2D(obs.Ncols(),N);
  double p;
  Int1D q = AllocInt_1D(obs.Ncols());

  re_viterbi(delta,psi,obs,obs.Ncols()-1);

  double x1=LOG_O,x2=0.0;
  int q_Time = 0;
  for(int i=0;i<N;i++){
    x2 = x1;
    x1 = delta[obs.Ncols()-1][i] > x1 ? delta[obs.Ncols()-1][i]: x1;
    q_Time = x1 >= x2 ? i: q_Time;
  }
 
  p = x1;
  q[obs.Ncols()-1] = q_Time;

  for(int t=obs.Ncols()-1;t > 0;t--){
    q[t-1] = psi[t][q[t]];
  }

  FreeDouble_2D(delta, obs.Ncols(), N);
  FreeInt_2D(psi, obs.Ncols(), N);
  FreeInt_1D(q);
  return p;
}



void HMM::viterbi_path(const Matrix& obs, Int1D q){
  Double2D delta = AllocDouble_2D(obs.Ncols(),N);
  Int2D psi = AllocInt_2D(obs.Ncols(),N);
  double p;
  
  re_viterbi(delta,psi,obs,obs.Ncols()-1);

  double x1=LOG_O,x2=0.0;
  int q_Time=0;
  x1 = delta[obs.Ncols()-1][0];
  for(int i=0;i<N;i++){
    x2 = x1;
    x1 = delta[obs.Ncols()-1][i] > x1 ? delta[obs.Ncols()-1][i]: x1;
    q_Time = x1 >= x2 ? i: q_Time;
  }
 
  p = x1;
  q[obs.Ncols()-1] = q_Time;
  
  for(int t=obs.Ncols()-1;t > 0;t--){
    q[t-1] = psi[t][q[t]];
  }

  FreeDouble_2D(delta, obs.Ncols(), N);
  FreeInt_2D(psi, obs.Ncols(), N);
}


void HMM::re_viterbi(Double2D delta,Int2D psi,const Matrix& obs,int t){
  if(!t){
    for(int i=0;i<N;i++){
      delta[0][i] = pi[i]*states[i].get_p(obs.Column(1));
      if(!delta[0][i]) delta[0][i] = LOG_O;
      else delta[0][i] = log(delta[0][i]);
      psi[0][i] = -1;
    }
  }
  else if(t>0){
    re_viterbi(delta,psi,obs,t-1);
    double x1, x2;
    int y=0;
    for(int j=0;j<N;j++){
      if(!a[0][j]) x1 = delta[t-1][0] + LOG_O;
      else x1 = delta[t-1][0] + log(a[0][j]);
      for(int k=0;k<N;k++){
	if(!a[k][j]) x2 = delta[t-1][k] + LOG_O;
	else x2 = delta[t-1][k] + log(a[k][j]);
    	y = x2 > x1 ? k: y;
    	x1 = x2 > x1 ? x2: x1;
      }
      x2 = states[j].get_p(obs.Column(t+1));
      if(!x2) delta[t][j] = x1 + LOG_O;
      else delta[t][j] = x1 + log(x2);
      psi[t][j] = y;
    }
  }
  else{
    cout << "error! in re_viterbi(): no such time t";
    exit(1);
  }
}


void HMM::graph_out(char* str, int n){
  ofstream fout(str);
  
  fout << "/*HMM DOT-file out*/\ndigraph G{\n\tnode [shape = circle, rankdir = LR];\n";
  
  for(int i=0;i<N;i++){
    if(!n) fout << "\tS" << i << " [label = \"S" << i << "\"];\n";
    else{
      if(pi_const.element(i)) fout << "\tS" << i << " [label = \"S" << i << "\", peripheries = 2];\n";
      else fout << "\tS" << i << " [label = \"S" << i << "\"];\n";
    }
  }
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
      if(Trans_const.element(i,j)){
	fout << "\tS" << i << " -> S" << j << " [label = \"" << a[i][j] << "\"];\n";
      }
    }
  } 

  fout << "}";
      
  fout.close();
}


void HMM::Load(char* str){
  ifstream fin(str);
  fin >> *this;
  fin.close();
}

void HMM::Save(char* str){
  ofstream fout(str);
  fout << *this;
  fout.close();
}


HMM &HMM::operator=(HMM &hmm){
  CleanUp();
  N=hmm.N;
  Dim=hmm.Dim;
  states = new State [N];
  pi_const.ReSize(N);
  Trans_const.ReSize(N,N);
  pi = AllocDouble_1D(N);
  a = AllocDouble_2D(N,N);
  
  for(int i=0;i<N;i++){
    states[i] = hmm.states[i];
    pi[i] = hmm.pi[i];
    pi_const.element(i) = hmm.pi_const.element(i);
    for(int j=0;j<N;j++){
      a[i][j] = hmm.a[i][j];
      Trans_const.element(i,j) = hmm.Trans_const.element(i,j);
     
    }
  }
  TH = hmm.TH;

  return *this;
}


ostream &operator<<(ostream &stream,HMM &hmm){
  stream << hmm.N << "\n" << hmm.Dim << "\n";
  if(hmm.a){
    stream << true << "\n";
    for(int i=0;i<hmm.N;i++){
      for(int j=0;j<hmm.N;j++){
	stream << setprecision(20) << hmm.a[i][j] << " ";
      }
      stream << "\n";
    }
  }
  else stream << false << "\n";

  for(int i=0;i<hmm.N;i++){
    for(int j=0;j<hmm.N;j++){
      stream << hmm.Trans_const.element(i,j) << " ";
    }
    stream << "\n";
  }
  
  if(hmm.pi){
    stream << true << "\n";
    for(int i=0;i<hmm.N;i++){
      stream << hmm.pi[i] << " ";
    }
  stream << "\n";
  }
  else stream << false << "\n";
  
  for(int i=0;i<hmm.N;i++){
      stream << hmm.pi_const.element(i) << " ";
    }
  stream << "\n";

  if(hmm.states){
    stream << true << "\n";
    for(int i=0;i<hmm.N;i++){
      stream << hmm.states[i];
    }
  }
  else stream << false << "\n";

  return stream;
}


istream &operator>>(istream &stream,HMM &hmm){
  bool tmp;
  
  hmm.CleanUp();
  stream >> hmm.N >>hmm.Dim;

  stream >> tmp;
  if(tmp){
    hmm.a = AllocDouble_2D(hmm.N,hmm.N);
    for(int i=0;i<hmm.N;i++){
      for(int j=0;j<hmm.N;j++){
	stream >> hmm.a[i][j];
      }
    }
  }
  else hmm.a = NULL;

  hmm.Trans_const.ReSize(hmm.N,hmm.N);
  for(int i=0;i<hmm.N;i++){
    for(int j=0;j<hmm.N;j++){
      stream >> hmm.Trans_const.element(i,j);
    }
  }
  
  stream >> tmp;
  if(tmp){
    hmm.pi = AllocDouble_1D(hmm.N);
    for(int i=0;i<hmm.N;i++){
      stream >> hmm.pi[i];
    }
  }
  else hmm.pi = NULL;

  hmm.pi_const.ReSize(hmm.N);
  for(int i=0;i<hmm.N;i++){
    stream >> hmm.pi_const.element(i);
  }

  stream >> tmp;
  if(tmp){
    hmm.states = new State [hmm.N];
    for(int i=0;i<hmm.N;i++){
      stream >> hmm.states[i];
    }
  }
  else hmm.states = NULL;

  return stream;
}

