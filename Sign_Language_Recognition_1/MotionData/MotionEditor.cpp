/* by Wei Yang. */
/* Last Modified:  2008/05/26 05:54:34 . */

#include "MotionEditor.h"
#include "../MotionData/Spline.h"
#include <algorithm>
#include <functional>

using namespace Swift;

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif


MotionEditor::MotionEditor()
{

}


MotionEditor::~MotionEditor()
{

}

MotionClip* MotionEditor::CreateMotion(
									   MotionClip* pMotion, int nLength
									   )
{
	MotionClip* pResult;
	pResult = pMotion->CloneSkeleton();
	FillDataField(pResult, nLength);

	return pResult;
}

MotionClip* MotionEditor::CreateEmptyMotion(
	MotionClip* pMotion, int nLength
	)
{
	MotionClip* pResult;
	pResult = pMotion->CloneSkeleton();
	ReserveDataField(pResult, nLength);

	return pResult;
}

MotionClip* MotionEditor::CreateSubMotion(
	MotionClip* pMotion, int nStartFrame, int nEndFrame
	)
{
	int nLength;
	int t1, t2, srcLastFrame, desStart, srcStart, nMiddlePartLength;
	MotionClip* pResult;

	nLength = nEndFrame - nStartFrame + 1;
	//pResult = CreateEmptyMotion(pMotion, nLength);
	pResult = CreateMotion(pMotion, nLength);

	t1 = t2 = 0;
	srcStart = desStart = nStartFrame;
	nMiddlePartLength = nLength;

	if (desStart<0)
	{
		t1 = -desStart ;
		desStart = t1;
		srcStart = 0;
	}
	else
	{
		desStart = 0;
		t1 = 0;
	}

	srcLastFrame = pMotion->getFrameCount() - 1;
	if (nEndFrame - srcLastFrame > 0)
	{
		t2 = nEndFrame - srcLastFrame;
	}
	else
	{
		t2 = 0;
	}

	nMiddlePartLength -= t1;
	nMiddlePartLength -= t2;

	// the main part.
	CopyMotion(
		pResult, desStart, nMiddlePartLength, 
		pMotion, srcStart
		);

	if ( t1 > 0 )
	{
		// replicate the first frame.
		ReplicateMotionFrame(
			pResult, 0, t1,
			pMotion, 0
			);
	}

	if ( t2 > 0 )
	{
		// replicate the last frame.
		ReplicateMotionFrame(
			pResult, nLength - t2, t2,
			pMotion, srcLastFrame
			);
	}

	pResult->setFrameCount( t1 + t2 + nMiddlePartLength );
	return pResult;
}


MotionClip* MotionEditor::CreateBlendMotion(
	MotionClip* pMotion1, MotionClip* pMotion2,
	BaseCurve * pCurve
	)
{
	if (! pMotion1->compareSkeleton(pMotion2))
	{
		throw MotionEditorError("Two skeletons are different.");
	}

	MotionClip* pResult;
	int len1, len2, len;

	len1 = pMotion1->getFrameCount();
	len2 = pMotion2->getFrameCount();
	len = (len1 < len2 ? len1 : len2); // min (len1, len2)

	if ( pCurve->getLength() < len)
	{
		throw MotionEditorError("Factor Curve is too short.");
	}

	pResult = CreateEmptyMotion( pMotion1, len );

	double startpos = pCurve->getStartPosition();
	for (int i = 0; i < len; i++)
	{
		BlendMotionFrame(
			pMotion1, i,
			pMotion2, i,
			pResult, i,
			true, true,
			pCurve->getDataAt(startpos + i));
	}

	return pResult;
}

/*
* Blend two clips together. \n The overlapping part of two clips are blended together, while data of other part is copied from origin clips to target clips. \n
* <PRE>
*                       0=pos1 pos2  pos3  pos4 
*                          |    |     |     |    
*                          V    V     V     V    
*        pSourceMotion1    |----+-----|          
*        pSourceMotion2         |-----+-----|    
*        returned clip     |----*******-----|    
*                            ^     ^      ^ 
*                          first blended  last   
* </PRE>
* @param pSourceMotion1 Pointer to source-motion-1.
* @param pSourceMotion2 Pointer to source-motion-2.
* @param pos2 Position2. See the graph above.
* @param pos3 Position3. See the graph above.
* @param pos4 Position4. See the graph above.
* @param bFirstPartFromFirstClip Set to be true if the first part of the result-motion-clip is copied from the source-motion-1.
* @param bLastPartFromFirstClip Set to be true if the last part of the result-motion-clip is copied from the source-motion-1.
* @param nPos1SourceFrame The starting frame number of source-motion, for copying at pos1. Source-motion is selected through the variable bFirstPartFromFirstClip.
* @param nPos2SourceFrame The starting frame number of source-motion, for copying at pos1. Source-motion is selected through the variable bLastPartFromFirstClip.
* @param nPos2Motion1SourceFrame. The starting frame number in source-motion-1 where the blending begins.
* @param nPos2Motion2SourceFrame. The starting frame number in source-motion-2 where the blending begins.
* @param pCurve Pointer to the factor curve for blending.
* @note There's no pos1 because pos1 always be zero.
* @note The factor curve starts for position 0 .
* @return Pointer to a new MotionClip holding the result.
*/
MotionClip* MotionEditor::BlendTwoMotions(
	MotionClip* pSourceMotion1, MotionClip* pSourceMotion2,
	int pos2, int pos3, int pos4,
	bool bFirstPartFromFirstClip, bool bLastPartFromFirstClip,
	int nPos1SourceFrame, int nPos3SourceFrame,
	int nPos2Motion1SourceFrame, int nPos2Motion2SourceFrame,
	BaseCurve* pCurve,
	std::vector<int> * pvecJointIndexes /* = NULL */
	)
{
	int pos1 = 0;
	MotionClip* pResult;
	//MotionClip* pMotionTemp1, *pMotionTemp2;

	int nLen1, nLen2, nLen3, nLenAll;

	if (! pSourceMotion1->compareSkeleton(pSourceMotion2))
	{
		throw MotionEditorError("Skeletons are different in func BlendTwoMotions_Aligned.");
	}

	nLenAll = pos4 - pos1 + 1;
	nLen1 = pos2 - pos1 ;
	nLen2 = pos3 - pos2 + 1;
	nLen3 = pos4 - pos3 ;

	pResult = CreateMotion(pSourceMotion1, nLenAll);

	if (bFirstPartFromFirstClip)
	{
		CopyMotion(
			pResult, 0, nLen1,
			pSourceMotion1, nPos1SourceFrame
			);
	}
	else
	{
		CopyMotion(
			pResult, 0, nLen1,
			pSourceMotion2, nPos1SourceFrame
			);
	}

	if (bLastPartFromFirstClip)
	{
		CopyMotion(
			pResult, nLen1 + nLen2, nLen3,
			pSourceMotion1, nPos3SourceFrame
			);
	}
	else
	{
		CopyMotion(
			pResult, nLen1 + nLen2, nLen3,
			pSourceMotion2, nPos3SourceFrame
			);
	}

	//BlendMotion(
	//	pSourceMotion1, nPos2Motion1SourceFrame,
	//	pSourceMotion2, nPos2Motion2SourceFrame,
	//	nLen2,
	//	pResult, nLen1
	//	);

	if ( pCurve->getLength() < nLen2)
	{
		throw MotionEditorError("Factor Curve is too short.");
	}


	if (pvecJointIndexes)
	{
		// joints that not blended with copy from first motion
		CopyMotion(
			pResult, nLen1, nLen2,
			pSourceMotion1, nPos2Motion1SourceFrame
			);
	}

	double startpos = pCurve->getStartPosition();

	for (int i = 0; i< nLen2; ++i)
	{
		BlendMotionFrame(
			pSourceMotion1, nPos2Motion1SourceFrame + i,
			pSourceMotion2, nPos2Motion2SourceFrame + i,
			pResult, nLen1 + i,
			false, false,
			pCurve->getDataAt(startpos + i),
			pvecJointIndexes
			);
	}

	return pResult;

}


