#ifndef MatrixH_h__
#define MatrixH_h__

#include "../IKManager/NumericalTypes.h"
#include "../IKManager/NumericalRecipes.h"

template<class T>
class MatrixH
{
private:
	int _rows;
	int _cols;
	T* _data;
public:
	//Default constructor
	MatrixH(void)
		:_data(0),_rows(0),_cols(0)
	{
	}

	//Constructor: the size is decided
	MatrixH(int rows,int cols)
	{
		assert(rows >= 0);
		assert(cols >= 0);

		_rows = rows;
		_cols = cols;
		_data = new T[rows * cols];
	}

	MatrixH(int rows,int cols, T t)
	{
		assert(rows >= 0);
		assert(cols >= 0);

		_rows = rows;
		_cols = cols;
		_data = new T[rows * cols];

		for (int i=0; i<_rows; i++)
		{
			for (int j=0; j<_cols; j++)
			{
				_data[_rows*i+j] = t;
			}
		}
	}

	//Constructor: copy from an array
	MatrixH(T* arrayData,int rows,int cols)
	{
		assert(rows > 0);
		assert(cols > 0);

		_rows = rows;
		_cols = cols;
		_data = new T[rows * cols];
		copyFromArray(arrayData,rows,cols);
	}

	//Copy constructor
	MatrixH(const MatrixH& other)
	{
		_rows = other.getRowCount();
		_cols = other.getColumnCount();
		_data = new T[_rows*_cols];

		for(int i=0; i<_rows*_cols; i++)
		{
			_data[i] = other._data[i];
		}
	}

	//Assignment operator
	MatrixH& operator=(const MatrixH& other)
	{
		assert(_rows == other.getRowCount());
		assert(_cols == other.getColumnCount());

		if(this != &other)
		{
			for(int i=0; i<_rows*_cols; i++)
			{
				_data[i] = other._data[i];
			}
		}
		return *this;
	}

	void resize(int rows, int cols)
	{
		assert(rows >= 0);
		assert(cols >= 0);
		T* pold = _data;

		_data = new T[rows * cols];
		for (int i=0; i<_rows; i++)
		{
			for (int j=0; j<_cols; j++)
			{
				_data[_rows*i+j] = pold[_rows*i+j];
			}
		}
		delete pold;

		_rows = rows;
		_cols = cols;
	}

	MatrixH<double> inverse()
	{
		LQL::MatrixMxN mat(_data, _rows, _cols);
		LQL::MatrixMxN inversemat = LQL::inverse(mat);
		MatrixH<double> matrix(inversemat.getDataPointer(), _rows, _cols);

		return matrix;
	}

	double determinant()
	{
		double d;
		int n=_cols;
		LQL::MatrixMxN mat(_data,n,n);
		LQL::VectorN indx(n);
		LQL::ludcmp(mat,indx,d);

		for (int j=0; j<n; j++ )
		{
			d *= mat(j,j);
		}
		return d;
	}

	void setIdentity(int matSize)
	{
		resize(matSize, matSize);
		setZero();
		for (unsigned int i=0; i<matSize; i++)
			_data[matSize*i+i] = 1;
	}

	//Fill the matrix with elements from an array
	void copyFromArray(T* arrayData,int rows,int cols)
	{
		assert(rows == _rows);
		assert(cols == _cols);

		for(int i=0; i<_rows*_cols; i++)
		{
			_data[i] = arrayData[i];
		}
	}

	//Get the number of columns in the matrix
	int getColumnCount() const
	{
		return _cols;
	}

	//Get the rows of the matrix
	int getRowCount() const
	{
		return _rows;
	}

	//Get an array pointer from the matrix
	T* getDataPointer()
	{
		return _data;
	}

	//Get a const array pointer from the matrix
	const T* getDataPointer() const
	{
		return _data;
	}

	//Get the total size of the matrix
	int getSize() const
	{
		return _rows * _cols;
	}

	//Subscript
	T& operator()(size_t row, size_t column)
	{
		return _data[_cols * row + column];
	}

