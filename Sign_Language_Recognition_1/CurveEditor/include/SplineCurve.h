/**  Spline Curve
 *	 Author : 毛旭辉
 *	 Date : 2006-12-18
 *	 Description: 
 *   Note : 
 */

#pragma once

#include <vector>
#include <cassert>

//spline curve
template<typename T>
class SplineCurve
{
public:
	SplineCurve(){ m_numCtrlPts = 0; }
	SplineCurve(std::vector<T> ctrlPts){ m_ctrlPts = ctrlPts; m_numCtrlPts = (int)m_ctrlPts.size(); }
	int GetCtrPtsNum()const{ return m_numCtrlPts; }
	T GetCtrlPts(int index)const{ assert(index>=0 && index < m_numCtrlPts); return m_ctrlPts[index]; }
	void SetCtrlPts(int index, T value){ assert(index>=0 && index < m_numCtrlPts); m_ctrlPts[index] = value; }
	virtual int GetSegNum()const=0;   //pieces of spline curve(样条曲线段数)
	virtual T GetCurvePt(int segment,double u)const=0;
	virtual ~SplineCurve(){}

protected:
	std::vector<T> m_ctrlPts;
	int m_numCtrlPts;
};



//===============================================================
/* Hermite interpolation   
 */

template<typename T>
class Hermite_SplineCurve : public SplineCurve<T>
{
	// to be added...

};




//================================================================
/* Kochanek-Bartels Splines(Cardinal Splines included)
 * Given four consecutive control points,P(k-1),p(k),p(k+1),p(k+2),then we have:
 * P(0) = P(k)
 * P(1) = p(k+1)
 * P'(0)in = 1/2*(1-t)[(1+b)(1-c)(p(k)-p(k-1)) + (1-b)(1+c)(P(k+1)-P(k))]
 * P'(1)out = 1/2*(1-t)[(1+b)(1+c)(p(k+1)-p(k)) + (1-b)(1-c)(P(k+2)-P(k+1))]
 * 
 *   Tension T=+1-->Tight T=-1--> Round  
 *   Bias B=+1-->Post Shoot B=-1--> Pre shoot 
 *   Continuity C=+1-->Inverted corners C=-1--> Box corners 
 *
 * For Cardinal Splines, t=0,b=0,c=0
*/
template<typename T>
class KB_SplineCurve : public SplineCurve<T>
{
public:
	KB_SplineCurve(){}
	KB_SplineCurve(std::vector<T> ctrlPts, double t=0.0, double b = 0.0, double c = 0.0)
		:SplineCurve<T>(ctrlPts),m_t(t),m_b(b),m_c(c){}

	~KB_SplineCurve(){}

	double GetParameter(double *t, double *b, double *c)const{ *t = m_t; *b = m_b; *c = m_c; }
	void SetParameter(double t, double b, double c){ m_t = t; m_b = b; m_c = c; }
	int GetSegNum()const{ return m_numCtrlPts - 1;}

	T GetCurvePt(int segment,double u)const
	{
		assert(segment>=0 && segment < m_numCtrlPts-1); //0--segment number-1

		T P1 = m_ctrlPts[segment];
		T P2 = m_ctrlPts[segment+1];
  
		T P0,P3;
		if(segment == 0)
			P0 = P1;
		else
			P0 = m_ctrlPts[segment-1];

		if(segment == m_numCtrlPts - 2)
			P3 = P2;
		else
			P3 = m_ctrlPts[segment + 2];

		T DP1 = ((P2 - P1) * (1 + m_c) * (1 - m_b) + (P1 - P0) * (1 - m_c) * (1 + m_b)) * (1 - m_t) * 0.5;
		T DP2 = ((P3 - P2) * (1 - m_c) * (1 - m_b) + (P2 - P1) * (1 + m_c) * (1 + m_b)) * (1 - m_t) * 0.5;

		double U3 = u * u * u;
		double U2 = u * u;

		T P = P1 * ( 2 * U3 - 3 * U2 + 1) + P2 * (-2 * U3 + 3 * U2) + DP1 * (U3 - 2 * U2 + u) + DP2 * (U3 - U2);

		return P;
	}

private:
	double m_t; //tension
	double m_b; //bias
	double m_c; //continues
};


