/* by Wei Yang. */
/* Last Modified:  2008/05/26 05:33:48 . */

#ifndef _MOTION_EDITOR_H_INCLUDED_
#define _MOTION_EDITOR_H_INCLUDED_

#include "../MotionData/MotionClip.h"
#include "../MathLib/Mathematics.h"
#include "../CurveEditor/include/Curve.h"

#include <exception>
#include <vector>

namespace Swift
{
	/// A class that contains some base motion-editing algorithm.
	/**
	*  This class is designed to contain algorithms.
	*/
	class   MotionEditor
	{
		//        enum BlendingAlgorithm
		//        {
		//            ALGO_CONSTANT,  /**< a * factor + b * (1 - factor) */
		//            ALGO_LINEAR,    /**< a * pos/length + b * (1 - pos/length) */
		//            ALGO_QUADRATIC  /**< quaternion slerp */
		//        };


	private:    
		/* LIFECYCLE  */ 

		/** 
		* Copy constructor. Private.
		*/
		MotionEditor ( const MotionEditor &other )
		{}  

		/* OPERATORS  */

		/**
		* Assign operator. Private.
		*/
		const MotionEditor& operator = ( const MotionEditor &other );
	

	public:
		/* LIFECYCLE  */ 

		/**
		* Constructor.
		*/
		MotionEditor ();
		/**
		* Cleans up.
		*/
		virtual ~MotionEditor ();

		/* OPERATORS  */

		/* OPERATIONS */

		/**
		* Build a MotionClip with given skeleton and length, use data to fill data field.
		* @param pMotion Pointer to a MotionClip which holds the skeleton we want.
		* @param nLength The length we want.
		* @return Target MotionClip.
		* @sa CreateEmptyMotion
		*/
		MotionClip* CreateMotion (MotionClip* pMotion, int nLength);

		/**
		* Build a MotionClip with given skeleton and length, DO NOT fill data in to data field.
		* @param pMotion Pointer to a MotionClip which holds the skeleton we want.
		* @param nLength The length we want.
		* @return Target MotionClip.
		* @sa CreateMotion
		*/
		MotionClip* CreateEmptyMotion (MotionClip* pMotion, int nLength);

		/**
		* Create a sub-motion-clip from a given motionclip.
		* @param pMotion Pointer to the source MotionClip.
		* @param nStartFrame The start-frame-number of sub-motion-clip.
		* @param nEndFrame The end-frame-number of sub-motion-clip.
		* @return Target MotionClip.
		*/
		MotionClip* CreateSubMotion(
			MotionClip* pMotion, int nStartFrame, int nEndFrame
			);

		/**
		* Create a new MotionClip, which is the result of blending of two MotionClip.
		* @param pMotion1 Pointer to Motion1.
		* @param pMotion2 Pointer to Motion2.
		* @param pCurve Pointer to the factor curve for blending.
		* @note The curve pCurve pointed to should NOT be shorter then the shorter one of the two MotionClips.
		* @return The new MotionClip.
		*/
		MotionClip* CreateBlendMotion(
			MotionClip* pMotion1, MotionClip* pMotion2,
			BaseCurve* pCurve
			);

		/**
		* Blend parts of motion from two motions, and put the result into target motion.
		* @param pSourceMotion1 Pointer to source-motion-1.
		* @param nSourceStartFrame1 Frame number of source-start-frame in source-motion-1.
		* @param pSourceMotion2 Pointer to source-motion-2.
		* @param nSourceStartFrame2 Frame number of source-start-frame in source-motion-2.
		* @param nLength The length of the result.
		* @param pTargetMotion Pointer to target-motion.
		* @param nTargetStartFrame Frame number of target-start-frame.
		* @param pCurve Pointer to the factor curve for blending.
		* @note The curve pCurve pointed to should NOT be shorter then nLength. 
		*/
		void BlendMotion(
			MotionClip* pSourceMotion1, int nSourceStartFrame1, 
			MotionClip* pSourceMotion2, int nSourceStartFrame2, 
			int nLength,  
			MotionClip* pTargetMotion, int nTargetStartFrame,
			BaseCurve* pCurve,
			std::vector<int> * pvecJointIndexes = NULL
			);

		/**
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
		* @param pvecJointIndexes Pointer to vector that contains indexes of affected joints.
		* @note There's no pos1 because pos1 always be zero.
		* @note The factor curve starts for position 0 .
		* @return Pointer to a new MotionClip holding the result.
		* @sa BlendTwoMotions_Aligned .
		*/
		MotionClip* BlendTwoMotions(
			MotionClip* pSourceMotion1, MotionClip* pSourceMotion2,
			int pos2, int pos3, int pos4,
			bool bFirstPartFromFirstClip, bool bLastPartFromFirstClip,
			int nPos1SourceFrame, int nPos3SourceFrame,
			int nPos2Motion1SourceFrame, int nPos2Motion2SourceFrame,
			BaseCurve* pCurve, 
			std::vector<int> * pvecJointIndexes = NULL
			) ;

