////////////////////////////////////////////////////////////////////////////
//
//  iMatrix
//  Copyright(c) Erik Chang
//
////////////////////////////////////////////////////////////////////////////
//
// $Id$
//
////////////////////////////////////////////////////////////////////////////

#ifndef __IMATRIX_H__
#define __IMATRIX_H__

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>

using namespace std;

template <typename T>
class iMatrix;

class badIndex;

const float PI = 3.1415926535897932384626433832795F;
inline float toDegrees(float rad) { return (rad * 180 / PI); }
inline float toRadians(float deg) { return (deg * PI / 180); }

// class for vectors with 3 elements
template <typename T>
class iVector {
public:
	T x, y, z;

	// initialization
	iVector() {
		clear();
	}

	inline void clear() {
		x = y = z = 0;
	}

	iVector(const iVector<T> &v1) : x(v1.x), y(v1.y), z(v1.z) {}

	iVector(T xx, T yy, T zz) :	x(xx), y(yy), z(zz) {}

	// addition
	iVector<T> operator+(const iVector<T> &v1) {
		iVector<T> v2;
		v2.x = x + v1.x;
		v2.y = y + v1.y;
		v2.z = z + v1.z;
		return v2;
	}

	// substraction
	iVector<T> operator-(const iVector<T> &v1) {
		iVector<T> v2;
		v2.x = x - v1.x;
		v2.y = y - v1.y;
		v2.z = z - v1.z;
		return v2;
	}

	// scaling with a parameter
	iVector<T> operator*(const T &a) {
		iVector<T> v2;
		v2.x = x * a;
		v2.y = y * a;
		v2.z = z * a;
		return v2;
	}

	// dot product
	T operator*(const iVector<T> &v1) {
		T i;
		i = x * v1.x + y * v1.y + z * v1.z;
		return i;
	}

	// cross product
	iVector<T> operator^(const iVector<T> &v1) {
		iVector<T> v2;
		v2.x = y * v1.z - z * v1.y;
		v2.y = z * v1.x - x * v1.z;
		v2.z = x * v1.y - y * v1.x;
		return v2;
	}

	// assign a value
	iVector<T>& operator=(const iVector<T> &v1) {
		x = v1.x;
		y = v1.y;
		z = v1.z;
		return *this;
	}

	// add a value
	iVector<T>& operator+=(const iVector<T> &v1) {
		x += v1.x;
		y += v1.y;
		z += v1.z;
		return *this;
	}

	// sub a value
	iVector<T>& operator-=(const iVector<T> &v1) {
		x -= v1.x;
		y -= v1.y;
		z -= v1.z;
		return *this;
	}

	// multiple a value
	iVector<T>& operator*=(const T &a) {
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}

	// distance
	T distance() {
		return sqrt(x * x + y * y + z * z);
	}

	// 3d geometry transformation
	// tanslation
	iVector<T> translate(T tx, T ty, T tz) {
		iVector<T> v1;
		iMatrix<T> m1;
		m1(0, 0) = 1;
		m1(1, 1) = 1;
		m1(2, 2) = 1;
		m1(3, 3) = 1;
		m1(0, 3) = tx;
		m1(1, 3) = ty;
		m1(2, 3) = tz;
		v1 = m1 * *this;
		return v1;
	}

	// rotation
	iVector<T> rotateByAxisX(T theta) {
		iVector<T> v1;
		iMatrix<T> m1;
		T cosTheta = cos(theta);
		T sinTheta = sin(theta);
		m1(0, 0) = 1;
		m1(1, 1) = cosTheta;
		m1(1, 2) = sinTheta;
		m1(2, 1) = - sinTheta;
		m1(2, 2) = cosTheta;
		m1(3, 3) = 1;
		v1 = m1 * *this;
		return v1;
	}

	iVector<T> rotateByAxisZ(T theta) {
		iVector<T> v1;
		iMatrix<T> m1;
		T cosTheta = cos(theta);
		T sinTheta = sin(theta);
		m1(0, 0) = cosTheta;
		m1(0, 1) = sinTheta;
		m1(1, 0) = - sinTheta;
		m1(1, 1) = cosTheta;
		m1(2, 2) = 1;
		m1(3, 3) = 1;
		v1 = m1 * *this;
		return v1;
	}

	iVector<T> rotateByAxisY(T theta) {
		iVector<T> v1;
		iMatrix<T> m1;
		T cosTheta = cos(theta);
		T sinTheta = sin(theta);
		m1(0, 0) = cosTheta;
		m1(0, 2) = - sinTheta;
		m1(1, 1) = 1;
		m1(2, 0) = sinTheta;
		m1(2, 2) = cosTheta;
		m1(3, 3) = 1;
		v1 = m1 * *this;
		return v1;
	}