//===============================================================================
//B_Spline Curve
template<typename T>
class B_Spline : public SplineCurve<T>
{
public:
	typedef enum
	{
		UNIFORM,
		OPEN_UNIFORM,
		NON_UNIFORM
	}BSplineType;

	B_Spline(){}

	B_Spline(std::vector<T> ctrlPts,BSplineType type, int degree)
		: SplineCurve<T>(ctrlPts),m_type(type),m_degree(degree)
	{
		//create knot vector
		m_knotVec.resize(m_numCtrlPts + m_degree);

		if(m_type == UNIFORM)
		{
			for(unsigned int i = 0; i < m_knotVec.size(); ++i)
				m_knotVec[i] = i;
		}
		else if(m_type == OPEN_UNIFORM)
		{
			for(int i = 0; i < m_degree; ++i)
				m_knotVec[i] = 0;
			for(int i = m_degree; i < m_numCtrlPts; ++i)
				m_knotVec[i] = i - m_degree + 1;
			for(unsigned int i = m_numCtrlPts; i < m_knotVec.size(); ++i)
				m_knotVec[i] = m_numCtrlPts - m_degree + 1;
		}
		else if(m_type == NON_UNIFORM)
		{
			//to do...
			;
		}
	}

	int GetDegree()const
	{
		return m_degree;
	}

	int GetSegNum()const
	{
		return m_numCtrlPts - m_degree + 1;
	}

	double GetMaxU()const
	{
		assert(m_knotVec.size() > 0);
		return m_knotVec[m_knotVec.size()-1];
	}

	double GetMinU()const
	{
		assert(m_knotVec.size() > 0);
		return m_knotVec[0];
	}

	/*std::vector<int> GetInfluenceCtrlPts()const
	{

	}*/

	T GetCurvePt(int segment,double u)const //按段得到曲线上的点，每段 u: 0.0-1.0
	{
		assert(segment>=0 && segment < m_numCtrlPts - m_degree + 1);

		return GetCurvePt(segment + u);
	}

	T GetCurvePt(double u)const //从整个曲线段上得到曲线上的点，参数为 u: umin-umax
	{
		//参数范围限制在umin－umax之间
		if(u < m_knotVec[0])
			u = m_knotVec[0];
		if(u > m_knotVec[m_knotVec.size()-1])
			u = m_knotVec[m_knotVec.size()-1];


		int k = int(u);
		if(u == m_knotVec[m_knotVec.size()-1]) //理论上整段曲线的参数范围是[umin,umax),当 u = umax时，把它归到最后一个曲线段
			k--;

		double sum = 0.0;
		for(int i= 0; i < m_degree; ++i)
		{
			sum += m_ctrlPts[k+i] * BlendFunc(k+i,m_degree,u);
		}
		return sum;
	}

	double BlendFunc(int k, int d, double u)const
	{
		//因为每个bleng function的定义域为[u(k),u(k+1)),所以对u＝umax来说没有了定义，所以补充定义为1.0
		if(k == m_numCtrlPts-1 && d == m_degree && u == GetMaxU())
			return 1.0;

		if(d == 1)
		{
			if(u >= m_knotVec[k] && u < m_knotVec[k+1])
				return 1.0;
			else
				return 0.0;
		}
		else
		{
			double sum = 0.0;
			if(m_knotVec[k+d-1] - m_knotVec[k] != 0)
				sum += (u - m_knotVec[k])/(m_knotVec[k+d-1] - m_knotVec[k]) * BlendFunc(k,d-1,u);

			if(m_knotVec[k+d] - m_knotVec[k+1] != 0)
				sum += (m_knotVec[k+d] - u)/(m_knotVec[k+d] - m_knotVec[k+1]) * BlendFunc(k+1,d-1,u);

			return sum;
		}
	}

private:
	
	int m_degree;
	BSplineType m_type;
	std::vector<double> m_knotVec;
};