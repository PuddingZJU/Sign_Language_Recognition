#ifndef VectorR_h__
#define VectorR_h__

#include "VectorT.h"

template<class T>
class VectorR
{
private:
	int _size;
	int _span;
	T* _data;
public:
	//Default constructor
	VectorR(void)
		:_size(0),_span(1),_data(0)
	{
	}

	//No copy constructor

	//Constructor: from an array
	VectorR(T* arrayData, int size, int span = 1)
	{
		fromArray(arrayData,size,span);
	}

	//Constructor: from a VectorT
	template<int SIZE>
	VectorR(const VectorT<T,SIZE>& aVectorT)
	{
		fromVectorT(aVectorT);
	}

	void fromArray(const T* arrayData, int size, int span = 1)
	{
		_data = arrayData;
		_size = size;
		_span = span;
	}

	template<int SIZE>
	void fromVectorT(const VectorT<T,SIZE>& aVectorT)
	{
		_data = aVectorT.getDataPointer();
		_size = aVectorT.getSize();
		_span = 1;
	}

	//Get the size of the vector
	int getSize() const
	{
		return _size;
	}

	int getSpan() const
	{
		return _span;
	};

	//Get the const data pointer in the vector.
	//Take care of the span!
	const T* getDataPointer() const
	{
		return _data;
	}

	//Get the data pointer in the vector
	//Take care of the span!
	T* getDataPointer()
	{
		return _data;
	}

	//Get a copy of VectorT from this VectorR
	template<int SIZE>
	VectorT<T, SIZE> toVectorT()
	{
		assert(_size > SIZE);

		VectorT<T, SIZE> aVectorT;
		for(int i=0; i<SIZE; i++)
		{
			//Do not use _data[i] here directly because of the span!
			aVectorT[i] = (*this)[i];
		}

		return aVectorT;
	}

	//Const subscript
	const T& operator[](size_t index) const
	{
		return _data[index * _span];
	}

	//subscript
	T& operator[](size_t index)
	{
		return _data[index * _span];
	}

	//Set every element zero
	void setZero()
	{
		for(int i=0; i<_size; i++)
		{
			//Do not use _data[i] here directly because of the span!
			(*this)[i]= 0;
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
		if(norm>0)
		{
			for(int i=0; i<_size; i++)
			{
				//Do not use _data[i] here directly because of the span!
				(*this)[i] = (*this)[i] / norm;
			}
		}
	};

	//Get the reference of the min and max of the vector
	int getMaxIndex() const
	{
		int indexMax = 0;
		for(int i=1; i<_size; i++)
		{
			if( (*this)[i] > (*this)[indexMax] )
			{
				indexMax = i;
			}
		}
		return indexMax;
	};

	T& getMax()
	{
		return (*this)[getMaxIndex()];
	};

	const T& getMax() const
	{
		return (*this)[getMaxIndex()];
	};

	int getMinIndex() const
	{
		int indexMin = 0;
		for(int i=1; i<_size; i++)
		{
			if( (*this)[i] < (*this)[indexMin] )
			{
				indexMin = i;
			}
		}
		return indexMin;
	};

	T& getMin()
	{
		return (*this)[getMinIndex()];
	};

	const T& getMin() const
	{
		return (*this)[getMinIndex()];
	};

	//Negative
	void negative()
	{
		for(int i=0; i<_size; i++)
		{
			(*this)[i] = -(*this)[i];
		}
	}

	//Plus: This = one + two
	void add(const VectorR<T>& one, const VectorR<T>& two)
	{
		assert(_size == one.getSize() && _size == two.getSize());

		for(int i=0; i<_size; i++)
		{
			(*this)[i] = one[i] + two[i];
		}
	}

	//Plus: This += aVectorR
	void add(const VectorR<T>& aVectorR)
	{
		assert(_size == aVectorR.getSize());

		for(int i=0; i<_size; i++)
		{
			(*this)[i] += aVectorR[i];
		}
	}

	//ScalePlus: This = one + two * scaleTwo
	void add(const VectorR<T>& one, const VectorR<T>& two, T scaleTwo)
	{
		assert(_size == one.getSize() && _size == two.getSize());

		for(int i=0; i<_size; i++)
		{
			(*this)[i] = one[i] + two[i] * scaleTwo;
		}
	}

	//ScalePlus: This += aVectorR * scale
	void add(const VectorR<T>& aVectorR, T scale)
	{
		assert(_size == aVectorR.getSize());

		for(int i=0; i<_size; i++)
		{
			(*this)[i] += aVectorR[i] * scale;
		}
	}

	//Minus: This = one - two
	void sub(const VectorR<T>& one, const VectorR<T>& two)
	{
		assert(_size == one.getSize() && _size == two.getSize());

		for(int i=0; i<_size; i++)
		{
			(*this)[i] = one[i] - two[i];
		}
	}

	//Minus: This -= aVectorR
	void sub(const VectorR<T>& aVectorR)
	{
		assert(_size == aVectorR.getSize());

		for(int i=0;i<_size;i++)
		{
			(*this)[i] -= aVectorR[i];
		}
	}

	//ScaleMinus: This = one - two * scaleTwo
	void sub(const VectorR<T>& one, const VectorR<T>& two, T scaleTwo)
	{
		assert(_size == one.getSize() && _size == two.getSize());

		for(int i=0;i<_size;i++)
		{
			(*this)[i] = one[i] - two[i] * scaleTwo;
		}
	}

	//ScaleMinus: This -= aVectorR * scale
	void sub(const VectorR<T>& aVectorR, T scale)
	{
		assert(_size == aVectorR.getSize());

		for(int i=0; i<_size; i++)
		{
			(*this)[i] -= aVectorR[i] * scale;
		}
	}

	//Scale
	void scale(T scaleValue)
	{
		for(int i=0; i<_size; i++)
		{
			(*this)[i] *= scaleValue;
		}
	}
};


//Distance: for VectorR only
template<class T>
T Distance(const VectorR<T>& left,const VectorR<T>& right)
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

//Dot: for VectorR only
//template<class T>
//T Dot(const VectorR<T>& left, const VectorR<T>& right)
//{
//	assert(left.getSize() == right.getSize());
//
//	T sum = 0;
//	for(int i=0; i<left.getSize(); i++)
//	{
//		sum += left[i] * right[i];
//	}
//
//	return sum;
//}

////Swap: for VectorR only
//template<class T>
//void Swap(const VectorR<T>& left, const VectorR<T>& right)
//{
//	assert(left.getSize() == right.getSize());
//
//	for(int i=0; i<left.getSize(); i++)
//	{
//		std::swap(left,right);
//	}
//}

//Equal: for VectorR only
template<class T>
bool operator==(const VectorR<T>& left, const VectorR<T>& right)
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

#endif // VectorR_h__
