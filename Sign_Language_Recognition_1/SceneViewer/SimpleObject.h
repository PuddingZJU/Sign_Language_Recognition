/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2007/09/03                                                  */
/* Last Modified: 2007/09/03                                            */
/************************************************************************/

#ifndef _SimpleSolid_H_
#define _SimpleSolid_H_

#include "SceneObject.h"
#include "../../MathLib/Math.h"

namespace Swift
{
	/// Type of the simple objects.
	typedef enum
	{
		SOT_CUBE = 0,
		SOT_SPHERE,
		SOT_CYLINDER,
		SOT_ELLIPSE,
	}SimpleObjectType;

	class Cube : public SceneObject
	{
	public:
		explicit Cube(AbstractSceneManager * p_mgr);
		Cube(double dLength, double dWidth, double dHeight,
			AbstractSceneManager * p_mgr);
		virtual ~Cube();

		void render();

		//virtual void  SetPicked(bool isPicked,int jointIndex) {}; 
		//virtual void  RenderToSelectBuffer(int frmNo, int colorCode) {};
		//virtual int   GetFrameCount()const {return 0;};
		//virtual int   GetType()const {return 0;};
		//virtual double GetFrameTime() const {return 1.0;};
		
	private:
		double m_dLength;
		double m_dWidth;
		double m_dHeight;
	};	//Cube

	class Sphere :public SceneObject
	{
	public:
		explicit Sphere(AbstractSceneManager * p_mgr);
		Sphere(double dRadius, AbstractSceneManager * p_mgr);
		virtual ~Sphere();

		void render();

		//void SetPos(Vector3 pos) {m_vCenter = pos;}
		//virtual void  SetPicked(bool isPicked,int jointIndex) {}; 
		//virtual void  RenderToSelectBuffer(int frmNo, int colorCode) {};
		//virtual int   GetFrameCount()const {return 0;};
		//virtual int   GetType()const {return 0;};
		//virtual double GetFrameTime() const {return 1.0;};

	private:
		double m_dRadius;
	};	//Sphere

	class Cylinder : public SceneObject
	{
	public:
		explicit Cylinder(AbstractSceneManager * p_mgr);
		Cylinder(double dRBase, double dRTop, double dHeight, 
				AbstractSceneManager * p_mgr);
		virtual ~Cylinder();

		void render();
		//virtual void  SetPicked(bool isPicked,int jointIndex) {}; 
		//virtual void  RenderToSelectBuffer(int frmNo, int colorCode) {};
		//virtual int   GetFrameCount()const {return 0;};
		//virtual int   GetType()const {return 0;};
		//virtual double GetFrameTime() const {return 1.0;};

	private:
		double m_dRadiusBase;  //下底面半径
		double m_dRadiusTop;   //上底面半径
		double m_dHeight;
	};	//Cylinder


	class TransformTool : public SceneObject{
	public:
		enum TransformToolType{
			transfer,
			rotate,
			scale
		};

	public:

		class SingleAxis : public SceneObject{
		public:
			SingleAxis(TransformTool *vpParent, 
						const Vector3 &vDir,
						AbstractSceneManager * p_mgr,
						IDType type);
			virtual ~SingleAxis(){};

		protected:
			TransformTool *m_pParent;
			Vector3 direction;

		public:
			TransformTool *getParent() const;
			Vector3 getDir() const;
			
		}; //SingleAxis

	public:
		TransformTool(AbstractSceneManager * p_mgr,
								IDType type);
		TransformTool(double vRadius, 
						AbstractSceneManager * p_mgr, 
						IDType type);
		virtual ~TransformTool();

	protected:
		bool m_bRender;
		double m_dRadius;
		TransformToolType type;

		SingleAxis *m_pX;
		SingleAxis *m_pY;
		SingleAxis *m_pZ;

		Vector3 direction;

		Quaternion m_Orientation;

		
	public:
		void setRender(bool vbRender);

		unsigned long getXID() const;
		unsigned long getYID() const;
		unsigned long getZID() const;

		virtual void renderX();
		virtual void renderY();
		virtual void renderZ();

		Vector3 getDir() const;
		void setDir(const Vector3 &vDir);

		double getRadius();
		void setRadius(double vRadius);
		TransformToolType getType();

		const Quaternion &orientation() const;
		void setOrientation(const Quaternion &o);
		void resetOrientation();


	}; //TransformTool

	class TransferAxes : public TransformTool{
	public:

		class TransferAxis : public TransformTool::SingleAxis{
		public:
			TransferAxis(TransformTool *vpParent, 
						const Vector3 &vDir,
						AbstractSceneManager * p_mgr);
			virtual ~TransferAxis();

		public:
			void render();

		}; //TransferAxis

	public:
		explicit TransferAxes(AbstractSceneManager * p_mgr);
		TransferAxes(double vdAxisLength, AbstractSceneManager * p_mgr);
		virtual ~TransferAxes();

	public:
		void render();

		void renderX();
		void renderY();
		void renderZ();

	}; //TransferAxes

	class RotateSphere : public TransformTool{
	public:
		class RotateCircle : public TransformTool::SingleAxis{
		public:
			RotateCircle(TransformTool *vpParent, 
						const Vector3 &vDir,
						AbstractSceneManager * p_mgr);
			virtual ~RotateCircle();

		public:
			void render();

		}; //RotateCircle

		explicit RotateSphere(AbstractSceneManager * p_mgr);
		RotateSphere(double vdRadius, AbstractSceneManager * p_mgr);
		virtual ~RotateSphere();

	public:
		void render();

	}; //RotateAxes

	
	class Arrow : public SceneObject{
	protected:
		double m_dRadius;
		Vector3 m_Dir;


	public:
		Arrow(AbstractSceneManager * p_mgr);
		Arrow(AbstractSceneManager * p_mgr, double aRadius, const Vector3 &aDir);
		~Arrow(){};

		const Vector3 &getDir() const;
		virtual void setDir(const Vector3 &aDir);

		double getRadius();
		virtual void setRadius(double aRadius);

		void render();
	};

}	//Swift

#endif	//_SimpleSolid_H_