		/**
		* Some as BlendTwoMotions(). But the root joints of two motions are aligned.
		* @sa BlendTwoMotions .
		*/
		MotionClip* BlendTwoMotions_Aligned(
			MotionClip* pSourceMotion1, MotionClip* pSourceMotion2,
			int pos2, int pos3, int pos4,
			bool bFirstPartFromFirstClip, bool bLastPartFromFirstClip,
			int nPos1SourceFrame, int nPos3SourceFrame,
			int nPos2Motion1SourceFrame, int nPos2Motion2SourceFrame,
			BaseCurve* pCurve,
			std::vector<int> * pvecJointIndexes = NULL
			) ;

		/**
		* Create the interpolate result MotionClip.
		* @param pMotion1 Pointer to Motion1.
		* @param nFrame1 The frame number in Motion1 where the interpolation starts.
		* @param pMotion2 Pointer to Motion1. 
		* @param nFrame2 The frame number in Motion2 where the interpolation ends.
		* @param nLength How many frames will the result have.
		* @param pCurve Pointer to a factor curve.
		*/
		MotionClip* CreateInterpolateMotion(
			MotionClip* pMotion1, int nFrame1, 
			MotionClip* pMotion2, int nFrame2, 
			int nLength,
			BaseCurve* pCurve
			);

		/**
		* Interpolate within the given MotionClip.\n This function will product frames between two given frames with interpolation.
		* @param pMotion Pointer to the MotionClip we want to operate.
		* @param nStartFrame The frame number of the start frame.
		* @param nEndFrame The frame number of the end frame.
		* @param pCurve Pointer to a factor curve.
		* @param bComplementalCurve Use (1.0 - curve value) when performing a interpolation. Set to false by default.
		*/
		void InterpolateMotion(
			MotionClip* pMotion, int nStartFrame, int nEndFrame,
			BaseCurve* pCurve, bool bComplementalCurve = false
			);

		/**
		 * This function calls Interpolate, to perform interpolation between given keys of the very Motion.
		 * @param pMotion Pointer to the source(and target) motion.
		 * @param vecKeyFrame A Vector of integer, the frame numbers of the keyframes we want to operate on.
		 * @param bSorted Indicate whether the frame numbers in the vector is sorted (ascend).
		 */
		void InterpolateBetweenKeys_Linear(
				MotionClip* pMotion,  std::vector<int> vecKeyFrame, bool bSorted = false
				);

		/**
		 * This function calls InterpolateBetweenKeys_Linear(),  operate on ALL keyframes.
		 * @param pMotion Pointer to the source(and target) motion.
		 * @sa InterpolateBetweenKeys_Linear.
		 */
		void InterpolateBetweenAllKeys_Linear(
				MotionClip* pMotion
				);

		/**
		* Copy a clip of Motion into another MotionClip.
		* @param pTargetMotion Pointer to target MotionClip.
		* @param nTargetStartFrame Where to put the copied frames, in the target MotionClip.
		* @param nLength How many frames we want to copy.
		* @param pSourceMotion Pointer to source MotionClip.
		* @param nSourceStartFrame Where to start copying, in the source MotionClip.
		* @note If there's NOT enough room in the target MotionClip, the target MotionClip will NOT be extended and some data will be ignored. If there's NOT enough frames left in source MotionClip to copy, we will copy as more as possibel.
		* @return How many frames do we actually copied.
		*/
		unsigned int CopyMotion(
			MotionClip* pTargetMotion, int nTargetStartFrame, int nLength,
			MotionClip* pSourceMotion, int nSourceStartFrame
			);

		/**
		* Copy a FRAME of Motion into some frames in other MotionClip.
		* @param pTargetMotion Pointer to target MotionClip.
		* @param nTargetStartFrame Where to put the copied frames, in the target MotionClip.
		* @param count How many frames we want to copy.
		* @param pSourceMotion Pointer to source MotionClip.
		* @param nSourceFrame Source frame number.
		* @note If there's NOT enough room in the target MotionClip, the target MotionClip will NOT be extended and some data will be ignored.
		* @return How many frames do we actually copied.
		*/
		unsigned int ReplicateMotionFrame(
			MotionClip* pTargetMotion, int nTargetStartFrame, int count,
			MotionClip* pSourceMotion, int nSourceFrame
			);

