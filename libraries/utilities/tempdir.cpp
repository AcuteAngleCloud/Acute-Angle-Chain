/**
 *  @file
 *  @copyright defined in AcuteAngleChain/LICENSE.txt
 */

#include <aacio/utilities/tempdir.hpp>

#include <cstdlib>

namespace aacio { namespace utilities {

fc::path temp_directory_path()
{
   const char* aac_tempdir = getenv("AAC_TEMPDIR");
   if( aac_tempdir != nullptr )
      return fc::path( aac_tempdir );
   return fc::temp_directory_path() / "aac-tmp";
}

} } // aacio::utilities
