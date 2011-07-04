/**
 * \file bs_assert.h
 * \brief smart assertation, based on Alexandrescu's ideas
 * \author Sergey Miryanov
 * \date 14.05.2008
 */
#ifndef BS_ASSERT_H_
#define BS_ASSERT_H_

#if defined (_WIN32)

#define BREAK_HERE __asm { int 3 }
  
#else
#define BREAK_HERE __asm__ __volatile__ ("int $0x3")
#endif

#include <boost/lexical_cast.hpp>

namespace blue_sky {

  namespace bs_assert {

    //! forward declaration
    class assert_factory;

    /** 
     * \brief perform assert actions
     * */
    class BS_API_PLUGIN asserter 
    {

    public:

      /** 
       * \brief user reaction on assert
       * */
      enum assert_state 
      {
        STATE_KILL,                               //! terminate process
        STATE_BREAK,                              //! break into file where assertion failed
        STATE_IGNORE,                             //! ignore current assert
        STATE_IGNORE_ALL                         //! ignore current and all following asserts
      };

    public:

      asserter (const char *file, int line, const char *cond_str)
        : line (line)
	, file (file)
	, cond (true)
        , cond_s (cond_str)
        , var_list ("")
      {
        ASSERTER_A = this;
        ASSERTER_B = this;
      }

      asserter (bool cond, const char *file, int line, const char *cond_str)
        : line (line) 
	, file (file)
	, cond (cond)        
	, cond_s (cond_str)
        , var_list ("")
      {
        ASSERTER_A = this;
        ASSERTER_B = this;
      }

      virtual ~asserter ()
      {
      }

      virtual assert_state ask_user () const;

      virtual bool handle () const;

      static asserter 
      workaround (const char * file_, int line_, const char *cond_str)
      {
        return asserter (file_, line_, cond_str);
      }

      asserter *make (bool cond); 
      
      template <class T> asserter *
      add_var (const T &t, const std::string &name)
      {
        std::string str = name + " = " + boost::lexical_cast <std::string> (t) + "\n";
        var_list = var_list + str;

        return this;
      }

      inline bool &
      ignore_all () const
      {
        static bool ignore_all_ = false;
        return ignore_all_;
      }

      static void 
      set_factory (assert_factory *f)
      {
        factory() = f;
      }

      virtual const char *what ()const  throw()
      {
        return "asserter";
      }

      asserter *warning ();
      asserter *debug ();
      asserter *error ();
      asserter *fatal ();

      static assert_factory *&
      factory ()
      {
        static assert_factory *factory_ = 0;
        return factory_;
      }

    public:

      asserter                *ASSERTER_A;
      asserter                *ASSERTER_B;

      enum TRACE_TYPE
      {
        TRACE_TYPE_WARNING,
        TRACE_TYPE_DEBUG,
        TRACE_TYPE_ERROR,
        TRACE_TYPE_FATAL
      };

      TRACE_TYPE              trace_type;

    public:
      int                     line;
      const char              *file;
      bool                    cond;
			const char              *cond_s;
      std::string             var_list;
      //static assert_factory   *factory;
    };  

    class BS_API_PLUGIN assert_factory 
    {
    public:

      virtual ~assert_factory () {}

      virtual asserter *make (bool b, const char *file, int line, const char *cond_str);
    };

    struct BS_API_PLUGIN assert_wrapper
    {
      assert_wrapper (asserter *pa)
      {
        if (pa && !pa->handle ())
          {
            BREAK_HERE;
          }

        delete pa;
      }
    };

