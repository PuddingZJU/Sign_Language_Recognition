#ifndef ZZ_CONFIGURATIONS_ABOUT_Recognition_20080625__H___
#define ZZ_CONFIGURATIONS_ABOUT_Recognition_20080625__H___
#pragma once
/**
 * @brief  Global Configurations
 *
 * @author Thinapple (thinapple@gmail.com)
 */


#include <string>

namespace Swift
{
;

/// The directory splitter
extern const std::string RecognitionDIRSplitter;

/// Where are the standard elements?

/// return the path to file according to the RecognitionGlobalBaseDIR
inline std::string RecognitionFile(const std::string & file);

/// The global base directory
extern const std::string RecognitionGlobalBaseDIR;

/// standard skeleton file name (T-pose)
extern const std::string RecognitionStdSkeletonFile;

/// standard mesh file name
extern const std::string RecognitionStdMeshFile;

/// standard mesh mapping file name
extern const std::string RecognitionStdMappingFile;

/// standard mesh skeleton
extern const std::string RecognitionStdMeshSkeletonFile;

/// wheather do background loading when entering draft retrieval
extern const bool RecognitionBgLoading_DraftRetrieval;


inline std::string RecognitionFile(const std::string & file)
{
	return RecognitionGlobalBaseDIR + RecognitionDIRSplitter + file;
} // RecognitionFile( file )


} // namespace Recognition

#endif