/**
* Same as BlendTwoMotions, but the root joints of two motions are aligned.
*/
MotionClip* MotionEditor::BlendTwoMotions_Aligned(
	MotionClip* pSourceMotion1, MotionClip* pSourceMotion2,
	int pos2, int pos3, int pos4,
	bool bFirstPartFromFirstClip, bool bLastPartFromFirstClip,
	int nPos1SourceFrame, int nPos3SourceFrame,
	int nPos2Motion1SourceFrame, int nPos2Motion2SourceFrame,
	BaseCurve* pCurve,
	std::vector<int> * pvecJointIndexes /* = NULL */
	)
{
	int pos1 = 0;
	MotionClip* pResult;
	//MotionClip* pMotionTemp1, *pMotionTemp2;

	int nLen1, nLen2, nLen3, nLenAll;

	if (! pSourceMotion1->compareSkeleton(pSourceMotion2))
	{
		throw MotionEditorError("Skeletons are different in func BlendTwoMotions_Aligned.");
	}

	nLenAll = pos4 - pos1 + 1;
	nLen1 = pos2 - pos1 ;
	nLen2 = pos3 - pos2 + 1;
	nLen3 = pos4 - pos3 ;

	pResult = CreateMotion(pSourceMotion1, nLenAll);

	//pMotionTemp1 = CreateMotion(pSourceMotion1, pSourceMotion1->getFrameCount());
	//CopyMotion( pMotionTemp1, 0, pSourceMotion1->getFrameCount(), pSourceMotion1, 0);

	//pMotionTemp2 = CreateMotion(pSourceMotion2, pSourceMotion2->getFrameCount());
	//CopyMotion( pMotionTemp2, 0, pSourceMotion2->getFrameCount(), pSourceMotion2, 0);

	//Vector3 vec;
	//MotionJoint* pmj1, *pmj2;
	//
	//pmj1 = pMotionTemp1->getRoot(nPos2Motion1SourceFrame);
	//pmj2 = pMotionTemp2->getRoot(nPos2Motion2SourceFrame);

	//if ( !bFirstPartFromFirstClip)
	//{
	//	//adjust motion1
	//	vec = pmj2->getAbsolutePosition(nPos2Motion2SourceFrame) 
	//		- pmj1->getAbsolutePosition(nPos2Motion1SourceFrame);
	//	TransformMotion(pMotionTemp1, true, 0, 0, vec, Quaternion::IDENTITY );		
	//}
	//else
	//{
	//	//adjust motion2
	//	vec = pmj1->getAbsolutePosition(nPos2Motion1SourceFrame) 
	//		- pmj2->getAbsolutePosition(nPos2Motion2SourceFrame);
	//	TransformMotion(pMotionTemp2, true, 0, 0, vec, Quaternion::IDENTITY );
	//}

	if (bFirstPartFromFirstClip)
	{
		CopyMotion(
			pResult, 0, nLen1,
			pSourceMotion1, nPos1SourceFrame
			);
	}
	else
	{
		CopyMotion(
			pResult, 0, nLen1,
			pSourceMotion2, nPos1SourceFrame
			);
	}

	if (bLastPartFromFirstClip)
	{
		CopyMotion(
			pResult, nLen1 + nLen2, nLen3,
			pSourceMotion1, nPos3SourceFrame
			);

		if (!bFirstPartFromFirstClip)
		{
			TransformMotion(
				pResult, false,
				nLen1 + nLen2, nLen3,
				GetDistance(
				pSourceMotion1, nPos2Motion1SourceFrame + nLen2 -1,
				pSourceMotion2, nPos2Motion2SourceFrame + nLen2 -1
				),
				Quaternion::IDENTITY
				);			
		}
	}
	else
	{
		CopyMotion(
			pResult, nLen1 + nLen2, nLen3,
			pSourceMotion2, nPos3SourceFrame
			);

		if (bFirstPartFromFirstClip)
		{
			TransformMotion(
				pResult, false,
				nLen1 + nLen2, nLen3,
				GetDistance(
				pSourceMotion2, nPos2Motion2SourceFrame + nLen2 -1,
				pSourceMotion1, nPos2Motion1SourceFrame + nLen2 -1
				),
				Quaternion::IDENTITY
				);
		}
	}

	//BlendMotion(
	//	pSourceMotion1, nPos2Motion1SourceFrame,
	//	pSourceMotion2, nPos2Motion2SourceFrame,
	//	nLen2,
	//	pResult, nLen1
	//	);

	if ( pCurve->getLength() < nLen2)
	{
		throw MotionEditorError("Factor Curve is too short.");
	}

	double startpos = pCurve->getStartPosition();

	if (! bFirstPartFromFirstClip)
	{
		for (int i = 0; i< nLen2; ++i)
		{
			BlendMotionFrame(
				pSourceMotion2, nPos2Motion2SourceFrame + i,
				pSourceMotion1, nPos2Motion1SourceFrame + i,
				pResult, nLen1 + i,
				true, false,
				pCurve->getDataAt(startpos + i),
				pvecJointIndexes
				);

		}

	}
	else
	{
		for (int i = 0; i< nLen2; ++i)
		{
			BlendMotionFrame(
				pSourceMotion1, nPos2Motion1SourceFrame + i,
				pSourceMotion2, nPos2Motion2SourceFrame + i,
				pResult, nLen1 + i,
				true, false,
				pCurve->getDataAt(startpos + i));

		}
	}
	return pResult;

}

