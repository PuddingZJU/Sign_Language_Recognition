// Copyright (c) 2006-2009 Wataru Kasai
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENCE file.

// This is the test(sample) code. Users need to prepare their original data.

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "HMM.h"
using namespace std;

#define Number 3
#define Dimention 21
#define MIX 1

#define TRAINING 14
#define SAMPLE 6
#define CLASS 200//1542

#define Only_Check 0

double check2(){
  ifstream fin;
  char str[80];
  int aaa = 0;
  int bbb = 0;
  int result = 0;
  double tmp;
  double val[50*SAMPLE];
  double max_val[50*SAMPLE];
  int max_val_n[50*SAMPLE];
  
  for(int i=0;i<50;i++){
    sprintf(str, "./NResult%d_%d/Result_%d.txt", Number, MIX, i*4+1);
    fin.open(str);
    if(!fin){
      cout << "Can't open input file: " << str << "\n";
      return 1;
    }

    
    fin >> aaa;
    if(aaa) bbb++;
    
    
    for(int j=0;j<50;j++){
      for(int k=0;k<SAMPLE;k++){
	fin >> val[j*SAMPLE+k];
      }
      for(int l=0;l<3*SAMPLE;l++) fin >> tmp;
    }

    fin.close();

    for(int j=0;j<50*SAMPLE;j++){
      if(!i){
	max_val[j] = val[j];
	max_val_n[j]=0;
      }
      else{
	if(max_val[j] < val[j]){
	  max_val[j] = val[j];
	  max_val_n[j] = i;
	}
      }
    }
  }

  for(int j=0;j<50*SAMPLE;j++)
    if(max_val_n[j] == (int) j / SAMPLE) result++;

  cout << (double) bbb / 50 << "\n";
  return (double) result / (50*SAMPLE);
}

double check(){
  ifstream fin;
  int result=0;
  int aaa = 0;
  int bbb = 0;
  char str[80];
  double val[CLASS*SAMPLE];
  double max_val[CLASS*SAMPLE];
  int max_val_n[CLASS*SAMPLE];

  for(int i=0;i<CLASS;i++){
    sprintf(str, "./AResult%d_%d/Result_%d.txt", Number, MIX, i+1);
    fin.open(str);
    
    if(!fin){
      cout << "Can't open input file: " << str << "\n";
      return 1;
    }
    
    
    fin >> aaa;
    if(aaa) bbb++;
    for(int j=0;j<CLASS*SAMPLE;j++) fin >> val[j];
    
    fin.close();
    
    for(int j=0;j<CLASS*SAMPLE;j++){
      if(!i){
	max_val[j] = val[j];
	max_val_n[j]=0;
      }
      else{
	if(max_val[j] < val[j]){
	  max_val[j] = val[j];
	  max_val_n[j] = i;
	}
      }
    }
  }
  
  for(int j=0;j<CLASS*SAMPLE;j++)
    if(max_val_n[j] == (int) j / SAMPLE) result++;

  cout << (double) bbb / CLASS << "\n";
  return (double) result / (CLASS*SAMPLE);
}

