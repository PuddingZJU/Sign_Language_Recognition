#ifndef _mkCGenericFilter_h
#define _mkCGenericFilter_h

//#include <mkBaseDefs.h>
//#include <mkBaseDll.h>
//#include <mkBaseTypes.h>

#include <vector>
#include <deque>
//#include "mkCQuaternion.h"
//#include "mkCVector.h"
#include "../Vector3.h"

namespace mkm
{
	class CGenericFilter
	{
	public:
		//************************************************************
		//! @name Constructor
		//************************************************************
		//@{
		//------------------------------------------------------------
		//! Create a filter object, not usable without providing cut frequency, sample frequency and ordeer
		//---------------------------------------------------------
		CGenericFilter();

		//------------------------------------------------------------
		//! Create a filter
		/*! \param fc Cut frequency
				\param fs Sample frequency
				\param order order of the filter
		*/ //---------------------------------------------------------
		CGenericFilter(const unsigned int fc, const unsigned int fe, const unsigned int order);

		//------------------------------------------------------------
		//! Destructor
		//---------------------------------------------------------
		virtual ~CGenericFilter();

		//------------------------------------------------------------
		//! Close function
		//---------------------------------------------------------
		virtual void close();

		//------------------------------------------------------------
		//! Is the filter initialized ?
		/*! \return ture if filter initialized
		*/ //---------------------------------------------------------
		bool isInitialized() const 
		{
			return _initialized;
		};

		//------------------------------------------------------------
		//! Copy the filter
		/*! \param filter Filter to copy
				\return the copied filter
		*/ //---------------------------------------------------------
		virtual CGenericFilter& operator = (const CGenericFilter& filter)
		{
			_initialized			= filter._initialized;

			_cut_frequency		= filter._cut_frequency;
			_sample_frequency = filter._sample_frequency;
			_order						= filter._order;

			_A_coefficients		= filter._A_coefficients;
			_B_coefficients		= filter._B_coefficients;
			return (*this);
		}

		//------------------------------------------------------------
		//! Copy the filter
		/*! \param filter Filter to copy
				\return the copied filter
		*/ //---------------------------------------------------------
		virtual CGenericFilter* copy() const = 0;
		//@}
		
		
		//************************************************************
		//! @name Accessors
		//************************************************************
		//@{
		//------------------------------------------------------------
		//! Give the A coefficients of filter
		/*! \return the A coefficients of filter
		*/ //---------------------------------------------------------
		const std::vector<double>& getACoefficients() const;

		//------------------------------------------------------------
		//! Give the B coefficients of butterworth filter
		/*! \return the B coefficients of butterworth filter
		*/ //---------------------------------------------------------
		const std::vector<double>& getBCoefficients() const;

		//------------------------------------------------------------
		//! Get the cut frequency of the filter
		/*	\return the cut frequency of the filter
		*/ //---------------------------------------------------------
		unsigned int getCutFrequency() const
		{
			return _cut_frequency;
		};

		//------------------------------------------------------------
		//! Get the sample frequency of the filter
		/*	\return the sample frequency of the filter
		*/ //---------------------------------------------------------
		unsigned int getSampleFrequency() const
		{
			return _sample_frequency;
		};

		//------------------------------------------------------------
		//! Get the order of the filter
		/*	\return the order of the filter
		*/ //---------------------------------------------------------
		unsigned int getOrder() const
		{
			return _order;
		};

		//------------------------------------------------------------
		//! Set the parameters of the filter, do nothing if the data are the same as previously
		/*! \param fc Cut frequency
				\param fs Sample frequency
				\param order order of the filter
		*/ //---------------------------------------------------------
		void setFilterParameters(const unsigned int fc, const unsigned int fe, const unsigned int order);

		//------------------------------------------------------------
		//! Compute the filter coefficients for the given cut frequency, the given smaple frequency and the filter order
		//---------------------------------------------------------
		virtual void computeFilterCoefficients() = 0;
		//@}

		//************************************************************
		//! @name Compute filtered values
		//************************************************************
		//@{
		//------------------------------------------------------------
		//! Compute the filtered sample given the not filtered sample, the previous raw samples and filtered sampled, for a double value
		/*! \param current Current raw sample to filter
				\param previousRaw Previous raw samples (order t0 - t1 - t2 - t3 ... t(n-1) if current is tn)
				\param previousFiltered Previous filtered samples (order t0 - t1 - t2 - t3 ... t(n-1) if current is tn)
				\return current filtered sample
		*/ //---------------------------------------------------------
		double computeCurrentFilteredSampleFromRawSample(
			const double& sample, const std::deque<double>& previousRaw, const std::deque<double>& previousFiltered) const ;

