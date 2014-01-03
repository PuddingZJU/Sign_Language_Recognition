#ifndef _mkCButterworth_h
#define _mkCButterworth_h

//*************************************************************************************
/*! \class mkm::CButterworth
\brief Cette classe calcule les coefficients d'un filtre de Butterworth
*/ //**********************************************************************************

//#include <mkBaseDefs.h>
//#include <mkBaseDll.h>
//#include <mkBaseTypes.h>

#include "mkCGenericFilter.h"
#include "mkCDynamicMatrix.h"
#include <complex>

namespace mkm
{
	class CButterworth : public CGenericFilter
	{
	public:
		//************************************************************
		//! @name Constructors and copy
		//************************************************************
		//@{
		//------------------------------------------------------------
		//! Create a butterworth filter object, not usable without providing cut frequency, sample frequency and ordeer
		//---------------------------------------------------------
		CButterworth();

		//------------------------------------------------------------
		//! Create a butterworth filter
		/*! \param fc Cut frequency
				\param fs Sample frequency
				\param order order of the filter
		*/ //---------------------------------------------------------
		CButterworth(const unsigned int fc, const unsigned int fe, const unsigned int order);

		//------------------------------------------------------------
		//! Copy the filter
		/*! \param filter Filter to copy
				\return the copied filter
		*/ //---------------------------------------------------------
		virtual CButterworth& operator = (const CButterworth& filter)
		{
			this->CGenericFilter::operator =(filter);
			return (*this);
		}

		//------------------------------------------------------------
		//! Copy the filter
		/*! \param filter Filter to copy
				\return the copied filter
		*/ //---------------------------------------------------------
		virtual CButterworth* copy() const 
		{
			CButterworth* copy = new CButterworth();
			*copy = *this;
			return copy;
		}		
		//@}

		//************************************************************
		//! @name Compute the filter coefficients
		//************************************************************
		//@{
		//------------------------------------------------------------
		//! Compute the filter coefficients for the given cut frequency, the given smaple frequency and the filter order
		//---------------------------------------------------------
		virtual void computeFilterCoefficients();

		//------------------------------------------------------------
		//! Give the butterworth coefficients for the given cut frequency, the given smaple frequency and the filter order
		/*! \param fc Cut frequency
				\param fs Sample frequency
				\param order order of the filter
				\param (out) den_A the A butterworth coefficients ((n+1) Ai coefficients)
				\param (out) num_B the B butterworth coefficients ((n+1) Bi coefficients)
		*/ //---------------------------------------------------------
		static void computeButterworthCoefficients(
			const unsigned int fc, const unsigned int fe, const unsigned int order, 
			std::vector<double>& den_A, std::vector<double>& num_B);
		//@}

	private:
		//------------------------------------------------------------
		//! Compute the zeros, poles and gain for an N-th order normalized prototype Butterworth analog lowpass filter.  
		//! The resulting filter has N poles around the unit circle in the left half plane, and no zeros.
		/*! \param n order of the filter
		\return 
		*/ //---------------------------------------------------------
		static std::vector<std::complex<double> > getButterworthZerosPolesAndGain(const unsigned int order, double* gain);

		//------------------------------------------------------------
		//! Compute the state space representation for a system given a set of pole locations in column vector P, a matrix Z with the zero locations in as many columns as there are
		//! outputs, and the gains for each numerator transfer function in vector K.  The A,B,C,D matrices are returned in block diagonal form.  
		/*! \param p xxxxx // TODO
		*/ //---------------------------------------------------------
		static void computeStateSpaceRepresentation(const std::vector<std::complex<double> >& p, const double& gain, 
			CDynamicMatrix<double>* a, CDynamicMatrix<double>* b, CDynamicMatrix<double>* c, double* d);

		//------------------------------------------------------------
		//! Sort the complex by pair, by increasing real part, then increasing complex part. Any purely real elements are placed after all the complex pairs.
		/*! \param tab the vector of complexes to sort
		*/ //---------------------------------------------------------
		static void sortComplexVector(std::vector<std::complex<double> >* p);

		//------------------------------------------------------------
		//! Converts the s-domain transfer function specified by a, b, and c to a z-transform discrete equivalent obtained from the bilinear transformation :
		//!  H(z) = H(s) with  s = 2*Fs*(z-1)/(z+1)
		/*! \param tab the vector of complexes to sort
		*/ //---------------------------------------------------------
		static void bilinear(CDynamicMatrix<double>* a, CDynamicMatrix<double>* b, CDynamicMatrix<double>* c, double* d, const double& fs);

		//------------------------------------------------------------
		//! Compute the roots to the polynomial
		/*! \param root
		\return polynomial solution
		*/ //---------------------------------------------------------
		static std::vector<double> polynomial(const CDynamicMatrix<double>& root);

		//------------------------------------------------------------
		//! Compute the roots to the polynomial
		/*! \param root
		\return polynomial solution
		*/ //---------------------------------------------------------
		static std::vector<std::complex<double> > computeEigenValuesOfMatrix(const CDynamicMatrix<double>& root);

	private:
		//************************************************************
		// Data
		//************************************************************
		//@{
		//@}
	};
}

#endif


