#include "MathLib/Filter/mkCButterworth.h"

//#include <Math/mkCMath.h>
//#include <Math/mkCVector.h>
//#include <System/mkCMKMException.h>

namespace mkm
{
	//------------------------------------------------------------
	CButterworth::CButterworth() : CGenericFilter()
	{
	}

	//------------------------------------------------------------
	CButterworth::CButterworth(const unsigned int fc, const unsigned int fe, const unsigned int order) :
		CGenericFilter(fc, fe, order)
	{
		computeFilterCoefficients();
	}

	//------------------------------------------------------------
	void CButterworth::computeFilterCoefficients()
	{
		computeButterworthCoefficients(_cut_frequency, _sample_frequency, _order, _A_coefficients, _B_coefficients);
	}

	//------------------------------------------------------------
	void CButterworth::computeButterworthCoefficients(
		const unsigned int fc, 
		const unsigned int fe, 
		const unsigned int order, 
		std::vector<double>& den_A, 
		std::vector<double>& num_B
	)
	{
		double fdc = 2*double(fc)/double(fe); // Normalized cut frequency [0 to 1], with 1 for half the sample frequency
		if(0 >= fdc || fdc >= 1)
			//throw(MKMException("CButterworth::computeButterworthCoefficients(...): ", 201));
			throw fdc;

		if(order > 500)
			//throw(MKMException("CButterworth::computeButterworthCoefficients(...): ", 202));
			throw order;

		double fs = 2;
		double Wn = 2*fs*std::tan(Math::PI*double(fdc)/fs); // convert to low-pass prototype estimate
		double gain;

		std::vector<std::complex<double> > p = getButterworthZerosPolesAndGain(order, &gain);

		CDynamicMatrix<double> a;
		CDynamicMatrix<double> b;
		CDynamicMatrix<double> c;
		double d;
		computeStateSpaceRepresentation(p, gain, &a, &b, &c, &d);

		// Transform lowpass to lowpass
		a = a*Wn;
		b = b*Wn;

		bilinear(&a, &b, &c, &d, fs);

		den_A = polynomial(a);

		double Wn2 = 2*std::atan2(Wn,4);
		CDynamicMatrix<double> r(order, 1, -1);
		num_B = polynomial(r);

		double sumDen = 0;
		for(unsigned int i = 0; i<den_A.size(); ++i)
			sumDen += den_A[i];

		double sumBp = 0;
		for(unsigned int i = 0; i<num_B.size(); ++i)
			sumBp += num_B[i];

		for(unsigned int i = 0; i<num_B.size(); ++i)
			num_B.at(i) = num_B.at(i)*sumDen/sumBp;
	}

	//------------------------------------------------------------
	std::vector<std::complex<double> > CButterworth::getButterworthZerosPolesAndGain(const unsigned int order, double* gain)
	{
		std::vector<std::complex<double> > res, resTmp;

		std::complex<double> p;
		double theta;
		for(unsigned int i = 1; i<order; i=i+2)
		{
			theta = Math::PI*i/(2*order) + Math::PI/2;
			p = std::complex<double>(std::cos(theta), std::sin(theta));
			resTmp.push_back(p);
		}

		unsigned int size = (unsigned int)(resTmp.size());
		for(unsigned int i = 0; i<size; ++i)
		{
			res.push_back(resTmp.at(i));
			res.push_back(std::complex<double>(resTmp.at(i).real(), - resTmp.at(i).imag()));
		}

		if(order%2 == 1)
			res.push_back(std::complex<double>(-1,0));

		p = res.at(0);
		for(unsigned int i = 1; i<res.size(); ++i)
			p = p*res.at(i);

		*gain = p.real();

		sortComplexVector(&res);

		return res;
	}