	//Const subscript
	const T& operator()(size_t row, size_t column) const
	{
		return _data[_cols * row + column];
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


	//Get a copy of the sub matrix
	MatrixH getSubMatrix(int subRows,int subCols) const
	{
		assert(subRows <= _rows);
		assert(subCols <= _cols);

		MatrixH subMat(subRows,subCols);
		for(int i=0;i<subRows;i++)
		{
			for(int j=0;j<subCols;j++)
			{
				subMat(i,j)=(*this)(i,j);
			}
		}
		return subMat;
	}

	//Destructor
	virtual ~MatrixH(void)
	{
		delete[] _data;
	}

	void setElement(int row, int col, T t)
	{
		_data[_rows*row+col] = t;
	}
};

//Negative
template<class T>
MatrixH<T> operator-(const MatrixH<T>& aMatrixH)
{
	int rows = aMatrixH.getRowCount();
	int cols = aMatrixH.getColumnCount();
	int size = aMatrixH.getSize();

	MatrixH<T>  negMat(rows,cols);

	for(int i=0; i<size; i++)
	{
		negMat[i] = -aMatrixH[i];
	}
	return negMat;
}

//PLUS
template<class T>
MatrixH<T> operator+(const MatrixH<T>& left,const MatrixH<T>& right)
{
	assert(left.getRowCount() == right.getRowCount());
	assert(left.getColumnCount() == right.getColumnCount());

	int rows = left.getRowCount();
	int cols = left.getColumnCount();
	int size = left.getSize();

	MatrixH<T> sumMat(rows,cols);

	for(int i=0; i<size; i++)
	{
		sumMat[i] = left[i] + right[i];
	}
	return sumMat;
}

//MINUS
template<class T>
MatrixH<T> operator-(const MatrixH<T>& left,const MatrixH<T>& right)
{
	assert(left.getRowCount() == right.getRowCount());
	assert(left.getColumnCount() == right.getColumnCount());

	int rows = left.getRowCount();
	int cols = left.getColumnCount();
	int size = left.getSize();

	MatrixH<T> diffMat(rows,cols);

	for(int i=0; i<size; i++)
	{
		diffMat[i] = left[i] - right[i];
	}
	return diffMat;
}

//MULTI
template<class T>
MatrixH<T> operator*(const MatrixH<T>& left,const MatrixH<T>& right)
{
	assert(left.getColumnCount() == right.getRowCount());

	int rows = left.getRowCount();
	int cols = right.getColumnCount();

	int cols1_rows2 = left.getColumnCount();

	MatrixH<T> retMat(rows,cols);

	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
		{
			int sum = 0;
			for(int k=0; k<cols1_rows2; k++)
			{
				sum += left(i,k) * right(k,j);
			}
			retMat(i,j) = sum;
		}
	}
	return retMat;
}

template<class T>
MatrixH<T> operator*(const MatrixH<T> matrix, T d)
{
	int rows = matrix.getRowCount();
	int cols = matrix.getColumnCount();
	MatrixH<T> retMat(rows,cols);
	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
		{
			retMat(i,j) = matrix(i,j)*d;
		}
	}

	return retMat;
}

template<class T>
MatrixH<T> operator*(T d, const MatrixH<T> matrix)
{
	return matrix*d;
}

template<class T>
MatrixH<T> operator/(const MatrixH<T> matrix, double d)
{
	int rows = matrix.getRowCount();
	int cols = matrix.getColumnCount();
	MatrixH<T> retMat(rows,cols);
	for(int i=0; i<rows; i++)
	{
		for(int j=0; j<cols; j++)
		{
			retMat(i,j) = matrix(i,j)/d;
		}
	}

	return retMat;
}

//Equal: for MatrixH only
template<class T>
bool operator==(const MatrixH<T>& left, const MatrixH<T>& right)
{
	if(left.getRowCount() != right.getRowCount() || left.getColumnCount() != right.getColumnCount())
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

#endif // MatrixH_h__
