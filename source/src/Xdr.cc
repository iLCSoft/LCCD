#include"lccd/Xdr.hh"

#include <iostream>

namespace lccd{

  //------------------------------------------
  // Code copied from SIO_functions.
  //------------------------------------------

#if defined(__alpha__) ||   defined(__i386__)  ||   defined(_M_ALPHA)  ||   defined(_M_IX86) || defined(__x86_64__)
#define XDR_LITTLE_ENDIAN
#endif
  
#if defined(_AIX)      ||   defined(__sparc__) ||   defined(_M_PPC)
#define XDR_BIG_ENDIAN
#endif
  
#ifdef XDR_BIG_ENDIAN
  void Xdr::copy( unsigned const char*     from,
		  unsigned char*     dest,
		  const int          size,
		  const int          count ) {
    int totlen;

    totlen = size * count;

    // Keep the byte ordering during the copy.
    memcpy( dest, from, totlen );
  }
#endif

#ifdef XDR_LITTLE_ENDIAN
  void Xdr::copy( unsigned const char*     from,
		  unsigned char*     dest,
		  const int          size,
		  const int          count ) {

    int icnt, ibyt, jump;
    dest += size;
    jump  = size << 1;

    //  Reverse the byte ordering during the copy.
    for( icnt = 0; icnt < count; icnt++ ) {

      for( ibyt = 0; ibyt < size; ibyt++  ) {

        *--dest = *from++;
      }

      dest += jump;
    }
    
  }
#endif



  /** Specialization for strings - count is ignored and taken to be 1.
   */
  template<>
  void Xdr::tostream( std::string& s , std::string* strVal, unsigned count ) {
    
    int  strSize = (int) strVal->size() ;

    int bytlen, padlen;
    bytlen = strSize + sizeof( int )  ;

    padlen = (bytlen + 3) & 0xfffffffc ; 
    
    unsigned index = s.size() ;
    
    s.resize( index + padlen ) ;
    

    // write the string's size first 

    unsigned char* dest = reinterpret_cast< unsigned char* > (  & s[ index ] ) ;
    unsigned char* from = reinterpret_cast< unsigned char* > ( & strSize )  ;

    Xdr::copy( from , dest , sizeof( int ) , 1  ) ;
    

    dest = reinterpret_cast< unsigned char* > (  & s[ index + sizeof( int ) ] ) ;
    from = reinterpret_cast< unsigned char* > ( & strVal->operator[](0) )  ;
 
//     std::cout << " tostream: - " << *strVal 
// 	      << " - lenght: " << strSize
// 	      << std::endl ;
    
    Xdr::copy( from , dest , 1 , strSize  ) ;
    
  }

  
  /** Specialization for strings - count is ignored and taken to be 1.
   *  @return the new position in the stream.
   */ 
  template <>
  unsigned  Xdr::fromstream( const std::string& s, unsigned index, 
			     std::string* strVal, unsigned count ) {
    
    if( count == 0 )
      return index ;
    
    
    // read the string length first 
    int strSize ;
    unsigned const char* from = reinterpret_cast< unsigned const char* > (  & s[ index ] ) ;
    unsigned char* dest = reinterpret_cast< unsigned char* > ( & strSize )  ;
    
    Xdr::copy( from , dest , sizeof( int ) , 1  ) ;
    
    index += sizeof( int ) ;
    
    
    unsigned bytlen, padlen;
    bytlen =  strSize ;
    padlen = (bytlen + 3) & 0xfffffffc ; 
    
    
    strVal->resize( strSize ) ;
    
    from = reinterpret_cast< unsigned const char* > (  & s[ index ] ) ;
    dest = reinterpret_cast< unsigned char* > ( & strVal->operator[](0)  )  ;
    
    Xdr::copy( from , dest , 1 , strSize  ) ;
    
//     std::cout << " fromstream: - " << *strVal 
// 	      << " - lenght: " << strSize
// 	      << " - [" << index + padlen << "] " 
// 	      << std::endl ;
    

    return index + padlen ;
  }
  
  


}