		/**
		* Blending data from two frames into one frame, and store it to target-frame.\n Spherical interpolation is used.
		* @param pSourceMotion1 Pointer to source-motion-1.
		* @param nSourceFrame1 Frame number for source-frame-1 in source-motion-1.
		* @param pSourceMotion2 Pointer to source-motion-2.
		* @param nSourceFrame2 Frame number for source-frame-2 in source-motion-2.
		* @param pTargetMotion Pointer to target MotionClip.
		* @param nTargetFrame Frame number for target frame in target MotionClip.
		* @param bAlignRoot Whether the offset position of two root-joints should be taken into consider.
		* @param bSynRoot Whether the offset orientation of two root-joints should be taken into consider.
		* @param dBlendFactor The blending factor used in spherical interpolation.
		*/
		void BlendMotionFrame(
			MotionClip* pSourceMotion1, int nSourceFrame1,
			MotionClip* pSourceMotion2, int nSourceFrame2,
			MotionClip* pTargetMotion, int nTargetFrame,
			bool bAlignRoot, bool bSynRoot,
			double dBlendFactor,
			std::vector<int> * pvecJointIndexes = NULL
            );


        /**
         * Concatenate two motion series.
         * @param pSourceMotion1 Pointer to source-motion-1.
         * @param nSourceStartFrame1 Frame number for source-start-frame in source-motion-1.
         * @param nSourceEndFrame1 Frame number for source-end-frame in source-motion-1.
         * @param pSourceMotion2 Pointer to source-motion-2.
         * @param nSourceStartFrame2 Frame number for source-start-frame in source-motion-2.
         * @param nSourceEndFrame2 Frame number for source-end-frame in source-motion-2.
		 * @param bFirstMotionComeFirst Tells if the first MotionClip comes first in a concatenation.
         * @param nTotalLength The length of the new MotionClip.
         * @return Pointer to new MotionClip.
         */
        MotionClip* CreateConcatenatedMotion(
                MotionClip* pSourceMotion1, int nSourceStartFrame1, int nSourceEndFrame1,
                MotionClip* pSourceMotion2, int nSourceStartFrame2, int nSourceEndFrame2,
				bool bFirstMotionComeFirst,
                int nTotalLength,
				BaseCurve* pCurve
                );

		/**
		 * Perform a transformation on the root joint, in every frame of the given range.
		 * @param pTargetMotion Pointer to target-motion.
		 * @param bTransformTheWholeMotion Set this if the all frames in the motion should be transformed.
		 * @param nStartFrame The starting frame of the transformation.
		 * @param nEndFrame The ending frame of the transformation.
		 * @param vecOffset The position-offset of the root joint.
		 * @param quatOrientation The orientation we want to apply on the root joint.
		 * @note If bTransformTheWholeMotion is set to be true, nStartFrame and nEndFrame would be ignored.
		 */
		void TransformMotion(
				MotionClip* pTargetMotion, 
				bool bTransformTheWholeMotion,
				int nStartFrame, int nEndFrame,
				Vector3 vecOffset = Vector3::ZERO,
				Quaternion quatOrientation = Quaternion::IDENTITY
				);

		/**
		 * Perform a inplace-blending for two adjacent motion-clips.
		 * <PRE>
		 *                              | range  |
		 *                              V        V
		 *                         |----+---|          
		 *        pTargetMotion              |---+-----|    
		 *                                   ^    
		 *                          1st   middle    2nd
		 * </PRE>
		 * @param pTargetMotion Pointer to target-motion.
		 * @param nMiddleFrame The starting frame of the second motion.
		 * @param nHalfRange Half of blending-range.
		 * @param curve Blending curve.
		 * @note nMiddleFrame should >= 1, nMiddleFrame - nHalfRange should >= 0
		 */
		void InplaceBlending(
				MotionClip* pTargetMotion,
				int nMiddleFrame,
				int nHalfRange,
				ComplexCurve& curve
				);

