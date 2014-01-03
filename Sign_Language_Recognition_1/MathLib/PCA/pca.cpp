// file: pca.cpp
// Copyright (c) 2007 Jyar
//
// implementation of principal component analysis

#include <cmath>
#include <cstdlib>
#include <memory>
#include <list>
#include "pca.h"
using namespace std;

#define ROTATE(a,i,j,k,l)\
    float g=a[j + i*n];\
    float h=a[l + k*n];\
    a[j + i*n]=g-s*(h+g*tau);\
    a[l + k*n]=h+s*(g-h*tau);

// constructor for class pca
pca::pca(int pn) : n(pn), count(0){//, subSpace(psub){
	covariance = new float[sizeof(float)*n*n];
	mean = new float[sizeof(float)*n];
	eigenvector = new float[sizeof(float)*n*n];
	eigenvalue = new float[sizeof(float)*n];
//	vectors = new list<pvector>;
	clear();
}

// destructor
pca::~pca(){
	delete []covariance;
	delete []mean;
	delete []eigenvector;
	delete []eigenvalue;
}

// clear pca
void pca::clear(){
	memset(mean, 0, sizeof(float)*n);
	memset(covariance, 0, sizeof(float)*n*n);
	memset(eigenvalue, 0, sizeof(float)*n);
//	vectors->clear();
}

// add vector to pca
void pca::addVector(float *pv){
	int i, j;
//	pvector ppv;
	for(i=0; i<n; i++){
		mean[i] += pv[i];
		for(j=i; j<n; j++)
			covariance[j + i*n] += pv[i] * pv[j];
//		ppv.v[i] = pv[i];
	}
//	vectors->push_back(ppv);
	count++;
}

// pca calculate
int pca::pcaCal(){
	int i, j, k, pass;
	float *z = new float[sizeof(float)*n];

	memset(eigenvector, 0, sizeof(float)*n*n);

	for(j=0; j<n; j++){
		mean[j] /= count;
		eigenvector[j + j*n] = 1.0;
		for(i=0; i<=j; i++){
			covariance[j + i*n] -= mean[i] * mean[j] * count;
			covariance[j + i*n] /= (count - 1);
			covariance[i + j*n] = covariance[j + i*n];		// real symmetric
		}
		eigenvalue[j] = covariance[j + j*n];
		z[j] = 0;
	}

	for(pass=0; pass<50; pass++){
		float sum = 0;

		for(i=0; i<n; i++)
			for(j=i+1; j<n; j++)
				sum += fabs(covariance[j + i*n]);
		
		if(sum == 0){
			for(i=0; i<n; i++){
				float maxvalue = -1;
				for(j=i; j<n; j++){
					if(eigenvalue[j] > maxvalue){
						maxvalue = eigenvalue[j];
						k = j;
					}
				}
				eigenvalue[k] = eigenvalue[i];
				eigenvalue[i] = maxvalue;
				for(j=0; j<n; j++){
					float tmp = eigenvector[k + j*n];
					eigenvector[k + j*n] = eigenvector[i + j*n];
					eigenvector[i + j*n] = tmp;
				}
			}
			return pass;
		}

		for(i=0; i<n; i++){
			for(j=i+1; j<n; j++){
				float covar = covariance[j + i*n];
				float t, c, s, tau, theta, h;

				if(pass<3 && fabs(covar)<sum/(5*n*n)) // FIXME why pass < 3
					continue;
				if(fabs(covar) == 0.0) // FIXME shouldnt be needed
					continue;
				if(pass >= 3 && fabs((eigenvalue[j] + z[j])/covar) > (1LL<<32) && fabs((eigenvalue[i]+z[i])/covar) > (1LL<<32)){
					covariance[j + i*n] = 0.0;
					continue;
				}

				h = (eigenvalue[j]+z[j]) - (eigenvalue[i]+z[i]);
				theta = 0.5 * h / covar;
				t = 1.0 / (fabs(theta) + sqrt(1.0 + theta*theta));
				if(theta < 0.0) t = -t;

				c = 1.0 / sqrt(1+t*t);
				s = t * c;
				tau = s / (1.0 + c);
				z[i] -= t * covar;
				z[j] += t * covar;

				for(k=0; k<n; k++){
					if(k!=i && k!=j){
						ROTATE(covariance,min(k,i),max(k,i),min(k,j),max(k,j));
					}
					ROTATE(eigenvector, k, i, k ,j);
				}
				covariance[j + i*n] = 0.0;
			}
		}
		for(i=0; i<n; i++){
			eigenvalue[i] += z[i];
			z[i] = 0.0;
		}
	}
/*
	list<pvector>::iterator it = vectors->begin();
	for(i=0; it!=vectors->end() && i<subSpace; it++,i++){
		vecMatMultiply((*it).v, eigenvector);
	}
*/
	return -1;
}

// multiply the original vectors to the eigenvector matrix
// the result store at the parameter v
void pca::vecMatMultiply(float *v, float *m){
	int i, j;
	float sum;
	float *tmpv = new float[3];
	for(i=0; i<n; i++){
		sum = 0.0;
		for(j=0; j<n; j++){
			sum += v[j] * m[i + j*n];
		}
		tmpv[i] = sum;
	}
	for(i=0; i<n; i++)
		v[i] = tmpv[i];

	delete []tmpv;
}

int pca::getSubSpace(){
	return subSpace;
}

void pca::setSubSpace(int psub){
	subSpace = psub;
}

float *pca::getEigenvector(){
	return eigenvector;
}

float* pca::getEigenvalue()
{
	return eigenvalue;
}

/*
list<pvector> *pca::getVectors(){
	return vectors;
}
*/