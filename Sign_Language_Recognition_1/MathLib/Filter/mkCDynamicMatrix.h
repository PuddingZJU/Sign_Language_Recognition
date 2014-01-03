#ifndef _mkCDynamicMatrix_h
#define _mkCDynamicMatrix_h

//*************************************************************************************
/*! \class mkm::CDynamicMatrix
		\brief Cette classe définit une matrice de taille créée dynamiquement.
*/ //**********************************************************************************

//#include <mkBaseDefs.h>
//#include <mkBaseDll.h>
//#include "mkCDynamicVector.h"

#include <vector>

#define TINY 1.0e-20;
namespace mkm
{
	template <class Type> class CDynamicMatrix
	{
		public:
		//************************************************************
		//! @name Constructors et Destructor
		//************************************************************
		//@{
		//------------------------------------------------------------
		//! Default constructor
		//------------------------------------------------------------
		CDynamicMatrix()
			: _nbLines(0), _nbColumns(0), m(NULL)
		{
		}

		//------------------------------------------------------------
		//! Constructor with size
		/*! \param nbL Number of lines
				\param nbC Number of columns
		*/ //---------------------------------------------------------
		//------------------------------------------------------------
		CDynamicMatrix(const unsigned int nbL, const unsigned int nbC)
			: _nbLines(nbL), _nbColumns(nbC), m(NULL)
		{
			resize(nbL, nbC);
		}

		//------------------------------------------------------------
		//! Assigne ?partir d'une même classe
		/*! \param mat Une référence ?une même classe
				\return La classe copiée
		*/ //---------------------------------------------------------
		CDynamicMatrix(const CDynamicMatrix<Type>& mat)
			: m(NULL)
		{
			resize(mat._nbLines,mat._nbColumns);
			for (unsigned int l=0; l<_nbLines; l++)
				m[l] = mat.m[l];
		}

		//------------------------------------------------------------
		//! Constructor from one-dimension array
		/*! Warning ! No control applied on the size of the parameter!
				It should contain the complete matrix line by line
				\param nbL Number of lines
				\param nbC Number of columns
				\param elements Elements of the matrix
		*/ //---------------------------------------------------------
		CDynamicMatrix(const unsigned int nbL, const unsigned int nbC, const Type element)
			: _nbLines(nbL), _nbColumns(nbC), m(NULL)
		{
			resize(nbL,nbC);
			// On stocke les éléments de la matrice
			for (unsigned int l=0; l<_nbLines; l++)
        for (unsigned int c=0; c<_nbColumns; c++)
          m[l][c]=element;
		}
		//------------------------------------------------------------
		//! Destructor
		//------------------------------------------------------------
		virtual ~CDynamicMatrix()
		{
			if (m)
				delete [] m;
			m = NULL;
		}
		//@}

		//************************************************************
		//! @name Accesseurs
		//************************************************************
		//@{
		//------------------------------------------------------------
		//! Permet de lire un élément de la matrice
		/*! \param line Indice de la ligne
				\param col Indice de la colonne
				\return Element (line,col) correspondant
		*/ //---------------------------------------------------------
		Type const & element(unsigned int line, unsigned int col) const
		{
			//mkASSERT_DEBUG( (line<_nbLines)&&(col<_nbColumns) , "CDynamicMatrix<_nbLines,_nbColumns>::element(line,col) indice hors limites" );
			return m[line][col];
		}

		//------------------------------------------------------------
		//! Permet de lire un élément de la matrice
		/*! \param line Indice de la ligne
				\param col Indice de la colonne
				\return Element (line,col) correspondant
		*/ //---------------------------------------------------------
		Type& element(unsigned int line, unsigned int col)
		{
			//mkASSERT_DEBUG( (line<_nbLines)&&(col<_nbColumns) , "CDynamicMatrix<_nbLines,_nbColumns>::element(line,col) indice hors limites" );
			return m[line][col];
		}

		//------------------------------------------------------------
		//! Permet de lire un vecteur ligne
		/*! \param line Indice de la ligne
				\return Vecteur ligne
		*/ //---------------------------------------------------------
		std::vector<Type> const & operator [] (const unsigned int line) const
		{
			//mkASSERT_DEBUG( (line<_nbLines) , "CDynamicMatrix<_nbLines,_nbColumns>::[line] indice hors limites" );
			return m[line];
		}

		//------------------------------------------------------------
		//! Permet de lire un vecteur ligne
		/*! \param line Indice de la ligne
				\return Vecteur ligne
		*/ //---------------------------------------------------------
		std::vector<Type> const & row(const unsigned int line) const
		{
			//mkASSERT_DEBUG( (line<_nbLines) , "CDynamicMatrix<_nbLines,_nbColumns>::row(line) indice hors limites" );
			return m[line];
		}

		//------------------------------------------------------------
		//! Permet de lire un vecteur colonne
		/*! \param col Indice de la colonne
				\return Vecteur colonne
		*/ //---------------------------------------------------------
		std::vector<Type> column(const unsigned int col) const
		{
			//mkASSERT_DEBUG( (col<_nbColumns) , "CDynamicMatrix<_nbLines,_nbColumns>::column(col) indice hors limites" );
			std::vector<Type> result(_nbLines);
			for (unsigned int i=0;i<_nbLines;i++)
				result[i]=m[i][col];
			return result;
		}

		//-------------------------------------------------------------------
		//! Fournit le nombre de lignes
		//-------------------------------------------------------------------
		unsigned int getNbLines() const
		{
			return _nbLines;
		}

		//-----------------------------------------------------------------
		//! Fournit le nombre de colonnes
		//-----------------------------------------------------------------
		unsigned int getNbColumns() const
		{
			return _nbColumns;
		}

		//-----------------------------------------------------------------
		//! Redimensionne la matrice
		/*! \param line Nombre de lignes
				\param line Nombre de colonnes
		*/ //-----------------------------------------------------------------
		void resize(const unsigned int line, const unsigned int col)
		{
			if (m)
				delete [] m;

			_nbLines = line;
			_nbColumns = col;

			m = new std::vector<Type>[_nbLines];
			for (unsigned int i=0; i<_nbLines; i++)
				m[i].resize(_nbColumns);
		}

		//-----------------------------------------------------------------
		//! Mets tous les éléments de la matice ?la valeur souhaitée
		/*! \param val Valeur souhaitée de tous les éléments de la matrice
		*/ //-----------------------------------------------------------------
		void setAllElementsTo(const Type& val)
		{
			for (unsigned int i=0; i<_nbLines; i++)
				for (unsigned int j=0; j<_nbColumns; j++)
					m[i][j] = val;
		}

		//-----------------------------------------------------------------
		//! Transforme la matrice en une matrice identitée de taille matSize*matSize
		/*! \param matSize Taille souhaitée de la matrice identit?		*/ //-----------------------------------------------------------------
		void setIdentity(const Type& matSize)
		{
			resize(matSize, matSize);
			setAllElementsTo(0);
			for (unsigned int i=0; i<matSize; i++)
				m[i][i] = 1;
		}

		//@}

		//************************************************************
		//! @name Assignements
		//************************************************************
		//@{
		//------------------------------------------------------------
		//! Assigne ?partir d'une même classe
		/*! \param mat Une référence ?une même classe
				\return La classe copiée
		*/ //---------------------------------------------------------
    CDynamicMatrix<Type> & operator = (const CDynamicMatrix<Type>& mat)
		{
			_nbLines = mat._nbLines;
			_nbColumns = mat._nbColumns;
			for (unsigned int l=0; l<_nbLines; l++)
				m[l] = mat.m[l];
			return (*this);
		}

		//------------------------------------------------------------
		//! Permet de modifier un élément de la matrice
		/*! \param line Indice de la ligne
				\param col Indice de la colonne
				\param val Nouvelle valeur de l'élément
		*/ //---------------------------------------------------------
		void element(unsigned int line, unsigned int col, const Type& val)
		{
			//mkASSERT_DEBUG( (line<_nbLines)&&(col<_nbColumns) , "CDynamicMatrix<_nbLines,_nbColumns>::element(line,col,val) indice hors limites" );
			m[line][col] = val;
		}

		//------------------------------------------------------------
		//! Permet de modifier un vecteur ligne
		/*! \param line Indice de la ligne
				\return Vecteur ligne
		*/ //---------------------------------------------------------
		std::vector<Type> & operator [] (const unsigned int line)
		{
			//mkASSERT_DEBUG( (line<_nbLines) , "CDynamicMatrix<_nbLines,_nbColumns>::[line] indice hors limites" );
			return m[line];
		}

		//------------------------------------------------------------
		//! Permet de modifier un vecteur ligne
		/*! \param line Indice de la ligne
				\param vec Nouveau vecteur ligne
		*/ //---------------------------------------------------------
		void row(const unsigned int line, const std::vector<Type>& vec)
		{
			//mkASSERT_DEBUG( (line<_nbLines) , "CDynamicMatrix<_nbLines,_nbColumns>::row(line,vec) indice hors limites" );
			//mkASSERT_DEBUG( ((unsigned int)vec.size() == _nbColumns) , "CDynamicMatrix<_nbLines,_nbColumns>::row(line,vec) : the length of the vector is incorrect!" );
			m[line]=vec;
		}

		//------------------------------------------------------------
		//! Permet de remplir une ligne avec une valeur unique
		/*! \param line Indice de la ligne
				\param value Valeur unique
		*/ //---------------------------------------------------------
		void row(const unsigned int line, const Type& value)
		{
			//mkASSERT_DEBUG( (line<_nbLines) , "CDynamicMatrix<_nbLines,_nbColumns>::row(line,value) indice hors limites" );
			m[line].clear(value);
		}

		//------------------------------------------------------------
		//! Permet de modifier un vecteur colonne
		/*! \param col Indice de la colonne
				\param vec Nouveau vecteur colonne
		*/ //---------------------------------------------------------
		void column(const unsigned int col,const std::vector<Type>& vec)
		{
			//mkASSERT_DEBUG( (col<_nbColumns) , "CDynamicMatrix<_nbLines,_nbColumns>::column(col,vec) indice hors limites" );
			//mkASSERT_DEBUG( ((unsigned int)vec.size() == _nbLines) , "CDynamicMatrix<_nbLines,_nbColumns>::column(line,vec) : the length of the vector is incorrect!" );
			for (unsigned int i=0;i<_nbLines;i++)
				m[i][col]=vec[i];
		}

		//@}

		//************************************************************
		//! @name Opérateurs de comparaison
		//************************************************************
		//@{
		//-------------------------------------------------------------
		//! Comparaison (élément ?élément) avec une même classe
		/*! \param mat Une référence sur une classe de même type
				\return Réponse booléenne sur l'égalit?		*/ //----------------------------------------------------------
		bool operator == (const CDynamicMatrix<Type>& mat) const
		{
			if (_nbLines != mat._nbLines)
				return false;
			if (_nbColumns != mat._nbColumns)
				return false;
			for (unsigned int i=0;i<_nbLines;i++)
				if (m[i]!=mat[i])
					return false;
			return true;
		}

		//-----------------------------------------------------------------
		//! Différence (élément ?élément) avec une même classe
		/*! \param mat Une référence sur une classe de même type
				\return Réponse booléenne sur la différence
		*/ //--------------------------------------------------------------
		bool operator != (const CDynamicMatrix<Type>& mat) const
		{
			if (_nbLines != mat._nbLines)
				return true;
			if (_nbColumns != mat._nbColumns)
				return true;
			for (unsigned int i=0;i<_nbLines;i++)
				if (m[i]!=mat[i])
					return true;
			return false;
		}

		//-----------------------------------------------------------------
		//! Renvoie le nombre de zéro de la matrice
		/*! \return Le nombre de zéro
		*/ //--------------------------------------------------------------
		int nb0 () const
		{
			int result=0;
			for (unsigned int l=0; l<_nbLines; l++)
				for (unsigned int c=0; c<_nbColumns; c++)
					if (m[l][c] == 0)
						result++;
			return result;
		}
		//@}

		//************************************************************
		//! @name Opérations arithmétiques sur la matrice courante
		//************************************************************
		//@{
		//-----------------------------------------------------------------
		//! Permet d'ajouter (élément ?élément) ?cette matrice une autre
		/*! \param mat Matrice
				\return Une référence sur cette matrice
		*/ //--------------------------------------------------------------
		CDynamicMatrix<Type>& operator += (const CDynamicMatrix<Type>& mat)
		{
			//mkASSERT_DEBUG( (_nbLines==mat._nbLines) , "CDynamicMatrix<Type>::operator += : matrices of different sizes!" );
			//mkASSERT_DEBUG( (_nbColumns==mat._nbColumns) , "CDynamicMatrix<Type>::operator += : matrices of different sizes!" );
			for (unsigned int l=0; l<_nbLines; l++)
				for (unsigned int c=0; c<_nbColumns; c++)
					m[l][c]+=mat[l][c];
			return *this;
		}

		//------------------------------------------------------------------------
		//! Permet de soustraire (élément ?élément) une autre matrice ?celle-ci
		/*! \param mat Matrice
				\return Une référence sur cette matrice
		*/ //---------------------------------------------------------------------
		CDynamicMatrix<Type>& operator -= (const CDynamicMatrix<Type>& mat)
		{
			//mkASSERT_DEBUG( (_nbLines==mat._nbLines) , "CDynamicMatrix<Type>::operator -= : matrices of different sizes!" );
			//mkASSERT_DEBUG( (_nbColumns==mat._nbColumns) , "CDynamicMatrix<Type>::operator -= : matrices of different sizes!" );
			for (unsigned int l=0; l<_nbLines; l++)
				for (unsigned int c=0; c<_nbColumns; c++)
					m[l][c]-=mat[l][c];
			return *this;
		}

		//------------------------------------------------------------------------
		//! Permet de multiplier (élément ?élément) une autre matrice ?celle-ci
		/*! \param mat Matrice
				\return Une référence sur cette matrice
		*/ //---------------------------------------------------------------------
		CDynamicMatrix<Type>& operator *= (const CDynamicMatrix<Type>& mat)
		{
			//mkASSERT_DEBUG( (_nbLines==mat._nbLines) , "CDynamicMatrix<Type>::operator *= : matrices of different sizes!" );
			//mkASSERT_DEBUG( (_nbColumns==mat._nbColumns) , "CDynamicMatrix<Type>::operator *= : matrices of different sizes!" );
			for (unsigned int l=0; l<_nbLines; l++)
				for (unsigned int c=0; c<_nbColumns; c++)
					m[l][c]*=mat[l][c];
			return *this;
		}

		//---------------------------------------------------------------------
		//! Permet de diviser (élément ?élément) une autre matrice ?celle-ci
		/*! \param mat Matrice
				\return Une référence sur cette matrice
		*/ //------------------------------------------------------------------
		CDynamicMatrix<Type>& operator /= (const CDynamicMatrix<Type>& mat)
		{
			//mkASSERT_DEBUG( (_nbLines==mat._nbLines) , "CDynamicMatrix<Type>::operator /= : matrices of different sizes!" );
			//mkASSERT_DEBUG( (_nbColumns==mat._nbColumns) , "CDynamicMatrix<Type>::operator /= : matrices of different sizes!" );
			//mkASSERT_DEBUG( (mat.nb0()==0) , "CDynamicMatrix<_nbLines,_nbColumns>::/=(mat) division par zéro" );
			for (unsigned int l=0; l<_nbLines; l++)
				for (unsigned int c=0; c<_nbColumns; c++)
					m[l][c]/=mat[l][c];
			return *this;
		}

		//----------------------------------------------------------------
		//! Permet d'ajouter une valeur ?chaque élément de cette matrice
		/*! \param val Valeur
				\return Une référence sur cette matrice
		*/ //-------------------------------------------------------------
		CDynamicMatrix<Type>& operator += (const Type& val)
		{
			for (unsigned int l=0; l<_nbLines; l++)
				for (unsigned int c=0; c<_nbColumns; c++)
					m[l][c]+=val;
			return *this;
		}

		//--------------------------------------------------------------------
		//! Permet de soustraire une valeur ?chaque élément de cette matrice
		/*! \param val Valeur
				\return Une référence sur cette matrice
		*/ //-----------------------------------------------------------------
		CDynamicMatrix<Type>& operator -= (const Type& val)
		{
			for (unsigned int l=0; l<_nbLines; l++)
				for (unsigned int c=0; c<_nbColumns; c++)
					m[l][c]-=val;
			return *this;
		}

		//-------------------------------------------------------------
		//! Permet de multiplier une valeur ?chaque élément de cette matrice
		/*! \param val Valeur
				\return Une référence sur cette matrice
		*/ //-----------------------------------------------------------------
		CDynamicMatrix<Type>& operator *= (const Type& val)
		{
			for (unsigned int l=0; l<_nbLines; l++)
				for (unsigned int c=0; c<_nbColumns; c++)
					m[l][c]*=val;
			return *this;
		}

		//------------------------------------------------------------
		//! Permet de diviser une valeur ?chaque élément de cette matrice
		/*! \param val Valeur
				\return Une référence sur cette matrice
		*/
		//------------------------------------------------------------
		CDynamicMatrix<Type>& operator /= (const Type& val)
		{
			//mkASSERT_DEBUG( (val!=0) , "CDynamicMatrix<_nbLines,_nbColumns>::/=(val) division par zéro" );
			for (unsigned int l=0; l<_nbLines; l++)
				for (unsigned int c=0; c<_nbColumns; c++)
					m[l][c]/=val;
			return *this;
		}
		//@}

		//************************************************************
		//! @name Opérations arithmétiques sur une troisième matrice
		//************************************************************
		//@{
		//------------------------------------------------------------
		//! Permet d'ajouter (élément ?élément) cette matrice une autre
		/*! \param mat Matrice
				\return La matrice résultante
		*/ //---------------------------------------------------------
		CDynamicMatrix<Type> operator + (const CDynamicMatrix<Type>& mat) const
		{
			//mkASSERT_DEBUG( (_nbLines==mat._nbLines) , "CDynamicMatrix<Type>::operator + : matrices of different sizes!" );
			//mkASSERT_DEBUG( (_nbColumns==mat._nbColumns) , "CDynamicMatrix<Type>::operator + : matrices of different sizes!" );
			CDynamicMatrix<Type>	result(_nbLines,_nbColumns);
			for (unsigned int l=0; l<_nbLines; l++)
				for (unsigned int c=0; c<_nbColumns; c++)
					result.m[l][c] = m[l][c]+mat.m[l][c];
			return result;
		}

		//------------------------------------------------------------
		//! Permet de soustraire (élément ?élément) une autre matrice ?celle-ci
		/*! \param mat Matrice
				\return La matrice résultante
		*/ //---------------------------------------------------------
		CDynamicMatrix<Type> operator - (const CDynamicMatrix<Type>& mat) const
		{
			//mkASSERT_DEBUG( (_nbLines==mat._nbLines) , "CDynamicMatrix<Type>::operator - : matrices of different sizes!" );
			//mkASSERT_DEBUG( (_nbColumns==mat._nbColumns) , "CDynamicMatrix<Type>::operator - : matrices of different sizes!" );
			CDynamicMatrix<Type>	result(_nbLines,_nbColumns);
			for (unsigned int l=0; l<_nbLines; l++)
				for (unsigned int c=0; c<_nbColumns; c++)
					result.m[l][c] = m[l][c]-mat.m[l][c];
			return result;
		}

		//------------------------------------------------------------
		//! Permet de multiplier une valeur ?cette matrice
		/*! \param val Valeur
				\return La matrice résultante
		*/ //---------------------------------------------------------
		CDynamicMatrix<Type> operator * (const Type& val) const
		{
			CDynamicMatrix<Type>	result(_nbLines,_nbColumns);
			for (unsigned int l=0; l<_nbLines; l++)
				for (unsigned int c=0; c<_nbColumns; c++)
					result.m[l][c] = m[l][c]*val;
			return result;
		}

		//------------------------------------------------------------
		//! Permet de diviser une valeur ?cette matrice
		/*! \param val Valeur
				\return La matrice résultante
		*/ //---------------------------------------------------------
		CDynamicMatrix<Type> operator / (const Type& val) const
		{
			//mkASSERT_DEBUG( (val!=0) , "CDynamicMatrix<_nbLines,_nbColumns>::/(val) division par zéro" );
			CDynamicMatrix<Type>	result(_nbLines,_nbColumns);
			for (unsigned int l=0; l<_nbLines; l++)
				for (unsigned int c=0; c<_nbColumns; c++)
					result.m[l][c] = m[l][c]/val;
			return result;
		}

		//------------------------------------------------------------
		//! Permet de multiplier mathématiquement une autre matrice ?celle-ci
		/*! \param mat Matrice
				\return La matrice résultante
		*/ //---------------------------------------------------------
		CDynamicMatrix<Type> operator * (const CDynamicMatrix<Type>& mat) const
		{
			// 'this' is RxN, 'M' is NxC, 'product = this*M' is RxC
			//mkASSERT_DEBUG( (_nbColumns==mat._nbLines) , "CDynamicMatrix<Type>::operator * : matrices of different sizes!" );
			CDynamicMatrix<Type>		result(_nbLines,mat._nbColumns,0);
			for (int iRow = 0; iRow < result.getNbLines(); iRow++)
			{
				for (int iCol = 0; iCol < result.getNbColumns(); iCol++)
				{
					for (int iMid = 0; iMid < _nbColumns; iMid++)
					{
						result.m[iRow][iCol] += m[iRow][iMid] *	mat.m[iMid][iCol];
					}
				}
			}
			return result;
		}

		//------------------------------------------------------------
		//! Permet de multiplier mathématiquement un vecteur ?cette matrice
		/*! \param vec Vecteur
				\return La matrice résultante
		*/ //---------------------------------------------------------
		std::vector<Type> operator * (const std::vector<Type>& vec) const
		{
			//mkASSERT_DEBUG( (_nbColumns==vec.size()) , "CDynamicMatrix<Type>::operator * : matrices of different sizes!" );
			std::vector<Type>	result(_nbLines);
			for (unsigned int i=0;i<_nbLines;i++)
				result[i]=m[i]*vec;
			return result;
		}

		//------------------------------------------------------------
		//! Permet de transposer cette matrice
		/*! \return La matrice résultante
		*/ //---------------------------------------------------------
		CDynamicMatrix<Type> transpose () const
		{
				CDynamicMatrix<Type> result(_nbColumns,_nbLines);
				for (int iRow = 0; iRow < _nbLines; iRow++)
				{
					for (int iCol = 0; iCol < _nbColumns; iCol++)
					{
						result.m[iCol][iRow] = m[iRow][iCol];
					}
				}
				return result;
		}

		//------------------------------------------------------------
		//! Permet de transposer cette matrice puis la multiplier (élément ?élément) par une autre matrice
		/*! \param mat Matrice
				\return La matrice résultante
		*/ //---------------------------------------------------------
		CDynamicMatrix<Type> transposeMultiply (const CDynamicMatrix<Type>& mat) const
		{
				// P = A^T*B, P[r][c] = sum_m A[m][r]*B[m][c]
				//mkASSERT_DEBUG( (_nbLines==mat._nbLines) , "CDynamicMatrix<Type>::transposeMultiply : matrices of different row sizes!" );
				CDynamicMatrix<Type> result(_nbColumns,mat._nbColumns,0);
				for (int iRow = 0; iRow < result._nbLines; iRow++)
				{		
					for (int iCol = 0; iCol < result._nbColumns; iCol++)
					{
						for (int iMid = 0; iMid < _nbLines; iMid++)
						{
							result.m[iRow][iCol] += m[iMid][iRow] *	mat.m[iMid][iCol];
							//result.m[iRow][iCol] = result.m[iRow][iCol] + (m[iMid][iRow] *	mat.m[iMid][iCol]);
						}
					}
				}
				return result;
		}

		//------------------------------------------------------------
		//! Permet de multiplier cette matrice puis la transposer (élément ?élément) par une autre matrice
		/*! \param mat Matrice
				\return La matrice résultante
		*/ //---------------------------------------------------------
		CDynamicMatrix<Type> multiplyTranspose (const CDynamicMatrix<Type>& mat) const
		{
				// P = A*B^T, P[r][c] = sum_m A[r][m]*B[c][m]
				//mkASSERT_DEBUG( (_nbColumns==mat._nbColumns) , "CDynamicMatrix<Type>::multiplyTranspose : matrices of different columm sizes!" );
				CDynamicMatrix<Type> result(_nbLines,mat._nbLines,0);
				for (int iRow = 0; iRow < result._nbLines; iRow++)
				{
					for (int iCol = 0; iCol < result._nbColumns; iCol++)
					{
						for (int iMid = 0; iMid < _nbColumns; iMid++)
						{
								result.m[iRow][iCol] +=  m[iRow][iMid] *	mat.m[iCol][iMid];
						}
					}
				}
				return result;
		}
	
		//------------------------------------------------------------
		//! Permet d'inverser une matrice non carr?par Pseudo inverse 
		/*!	\return mat La matrice résultante 
		*/ //---------------------------------------------------------
		CDynamicMatrix<Type> pseudoInverse() 
		{
			if (_nbColumns!=_nbLines)
			{
				if (_nbColumns<_nbLines)
				{
					//=(JTJ)-1 *JT 
					CDynamicMatrix<Type> JtJ(_nbColumns,_nbColumns);
					JtJ = transposeMultiply(*this);
					CDynamicMatrix<Type> JtJInverse(_nbColumns,_nbColumns);
					JtJInverse = JtJ.inverse();
					return JtJInverse.multiplyTranspose(*this);
				}
				else
				{
					//J+ = JT*(JJT)-l
					CDynamicMatrix<Type> JJt(_nbLines,_nbLines);
					JJt = multiplyTranspose(*this);
					CDynamicMatrix<Type> JJtInverse(_nbLines,_nbLines);
					JJtInverse = JJt.inverse();
					return transposeMultiply(JJtInverse);
				}
			}
			else
			{
				return inverse();
			}
		}

		//------------------------------------------------------------
		//! Permet d'inverser une matrice carr?par LU decomposition
		/*!	\return mat La matrice résultante 
		*/ //---------------------------------------------------------
		CDynamicMatrix<Type> inverse()
		{
			int n(_nbColumns);
			std::vector<int>	indx(n);
			CDynamicMatrix<Type>	mat(n,n);
			CDynamicMatrix<Type>	res(n,n);
			std::vector<Type> col(n);
			double d;
			
			mat = *this;
			//mkASSERT_DEBUG( (_nbLines==_nbColumns) , "CDynamicMatrix<Type>::inverse : non square matrice!" );

			ludcmp(mat,n,indx,&d);

			for (int j=0; j<n; j++ ) 
			{
				for (int i=0; i<n; i++ )
				{
					col[i] = 0.0;
				}
				col[j] = 1.0;
				lubksb(mat,n,indx,col);
				for (int i=0; i<n; i++ )
				{
					res[i][j] = col[i];
				}
			}
			return res;
		}
		//------------------------------------------------------------
		/*! calcul de déterminant d'une matrice 
		  ! \param n dimension de la matrice ?décomposer(n*n) eventuellement sous matrice de mat!
			!	\return le determinant
		*/ //---------------------------------------------------------
		double	determinant()
		{
			double d;
			int n(_nbColumns);
			CDynamicMatrix<Type>	mat(n,n);
			mat = *this;
			std::vector<int>	indx(n);
			std::vector<Type> col(n);
			ludcmp(mat,n,indx,&d);

			for (int j=0; j<n; j++ )
			{
				d *= mat[j][j];
			}
			return d;
		}

		private:
		//------------------------------------------------------------
		//! Permet de décomposer une matrice (LU decomposition)
		/*! \param mat Matrice ?décomposer
		  ! \param n dimension de la matrice ?décomposer(n*n) eventuellement sous matrice de mat!
		  ! \param indx sauvegarde les permutations (pivot de Gauss)
			! \param d order of permutation 
				\return mat La matrice résultante (remplace la matrice d'origine)
		*/ //---------------------------------------------------------
		void ludcmp(CDynamicMatrix<Type>&	mat, int n, std::vector<int>&	indx, double *d ) const
		{
				int i,imax,j,k;
				double big,dum,sum,temp;
				std::vector<Type> vv(n);

			*d=1.0;
			for (i=0; i<n; i++ ) 
			{
				big = 0.0;
				temp;
				for (j=0; j<n; j++ )
					if ((temp=fabs(mat[i][j])) > big)
					{
						big = temp;
					}
				if (big==0.0) 
				{
					//mkASSERT_DEBUG( (big != 0.0) , "CDynamicMatrix<Type>::ludcmp() : Singular matrix!" );
				}
				vv[i] = 1.0/big;
			}

			for (j=0; j<n; j++ ) 
			{
				for (i=0; i<j; i++ ) 
				{
					sum= mat[i][j];
					for (k=0; k<i; k++ )
					{
						sum-=mat[i][k]*mat[k][j];
					}
					mat[i][j] = sum;
				}

				imax=-1;
				big =0.0;

				for (i=j; i<n; i++ ) 
				{
					sum=mat[i][j];
					for (k=0; k<j; k++ )
					{
						sum-=mat[i][k]*mat[k][j];
					}
					mat[i][j] = sum;
					dum = vv[i]*fabs(sum);
					if (dum > big) 
					{ 
						big = dum;
						imax=i;
					}
				}
					if (j!=imax) 
					{
						for (k=0; k<n; k++) 
						{
							dum=mat[imax][k];
							mat[imax][k]=mat[j][k];
							mat[j][k]=dum;
						}
						*d = -(*d);
						vv[imax]=vv[j];
					}
				indx[j] = imax;

				if (mat[j][j] == 0.0 )
				{
					mat[j][j] = TINY;
				}

				if (j != n-1)
				{ 
					dum = 1.0/(mat[j][j]);
					for (int i=j+1; i<n; i++ )
					{
						mat[i][j] *= dum;
					}
				}
			}
		}

		//------------------------------------------------------------
		//! Permet de recomposer une matrice 
		/*! \param mat Matrice ?recomposer
		  ! \param n dimension de la matrice ?recomposer(n*n)
		  ! \param indx sauvegarde les permutations (pivot de Gauss)
		  ! \param b 
				\return mat La matrice résultante (remplace la matrice d'origine)
		*/ //---------------------------------------------------------
		void lubksb(CDynamicMatrix<Type>&	mat, int n, std::vector<int>&	indx, std::vector<Type>& b )
		{
			int ii=-1;

			for (int i=0; i<n; i++ ) 
			{
				int ip = indx[i];
				double sum = b[ip];
				b[ip]=b[i];
				if (ii >= 0)

					for (int j=ii; j<=i-1; j++ )
					{
						sum -= mat[i][j] * b[j];
					}
				else
					if (sum != 0.0)
					{
						ii =i;
					}
				b[i] = sum;
			}

			for (int i=n-1; i>=0;i-- ) 
			{
				double sum = b[i];
				for (int j=i+1; j<n; j++ )
				{
					sum -= mat[i][j]*b[j];
				}
				b[i] = sum / mat[i][i];
			}
		}
		//@}

		//**************************************************************************************************
		//! Matrice représentée sous forme d'un tableau de vecteurs lignes (_nbLines * lignes de _nbColumns éléments)
		//**************************************************************************************************
	protected:
		//! Number of lines
		unsigned int _nbLines;
		//! Number of columns
		unsigned int _nbColumns;
		//! Matrix of _nbLines vectors of dimension _nbColumns
		std::vector<Type> *m;
	
	};

//*******************************************************************************************************
// OPERATEURS EXTERNES A LA CLASSE //////////////////////////////////////////////////////////////////////
//*******************************************************************************************************

