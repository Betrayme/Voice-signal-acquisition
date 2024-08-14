/*****************************************************************************/
/* string.h   v5.0.0B3                                                         */
/* Copyright (c) 1993-2007 Texas Instruments Incorporated                    */
/*****************************************************************************/

#ifndef _STRING
#define _STRING

#ifdef __cplusplus
//----------------------------------------------------------------------------
// <cstring> IS RECOMMENDED OVER <string.h>.  <string.h> IS PROVIDED FOR
// COMPATIBILITY WITH C AND THIS USAGE IS DEPRECATED IN C++
//----------------------------------------------------------------------------
extern "C" namespace std
{
#endif /* __cplusplus */
 
#ifndef NULL
#define NULL 0
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef __SIZE_T_TYPE__ size_t;
#endif

#include <linkage.h>

#if defined(_OPTIMIZE_FOR_SPACE) && (defined(__TMS470__) || 		\
				     defined(__TMS320C2000__)  ||       \
                                     defined(__MSP430__))
#define _OPT_IDECL
#else
#define _OPT_IDECL	_IDECL
#endif

_OPT_IDECL size_t  strlen(const char *_string);

_OPT_IDECL char *strcpy(char *_dest, const char *_src);
_OPT_IDECL char *strncpy(char *_to, const char *_from, size_t _n);
_OPT_IDECL char *strcat(char *_string1, const char *_string2);
_OPT_IDECL char *strncat(char *_to, const char *_from, size_t _n);
_OPT_IDECL char *strchr(const char *_string, int _c);
_OPT_IDECL char *strrchr(const char *_string, int _c);

_OPT_IDECL int  strcmp(const char *_string1, const char *_string2);
_OPT_IDECL int  strncmp(const char *_string1, const char *_string2, size_t _n);

_CODE_ACCESS int     strcoll(const char *_string1, const char *_string2);
_CODE_ACCESS size_t  strxfrm(char *_to, const char *_from, size_t _n);
_CODE_ACCESS char   *strpbrk(const char *_string, const char *_chs);
_CODE_ACCESS size_t  strspn(const char *_string, const char *_chs);
_CODE_ACCESS size_t  strcspn(const char *_string, const char *_chs);
_CODE_ACCESS char   *strstr(const char *_string1, const char *_string2);
_CODE_ACCESS char   *strtok(char *_str1, const char *_str2);
_CODE_ACCESS char   *strerror(int _errno);

_CODE_ACCESS void   *memmove(void *_s1, const void *_s2, size_t _n);

_OPT_IDECL int     memcmp(const void *_cs, const void *_ct, size_t _n);
_OPT_IDECL void   *memchr(const void *_cs, int _c, size_t _n);

_OPT_IDECL   void   *memset(void *_mem, int _ch, size_t _n);


#ifndef __cplusplus
#define _memcpy __memcpy_ff
#define _strcpy strcpy_ff
size_t    _strlen(const  char *s);
char     *strcpy_nf(char *s1, const char  *s2);
char  *strcpy_fn(char  *s1, const char *s2);
char  *strcpy_ff(char  *s1, const char  *s2);
 char *_strncpy( char *s1,  const char *s2, size_t n);
 char *_strcat( char *s1, const  char *s2);
 char *_strncat( char *s1, const  char *s2, size_t n);
 char *_strchr(const  char *s, int c);
 char *_strrchr(const  char *s, int c);
int       _strcmp(const  char *s1, const  char *s2);
int       _strncmp(const  char *s1, const  char *s2, size_t n);
int       _strcoll(const  char *s1, const  char *s2);
size_t    _strxfrm( char *s1, const  char *s2, size_t n);
 char *_strpbrk(const  char *s1, const  char *s2);
size_t    _strspn(const  char *s1, const  char *s2);
size_t    _strcspn(const  char *s1, const  char *s2);
 char *_strstr(const  char *s1, const  char *s2);
 char *_strtok( char *s1, const  char *s2);
 char *_strerror(int errno);