	//------------------------------------------------------------
	void CButterworth::computeStateSpaceRepresentation(
		const std::vector<std::complex<double> >& p, 
		const double& gain, 
		CDynamicMatrix<double>* a,
		CDynamicMatrix<double>* b,
		CDynamicMatrix<double>* c,
		double* d)
	{
		CDynamicMatrix<double> aDyn(0,0), aDynTmp;
		CDynamicMatrix<double> cTmp;
		std::vector<double> bVec, cVec;
		unsigned int pLength = (unsigned int)(p.size());

		// Passage en matrices dynamique et quand n impaire

		if(pLength%2 != 0)
		{
			aDyn.resize(1,1);
			aDyn[0][0] = -1;
      bVec.push_back(1);
			cVec.push_back(1);
			*d = 0;
			pLength--;
		}

		// Now we have an even number of poles and zeros, although not necessarily the same number - there may be more poles.
		// H(s) = (s^2+num(2)s+num(3))/(s^2+den(2)s+den(3))
		// Take care of any left over unmatched pole pairs.

		double wn;
		std::complex<double> p1, p2;
		double absp1, absp2;
		CDynamicMatrix<double> t(2,2), tInv(2,2), denMat(2,2);
		CDynamicMatrix<double> a1(2,2);
		CDynamicMatrix<double> b1(2,1);
		CDynamicMatrix<double> c1(1,2);
		double d1 = 0;
		Vector3 den;
		t[0][0] = tInv[0][0] = 1;
		t[0][1] = t[1][0] = tInv[0][1] = tInv[1][0] = 0;

		denMat[1][0] = 1;
		denMat[1][1] = 0;

		b1[0][0] = 1;
		b1[1][0] = 0;
		
		c1[0][0] = 0;
		*d = 1;
					
		for(unsigned int i = 0; i<pLength-1; i=i+2)
		{	
			p1 = p.at(i);
			p2 = p.at(i+1);

			absp1 = std::sqrt(p1.real()*p1.real() + p1.imag()*p1.imag());
			absp2 = std::sqrt(p2.real()*p2.real() + p2.imag()*p2.imag());

			den[0] = 1;
			den[1] = (-p1-p2).real();
			den[2] = (p1*p2).real();
						
			wn = std::sqrt(absp1*absp2);
      if (wn == 0)
				wn = 1;

			t[1][1] = double(1)/wn; // Balancing transformation
			tInv[1][1] = wn;

			denMat[0][0] = -den[1];
			denMat[0][1] = -den[2];

			a1 = tInv*denMat*t;
			c1[0][1] = t[1][1];

			unsigned int ma1 = aDyn.getNbColumns();
			unsigned int ma2 = a1.getNbColumns();

			aDynTmp.resize(aDyn.getNbLines(), aDyn.getNbColumns());
			aDynTmp = aDyn;

			// on recopie le bloc 1 (haut gauche) = aDynPrécédent (aDynTmp)
			aDyn.resize(ma1 + 2, ma1+ma2);
			aDyn.setAllElementsTo(0);
			for(unsigned int j = 0; j<aDynTmp.getNbLines(); ++j)
				for(unsigned int k = 0; k<aDynTmp.getNbColumns(); ++k)
					aDyn[j][k] = aDynTmp[j][k];

			cTmp.resize(1,(unsigned int)cVec.size());
			for(unsigned int j = 0; j<cVec.size(); ++j)
				cTmp[0][j] = cVec.at(j);
			CDynamicMatrix<double> tmp = b1*cTmp;
			
			// On copie b1*c dans le bloc bas gauche
			for(unsigned int j = 0; j<tmp.getNbLines(); ++j)
				for(unsigned int k = 0; k<tmp.getNbColumns(); ++k)
					aDyn[j+aDyn.getNbLines()-tmp.getNbLines()][k] = tmp[j][k];

			// On copie a1 dans le bloc bas droit
			for(unsigned int j = 0; j<a1.getNbLines(); ++j)
				for(unsigned int k = 0; k<a1.getNbColumns(); ++k)
					aDyn[j+aDyn.getNbLines()-a1.getNbLines()][k+aDyn.getNbColumns()-a1.getNbColumns()] = a1[j][k];

			bVec.push_back(b1[0][0]*(*d));
			bVec.push_back(b1[1][0]*(*d));
			
			for(unsigned int j = 0; j<cVec.size(); ++j)
				cVec.at(j) *= d1;
			cVec.push_back(c1[0][0]);
			cVec.push_back(c1[0][1]);
			
			*d = d1*(*d);
		}

		a->resize(aDyn.getNbLines(), aDyn.getNbColumns());
		*a = aDyn;

		b->resize((unsigned int)bVec.size(),1);
		for(unsigned int i = 0; i<bVec.size(); ++i)
			b->element(i,0,bVec.at(i));

		c->resize(1, (unsigned int)cVec.size());
		for(unsigned int i = 0; i<cVec.size(); ++i)
			c->element(0,i,cVec.at(i)*gain);

		*d = *d*gain;
	}

	//------------------------------------------------------------
	void CButterworth::sortComplexVector(std::vector<std::complex<double> >* p)
	{
		bool ordered = false;
		std::complex<double> tmp;

		for(unsigned int i = 0; i<p->size() && !ordered; ++i)
		{
			ordered = true;
			for(unsigned int j = 0; j < p->size() - i - 1; ++j)
				// Order by increasing real part
				if (p->at(j).real() > p->at(j+1).real())
				{
					tmp = p->at(j);
					p->at(j) = p->at(j+1);
					p->at(j+1) = tmp;
					ordered = false;
 				}
				// Order same real part by imaginary part
				else if (p->at(j).real() == p->at(j+1).real() && p->at(j).imag() > p->at(j+1).imag())
				{
					tmp = p->at(j);
					p->at(j) = p->at(j+1);
					p->at(j+1) = tmp;
					ordered = false;
 				}
		}

		// Put real numbers at the end
		int numberOfReal = 0;
		for(int i = (int)p->size()-1; i>=0; --i)
		{
			if(p->at(i).imag() == double(0))
			{
				tmp = p->at(i);
				for(unsigned int j = i; j < p->size() - numberOfReal - 1; ++j)
					p->at(j) = p->at(j+1);
				p->at(p->size()-1) = tmp;
				numberOfReal ++;
			}
		}
	}

