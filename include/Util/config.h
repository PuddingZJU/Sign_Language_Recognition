#ifndef ZZ_CONFIGURATIONS_ABOUT_Swift_20080625__H___
#define ZZ_CONFIGURATIONS_ABOUT_Swift_20080625__H___
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
extern const std::string SwiftDIRSplitter;

/// Where are the standard elements?

/// return the path to file according to the SwiftGlobalBaseDIR
inline std::string SwiftFile(const std::string & file);

/// The global base directory
extern const std::string SwiftGlobalBaseDIR;

/// standard skeleton file name (T-pose)
extern const std::string SwiftStdSkeletonFile;

/// standard mesh file name
extern const std::string SwiftStdMeshFile;

/// standard mesh mapping file name
extern const std::string SwiftStdMappingFile;

/// standard mesh skeleton
extern const std::string SwiftStdMeshSkeletonFile;

/// wheather do background loading when entering draft retrieval
extern const bool SwiftBgLoading_DraftRetrieval;


inline std::string SwiftFile(const std::string & file)
{
	return SwiftGlobalBaseDIR + SwiftDIRSplitter + file;
} // SwiftFile( file )


} // namespace Swift

#endif