int main(){
  int l;
  float number;
  ifstream fin;
  ofstream fout;
  char str[80];
  float tmp = 0.0;
  ColumnVector P(Number);
  Matrix T(Number,Number);
  Matrix test;

  if(Only_Check){
    cout << check2() << "\n";
    return 0;
  }
  
  /*  left-to-right*/
  for(int i=0;i<Number;i++){
    if(i) P.element(i) = 0.0;
    else P.element(i) = 1.0;
  }
  for(int i=0;i<Number;i++){
    for(int j=0;j<Number;j++){
      T.element(i,j) = 0.0;
      if(i==j-1) T.element(i,j) = 1.0;
      else if(i==j) T.element(i,j) = 1.0;
    }
  }
  
  /*circular
    for(int i=0;i<Number;i++){
    P.element(i) = 1.0;
    }
    for(int i=0;i<Number;i++){
    for(int j=0;j<Number;j++){
    T.element(i,j) = 0.0;
    if(i==j-1) T.element(i,j) = 1.0;
    else if(i==j) T.element(i,j) = 1.0;
    else if(i==Number-1&&j==0) T.element(i,j) = 1.0;
    }
    }*/
  
  /*エルゴディック
    for(int i=0;i<Number;i++){
    if(i) P.element(i) = 0.0;
    else P.element(i) = 1.0;
    }
    for(int i=0;i<Number;i++){
    for(int j=0;j<Number;j++){
    T.element(i,j) = 1.0;
      }
      }
  */
  
  /*モデルの作成*/
  HMM model(Number,Dimention,MIX,P,T);
  
  Matrix* train = new Matrix[TRAINING];
  for(int N=0;N<CLASS;N++){
    sprintf(str, "./Result%d_%d/Result_%d.txt", Number, MIX, N+1);
    fout.open(str);

    if(!fout){
      cout << "Can't open input file: " << str << "\n";
      return 1;
    }
    
    
    /*reading training data*/
    for(int j=0;j<TRAINING;j++){
      if(N+1<10){
	if(j<7) sprintf(str, "./mcep1542/mcep0%d/000%d0%d.mcep", j+1, N+1, j+1);
	else sprintf(str, "./sansou_data/000%d%d.mcep", N+1, j+4);
      }
      else if(N+1<100){
	if(j<7) sprintf(str, "./mcep1542/mcep0%d/00%d0%d.mcep", j+1, N+1, j+1);
	else sprintf(str, "./sansou_data/00%d%d.mcep", N+1, j+4);
      }
      else if(N+1<1000){
	if(j<7) sprintf(str, "./mcep1542/mcep0%d/0%d0%d.mcep", j+1, N+1, j+1);
	else sprintf(str, "./sansou_data/0%d%d.mcep", N+1, j+4);
      }

      fin.open(str, ios::in|ios::binary);
      
      if(!fin){
	cout << "Can't open input file: " << str << "\n";
	return 1;
      }
      l=0;
      while(!fin.eof()){
	l++;
	fin.read((char *) &number , sizeof(float));
      }
      fin.clear();
      fin.seekg(0, ios::beg);
      if(!fin.good()) return 1;
      
      train[j].ReSize(Dimention,(l-1)/Dimention);
      
      for(int t=0;t<(l-1)/Dimention;t++){
	for(int k=0;k<Dimention;k++){
	  /*if(!k) fin.read((char *) &tmp, sizeof(float));
	    else{*/
	  fin.read((char *) &tmp, sizeof(float));
	  train[j].element(k,t) = (double) tmp;
	  //}
	}
      }
      
      fin.close();
    }
    
    //model.init_k_mean(train,TRAINING);
    model.init_segmental(train,TRAINING);
    //model.set_HMM(train[0]);

    
    cout << "No. " << N+1 << "\n";
    model.estimations(train,TRAINING,1);

    

    //cout << model.viterbi_p(obs[i]) << "\n";

    
    /* Viterbi algorithm
      Int1D q = AllocInt_1D(train[0].Ncol());
      model.viterbi_path(train[0], q);
      cout << model.viterbi_p(train[0]) << "\n";
      for(int i=0;i<train[0].Ncols()-1;i++){
      cout << q[i] << " ";
      }
      cout << "\n";
      FreeInt_1D(q);
    */
    
    /*reading test data*/
    
    for(int i2=0;i2<CLASS;i2++){
      for(int j2=0;j2<6;j2++){
	if(i2+1<10){
	  if(j2<2) sprintf(str, "./mcep1542/mcep0%d/000%d0%d.mcep", j2+8, i2+1, j2+8);
	  else if(j2==2) sprintf(str, "./mcep1542/mcep%d/000%d%d.mcep", 10, i2+1, 10);
	  else sprintf(str, "./sansou_data/000%d%d.mcep", i2+1, j2+15);
	}
	else if(i2+1<100){
	  if(j2<2) sprintf(str, "./mcep1542/mcep0%d/00%d0%d.mcep", j2+8, i2+1, j2+8);
	  else if(j2==2) sprintf(str, "./mcep1542/mcep%d/00%d%d.mcep", 10, i2+1, 10);
	  else sprintf(str, "./sansou_data/00%d%d.mcep", i2+1, j2+15);
	}
	else if(i2+1<1000){
	  if(j2<2) sprintf(str, "./mcep1542/mcep0%d/0%d0%d.mcep", j2+8, i2+1, j2+8);
	  else if(j2==2) sprintf(str, "./mcep1542/mcep%d/0%d%d.mcep", 10, i2+1, 10);
	  else sprintf(str, "./sansou_data/0%d%d.mcep", i2+1, j2+15);
	}


	fin.open(str, ios::in|ios::binary);
	if(!fin){
	  cout << "Can't open input file: " << str << "\n";
	  return 1;
	}
      
	l=0;
	while(!fin.eof()){
	  l++;
	  fin.read((char *) &number , sizeof(float));
	}
	fin.clear();
	fin.seekg(0, ios::beg);
	if(!fin.good()) return 1;
	
	test.ReSize(Dimention,(l-1)/Dimention);
	
	for(int t=0;t<(l-1)/Dimention;t++){
	  for(int k=0;k<Dimention;k++){
	    /*if(!k) fin.read((char *) &tmp, sizeof(float));
	    else{*/
	      fin.read((char *) &tmp, sizeof(float));
	      test.element(k,t) = (double) tmp;
	      //}
	  }
	}
	
	fout << model.viterbi_p(test) << " ";
	//cout << str << " " << "\n";
	fin.close();
      }
    }
    
    fout.close();
    
    for(int i=0;i<TRAINING;i++)  train[i].CleanUp();
    //model.graph_out("./test.dot");
  }
  test.CleanUp();
  delete[] train;
  model.CleanUp();
  
  cout << check() << "\n";
  
  return 0;
}