	//------------------------------------------------------------
	void CButterworth::bilinear(
		CDynamicMatrix<double>* a,
		CDynamicMatrix<double>* b,
		CDynamicMatrix<double>* c,
		double* d,
		const double& fs)
	{
		// do state-space version of bilinear transformation
		double t = double(1)/fs;
		double r = std::sqrt(t);

		CDynamicMatrix<double> identity;
		identity.setIdentity(a->getNbLines());
		CDynamicMatrix<double> t1 = identity + (*a)*t/2;
		CDynamicMatrix<double> t2 = identity - (*a)*t/2;
		CDynamicMatrix<double> invt2 = t2.inverse();
		CDynamicMatrix<double> ad = invt2*t1;
		CDynamicMatrix<double> bd = (invt2*(*b))*double(t/r);
		CDynamicMatrix<double> cdt2 = (*c)*t2.inverse();
		CDynamicMatrix<double> cd = cdt2*r;
		CDynamicMatrix<double> dd = cdt2*(*b)*t/2;
		dd[0][0] += (*d);

		*a = ad;
		*b = bd;
		*c = cd;
		*d = dd[0][0];
	}

	//------------------------------------------------------------
	std::vector<double> CButterworth::polynomial(const CDynamicMatrix<double>& root)
	{
		std::vector<std::complex<double> > e;
		if(root.getNbColumns() == 1)
			for(unsigned int i = 0; i<root.getNbLines(); ++i)
				e.push_back(root[i][0]);
		else if(root.getNbLines() == 1)
			for(unsigned int i = 0; i<root.getNbColumns(); ++i)
				e.push_back(root[0][i]);
		else
			e	= computeEigenValuesOfMatrix(root);

		std::vector<std::complex<double> > c((unsigned int)(e.size()) + 1, std::complex<double>(0,0));
		std::vector<std::complex<double> > tmp(c);
		c[0] = std::complex<double>(1,0);

		std::vector<double> tmp1, tmp2;
		for(unsigned int i = 0; i<e.size(); ++i)
		{
 			tmp = c;
			for(unsigned int j = 0; j<i+1; ++j)
				c[j+1] = tmp[j+1] - e.at(j)*tmp[j];
		}

		// The result should be real if the roots are complex conjugates.
		std::vector<double> res;
		for(unsigned int i = 0; i< c.size(); ++i)
			res.push_back(c[i].real());

		return res;
	}

	//------------------------------------------------------------
	std::vector<std::complex<double> > CButterworth::computeEigenValuesOfMatrix(const CDynamicMatrix<double>& root)
	{
		std::vector<std::complex<double> > eigenvalues;

		//if(root.getNbColumns() != root.getNbLines())
			//CErrorHandler::error("CButterworth::computeEigenValuesOfMatrix : Matrix not square, impossible to compute eigenvalues");
		assert(root.getNbColumns() == root.getNbLines());

		if(root.getNbColumns() == 1)
			eigenvalues.push_back(root[0][0]);
		else if(root.getNbColumns() == 2)
		{
			double A = 1;
			double B = -(root[0][0] + root[1][1]);
			CDynamicMatrix<double> tmp = root;
			double C = tmp.determinant();

			double Delta = B*B - 4*A*C;

			if(Delta < 0)
			{
				eigenvalues.push_back(std::complex<double>(-B/(2*A), std::sqrt(-Delta)/(2*A)));
				eigenvalues.push_back(std::complex<double>(-B/(2*A),-std::sqrt(-Delta)/(2*A)));
			}
			else if(Delta == 0)
			{
				eigenvalues.push_back(-B/(2*A));
				eigenvalues.push_back(-B/(2*A));
			}
			else
			{
				eigenvalues.push_back((-B+std::sqrt(Delta))/(2*A));
				eigenvalues.push_back((-B-std::sqrt(Delta))/(2*A));
			}

		}
		//else
		//	CErrorHandler::error("CButterworth::computeEigenValuesOfMatrix : Not implemented yet for Matrix more than 2x2");
			
		return eigenvalues;
	}

} // end namespace mkm

