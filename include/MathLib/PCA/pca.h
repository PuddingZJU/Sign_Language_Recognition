// file: pca.h
// Copyright (c) 2007 Jyar
//
// Principal component analysis

#ifndef __PCA_H__
#define __PCA_H__

#include "imatrix.h"

struct pvector{
	union{
		float v[3];

		struct{
			float x, y, z;
		};
	};
};

class pca{
public: // private members
	int count;		// number of vectors
	int n;			// dimension of each vector
	int subSpace;	// number of the eigenvector
	float *covariance;
	float *mean;
	float *eigenvector;
	float *eigenvalue;

//	std::list<pvector> *vectors;

public: // public members
	// constructor & destructor
	pca(int pn);//, int psub);
	~pca();

	void clear();

	// add vector
	void addVector(float *pv);

	// pca calculate
	int pcaCal();

	// vector mat multiply
	void vecMatMultiply(float *v, float *m);

	int getSubSpace();
	void setSubSpace(int psub);

	float *getEigenvector();

	float* getEigenvalue();

//	std::list<pvector> *getVectors();
};

// class for 4x4 matrices
template <typename T>
class matrix3 {
	T m[9];
public:

	// initialization
	matrix3() {
		clear();
	}

	inline void clear() {
		memset(m, 0, sizeof(m));
	}

	matrix3(const matrix3<T> &m1) {
		for (unsigned int i = 0; i < 9; i ++) m[i] = m1[i];
	}

	matrix3(const T m1[]) {
		for (unsigned int i = 0; i < 9; i ++) m[i] = m1[i];
	}

	// transpose
	matrix3<T> transpose(){
		matrix3<T> m2;
		for(unsigned int row=0; row<3; row++){
			for(unsigned int col=0; col<3; col++){
				m2(row, col) = m(col*3 + row);
			}
		}
		return m2;
	}

	// access elements
	T& operator()(unsigned int row, unsigned col) {
		if ((row > 2) && (col > 2)) throw badIndex("Matrix subscript out of bounds");
		return m[row * 3 + col];
	}

	T operator()(unsigned int row, unsigned col) const {
		if ((row > 2) && (col > 2)) throw badIndex("const Matrix subscript out of bounds");
		return m[row * 3 + col];
	}

	T& operator[](unsigned int idx) {
		if (idx > 8)	throw badIndex("Matrix subscript out of bounds");
		return m[idx];
	}

	T operator[](unsigned int idx) const {
		if (idx > 8)	throw badIndex("const Matrix subscript out of bounds");
		return m[idx];
	}

	// addition
	matrix3<T> operator+(const matrix3<T> &m1) {
		matrix3<T> m2;
		for (unsigned int i = 0; i < 9; i ++) m2[i] = m[i] + m1[i];
		return m2;
	}

	// multiplication with another 4x4 matrix
	matrix3<T> operator*(const matrix3<T> &m1) {
		matrix3<T> m2;
		unsigned baseIdx;
		for (unsigned int row = 0; row < 3; row ++) {
			baseIdx = row * 3;
			for (unsigned int col = 0; col < 3; col ++) {
				m2(row, col) =
					m[baseIdx + 0] * m1(0, col) +
					m[baseIdx + 1] * m1(1, col) +
					m[baseIdx + 2] * m1(2, col);
			}
		}
		return m2;
	}

	// multiplication with a vector(m X v)
	iVector<T> operator*(const iVector<T> &v1) {
		iVector<T> v2;
		v2.x = v1.x * m[0] + v1.y * m[1] + v1.z * m[2];
		v2.y = v1.x * m[3] + v1.y * m[4] + v1.z * m[5];
		v2.z = v1.x * m[6] + v1.y * m[7] + v1.z * m[8];
		return v2;
	}

	// assign a value
	matrix3<T>& operator=(const matrix3<T> &m1) {
		for (unsigned int i = 0; i < 9; i ++) m[i] = m1[i];
		return *this;
	}

	// the line below is important!
	#ifdef __GNUC__
	friend ostream& operator<<(ostream &out, matrix3<T> &temp) {
	#else
	friend ostream& operator<< <T>(ostream &out, matrix3<T> &temp) {
	#endif
		temp.output(out);
		return out;
	}

private:
	// overriding standard output operator
	void output(ostream &out) {
		unsigned int idx = 0;
		out << "[";
		for (unsigned int row = 0; row < 3; row ++) {
			for (unsigned int col = 0; col < 3; col ++) {
				out << "\t" << m[idx ++];
			}
			if (2 == row) out << "\t]";
			out << endl;
		}
	}
};

#endif  