void MotionEditor::BlendMotion(
							   MotionClip* pSourceMotion1, int nSourceStartFrame1, 
							   MotionClip* pSourceMotion2, int nSourceStartFrame2, 
							   int nLength,  
							   MotionClip* pTargetMotion, int nTargetStartFrame,
							   BaseCurve* pCurve,
							   std::vector<int> * pvecJointIndexes /* = NULL */
							   )
{
	if (! pSourceMotion1->compareSkeleton(pSourceMotion2))
	{
		throw MotionEditorError("Two skeletons are different.");
	}

	if (pCurve->getLength() < nLength)
	{
		throw MotionEditorError("Factor Curve is too short.");
	}

	double startpos = pCurve->getStartPosition();

	for (int i = 0; i < nLength; ++i)
	{
		BlendMotionFrame(
			pSourceMotion1, nSourceStartFrame1 + i,
			pSourceMotion2, nSourceStartFrame2 + i,
			pTargetMotion,  nTargetStartFrame + i,
			false, false,
			pCurve->getDataAt(startpos + i),
			pvecJointIndexes
			);
	}
}

MotionClip* MotionEditor::CreateInterpolateMotion(
	MotionClip* pMotion1, int nFrame1,
	MotionClip* pMotion2, int nFrame2,
	int nLength,
	BaseCurve* pCurve
	)
{
	if (! pMotion1->compareSkeleton(pMotion2))
	{
		throw MotionEditorError("Two skeletons are different.");
	}

	MotionClip* pResult;

	pResult = CreateEmptyMotion(pMotion1, nLength);
	CopyMotion(
		pResult, 0, 1,
		pMotion1, nFrame1
		);
	CopyMotion(
		pResult, nLength-1, 1,
		pMotion2, nFrame2
		);

	InterpolateMotion(
		pResult, 0, nLength-1,
		pCurve
		);

	return pResult;

}



void MotionEditor::InterpolateMotion(
									 MotionClip* pMotion, int nStartFrame, int nEndFrame,
									 BaseCurve* pCurve, bool bComplementalCurve /* = false */
									 )
{
	if (nStartFrame < 0 || nEndFrame > pMotion->getFrameCount() - 1)
		throw MotionEditorError("Range error in MotionEditor::InterpolateMotion().");

	int nLength = nEndFrame - nStartFrame + 1 - 2;
	double value =0.0;

	//int nLengthPlusOne = nLength + 1;
	if (!bComplementalCurve)
	{	
		for (int i = 1; i <= nLength; i++)
		{
			//		value = 1 - static_cast<double>(i) / nLengthPlusOne;
			value = pCurve->getDataAt(i);

			BlendMotionFrame(
				pMotion, nStartFrame,
				pMotion, nEndFrame,
				pMotion, nStartFrame + i,  // be careful here...
				false, false,
				value
				);
		}
	}
	else
	{
		// use 1.0 - curve value
		for (int i = 1; i <= nLength; i++)
		{
			//		value = 1 - static_cast<double>(i) / nLengthPlusOne;
			value = pCurve->getDataAt(i);

			BlendMotionFrame(
				pMotion, nStartFrame,
				pMotion, nEndFrame,
				pMotion, nStartFrame + i,  // be careful here...
				false, false,
				1.0 - value  // change take place here
				);
		}
	}

	return;	
}

void MotionEditor::InterpolateBetweenKeys_Linear(
		MotionClip* pMotion, std::vector<int> vecKeyFrame, bool bSorted /* = false */
		)
{
	if (vecKeyFrame.size() < 2)
		return;  // too few keyframes. do nothing.

	if (! bSorted)
	{
		std::sort ( vecKeyFrame.begin(), vecKeyFrame.end(), std::greater<int>() );
	}

	int last;
	last = vecKeyFrame.at(0);
	for (std::vector<int>::iterator it = vecKeyFrame.begin() +1;
			it != vecKeyFrame.end(); ++it)
	{
		ComplexCurve curve;
		CurveKeyPoint k;
		if ((*it) - last <1)
			continue; // it should never come here...

		curve.clear();
		k.clear();
		k.setPosition(0);
		k.setData(1.0);
		k.setOutType(CurveKeyPoint::Linear);
		curve.add_keypoint(k);

		k.clear();
		k.setPosition( (*it) - last );
		k.setData(0.0);
		k.setInType(CurveKeyPoint::Linear);
		curve.add_keypoint(k);

		InterpolateMotion(
			pMotion, last, (*it),
			&curve, false
			);

		last = (*it);
	}
}

//void MotionEditor::InterpolateBetweenAllKeys_Linear(
//		MotionClip* pMotion
//		)
//{
//	if (!pMotion)
//		return; 	// no motion !
//	MotionKeyframeSeries* pmkfs = pMotion->getKeyframeSeries();
//	if (!pmkfs)	
//		return; 	// no keyframes!
//
//	std::vector<int> vec;
//	for (MotionKeyframeSeries::iterator it = pmkfs->begin();
//			it != pmkfs->end(); ++it)
//	{
//		vec.push_back( (*it).first );
//	}
//
//	// by default, map is sorted internally using greater<int>(), so sorting is not need here.
//	//std::sort(vecKey.begin(), vecKey.end());
//
//	InterpolateBetweenKeys_Linear(
//			pMotion, vec, true
//			);
//}

