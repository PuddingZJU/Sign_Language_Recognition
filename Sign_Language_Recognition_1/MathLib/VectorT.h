#ifndef VectorT_h__
#define VectorT_h__

#include <cmath>
#include <cassert>

#include "MatrixT.h"

template<class T, int SIZE>
class VectorT
{
protected:
	T _data[SIZE];
public:
	//Default constructor
	VectorT(void)
	{
	}

	//Constructor:copy from an array
	VectorT(const T* arrayData)
	{
		copyFromArray(arrayData);
	}

	//Do not implement Copy-constructor and Assignment-overloading

	//Fill the vector with elements from an array
	void copyFromArray(const T* arrayData)
	{
		for(int i=0; i<SIZE; i++)
		{
			_data[i] = arrayData[i];
		}
	}

	//Fill the vector with elements from a matrix
	void copyFromMatrix(const MatrixT<T, SIZE, 1>& mat)
	{
		for(int i=0;i<SIZE;i++)
		{
			_data[i] = mat[i];
		}
	}

	//Get the size of the vector
	int getSize() const
	{
		return SIZE;
	}

	//Subscript
	inline T& operator[](size_t index)
	{
		return _data[index];
	}

	//Const subscript
	const T& operator[](size_t index) const
	{
		return _data[index];
	}

	//Set every element zero
	void setZero()
	{
		for(int i=0; i<SIZE; i++)
		{
			_data[i] = 0;
		}
	}

	//Get the data pointer in the vector
	T* getDataPointer()
	{
		return _data;
	}

	//Get the const data pointer in the vector
	const T* getDataPointer() const
	{
		return _data;
	}

	//Get a matrix from the vector
	MatrixT<T,SIZE,1> toMatrix()
	{
		MatrixT<T, SIZE, 1> retMat;
		for(int i=0;i<SIZE;i++)
		{
			retMat[i] = _data[i];
		}
		return retMat;
	}

	T getNormSquare() const
	{
		return Dot(*this,*this);
	}

	T getNorm() const
	{
		return sqrt(getNormSquare());
	}

	void normalise()
	{
		T norm = getNorm();
		if(norm > 0)
		{
			for(int i=0; i<SIZE; i++)
			{
				_data[i] = _data[i] / norm;
			}
		}
	};


	//Get the reference of the min and max of the vector
	int getMaxIndex() const
	{
		int indexMax = 0;
		for(int i=1; i<SIZE; i++)
		{
			if(_data[i] > _data[indexMax])
			{
				indexMax = i;
			}
		}
		return indexMax;
	};
	
	T& getMax()
	{
		return _data[getMaxIndex()];
	};

	const T& getMax() const
	{
		return _data[getMaxIndex()];
	};

	int getMinIndex() const
	{
		int indexMin = 0;
		for(int i=1; i<SIZE; i++)
		{
			if(_data[i] < _data[indexMin])
			{
				indexMin = i;
			}
		}
		return indexMin;
	};

	T& getMin()
	{
		return _data[getMinIndex()];
	};

	const T& getMin() const
	{
		return _data[getMinIndex()];
	};

	//Negative
	//void negative()
	//{
	//	for(int i=0; i<SIZE; i++)
	//	{
	//		_data[i] = -_data[i];
	//	}
	//}

	//added by lxb
	VectorT<T, SIZE> negative() const
	{
		VectorT<T, SIZE> vminus;
		for(int i=0; i<SIZE; i++)
		{
			vminus._data[i] = -_data[i];
		}
		return vminus;
	}

	//Add
	VectorT<T, SIZE>& operator+= (const VectorT<T,SIZE>& v)
	{
		for(int i=0; i<SIZE; ++i)
		{
			_data[i] += v._data[i];
		}

		return *this;
	}

	//Sub
	VectorT<T, SIZE>& operator-= (const VectorT<T,SIZE>& v)
	{
		for (int i=0; i<SIZE; ++i)
		{
			_data[i] -= v._data[i];
		}

		return *this;
	}
	
	
	//Get a copy of the sub vector
	template<int SUBSIZE>
	inline VectorT<T, SUBSIZE> getSubVector() const
	{
		assert(SUBSIZE <= SIZE);

		VectorT<T, SUBSIZE> subVec;
		for(int i=0; i<SUBSIZE; i++)
		{
			subVec[i] = _data[i];
		}
		return subVec;
	}

	//Do not implement the destructor
};

//Distance: for VectorT only
template<class T, int SIZE>
T Distance(const VectorT<T, SIZE>& left,const VectorT<T, SIZE>& right)
{
	T sum = 0;
	for(int i = 0; i<SIZE; i++)
	{
		T diff = left[i] - right[i];
		sum += diff * diff;
	}

	return sqrt(sum);
}

//Dot: for VectorT only
template<class T, int SIZE>
T Dot(const VectorT<T, SIZE>& left,const VectorT<T, SIZE>& right)
{
	T sum = 0;
	for(int i=0; i<SIZE; i++)
	{
		sum += left[i] * right[i];
	}
	return sum;
}

//Scale: for VectorT only
template<class T, int SIZE>
VectorT<T, SIZE> operator*(const VectorT<T, SIZE>& vec,T scale)
{
	VectorT<T,SIZE> scaledVec;
	for(int i=0; i<SIZE; i++)
	{
		scaledVec[i] = vec[i] * scale;
	}
	return scaledVec;
}

template<class T, int SIZE>
VectorT<T, SIZE> operator*(T scale, const VectorT<T,SIZE>& vec)
{
	return vec * scale;
}

template<class T, int SIZE>
VectorT<T, SIZE> operator+(const VectorT<T,SIZE>& v1, const VectorT<T,SIZE>& v2)
{
	VectorT<T,SIZE> t;
	t = v1;
	t += v2;
	return t;
}

template<class T, int SIZE>
VectorT<T, SIZE> operator-(const VectorT<T,SIZE>& v1, const VectorT<T,SIZE>& v2)
{
	VectorT<T,SIZE> t;
	t = v1;
	t -= v2;
	return t;
}


//M*V
template<class T, int ROWS, int COLS>
VectorT<T, ROWS> operator*(const MatrixT<T, ROWS, COLS>& mat,const VectorT<T, ROWS>& vec)
{
	VectorT<T, ROWS> retVec;
	for(int i=0; i<ROWS; i++)
	{
		retVec[i] = 0;
		for(int j=0; j<COLS; j++)
		{
			retVec[i] += mat(i,j) * vec[j];
		}
	}
	return retVec;
}

#endif // VectorT_h__
