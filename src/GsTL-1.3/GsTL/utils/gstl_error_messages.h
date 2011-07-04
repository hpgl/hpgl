#ifndef __GSTL_UTILS_GSTL_ERROR_MESSAGE_H__
#define __GSTL_UTILS_GSTL_ERROR_MESSAGE_H__

#include <iostream>

/** This file contains macros that output warnings and messages.
 * 3 macros are defined:
 *   - gstl_message : outputs a message
 *   - gstl_warning : outputs a warning
 *   - gstl_assert : aborts execution if a condition is not met.
 * These 3 macros can be selectively disabled by defining various variables.
 * Defining GSTL_NOMESSAGE disables gstl_message.
 * Defining GSTL_NOWARNING disables gstl_warning.
 * Defining GSTL_SILENT disables both gstl_message and gstl_warning.
 * Defining GSTL_NOCHECK disables gstl_assert.
 * Defining GSTL_NDEBUG disables all 3 macros.
 */


#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS /* empty */
# define __END_DECLS /* empty */
#endif


# define GSTL_VOID_CAST static_cast<void>

/*==============================
 * if GSTL_NDEBUG is defined, disable gstl_assert 
 * and don't output messages or warnings
 */
#ifdef GSTL_NDEBUG
#  define GSTL_NOCHECK
#  define GSTL_SILENT
#endif

/*==============================
 * We define two levels of assertions: gstl_assert and gstl_assert_2.
 * gstl_assert_2 can be disabled by defining GSTL_NOCHECK_2 or GSTL_NOCHECK.
 * gstl_assert can be disabled by defining GSTL_NOCHECK.
 * gstl_assert(_2) checks if the supplied expression is 0 and aborts the
 * execution.
 */

#ifdef  GSTL_NOCHECK_2
#define gstl_assert_2(expr)             (GSTL_VOID_CAST (0))
#else
#include <cassert>
#define gstl_assert_2(expr) \
  (assert(expr))
#endif

#ifdef	GSTL_NOCHECK
#define GSTL_NOCHECK_2
# define gstl_assert(expr)		(GSTL_VOID_CAST (0))

#else /* Not GSTL_NOCHECK  */
#include <cassert>
# define gstl_assert(expr) \
  (assert(expr))

#endif 


  
/*==============================
 * if GSTL_SILENT is defined, do not output messages and
 * warnings.
 */

#ifdef GSTL_SILENT
#  define GSTL_NOMESSAGE
#  define GSTL_NOWARNING
#  define GSTL_NOWARNING_2
#endif


#ifdef GSTL_NOMESSAGE
#  define gstl_message(expr)

#else
  
# define gstl_message(expr) \
{ \
  std::cout << __FILE__ << ":" << __LINE__ << "  " << expr << std::endl; \
}  
#endif  
  
 

#ifdef GSTL_NOWARNING
#define GSTL_NOWARNING_2
#  define gstl_warning(expr)

#else

# define gstl_warning(expr) \
{ \
  std::cerr << __FILE__ << ":" << __LINE__ << "  " << expr << std::endl; \
} 
#endif  
  
#ifdef GSTL_NOWARNING_2
#  define gstl_warning_2(expr)
 
#else

# define gstl_warning_2(expr) \
{ \
  std::cerr << __FILE__ << ":" << __LINE__ << "  " << expr << std::endl; \
} 
#endif



#endif