unsigned int MotionEditor::CopyMotion(
									  MotionClip* pTargetMotion, int nTargetStartFrame, int nLength,
									  MotionClip* pSourceMotion, int nSourceStartFrame
									  )
{
	unsigned int nFrameToCopy;
	int t1, t2;
	t1 = pTargetMotion->getFrameCount() - nTargetStartFrame;  // count -1 - start + 1
	t1 = ( t1 < 0 ? 0 : t1 ); // should be positive
	t1 = ( t1 < nLength ? t1 : nLength ); // min (t1, nLength)
	t2 = pSourceMotion->getFrameCount() - nSourceStartFrame;  // count -1 - start + 1
	t2 = ( t2 < 0 ? 0 : t2 ); // should be positive	
	t2 = ( t2 < nLength ? t2 : nLength ); // min (t2, nLength)
	nFrameToCopy = ( t1 < t2 ? t1 : t2 ); // Get the small one

	CopyMotionRecursively(
		pTargetMotion, pTargetMotion->getRoot(), nTargetStartFrame,
		pSourceMotion, pSourceMotion->getRoot(), nSourceStartFrame,
		nFrameToCopy
		);

	return nFrameToCopy;
}

unsigned int MotionEditor::ReplicateMotionFrame(
	MotionClip* pTargetMotion, int nTargetStartFrame, int count,
	MotionClip* pSourceMotion, int nSourceFrame
	)
{
	unsigned int nFrameToCopy;
	int t1;
	t1 = pTargetMotion->getFrameCount()-1 - nTargetStartFrame;
	t1 = ( t1 < 0 ? 0 : t1 );
	t1 = ( t1 < count ? t1 : count ); // min (t1, count)
	nFrameToCopy = t1;

	ReplicateMotionFrameRecursively(
		pTargetMotion, pTargetMotion->getRoot(), nTargetStartFrame,
		pSourceMotion, pSourceMotion->getRoot(), nSourceFrame,
		nFrameToCopy
		);

	return nFrameToCopy;
}

void MotionEditor::BlendMotionFrame(
									MotionClip* pSourceMotion1, int nSourceFrame1,
									MotionClip* pSourceMotion2, int nSourceFrame2,
									MotionClip* pTargetMotion,  int nTargetFrame,
									bool bAlignRoot, bool bSynRoot,
									double dBlendFactor,
									std::vector<int> * pvecJointIndexes /* = NULL */
									)
{
	Vector3 preTransition(0,0,0);
	Quaternion preOrientation(0,0,0, 1.0f);

	MotionJoint* pRoot1 = pSourceMotion1->getRoot(0);
	MotionJoint* pRoot2 = pSourceMotion2->getRoot(0);
	if (! pSourceMotion1->compareSkeleton(pSourceMotion2) )
	{
		throw MotionEditorError("Can not blend two frames when the skeletons are different.");
		//return;
	}

	pSourceMotion1->setCurrentFrame(nSourceFrame1);
	pSourceMotion2->setCurrentFrame(nSourceFrame2);	

	if (bAlignRoot)
	{
		preTransition = pRoot1->getAbsolutePosition(nSourceFrame1)
			- pRoot2->getAbsolutePosition(nSourceFrame2) ;
	}
	if (bSynRoot)
	{
		preOrientation = (pRoot1->getOrientation(nSourceFrame1)).TransitionFromCurrentTo(
				pRoot2->getOrientation(nSourceFrame2) );
	}

	if (!pvecJointIndexes) 
	{
		// blending on all joints.

		int c = pSourceMotion1->GetIndexCount();
		for (int index = 0; index < c; index ++)
		{
			MotionJoint* resultJoint = 	pTargetMotion->findJoint(index);
			MotionJoint* pmj1 = 		pSourceMotion1->findJoint(index);
			MotionJoint* pmj2 =			pSourceMotion2->findJoint(index);
			Vector3 pos1, pos2, posResult;
			Quaternion qRot;

			if (resultJoint->m_JointType == MotionJoint::ROOT_JOINT)
			{
				pos1 = pmj1->getAbsolutePosition(nSourceFrame1);
				pos2 = pmj2->getAbsolutePosition(nSourceFrame2);
				posResult = pos1 * static_cast<double>(dBlendFactor) + (pos2 + preTransition) * (1 - static_cast<double>(dBlendFactor));
				resultJoint->setAbsolutePosition(posResult, nTargetFrame);

				qRot = Quaternion::nlerp( 1 - static_cast<double>(dBlendFactor),
						(pmj1->getOrientation(nSourceFrame1)* preOrientation),
						pmj2->getOrientation(nSourceFrame2) ,
						true
						);
				resultJoint->setOrientation(qRot, nTargetFrame);
			}		
			else if (resultJoint->m_JointType == MotionJoint::NORMAL_JOINT)
			{
				qRot = Quaternion::nlerp( 1 - static_cast<double>(dBlendFactor),
						pmj1->getOrientation(nSourceFrame1),
						(pmj2->getOrientation(nSourceFrame2)),
						true
						);
				resultJoint->setOrientation(qRot, nTargetFrame);
			}	
		}
	}
	else
	{
		// blending only on selected joints
		int index;

		for(int i = 0; i < static_cast<int>((*pvecJointIndexes).size()); ++i)

			//for (std::vector<int>::iterator it = (*pvecJointIndexes).begin();
			//	it != (*pvecJointIndexes).end(); ++it)
		{
			index = (*pvecJointIndexes)[i];

			MotionJoint* resultJoint = 	pTargetMotion->findJoint(index);
			MotionJoint* pmj1 = 		pSourceMotion1->findJoint(index);
			MotionJoint* pmj2 =			pSourceMotion2->findJoint(index);
			Vector3 pos1, pos2, posResult;
			Quaternion qRot;

			if (resultJoint->m_JointType == MotionJoint::ROOT_JOINT)
			{
				pos1 = pmj1->getAbsolutePosition(nSourceFrame1);
				pos2 = pmj2->getAbsolutePosition(nSourceFrame2);
				posResult = pos1 * static_cast<double>(dBlendFactor) + (pos2 + preTransition) * (1 - static_cast<double>(dBlendFactor));
				resultJoint->setAbsolutePosition(posResult, nTargetFrame);

				qRot = Quaternion::nlerp( 1 - static_cast<double>(dBlendFactor),
						(pmj1->getOrientation(nSourceFrame1)* preOrientation),
						pmj2->getOrientation(nSourceFrame2) ,
						true
						);
				resultJoint->setOrientation(qRot, nTargetFrame);
			}		
			else if (resultJoint->m_JointType == MotionJoint::NORMAL_JOINT)
			{
				qRot = Quaternion::nlerp( 1 - static_cast<double>(dBlendFactor),
						pmj1->getOrientation(nSourceFrame1),
						(pmj2->getOrientation(nSourceFrame2)),
						true
						);
				resultJoint->setOrientation(qRot, nTargetFrame);
			}	
		}
	}

	return ;
}