    struct BS_API_PLUGIN trace_wrapper
    {
      trace_wrapper (asserter *pa);
    };

  } // namespace bs_assert

#ifdef _DEBUG
#define BS_ASSERT(cond)                                         \
  if (false) ; else												\
	blue_sky::bs_assert::assert_wrapper wrapper_ = blue_sky::bs_assert::asserter::workaround(__FILE__, __LINE__, (#cond)).make(!!(cond))->ASSERTER_A
#else
#define BS_ASSERT(cond)                                         \
  if (true) ; else												\
	blue_sky::bs_assert::assert_wrapper wrapper_ = blue_sky::bs_assert::asserter::workaround(__FILE__, __LINE__, (#cond)).make(!!(cond))->ASSERTER_A
#endif

#ifndef BS_DISABLE_TRACE
#define BS_TRACE(cond)                    \
  if (false) ; else                       \
  blue_sky::bs_assert::trace_wrapper wrapper_ = blue_sky::bs_assert::asserter::workaround (__FILE__, __LINE__, (#cond)).make (!!(cond))->ASSERTER_A
#else
#define BS_TRACE(cond)                    \
  if (true) ; else                        \
    blue_sky::bs_assert::trace_wrapper wrapper_ = blue_sky::bs_assert::asserter::workaround (__FILE__, __LINE__, (#cond)).make (!!(cond))->ASSERTER_A
#endif

#if !defined(BS_DISABLE_TRACE) && !defined(BS_DISABLE_TRACE_WARNING)
#define BS_WARNING(cond)                  \
  if (false) ; else                       \
  blue_sky::bs_assert::trace_wrapper wrapper_ = blue_sky::bs_assert::asserter::workaround (__FILE__, __LINE__, (#cond)).make (!! (cond))->warning ()->ASSERTER_A
#else
#define BS_WARNING(cond)                  \
  if (true) ; else                        \
  blue_sky::bs_assert::trace_wrapper wrapper_ = blue_sky::bs_assert::asserter::workaround (__FILE__, __LINE__, (#cond)).make (!! (cond))->warning ()->ASSERTER_A
#endif

#if !defined(BS_DISABLE_TRACE) && !defined(BS_DISABLE_TRACE_DEBUG)
#define BS_DEBUG(cond)                    \
  if (false) ; else                       \
  blue_sky::bs_assert::trace_wrapper wrapper_ = blue_sky::bs_assert::asserter::workaround (__FILE__, __LINE__, (#cond)).make (!! (cond))->debug ()->ASSERTER_A
#else
#define BS_DEBUG(cond)                    \
  if (true) ; else                        \
  blue_sky::bs_assert::trace_wrapper wrapper_ = blue_sky::bs_assert::asserter::workaround (__FILE__, __LINE__, (#cond)).make (!! (cond))->debug ()->ASSERTER_A
#endif

#if !defined(BS_DISABLE_TRACE) && !defined(BS_DISABLE_TRACE_ERROR)
#define BS_ERROR(cond)                    \
  if (false) ; else                       \
  blue_sky::bs_assert::trace_wrapper wrapper_ = blue_sky::bs_assert::asserter::workaround (__FILE__, __LINE__, (#cond)).make (!! (cond))->error ()->ASSERTER_A
#else
#define BS_ERROR(cond)                    \
  if (true) ; else                        \
  blue_sky::bs_assert::trace_wrapper wrapper_ = blue_sky::bs_assert::asserter::workaround (__FILE__, __LINE__, (#cond)).make (!! (cond))->error ()->ASSERTER_A
#endif

#if !defined(BS_DISABLE_TRACE) && !defined(BS_DISABLE_TRACE_FATAL)
#define BS_FATAL(cond)                    \
  if (false) ; else                       \
  blue_sky::bs_assert::trace_wrapper wrapper_ = blue_sky::bs_assert::asserter::workaround (__FILE__, __LINE__, (#cond)).make (!! (cond))->fatal ()->ASSERTER_A
#else
#define BS_FATAL(cond)                    \
  if (true) ; else                        \
  blue_sky::bs_assert::trace_wrapper wrapper_ = blue_sky::bs_assert::asserter::workaround (__FILE__, __LINE__, (#cond)).make (!! (cond))->fatal ()->ASSERTER_A
#endif

#define ASSERTER_A(x)           ASSERTER_OP_(x, B)
#define ASSERTER_B(x)           ASSERTER_OP_(x, A)
#define ASSERTER_OP_(x, next)   ASSERTER_A->add_var ((x), #x)->ASSERTER_##next

} // namespace blue_sky


#endif  // #ifndef BS_ASSERT_H_