	//------------------------------------------------------------
	//! Multiplication d'un vecteur par une matrice
	/*! \param vec Vecteur
			\param mat Matrice
			\return Résultat de la multiplication
	*/ //----------------------------------------------------------
	template <class Type>
	inline std::vector<Type> operator * (const std::vector<Type>& vec,
																					const CDynamicMatrix<Type>& mat)
	{
		//mkASSERT_DEBUG( (mat._nbLines==vec.dimension()) , "CDynamicMatrix<Type>::operator * : matrices of different sizes!" );
		std::vector<Type>	result(mat._nbColumns);
		for (unsigned int i=0;i<mat._nbColumns;i++)
			result[i]=vec*mat.column(i);
		return result;
	}

	//------------------------------------------------------------
	//! Insertion dans un flux
	/*! \param stream Flux
			\param mat La matrice ?insérer
			\return Le flux
	*/ //---------------------------------------------------------
	template <class Type>
	inline std::ostream& operator << (std::ostream& stream, const CDynamicMatrix<Type>& mat)
	{
			for (unsigned int i=0;i<mat.getNbLines();i++)
			{
				for (unsigned int j=0;j<mat.getNbColumns();j++)
				{
					stream << mat[i][j] << " ";
				}				
				stream << std::endl;
			}
		return stream;
	}

	//------------------------------------------------------------
	//! Extraction d'un flux
	/*! \param stream Flux
			\param mat La matrice ?extraire
			\return Le flux
	*/ //---------------------------------------------------------
	template <class Type>
	inline std::istream& operator >> (std::istream& stream, CDynamicMatrix<Type>& mat)
	{
			for (unsigned int i=0;i<mat.getNbLines();i++)
				for (unsigned int j=0;j<mat.getNbColumns();j++)
				stream >> mat[i][j];
		return stream;
	}

}

#endif