MotionClip* MotionEditor::CreateConcatenatedMotion(
	MotionClip* pSourceMotion1, int nSourceStartFrame1, int nSourceEndFrame1,
	MotionClip* pSourceMotion2, int nSourceStartFrame2, int nSourceEndFrame2,
	bool bFirstMotionComeFirst,
	int nTotalLength,
	BaseCurve* pCurve
	)
{
	MotionClip * pmc;

	if (! pSourceMotion1->compareSkeleton(pSourceMotion2) )
	{
		//AfxMessageBox("两动作骨架应当相同!");
		return NULL;
	}

	// create new
	pmc = CreateMotion( pSourceMotion1, nTotalLength );

	if (bFirstMotionComeFirst)
	{
		// copy the first part
		CopyMotion( pmc, 0, nSourceEndFrame1 - nSourceStartFrame1 + 1,
			pSourceMotion1, nSourceStartFrame1
			);

		// copy the third part
		CopyMotion( 
			pmc, nTotalLength - (nSourceEndFrame2 - nSourceStartFrame2 + 1), nSourceEndFrame2 - nSourceStartFrame2 + 1,
			pSourceMotion2, nSourceStartFrame2
			);

		// Interpolate for the second part
		InterpolateMotion(
			pmc, 
			nSourceEndFrame1 - nSourceStartFrame1 ,
			nTotalLength - (nSourceEndFrame2 - nSourceStartFrame2 + 1),
			pCurve,
			false
			);
	}
	else
	{	// second motion comes first.

		// copy the first part
		CopyMotion( pmc, 0, nSourceEndFrame2 - nSourceStartFrame2 + 1,
			pSourceMotion2, nSourceStartFrame2
			);

		// copy the third part
		CopyMotion( 
			pmc, nTotalLength - (nSourceEndFrame1 - nSourceStartFrame1 + 1), nSourceEndFrame1 - nSourceStartFrame1 + 1,
			pSourceMotion1, nSourceStartFrame1
			);

		// Interpolate for the second part
		InterpolateMotion(
			pmc, 
			nSourceEndFrame1 - nSourceStartFrame1 ,
			nTotalLength - (nSourceEndFrame2 - nSourceStartFrame2 + 1),
			pCurve,
			true
			);

	}
	return pmc;

}



void MotionEditor::TransformMotion(
	MotionClip* pTargetMotion, 
	bool bTransformTheWholeMotion,
	int nStartFrame, int nEndFrame,
	Vector3 vecOffset /* = Vector3::ZERO*/,
	Quaternion quatOrientation /* = Quaternion::IDENTITY*/
	)
{
	if (!pTargetMotion)
	{
		throw MotionEditorError("Null pointer inputed in MotionEditor::TransformMotion");
	}

	if (bTransformTheWholeMotion)
	{
		nStartFrame = 0;
		nEndFrame = pTargetMotion->getFrameCount()-1;
	}

	if (nEndFrame < nStartFrame)
	{
		// bad data inputed.
		throw MotionEditorError("bad range data inputed in MotionEditor::TransformMotion");
	}

	Vector3 vec, vecStart;
	Quaternion quat;
	MotionJoint* pRoot;
	pRoot = pTargetMotion->getRoot();

	// the first frame
	vec	= pRoot->getAbsolutePosition(nStartFrame);
	vecStart = vec;
	vec += vecOffset;
	pRoot->setAbsolutePosition(vec, nStartFrame);

	quat = pRoot->getOrientation(nStartFrame);
	//quat = quat * quatOrientation;
	quat = quatOrientation * quat;  // finally correct it...
	pRoot->setOrientation(quat, nStartFrame);

	for (int i = nStartFrame+ 1; i <= nEndFrame; ++i)
	{
		vec	= pRoot->getAbsolutePosition(i);
		vec = quatOrientation * (vec - vecStart) + vecStart;
		vec += vecOffset;
		pRoot->setAbsolutePosition(vec, i);

		quat = pRoot->getOrientation(i);
		//quat = quat * quatOrientation;
		quat = quatOrientation * quat;  // finally correct it...
		pRoot->setOrientation(quat, i);
	}

	return;
}


void MotionEditor::InplaceBlending(
		MotionClip* pTargetMotion,
		int nMiddleFrame,
		int nHalfRange,
		ComplexCurve& curve
		)
{
	if ( pTargetMotion == NULL 
			|| nHalfRange < 1
			|| nMiddleFrame - nHalfRange < 0
			|| curve.getLength() < 2 * nHalfRange
	   )
	{
		// error!
		//throw MotionEditorError("wrong data");

		//  emm...
		return;
	}

	// we assume motions are already aligned.
	int c = pTargetMotion->GetIndexCount();
	int nStartFrame1 = nMiddleFrame - nHalfRange;
	int nEndFrame1 = nMiddleFrame -1;
	int nStartFrame2 = nMiddleFrame;
	int nEndFrame2 = nMiddleFrame + nHalfRange -1;

	MotionJoint *pmj;
	Vector3 pos1, pos2, posResult;
	Quaternion qRot, q1, q2;
	
	// part one
	//   nMiddleFrame is the start of 2nd motion
	for (int i = nStartFrame1; i < nStartFrame2; ++i)
	{
		for (int j = 0; j < c; ++j)
		{ // for every joint
			pmj = pTargetMotion->findJoint(j);

			if (pmj->m_JointType == MotionJoint::ROOT_JOINT)
			{
				pos2 = pmj->getAbsolutePosition(nStartFrame2);
				q2 = pmj->getOrientation(nStartFrame2);
				pos1 = pmj->getAbsolutePosition(i);
				q1 = pmj->getOrientation(i);

				posResult = pos1 * static_cast<double>(curve.getDataAt(i - nStartFrame1))
						+ pos2 * (1 - static_cast<double>(curve.getDataAt(i - nStartFrame1)));
				qRot = Quaternion::nlerp( 1 - static_cast<double>(curve.getDataAt(i - nStartFrame1)),
						q1, q2, true);

				pmj->setAbsolutePosition(posResult, i);
				pmj->setOrientation(qRot, i);
			}
			else if(pmj->m_JointType == MotionJoint::NORMAL_JOINT)
			{
				q2 = pmj->getOrientation(nStartFrame2);
				q1 = pmj->getOrientation(i);
				qRot = Quaternion::nlerp( 1 - static_cast<double>(curve.getDataAt(i - nStartFrame1)),
						q1, q2, true);

				pmj->setOrientation(qRot, i);
			}

		} // for every joint

	} // every frame

	// part two
	for (int i = nStartFrame2; i <= nEndFrame2; ++i)
	{
		for (int j = 0; j < c; ++j)
		{ // for every joint
			pmj = pTargetMotion->findJoint(j);

			if (pmj->m_JointType == MotionJoint::ROOT_JOINT)
			{
				pos1 = pmj->getAbsolutePosition(nEndFrame1);
				q1 = pmj->getOrientation(nEndFrame1);
				pos2 = pmj->getAbsolutePosition(i);
				q2 = pmj->getOrientation(i);

				posResult = pos1 * static_cast<double>(curve.getDataAt(i - nStartFrame1))
						+ pos2 * (1 - static_cast<double>(curve.getDataAt(i - nStartFrame1)));
				qRot = Quaternion::nlerp( 1 - static_cast<double>(curve.getDataAt(i - nStartFrame1)),
						q1, q2, true);

				pmj->setAbsolutePosition(posResult, i);
				pmj->setOrientation(qRot, i);
			}
			else if(pmj->m_JointType == MotionJoint::NORMAL_JOINT)
			{
				q1 = pmj->getOrientation(nEndFrame1);
				q2 = pmj->getOrientation(i);
				qRot = Quaternion::nlerp( 1 - static_cast<double>(curve.getDataAt(i - nStartFrame1)),
						q1, q2, true);

				pmj->setOrientation(qRot, i);
			}

		} // for every joint

	} // every frame

}