void  *_memmove( void *s1,  const void *s2, size_t n);
void     *__memcpy_nf (void *_s1,  const void *_s2, size_t _n);
void  *__memcpy_fn ( void *_s1, const void *_s2, size_t _n);
void  *__memcpy_ff ( void *_s1,  const void *_s2, size_t _n);
int       _memcmp(const  void *s1, const  void *s2, size_t n);
 void *_memchr(const  void *s, int c, size_t n);
 void *_memset( void *s, int c, size_t n);
 void *_memlcpy( void *to, const  void *from, unsigned long n);
 void *_memlmove( void *to, const  void *from, unsigned long n);
#else /* __cplusplus */
long      _memlcpy(long to, long from, unsigned long n);
long      _memlmove(long to, long from, unsigned long n);
#endif /* __cplusplus */


#ifdef __cplusplus
} /* extern "C" namespace std */

#ifndef _CPP_STYLE_HEADER
using std::size_t;
using std::strlen;
using std::strcpy;
using std::strncpy;
using std::strcat;
using std::strncat;
using std::strchr;
using std::strrchr;
using std::strcmp;
using std::strncmp;
using std::strcoll;
using std::strxfrm;
using std::strpbrk;
using std::strspn;
using std::strcspn;
using std::strstr;
using std::strtok;
using std::strerror;
using std::memmove;
using std::memcpy;
using std::memcmp;
using std::memchr;
using std::memset;

using std::_memlcpy;
using std::_memlmove;

#endif /* _CPP_STYLE_HEADER */

#endif /* __cplusplus */

#if defined(_INLINE) || defined(_STRING_IMPLEMENTATION)

#if (defined(_STRING_IMPLEMENTATION) ||					\
     !(defined(_OPTIMIZE_FOR_SPACE) && (defined(__TMS470__) || 		\
					defined(__TMS320C2000__)  ||    \
                                        defined(__MSP430__))))

#ifdef __cplusplus
namespace std {
#endif

#if (defined(_OPTIMIZE_FOR_SPACE) && (defined(__TMS470__) || 		\
				      defined(__TMS320C2000__) ||       \
                                      defined(__MSP430__)))
#define _OPT_IDEFN
#else
#define _OPT_IDEFN	_IDEFN
#endif

#if defined(_INLINE) || defined(_STRLEN)
_OPT_IDEFN size_t strlen(const char *string)
{
   size_t      n = (size_t)-1;
   const char *s = string - 1;

   do n++; while (*++s);
   return n;
}
#endif /* _INLINE || _STRLEN */

#if defined(_INLINE) || defined(_STRCPY)
_OPT_IDEFN char *strcpy(register char *dest, register const char *src)
{
     register char       *d = dest - 1;     
     register const char *s = src  - 1;     

     while (*++d = *++s);
     return dest;
}
#endif /* _INLINE || _STRCPY */

#if defined(_INLINE) || defined(_STRNCPY)
_OPT_IDEFN char *strncpy(register char *dest,
		     register const char *src,
		     register size_t n)
{
     if (n) 
     {
	 register char       *d = dest - 1;
	 register const char *s = src - 1;
	 while ((*++d = *++s) && --n);              /* COPY STRING         */
	 if (n-- > 1) do *++d = '\0'; while (--n);  /* TERMINATION PADDING */
     }
     return dest;
}
#endif /* _INLINE || _STRNCPY  */

#if defined(_INLINE) || defined(_STRCAT)
_OPT_IDEFN char *strcat(char *string1, const char *string2)
{
   char       *s1 = string1 - 1;
   const char *s2 = string2 - 1;

   while (*++s1);		     /* FIND END OF STRING   */
   s1--;  			     /* BACK UP OVER NULL    */
   while (*++s1 = *++s2);	     /* APPEND SECOND STRING */
   return string1;
}
#endif /* _INLINE || _STRCAT */

#if defined(_INLINE) || defined(_STRNCAT)
_OPT_IDEFN char *strncat(char *dest, const char *src, register size_t n)
{
    if (n)
    {
	char       *d = dest - 1;
	const char *s = src  - 1;

	while (*++d);                      /* FIND END OF STRING   */
	d--;                               /* BACK UP OVER NULL    */

	while (n--)
	  if (!(*++d = *++s)) return dest; /* APPEND SECOND STRING */
	*++d = 0;
    }
    return dest;
}
#endif /* _INLINE || _STRNCAT */