		//------------------------------------------------------------
		//! Compute the filtered sample given the not filtered sample, the previous raw samples and filtered sampled, for a Vector3 value
		/*! \param current Current raw sample to filter
				\param previousRaw Previous raw samples (order t0 - t1 - t2 - t3 ... t(n-1) if current is tn)
				\param previousFiltered Previous filtered samples (order t0 - t1 - t2 - t3 ... t(n-1) if current is tn)
				\return current filtered sample
		*/ //---------------------------------------------------------
		Vector3 computeCurrentFilteredSampleFromRawSample(
			const Vector3& sample, const std::deque<Vector3>& previousRaw, const std::deque<Vector3>& previousFiltered) const;

		////------------------------------------------------------------
		////! Compute the filtered sample given the not filtered sample, the previous raw samples and filtered sampled, for a double value
		///*! \param current Current raw sample to filter
		//		\param previousRaw Previous raw samples (order t0 - t1 - t2 - t3 ... t(n-1) if current is tn)
		//		\param previousFiltered Previous filtered samples (order t0 - t1 - t2 - t3 ... t(n-1) if current is tn)
		//		\return current filtered sample
		//*/ //---------------------------------------------------------
		//mkm::CQuaternion computeCurrentFilteredSampleFromRawSample(
		//	const mkm::CQuaternion& sample, const std::deque<mkm::CQuaternion>& previousRaw, const std::deque<mkm::CQuaternion>& previousFiltered) const;

		//------------------------------------------------------------
		//! Compute the filtered sample given the not filtered sample, the previous raw samples and filtered sampled, for a double value
		/*! \param current Current raw sample to filter
				\param previousRaw Previous raw samples (order t0 - t1 - t2 - t3 ... t(n-1) if current is tn)
				\param previousFiltered Previous filtered samples (order t0 - t1 - t2 - t3 ... t(n-1) if current is tn)
				\param coeffsA A coefficients of the filter
				\param coeffsB B coefficients of the filter
				\return current filtered sample
		*/ //---------------------------------------------------------
		static double computeCurrentFilteredSampleFromRawSample(
			const double& sample, const std::deque<double>& previousRaw, const std::deque<double>& previousFiltered, 
			const std::vector<double>& coeffsA, const std::vector<double>& coeffsB);

		//------------------------------------------------------------
		//! Compute the filtered sample given the not filtered sample, the previous raw samples and filtered sampled, for a Vector3 value
		/*! \param current Current raw sample to filter
				\param previousRaw Previous raw samples (order t0 - t1 - t2 - t3 ... t(n-1) if current is tn)
				\param previousFiltered Previous filtered samples (order t0 - t1 - t2 - t3 ... t(n-1) if current is tn)
				\param coeffsA A coefficients of the filter
				\param coeffsB B coefficients of the filter
				\return current filtered sample
		*/ //---------------------------------------------------------
		static Vector3 computeCurrentFilteredSampleFromRawSample(
			const Vector3& sample, const std::deque<Vector3>& previousRaw, const std::deque<Vector3>& previousFiltered, 
			const std::vector<double>& coeffsA, const std::vector<double>& coeffsB);

		////------------------------------------------------------------
		////! Compute the filtered sample given the not filtered sample, the previous raw samples and filtered sampled, for a double value
		///*! \param current Current raw sample to filter
		//		\param previousRaw Previous raw samples (order t0 - t1 - t2 - t3 ... t(n-1) if current is tn)
		//		\param previousFiltered Previous filtered samples (order t0 - t1 - t2 - t3 ... t(n-1) if current is tn)
		//		\param coeffsA A coefficients of the filter
		//		\param coeffsB B coefficients of the filter
		//		\return current filtered sample
		//*/ //---------------------------------------------------------
		//static mkm::CQuaternion computeCurrentFilteredSampleFromRawSample(
		//	const mkm::CQuaternion& sample, const std::deque<mkm::CQuaternion>& previousRaw, const std::deque<mkm::CQuaternion>& previousFiltered, 
		//	const std::vector<double>& coeffsA, const std::vector<double>& coeffsB);
		////@}


	protected :
		//************************************************************
		// Data
		//************************************************************
		//@{
		bool										_initialized;				//!< True if the filter is initialized (at least cut frequency, sample frequency and order)

		unsigned int						_cut_frequency;			//!< cut frequency of the butterworth filter
		unsigned int						_sample_frequency;	//!< sample frequency of the butterworth filter
		unsigned int						_order;							//!< order of the butterworth filter

		std::vector<double>	_A_coefficients;		//!< A coefficients of the butterworth filter
		std::vector<double>	_B_coefficients;		//!< B coefficients of the butterworth filter
		//@}
	};
}

#endif