void MotionEditor::OptimalAlign(
		MotionClip *pMotion1, int nFrame1,
		MotionClip *pMotion2, int nFrame2,
		Vector3 &vecResult,
		Quaternion &quatResult
		)
{
	int c1 = pMotion1->GetIndexCount();
	int c2 = pMotion2->GetIndexCount();

	if (c1 != c2)
	{
		// skeleton not the same.
		throw MotionEditorError("Two skeletons are different.");
	}

	if (c1 != 24)
	{
		// skeleton not the same.
		throw MotionEditorError("Won't work for this skeleton.");
	}

	double sum_x1, sum_x2, sum_z1, sum_z2;
	double avg_x1, avg_x2, avg_z1, avg_z2;
	double x1, z1, x2, z2;
	double s1;  // sum (x1 z2 - x2 z1)
	double s2;  // sum (x1 x2 + z1 z2)
	double sum_w; // sum of weights
	double thetaD;
	double cosTheta, sinTheta;
	MotionJoint *pmj1, *pmj2;
	Vector3 v1, v2;
	double x0, z0; // result

	// weights.
	double w[] = {
		1, 		0, 		0, 		0, 		0, 		//  0 -  4
		0, 		0,		0,		0, 		0,		//  5 -  9
		0, 		0, 		0, 		0,		0,		// 10 - 14
		0, 		0.5,	0,		0,		0,		// 15 - 19
		0.5, 	0,		0,		0				// 20 - 23
	};
	//double w[] = {
	//	1, 		0, 		0, 		0, 		0, 		//  0 -  4
	//	0, 		0,		0,		0, 		0,		//  5 -  9
	//	0, 		0, 		0, 		0,		0,		// 10 - 14
	//	0, 		0,		0,		0,		0,		// 15 - 19
	//	0, 		0,		0,		0				// 20 - 23
	//};

	s1 = s2 = 0.0;
	sum_x1 = sum_x2 = sum_z1 = sum_z2 = 0.0;
	sum_w = 0.0;

	for (int i = 0; i < c1; ++i)
	{
		pmj1 = pMotion1->findJoint(i);
		v1 = pmj1->getAbsolutePosition(nFrame1);
		x1 = v1.x();
		z1 = v1.z();

		pmj2 = pMotion2->findJoint(i);
		v2 = pmj2->getAbsolutePosition(nFrame2);
		x2 = v2.x();
		z2 = v2.z();

		sum_x1 += w[i] * x1;
		sum_z1 += w[i] * z1;
		sum_x2 += w[i] * x2;
		sum_z2 += w[i] * z2;

		s1 += w[i] * (x1 * z2 - x2 * z1);
		s2 += w[i] * (x1 * x2 + z1 * z2);

		sum_w += w[i];
	}

//	avg_x1 = sum_x1 / static_cast<double>(c1);
//	avg_x2 = sum_x2 / static_cast<double>(c1);
//	avg_z1 = sum_z1 / static_cast<double>(c1);
//	avg_z2 = sum_z2 / static_cast<double>(c1);


	thetaD = Math::ATanD2(
			s1 - (sum_x1 * sum_z2 - sum_x2 * sum_z1) / sum_w,
			s2 - (sum_x1 * sum_x2 + sum_z1 * sum_z2) / sum_w
			);
	cosTheta = Math::CosD(thetaD);
	sinTheta = Math::SinD(thetaD);

	x0 = (sum_x1 - sum_x2 * cosTheta - sum_z2 * sinTheta) / sum_w;
	z0 = (sum_z1 + sum_x2 * sinTheta - sum_z2 * cosTheta) / sum_w;


	// ok, the result.
	
	vecResult.x(x0);
	vecResult.y(0);
	vecResult.z(z0);

	quatResult = quatResult.fromAngleAxis(thetaD, 0.0, 1.0, 0.0);

}

void MotionEditor::Align(
		MotionClip *pMotion1, int nFrame1,
		MotionClip *pMotion2, int nFrame2,
		Vector3 &vecResult,
		Quaternion &quatResult
		)
{
	int c1 = pMotion1->GetIndexCount();
	int c2 = pMotion2->GetIndexCount();

	if (c1 != c2)
	{
		// skeleton not the same.
		throw MotionEditorError("Two skeletons are different.");
	}

	double thetaD;
	MotionJoint *pmj1, *pmj2;
	Vector3 v1, v2;
	Vector3 dirForward; // the forward direction of root joint, at T-pose.
	Vector3 dir1, dir2, vTemp;
	double temp;
	Quaternion q1, q2;

	pmj1 = pMotion1->getRoot();
	v1 = pmj1->getAbsolutePosition(nFrame1);
	q1 = pmj1->getOrientation(nFrame1);

	pmj2 = pMotion2->getRoot();
	v2 = pmj2->getAbsolutePosition(nFrame2);
	q2 = pmj2->getOrientation(nFrame2);

	vecResult = v1 - v2; // result 1

	dirForward = Vector3::UNIT_Z;
	dir1 = q1 * dirForward;
	dir2 = q2 * dirForward;

	dir1.set_y(0.0);
	dir2.set_y(0.0);

	temp = Dot(dir2, dir1);
	thetaD = Math::ACosD(temp);
	vTemp = Cross(dir2, dir1);
	if(vTemp.y() < 0)
	{
		thetaD = -thetaD;
	}

	quatResult = quatResult.fromAngleAxis(thetaD, 0.0, 1.0, 0.0); // result 2

}






