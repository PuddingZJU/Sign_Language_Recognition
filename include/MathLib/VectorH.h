#ifndef VectorH_h__
#define VectorH_h__

#include "VectorT.h"

template<class T>
class VectorH
{
private:
	int _size;
	T* _data;
public:
	//Default constructor
	VectorH(void)
		:_data(0),_size(0)
	{
	}

	//Constructor: the size is decided
	explicit inline VectorH(int size)
	{
		_size = size;
		_data = new T[_size];
	}

	//Constructor from an array
	VectorH(T* arrayData,int size)
	{
		_size = size;
		_data = new T[_size];
		copyFromArray(arrayData,size);
	}

	//Fill the matrix with elements from an array
	void copyFromArray(T* arrayData,int size)
	{
		assert(size == _size);

		for(int i=0;i<_size;i++)
		{
			_data[i] = arrayData[i];
		}
	}

	//Copy constructor
	VectorH(const VectorH& other)
	{
		_size = other.getSize();
		_data = new T[_size];
		for(int i=0;i<_size;i++)
		{
			_data[i] = other[i];
		}
	}

	//Constructor: from a VectorT
	template<int SIZE>
	VectorH(const VectorT<T,SIZE>& aVectorT)
	{
		_size = aVectorT.getSize();
		_data = new T[_size];

		for(int i=0;i<_size;i++)
		{
			_data[i] = aVectorT[i];
		}
	}

	//Constructor: from a VectorR
	VectorH(const VectorR<T>& aVectorR)
	{
		_size = aVectorR.getSize();
		_data = new T[_size];

		for(int i=0;i<_size;i++)
		{
			_data[i] = aVectorR[i];
		}
	}

	//Size of the vector
	int getSize() const
	{
		return _size;
	}

	//Get the const data pointer in the vector
	const T* getDataPointer() const
	{
		return _data;
	}

	//Get the data pointer in the vector
	T* getDataPointer()
	{
		return _data;
	}

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

	template<int SIZE>
	inline VectorT<T,SIZE> toVectorT()
	{
		assert(SIZE < _size);

		return VectorT<T,SIZE>(_data);
	}

	//Set every element zero
	void setZero()
	{
		for(int i=0; i<_size; i++)
		{
			_data[i] = 0;
		}
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

	//Negative
	void negative()
	{
		for(int i=0; i<_size; i++)
		{
			_data[i] = -_data[i];
		}
	}
};


//Distance
template<class T>
T Distance(const VectorH<T>& left,const VectorH<T>& right)
{
	assert(left.getSize() == right.getSize());

	T sum = 0;
	for(int i = 0;i < left.getSize();i++)
	{
		T diff = left[i] - right[i];
		sum += diff * diff;
	}

	return sqrt(sum);
}

//Dot
template<class T>
T Dot(const VectorH<T>& left,const VectorH<T>& right)
{
	assert(left.getSize() == right.getSize());

	T sum = 0;
	for(int i=0; i<left.getSize(); i++)
	{
		sum += left[i] * right[i];
	}

	return sum;
}

//Equal
template<class T>
bool operator==(const VectorH<T>& left,const VectorH<T>& right)
{
	if(left.getSize() != right.getSize())
	{
		return false;
	}

	bool bEqual = true;
	for(int i=0; i<left.getSize(); i++)
	{
		if(left[i] != right[i])
		{
			bEqual = false;
			break;
		}
	}
	return bEqual;
}

//////////////////////////////////////////////////////////////////////////


//Negative
template<class T>
VectorH<T> operator-(const VectorH<T>& aVec)
{
	int size = aVec.getSize();
	VectorH<T> negVec(size);
	for(int i=0;i<size;i++)
	{
		negVec[i] = -aVec[i];
	}
	return negVec;
}

//Scale Multiply
template<class T>
VectorH<T> operator*(const VectorH<T>& aVec, T scale)
{
	int size = aVec.getSize();
	VectorH<T> scaledVec(size);
	for(int i=0;i<size;i++)
	{
		scaledVec[i] = aVec[i] * scale;
	}
	return scaledVec;
}

template<class T>
inline VectorH<T> operator*(T scale,const VectorH<T>& aVec)
{
	return aVec * scale;
}

//Plus
template<class T>
inline VectorH<T> operator+(const VectorH<T>& left, const VectorH<T>& right)
{
	assert(left.getSize() == right.getSize());

	int size = left.getSize();

	VectorT<T> sumVec(size);
	for(int i=0;i<size;i++)
	{
		sumVec[i] = left[i] + right[i];
	}
	return sumVec;
}

//Minus
template<class T>
inline VectorH<T> operator-(const VectorH<T>& left, const VectorH<T>& right)
{
	assert(left.getSize() == right.getSize());

	int size = left.getSize();

	VectorT<T> diffVec(size);

	for(int i=0; i<size; i++)
	{
		diffVec[i] = left[i] - right[i];
	}
	return diffVec;
}

////M*V
//template<class T,int ROWS,int COLS>
//VectorT<T,ROWS> operator*(const MatrixT<T,ROWS,COLS>& mat,const VectorT<T,ROWS>& vec)
//{
//	VectorT<T,ROWS> retVec;
//	for(int i=0;i<ROWS;i++)
//	{
//		retVec[i] = 0;
//		for(int j=0;j<COLS;j++)
//		{
//			retVec[i] += mat[i][j]*vec[j];
//		}
//	}
//	return retVec;
//}

#endif // VectorH_h__
