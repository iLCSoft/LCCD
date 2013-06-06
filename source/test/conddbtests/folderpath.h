#ifndef folderpath_h
#define folderpath_h 1

#include <string>

/** simple helper function for separating a complete path into a 
 *  folder and a collection name:
 *  on return folder will either contain the path given in arg
 *  (starting with '/') or '/lccd/' and col will contain the 
 *  last part of arg.
 */

inline void split_folder_col( const std::string& arg , std::string& folder , std::string& col  ){

  if( arg[0] == '/' ){

    folder = arg ;

    unsigned index = folder.rfind("/") ;

    col = arg.substr( index + 1 ,  arg.size() ) ;

  } else {

    col = arg ;
    folder = "/lccd/" + col ;
  }

}


#endif



