#ifndef LCCD_EXCEPTION_H
#define LCCD_EXCEPTION_H 1

#include <string>
#include <exception> 
#include "lccd.h"

// define some exceptions similar to the ones used in lcio
// the exceptions should be part of the corresponding namespace
// even though they are not defined in the package (subdirectory)
// as these hold interfaces (abstract base classes) only ....

namespace lccd{

  /**Exception used for general LCCD errors.
   * @author rdiener
   * @version July 02, 2010
   */
  class LCCDException : public std::exception{

  protected:
    std::string message{} ;
    
    LCCDException(){  /*no_op*/ ; } 
      
  public: 
    virtual ~LCCDException() noexcept { /*no_op*/; }

    LCCDException( const std::string& text ) {
      message = "lccd::Exception: " + text ;
    }
    
    virtual const char* what() const noexcept { return  message.c_str() ; }
  }; 
      
  /**DatabaseException used for errors accessing the database data.
   * @author rdiener
   * @version July 02, 2010
   */
  class DatabaseException : public LCCDException{
    
  protected:
    DatabaseException() {  /*no_op*/ ; } 
  public: 
    virtual ~DatabaseException() noexcept { /*no_op*/; }

    DatabaseException( std::string text ){
      message = "lccd::DatabaseException: " + text ;
    }
  }; 

  /**Exception used for data not available.
   * @author rdiener
   * @version July 02, 2010
   */
  class DataNotAvailableException : public LCCDException{

  public: 
    virtual ~DataNotAvailableException() noexcept { /*no_op*/; }

    DataNotAvailableException( std::string text ) {
      message = "lccd::DataNotAvailableException: " + text ;
    }
  }; 

  /**Exception used for signaling a 'read only exception'.
   * @author rdiener
   * @version July 02, 2010
   */
  class ReadOnlyException : public LCCDException{

  public: 
    virtual ~ReadOnlyException() noexcept { /*no_op*/; }

    ReadOnlyException( std::string text ){
      message = "lccd::ReadOnlyException: " + text ;
    }
  }; 

  /**Exception used for (possible) inconsistency warnings
   * @author rdiener
   * @version July 02, 2010
   */
  class InconsistencyException : public LCCDException{

  public: 
    virtual ~InconsistencyException() noexcept { /*no_op*/; }

    InconsistencyException( std::string text ) {
      message = "lccd::InconsistencyException: " + text ;
    }
  }; 

  /**Exception used for not implemented member functions
   * @author rdiener
   * @version July 02, 2010
   */
  class MemberNotImplementedException : public LCCDException{

  public: 
    virtual ~MemberNotImplementedException() noexcept { /*no_op*/; }

    MemberNotImplementedException( std::string text ) {
      message = "lccd::MemberNotImplementedException: " + text ;
    }
  }; 


} // namespace

#endif /* ifndef LCCD_EXCEPTION_H */