	// scaling
	iVector<T> scale(T sx, T sy, T sz) {
		iVector<T> v1;
		iMatrix<T> m1;
		m1(0, 0) = sx;
		m1(1, 1) = sy;
		m1(2, 2) = sz;
		m1(3, 3) = 1;
		v1 = m1 * *this;
		return v1;
	}

	// the line below is important!
	#ifdef __GNUC__
	friend ostream& operator<<(ostream &out, iVector<T> &temp) {
	#else
	friend ostream& operator<< <T>(ostream &out, iVector<T> &temp) {
	#endif
		temp.output(out);
		return out;
	}

private:
	// overriding standard output operator
	void output(ostream &out) {
		out << "(" << x << ", " << y << ", " << z << ")";
	}
};

// class for 4x4 matrices
template <typename T>
class iMatrix {
	T m[16];
public:

	// initialization
	iMatrix() {
		clear();
	}

	inline void clear() {
		memset(m, 0, sizeof(m));
	}

	iMatrix(const iMatrix<T> &m1) {
		for (unsigned int i = 0; i < 16; i ++) m[i] = m1[i];
	}

	iMatrix(const T m1[]) {
		for (unsigned int i = 0; i < 16; i ++) m[i] = m1[i];
	}

	// access elements
	T& operator()(unsigned int row, unsigned col) {
		if ((row > 3) && (col > 3)) throw badIndex("Matrix subscript out of bounds");
		return m[row * 4 + col];
	}

	T operator()(unsigned int row, unsigned col) const {
		if ((row > 3) && (col > 3)) throw badIndex("const Matrix subscript out of bounds");
		return m[row * 4 + col];
	}

	T& operator[](unsigned int idx) {
		if (idx > 15)	throw badIndex("Matrix subscript out of bounds");
		return m[idx];
	}

	T operator[](unsigned int idx) const {
		if (idx > 15)	throw badIndex("const Matrix subscript out of bounds");
		return m[idx];
	}

	// addition
	iMatrix<T> operator+(const iMatrix<T> &m1) {
		iMatrix<T> m2;
		for (unsigned int i = 0; i < 16; i ++) m2[i] = m[i] + m1[i];
		return m2;
	}

	// multiplication with another 4x4 matrix
	iMatrix<T> operator*(const iMatrix<T> &m1) {
		iMatrix<T> m2;
		unsigned baseIdx;
		for (unsigned int row = 0; row < 4; row ++) {
			baseIdx = row * 4;
			for (unsigned int col = 0; col < 4; col ++) {
				m2(row, col) =
					m[baseIdx + 0] * m1(0, col) +
					m[baseIdx + 1] * m1(1, col) +
					m[baseIdx + 2] * m1(2, col) +
					m[baseIdx + 3] * m1(3, col);
			}
		}
		return m2;
	}

	// multiplication with a vector
	iVector<T> operator*(const iVector<T> &v1) {
		iVector<T> v2;
		v2.x = v1.x * m[0] + v1.y * m[1] + v1.z * m[2] + m[3];
		v2.y = v1.x * m[4] + v1.y * m[5] + v1.z * m[6] + m[7];
		v2.z = v1.x * m[8] + v1.y * m[9] + v1.z * m[10] + m[11];
		return v2;
	}

	// assign a value
	iMatrix<T>& operator=(const iMatrix<T> &m1) {
		for (unsigned int i = 0; i < 16; i ++) m[i] = m1[i];
		return *this;
	}

	// the line below is important!
	#ifdef __GNUC__
	friend ostream& operator<<(ostream &out, iMatrix<T> &temp) {
	#else
	friend ostream& operator<< <T>(ostream &out, iMatrix<T> &temp) {
	#endif
		temp.output(out);
		return out;
	}

private:
	// overriding standard output operator
	void output(ostream &out) {
		unsigned int idx = 0;
		out << "[";
		for (unsigned int row = 0; row < 4; row ++) {
			for (unsigned int col = 0; col < 4; col ++) {
				out << "\t" << m[idx ++];
			}
			if (3 == row) out << "\t]";
			out << endl;
		}
	}
};

// class for exception
class badIndex {
	string msg;		// message of exception
public:
	badIndex() {};
	badIndex(string str) : msg(str) {}
};

#endif	// #define __IMATRIX_H__