#if defined(_INLINE) || defined(_STRCHR)
_OPT_IDEFN char *strchr(const char *string, int c)
{
   char        tch, ch  = c;
   const char *s        = string - 1;

   for (;;)
   {
       if ((tch = *++s) == ch) return (char *) s;
       if (!tch)               return (char *) 0;
   }
}
#endif /* _INLINE || _STRCHR */

#if defined(_INLINE) || defined(_STRRCHR)
_OPT_IDEFN char *strrchr(const char *string, int c)
{
   char        tch, ch = c;
   char       *result  = 0;
   const char *s       = string - 1;

   for (;;)
   {
      if ((tch = *++s) == ch) result = (char *) s;
      if (!tch) break;
   }

   return result;
}
#endif /* _INLINE || _STRRCHR */

#if defined(_INLINE) || defined(_STRCMP)
_OPT_IDEFN int strcmp(register const char *string1,
		  register const char *string2)
{
   register int c1, res;

   for (;;)
   {
       c1  = (unsigned char)*string1++;
       res = c1 - (unsigned char)*string2++;

       if (c1 == 0 || res != 0) break;
   }

   return res;
}
#endif /* _INLINE || _STRCMP */

#if defined(_INLINE) || defined(_STRNCMP)
_OPT_IDEFN int strncmp(const char *string1, const char *string2, size_t n)
{
     if (n) 
     {
	 const char *s1 = string1 - 1;
	 const char *s2 = string2 - 1;
	 unsigned char cp;
	 int         result;

	 do 
	    if (result = (unsigned char)*++s1 - (cp = (unsigned char)*++s2))
                return result;
	 while (cp && --n);
     }
     return 0;
}
#endif /* _INLINE || _STRNCMP */

#if defined(_INLINE) || defined(_MEMCMP)
_OPT_IDEFN int memcmp(const void *cs, const void *ct, size_t n)
{
   if (n) 
   {
       const unsigned char *mem1 = (unsigned char *)cs - 1;
       const unsigned char *mem2 = (unsigned char *)ct - 1;
       int                 cp;

       while ((cp = *++mem1) == *++mem2 && --n);
       return cp - *mem2;
   }
   return 0;
}
#endif /* _INLINE || _MEMCMP */

#if defined(_INLINE) || defined(_MEMCHR)
_OPT_IDEFN void *memchr(const void *cs, int c, size_t n)
{
   if (n)
   {
      const unsigned char *mem = (unsigned char *)cs - 1;   
      unsigned char        ch  = c;

      do if ( *++mem == ch ) return (void *)mem;
      while (--n);
   }
   return NULL;
}
#endif /* _INLINE || _MEMCHR */

#if ((defined(_INLINE) || defined(_MEMSET)) && !defined(_TMS320C6X)) && !defined(__TMS470__)
_OPT_IDEFN void *memset(void *mem, register int ch, register size_t length)
{
     register char *m = (char *)mem - 1;

     while (length--) *++m = ch;
     return mem;
}
#endif /* _INLINE || _MEMSET */

#ifdef __cplusplus
} /* namespace std */
#endif

#endif /* (_STRING_IMPLEMENTATION || !(_OPTIMIZE_FOR_SPACE && __TMS470__)) */

#endif /* (_INLINE || _STRING_IMPLEMENTATION) */

#else

#ifdef __cplusplus

#ifndef _CPP_STYLE_HEADER
using std::size_t;
using std::strlen;
using std::strcpy;
using std::strncpy;
using std::strcat;
using std::strncat;
using std::strchr;
using std::strrchr;
using std::strcmp;
using std::strncmp;
using std::strcoll;
using std::strxfrm;
using std::strpbrk;
using std::strspn;
using std::strcspn;
using std::strstr;
using std::strtok;
using std::strerror;
using std::memmove;
using std::memcpy;
using std::memcmp;
using std::memchr;
using std::memset;

using std::_memlcpy;
using std::_memlmove;

#endif /* _CPP_STYLE_HEADER */

#endif /* __cplusplus */

#endif /* ! _STRING */
