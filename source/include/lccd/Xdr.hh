#ifndef Xdr_h
#define Xdr_h

#ifdef USE_CONDDB

// 
#include "lccd.h"

#include <typeinfo>

namespace lccd {
  
  /** Utility functions for DB streamer objects. 
   *  These functions take care of the byte ordering and
   *  make sure the streamed data adheres to the xdr convention, e.g.
   *  every data item starts at a 32-bit word boundary.
   * 
   *  @author F.Gaede, DESY
   *  @version $Id: Xdr.hh,v 1.2 2005-02-17 15:45:53 gaede Exp $
   */

  class Xdr {
    
  public:
    
    /** Copies the the data in XDR, i.e. machine independent format 
     *  from one location to the another
     */
    static void copy( unsigned const char* from, unsigned char* dest, 
		      const int size, const int count);



    /** Writes count objects of (simple) type T to the string s
     *  in Xdr format.
     */
    template <class T>
    static void tostream( std::string& s , T* t, unsigned count ) {
      
      int bytlen, padlen;
      bytlen = count * sizeof( T ) ;
      padlen = (bytlen + 3) & 0xfffffffc ; 
      
      
      unsigned index = s.size() ;
      
      s.resize( s.size() + padlen ) ;
      
      unsigned char* dest = reinterpret_cast< unsigned char* > (  & s[ index ] ) ;
      unsigned char* from = reinterpret_cast< unsigned char* > ( t )  ;

//        std::cout << " tostream: 0x" << std::hex << *t  << std::dec << std::endl ; 

      Xdr::copy( from , dest , sizeof( T ) , count  ) ;
      
    }

    /** Read count objects of (simple) type t from the string/stream s at position.<br>
     *  Callers have to keep the current position within the string.
     * 
     *  @return the new position in the stream.
     */ 
    template <class T>
    static unsigned fromstream( const std::string& s , unsigned position, T* t, unsigned count ) {
      
      if( count == 0 )
	return position ;

      unsigned bytlen, padlen;
      bytlen = count * sizeof( T ) ;
      padlen = (bytlen + 3) & 0xfffffffc ; 
      

      unsigned const char* from = reinterpret_cast< unsigned const char* > (  & s[ position ] ) ;
      unsigned char* dest = reinterpret_cast< unsigned char* > ( t )  ;
      


      Xdr::copy( from , dest , sizeof( T ) , count  ) ;

     
//       std::cout << " fromstream: " ; 
//       for( unsigned int i=0; i < count; i++ )
//  	std::cout	<< std::hex << t[i]  << ", " << std::dec  ;
//       std::cout	<< "   - [" << position << "] - type:" 
//  		<< typeid( *t ).name()  
//  		<< std::endl ; 
      
      
      return position + padlen ;
    }
           
  };
  
} ; //end namespace

#endif // USE_CONDDB
#endif // Xdr_h
