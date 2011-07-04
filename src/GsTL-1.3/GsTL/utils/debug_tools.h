#ifndef __GSTL_UTILS_DEBUG_TOOLS_H__
#define __GSTL_UTILS_DEBUG_TOOLS_H__

#include <fstream> 
#include <string> 

namespace gstl_debug {

  template < class Location_, class Stream_ >
  void print_location( const Location_& loc, Stream_& os ) {
    os << "(" << loc[0] << ", " << loc[1] << ", " << loc[2] << ")";
  }

  template <class InputIterator, class Stream_ >
  void print_range( InputIterator begin, InputIterator end , 
                    Stream_& os ) {
    for( ; begin != end ; ++begin )
      os << *begin << " ";
  }
    

  template <class NeighborhoodIterator, class Stream_>
  void print_neighbors( NeighborhoodIterator first, 
                        NeighborhoodIterator last, Stream_& os ) {
    for( ; first != last ; ++first ) {
      print_location( first->location(), os );
      os << ":";
      if( first->is_informed() )
        os << first->property_value();
      else
        os << "?";
      os << "   ";
    }
    os << std::endl;
  }

  template < class Matrix_, class Vector_, class Stream_ >
  void print_kriging_system( const Matrix_& A, const Vector_& b, 
                             Stream_& os ) {
    os << "kriging matrix: " << std::endl
       << A << std::endl << std::endl
       << "kriging rhs: " << std::endl << b << std::endl;
   
  }

  template < class Stream_ >
  class Global_stream {
    public:
      static void open( const std::string& filename ) {
        if( stream_ ) {
          stream_->close();
          stream_->open( filename.c_str() );
        }
        else
          stream_ = new Stream_( filename.c_str() );
      }
      static Stream_& instance() {
        if( !stream_ ) 
          open( "gstl_debug.dbg" );
        return *stream_;
      }

    private:
      static Stream_* stream_;
  };

  template<class Stream_> Stream_* Global_stream<Stream_>::stream_ = 0;
}



#ifndef GSTL_DEBUG_2
#define DEBUG_PRINT_KRIGING_SYSTEM( A,b )

#else
#define GSTL_DEBUG_1

#define DEBUG_PRINT_KRIGING_SYSTEM( A,b ) \
{ \
  gstl_debug::print_kriging_system( A,b,gstl_debug::Global_stream<std::ofstream>::instance() );\
  gstl_debug::Global_stream<std::ofstream>::instance() << std::endl; \
}

#endif



#ifndef GSTL_DEBUG_1
#define OPEN_DEBUG_STREAM( file )
#define WRITE_TO_DEBUG_STREAM( expr ) 
#define DEBUG_PRINT_LOCATION( label, loc )
#define DEBUG_PRINT_NEIGHBORHOOD( label, pointer )
#define DEBUG_PRINT_RANGE( label, begin, end )

#else

#define OPEN_DEBUG_STREAM( file ) \
{ \
  gstl_debug::Global_stream<std::ofstream>::open( file ); \
}

#define WRITE_TO_DEBUG_STREAM( expr ) \
{ \
  gstl_debug::Global_stream<std::ofstream>::instance() << expr; \
}

#define DEBUG_PRINT_LOCATION( label, loc ) \
{ \
  gstl_debug::Global_stream<std::ofstream>::instance() << label << ":"; \
  gstl_debug::print_location( loc, gstl_debug::Global_stream<std::ofstream>::instance() ); \
  gstl_debug::Global_stream<std::ofstream>::instance() << std::endl; \
}


#define DEBUG_PRINT_NEIGHBORHOOD( label, pointer ) \
{ \
  gstl_debug::Global_stream<std::ofstream>::instance() << label << ": "; \
  gstl_debug::print_neighbors( (pointer)->begin(), (pointer)->end(), \
                               gstl_debug::Global_stream<std::ofstream>::instance() );\
  gstl_debug::Global_stream<std::ofstream>::instance() << std::endl; \
} 

#define DEBUG_PRINT_RANGE( label, begin, end ) \
{ \
  gstl_debug::Global_stream<std::ofstream>::instance() << label << ": "; \
  gstl_debug::print_range( begin, end, gstl_debug::Global_stream<std::ofstream>::instance() ); \
  gstl_debug::Global_stream<std::ofstream>::instance() << std::endl; \
}

#endif




#endif