/* protected functions */

void MotionEditor::FillDataField(MotionClip* pMotion, const int count)
{
	pMotion->setFrameCount(count);
	FillDataFieldRecursively(pMotion, count, pMotion->getRoot());
}

void MotionEditor::FillDataFieldRecursively(
	MotionClip* pMotion, const int count, MotionJoint* pmj
	)
{

	if (pmj->getPositionDataPointer())
	{
		pmj->getPositionDataPointer()->reserve(count);
		pmj->getPositionDataPointer()->insert(
			pmj->getPositionDataPointer()->begin(), 
			count, Vector3::ZERO
			);
	}

	if (pmj->getRotationDataPointer())
	{
		pmj->getRotationDataPointer()->reserve(count);
		pmj->getRotationDataPointer()->insert(
			pmj->getRotationDataPointer()->begin(),
			count, Vector3::ZERO
			);
	}

	if (pmj->getQuaternionDataPointer())
	{
		pmj->getQuaternionDataPointer()->reserve(count);
		pmj->getQuaternionDataPointer()->insert(
			pmj->getQuaternionDataPointer()->begin(),
			count, Quaternion::IDENTITY
			);
	}

	if (pmj->getScaleDataPointer())
	{
		pmj->getScaleDataPointer()->reserve(count);
		pmj->getScaleDataPointer()->insert(
			pmj->getScaleDataPointer()->begin(),
			count, Vector3::UNIT_SCALE
			);
	}

	pmj->NeedUpdate();

	int c = pMotion->getChildrenCount(pmj);
	for (int i = 0; i < c; i++)
	{
		FillDataFieldRecursively(pMotion, count, pMotion->getChild(pmj, i));
	}

}

void MotionEditor::ReserveDataField(MotionClip* pMotion, const int count)
{
	ReserveDataFieldRecursively(pMotion, count, pMotion->getRoot());
}

void MotionEditor::ReserveDataFieldRecursively(MotionClip* pMotion, const int count, MotionJoint* pmj)
{

	if (pmj->getPositionDataPointer())
	{
		pmj->getPositionDataPointer()->reserve(count);
	}

	if (pmj->getRotationDataPointer())
	{
		pmj->getRotationDataPointer()->reserve(count);
	}

	if (pmj->getQuaternionDataPointer())
	{
		pmj->getQuaternionDataPointer()->reserve(count);
	}

	if (pmj->getScaleDataPointer())
	{
		pmj->getScaleDataPointer()->reserve(count);
	}

	int c = pMotion->getChildrenCount(pmj);
	for (int i = 0; i < c; i++)
	{
		FillDataFieldRecursively(pMotion, count, pMotion->getChild(pmj, i));
	}

}

void MotionEditor::CopyMotionRecursively(
		MotionClip* pTargetMotion, MotionJoint* pmjTarget, int nTargetStartFrame,
		MotionClip* pSourceMotion, MotionJoint* pmjSource, int nSourceStartFrame,
		int nLength
		)
{
	for (int i = 0; i < nLength; i++)
	{
		if (pmjTarget->getPositionDataPointer())
		{	
			(*(pmjTarget->getPositionDataPointer())).at(nTargetStartFrame + i) 
				= (*(pmjSource->getPositionDataPointer())).at(nSourceStartFrame + i);
		}

		if (pmjTarget->getRotationDataPointer())
		{
			(*(pmjTarget->getRotationDataPointer())).at(nTargetStartFrame + i)
				= (*(pmjSource->getRotationDataPointer())).at(nSourceStartFrame + i);
		}

		if (pmjTarget->getQuaternionDataPointer())
		{
			(*(pmjTarget->getQuaternionDataPointer())).at(nTargetStartFrame + i)
				= (*(pmjSource->getQuaternionDataPointer())).at(nSourceStartFrame + i);
		}

		if (pmjTarget->getScaleDataPointer())
		{
			(*(pmjTarget->getScaleDataPointer())).at(nTargetStartFrame + i)
				= (*(pmjSource->getScaleDataPointer())).at(nSourceStartFrame + i);			
		}
	}

	pmjTarget->NeedUpdate();

	int c = pTargetMotion -> getChildrenCount( pmjTarget );

	for (int k = 0; k < c; k++)
	{
		CopyMotionRecursively(
				pTargetMotion, pTargetMotion->getChild(pmjTarget, k), nTargetStartFrame,
				pSourceMotion, pSourceMotion->getChild(pmjSource, k), nSourceStartFrame,
				nLength
				);
	}

	return;
}

void MotionEditor::ReplicateMotionFrameRecursively(
		MotionClip* pTargetMotion, MotionJoint* pmjTarget, int nTargetStartFrame,
		MotionClip* pSourceMotion, MotionJoint* pmjSource, int nSourceFrame,
		int count
		)
{
	for (int i = 0; i < count; i++)
	{
		if (pmjTarget->getPositionDataPointer())
		{	
			(*(pmjTarget->getPositionDataPointer())).at(nTargetStartFrame + i) 
				= (*(pmjSource->getPositionDataPointer())).at(nSourceFrame);
		}

		if (pmjTarget->getRotationDataPointer())
		{
			(*(pmjTarget->getRotationDataPointer())).at(nTargetStartFrame + i)
				= (*(pmjSource->getPositionDataPointer())).at(nSourceFrame);
		}

		if (pmjTarget->getQuaternionDataPointer())
		{
			(*(pmjTarget->getQuaternionDataPointer())).at(nTargetStartFrame + i)
				= (*(pmjSource->getQuaternionDataPointer())).at(nSourceFrame);
		}

		if (pmjTarget->getScaleDataPointer())
		{
			(*(pmjTarget->getScaleDataPointer())).at(nTargetStartFrame + i)
				= (*(pmjSource->getScaleDataPointer())).at(nSourceFrame);			
		}
	}

	pmjTarget->NeedUpdate();

	int c = pTargetMotion -> getChildrenCount( pmjTarget );

	for (int k = 0; k < c; k++)
	{
		CopyMotionRecursively(
				pTargetMotion, pTargetMotion->getChild(pmjTarget, k), nTargetStartFrame,
				pSourceMotion, pSourceMotion->getChild(pmjSource, k), nSourceFrame,
				count
				);
	}

	return;
}