		/**
		 * Given two frames of motion, calculate the [best transition on plane xz, and rotation about axis-y] for second motion frame that makes the distance minimal.
		 * @note Use the method on "Motion Graph" [SIGGRAPH02]
		 * @param pMotion1 Pointer to first motion.
		 * @param nFrame1 Frame number for first motion frame.
		 * @param pMotion2 Pointer to second motion.
		 * @param nFrame2 Frame number for first motion frame.
		 * @param vecResult [Out] transition on plane xz.
		 * @param quatResult [Out] rotation about axis-y.
		 */
		void OptimalAlign(
				MotionClip* pMotion1,
				int nFrame1,
				MotionClip* pMotion2,
				int nFrame2,
				Vector3 &vecResult,
				Quaternion &quatResult
				);
		/**
		 * Given two frames of motion, calculate the [best transition on plane xz, and rotation about axis-y] for second motion frame that makes the distance minimal.
		 * @note Use my own method.
		 * @param pMotion1 Pointer to first motion.
		 * @param nFrame1 Frame number for first motion frame.
		 * @param pMotion2 Pointer to second motion.
		 * @param nFrame2 Frame number for first motion frame.
		 * @param vecResult [Out] transition on plane xz.
		 * @param quatResult [Out] rotation about axis-y.
		 */
		void Align(
				MotionClip* pMotion1,
				int nFrame1,
				MotionClip* pMotion2,
				int nFrame2,
				Vector3 &vecResult,
				Quaternion &quatResult
				);

/** ********* protected methods ************************************************** **/

       
        /**
         * Extend and fill data field for a MotionClip, usually used after CloneSkeleton() is called.
         * @param pMotion Pointer to the MotionClip we want to operate.
         * @param count How many frames will the MotionClip contain.
         * @note Position data is filled with (0,0,0), \n Quaternion Orientation is filled with ((0,0,0),1), \n Euler Angles Rotation is filled with (0,0,0), \n Scale factor is filled with (1,1,1).
         */
        void FillDataField(MotionClip* pMotion, const int count);

		 protected:
        /**
         * Called by FillDataField()
         * @sa FillDataField
         */
        void FillDataFieldRecursively(MotionClip* pMotion, const int count, MotionJoint* pmj);

        /**
         * Extend data field for a MotionClip, usually used after CloneSkeleton() is called.
         * @param pMotion Pointer to the MotionClip we want to operate.
         * @param count How many frames will the MotionClip contain.
         * @sa FillDataField
         */
        void ReserveDataField(MotionClip* pMotion, const int count);

        /**
         * Called by ReserveDataField()
         * @sa ReserveDataField
         */
        void ReserveDataFieldRecursively(MotionClip* pMotion, const int count, MotionJoint* pmj);

        /**
         * Called by CopyMotion()
         * @sa CopyMotion
         * @note CopyMotion() will guarantee that TargetMotion will have enough room for the data.
         */
        void CopyMotionRecursively(
                MotionClip* pTargetMotion, MotionJoint* pmjTarget, int nTargetStartFrame,
                MotionClip* pSourceMotion, MotionJoint* pmjSource, int nSourceStartFrame,
                int nLength
                );


        /**
         * Called by ReplicateMotionFrame()
         * @sa ReplicateMotionFrame
         * @note ReplicateMotionFrame() will guarantee that TargetMotion will have enough room for the data.
         */
        void ReplicateMotionFrameRecursively(
                MotionClip* pTargetMotion, MotionJoint* pmjTarget, int nTargetStartFrame,
                MotionClip* pSourceMotion, MotionJoint* pmjSource, int nSourceFrame,
                int count
                );

		/**
		 * Get the offset of root joints, from different MotionClips and different frame-numbers.
		 * @param pMotionFrom Pointer to the MotionClip from which we calculate.
		 * @param pMotionTo Pointer to the MotionClip to which we calculate.
		 * @return The offset we want.
		 * @note If we add the result Vector3 to the root of MotionFrom at frame nFrameFrom, we get the position of root joint of MotionTo at frame nFrameTo.
		 */
		Vector3 GetDistance(
				MotionClip* pMotionFrom, int nFrameFrom,
				MotionClip* pMotionTo, int nFrameTo
				);


        /*   ACCESS   */

        /*  INQUIRY   */



		//xiubo.liang
	public:
		void ResampleMotion(MotionClip* pMotionClip, int nNewFrameCount, std::vector<int>* pJointIndex=NULL);

		void ResampleRecursively(MotionClip* pMotionClip, MotionJoint* pMotionJoint, int nFrameCount, int nNewFrameCount);

		void ResampleJoint(MotionJoint* pMotionJoint, int nFrameCount, int nNewFrameCount);

		/// resample the data.
		/**
		* @param n, the number of points in the data set.
		* @param pData, the data set.
		* @param m, the new number of the points.
		* @return the resampled data set.
		*/
		double* resample(int n, double* pData, int m);

		/// resample the data.
		/**
		* @param vData, the data set.
		* @param m, the new number of the points.
		*/
		void resample(std::vector<Vector3>& vData, int m);

    }; /* -----  end of class  MotionEditor  ----- */


    /// Exception class for MotionEditor.
    /**
     * An object of this class would be throw once an error occurred in MotionEditor.
     */
    class   MotionEditorError : public std::exception
    {
        public:
            MotionEditorError(const std::string& str) 
                : std::exception(str.c_str())
            {
            }

            MotionEditorError(const char* pmsg) 
                : std::exception(pmsg)
            {
            }
    }; /* -----  end of class  MotionEditorError ----- */

}

#endif //_MOTION_EDITOR_H_INCLUDED_

