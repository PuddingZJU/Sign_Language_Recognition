#ifndef MatrixT_h__
#define MatrixT_h__

#include <cassert>

template<class T, int ROWS, int COLS>
class MatrixT
{
protected:
	T _data[ROWS*COLS];
public:
	//Default constructor
	MatrixT(void)
	{
	}

	//Constructor:copy from an array
	MatrixT(T* arrayData)
	{
		copyFromArray(arrayData);
	}

	//Do not implement Copy-constructor and Assignment-overloading

	//Fill the matrix with elements from an array
	void copyFromArray(T* arrayData)
	{
		for(int i=0; i<ROWS*COLS; i++)
		{
			_data[i] = arrayData[i];
		}
	}

	//Get the data pointer in the matrix
	T* getDataPointer()
	{
		return _data;
	}

	//Get the const data pointer in the matrix
	const T* getDataPointer() const
	{
		return _data;
	}

	//Get the number of columns in the matrix
	int getColumnCount()
	{
		return COLS;
	}

	//Get the rows of the matrix
	int getRowCount()
	{
		return ROWS;
	}

	//Get the size of the matrix
	int getSize()
	{
		return COLS * ROWS;
	}

	//Subscript
	T& operator()(int row, int column)
	{
		return _data[COLS * row + column];
	}

	//Const subscript
	const T& operator()(size_t row, size_t column) const
	{
		return _data[COLS * row + column];
	}

	//Array subscript
	T& operator[](size_t index)
	{
		return _data[index];
	}

	//Const array subscript
	const T& operator[](size_t index) const
	{
		return _data[index];
	}

	//Set every element zero
	void setZero()
	{
		for(int i=0; i<getSize(); i++)
		{
			_data[i] = 0;
		}
	}

	//TODO: getRow(row) get the reference vector of the row
	//TODO: getColumn(col) get the reference vector to the column

	//Get a copy of the sub matrix
	template<int SUBROWS, int SUBCOLS>
	MatrixT<T, SUBROWS, SUBCOLS> getSubMatrix() const
	{
		assert(SUBROWS <= ROWS);
		assert(SUBCOLS <= COLS);

		MatrixT<T, SUBROWS, SUBCOLS> subMat;

		for(int i=0; i<SUBROWS; i++)
		{
			for(int j=0; j<SUBCOLS; j++)
			{
				subMat(i, j) = (*this)(i, j);
			}
		}
		return subMat;
	}
	
	//Do not implement the destructor
};

////Negative: for both vector and matrix
//template<class T>
//T operator-(const T& value)
//{
//	T negValue;
//	for(int i=0; i<value.getSize(); i++)
//	{
//		negValue[i] = -value[i];
//	}
//	return negValue;
//}
//
////Plus: for both vector and matrix
//template<class T>
//T operator+(const T& left, const T& right)
//{
//	T sumValue;
//	for(int i=0; i<left.getSize(); i++)
//	{
//		sumValue[i] = left[i] + right[i];
//	}
//	return sumValue;
//}
//
////Minus: for both vector and matrix
//template<class T>
//T operator-(const T& left, const T& right)
//{
//	T diffValue;
//	for(int i=0; i<left.getSize(); i++)
//	{
//		diffValue[i]= left[i] - right[i];
//	}
//	return diffValue;
//}

//Multiply: for MatrixT only
template<class T, int ROWS1, int COLS1ROWS2, int COLS2>
MatrixT<T, ROWS1, COLS2> operator*(const MatrixT<T, ROWS1, COLS1ROWS2>& left, const MatrixT<T, COLS1ROWS2, COLS2>& right)
{
	MatrixT<T, ROWS1, COLS2> retMat;
	for(int i=0; i<ROWS1; i++)
	{
		for(int j=0; j<COLS2; j++)
		{
			retMat(i, j) = 0;
			for(int k = 0; k<COLS1ROWS2; k++)
			{
				retMat(i,j) += left(i,j) * right(i,j);
			}
		}
	}
	return retMat;
}


//never do such things inside namespace std!! T can be anything.

// //Equal: for all vector and matrix
//template<class T>
//bool operator==(const T& left,const T& right)
//{
//	bool bEqual = true;
//	for(int i=0; i<left.getSize(); i++)
//	{
//		if(left[i] != right[i])
//		{
//			bEqual = false;
//			break;
//		}
//	}
//	return bEqual;
//}

//Do we need a swap here?

/**
 * @brief Transpose
 */
template<class T, int ROWS, int COLS>
MatrixT<T, ROWS, COLS> & Transpose( MatrixT<T, ROWS, COLS> & m )
{
	assert( ROWS == COLS );
	for(int row = 0; row < ROWS; ++row){
		for(int col = 0; col < row; ++col){
			T t = m(col, row);
			m(col, row) = m(row, col);
			m(row, col) = t;
		}
	}

	return m;
}

#endif // MatrixT_h__