Vector3 MotionEditor::GetDistance (
		MotionClip* pMotionFrom, int nFrameFrom,
		MotionClip* pMotionTo, int nFrameTo
		)
{
	MotionJoint *pmjFrom, *pmjTo;
	
	pmjFrom = pMotionFrom->getRoot();
	pmjTo 	= pMotionTo->getRoot();

	return pmjTo->getAbsolutePosition( nFrameTo ) 
		- pmjFrom->getAbsolutePosition( nFrameFrom );
}

void MotionEditor::ResampleMotion(MotionClip* pMotionClip, int nNewFrameCount, std::vector<int>* pJointIndex/* =NULL */)
{
	int nFrameCount = pMotionClip->getFrameCount();
	MotionJoint* pRoot = pMotionClip->getRoot();
	Vector3 vPos = pRoot->getAbsolutePosition(pMotionClip->getFrameCount()-1);
	if (nNewFrameCount > nFrameCount)
	{
		pMotionClip->insertFrameBehind(nFrameCount-1, nNewFrameCount-nFrameCount);
		for (int i=nFrameCount; i<nNewFrameCount; i++)
		{
			pRoot->setAbsolutePosition(vPos, i);
		}
	}

	if (!pJointIndex)
	{
		ResampleRecursively(pMotionClip, pRoot, nFrameCount, nNewFrameCount);
		//for (std::map<int, MotionJoint*>::iterator it=pMotionClip->m_JointsIndexMap.begin(); it!=pMotionClip->m_JointsIndexMap.end(); it++)
		//{
		//	ResampleJoint(pMotionClip->findJoint(it->first), nFrameCount, nNewFrameCount);
		//}
	}
	else
	{
		for (int i=0; i<pJointIndex->size(); i++)
		{
			ResampleJoint(pMotionClip->findJoint(pJointIndex->at(i)), nFrameCount, nNewFrameCount);
		}
	}

	if (nNewFrameCount < nFrameCount)
	{
		pMotionClip->deleteFrame(nNewFrameCount, nFrameCount-nNewFrameCount);
	}
}

void MotionEditor::ResampleRecursively(MotionClip* pMotionClip, MotionJoint* pMotionJoint, int nFrameCount, int nNewFrameCount)
{
	std::vector<Vector3> vAxis;
	std::vector<double> vAngle;
	Quaternion qOri;
	Vector3 axis;
	double angle;
	for (int i=0; i<nFrameCount; i++)
	{
		qOri = pMotionJoint->getOrientation(i);
		qOri.toAngleAxis(angle, axis);
		axis = axis * angle;
		vAxis.push_back(axis);
		vAngle.push_back(angle);
	}
	resample(vAxis, nNewFrameCount);
	for (int i=0; i<nNewFrameCount; i++)
	{
		axis = vAxis.at(i);
		axis.normalise();
		qOri.fromAngleAxis(vAxis.at(i).getNorm(), axis);
		pMotionJoint->setOrientation(qOri,i);
	}

	int nChildCount = pMotionClip->getChildrenCount(pMotionJoint);
	for (int i=0; i<nChildCount; i++)
	{
		MotionJoint* pmj = pMotionClip->getChild(pMotionJoint, i);
		ResampleRecursively(pMotionClip, pmj, nFrameCount, nNewFrameCount);
	}
}

void MotionEditor::ResampleJoint(MotionJoint* pMotionJoint, int nFrameCount, int nNewFrameCount)
{
	std::vector<Vector3> vAxis;
	std::vector<double> vAngle;
	Quaternion qOri;
	Vector3 axis;
	double angle;
	for (int i=0; i<nFrameCount; i++)
	{
		qOri = pMotionJoint->getOrientation(i);
		qOri.toAngleAxis(angle, axis);
		axis = axis * angle;
		vAxis.push_back(axis);
		vAngle.push_back(angle);
	}
	resample(vAxis, nNewFrameCount);
	for (int i=0; i<nNewFrameCount; i++)
	{
		axis = vAxis.at(i);
		axis.normalise();
		qOri.fromAngleAxis(vAxis.at(i).getNorm(), axis);
		pMotionJoint->setOrientation(qOri,i);
	}
}

void MotionEditor::resample(std::vector<Vector3>& vData, int m)
{
	int n = vData.size();
	double* px = ::new double[n];
	double* py = new double[n];
	double* pz = new double[n];

	for (size_t i=0; i<n; i++)
	{
		px[i] = vData.at(i).x();
		py[i] = vData.at(i).y();
		pz[i] = vData.at(i).z();
	}

	double *pnewx, *pnewy, *pnewz;
	pnewx = resample(n, px, m);
	pnewy = resample(n, py, m);
	pnewz = resample(n, pz, m);
	vData.clear();

	for (int j=0; j<m; j++)
	{
		Vector3 v;
		v.set_x(pnewx[j]);
		v.set_y(pnewy[j]);
		v.set_z(pnewz[j]);
		vData.push_back(v);
	}

	delete []px;
	delete []py;
	delete []pz;
	delete []pnewx;
	delete []pnewy;
	delete []pnewz;
}

double* MotionEditor::resample(int n, double* pData, int m)
{
	double* pNewData = new double[m];
	double* t = new double[n];
	for (int i=0; i<n; i++)
	{
		t[i] = i;
	}


	// 计算各已知点的二阶导数
	double *dd;
	dd = spline_cubic_set(n, t, pData, 0, 0, 0, 0);		// 端点二阶导数待测试

	// 重采样，采样数由sampleNum决定
	double dt = (double)n / (double)m;
	double tt = 0;
	double dv = 0;
	double ddv = 0;

	Vector3 accTemp;
	for(int j=0; j<m; j++)
	{
		pNewData[j] = spline_cubic_val(n, t, tt, pData, dd, &dv, &ddv);
		tt += dt;
	}

	delete []t;
	return pNewData;
}

