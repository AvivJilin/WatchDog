#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>      /*  fputs   */

#define UNUSED(x) (void)(x)

#ifdef NDEBUG
#define DEBUG_ONLY(x) (x)
#else
#define DEBUG_ONLY(x)
#endif

#define RETURN_IF_BAD(IS_GOOD, MSG, RETVAL) if (!IS_GOOD) \
                                            { \
                                                fputs(MSG, stderr); \
                                                return(RETVAL); \
                                            }

#define RETURN_IF_BAD_CLEAN(IS_GOOD, MSG, RETVAL, CLEANUP) if (!IS_GOOD) \
                                                    { \
                                                        CLEANUP; \
                                                        fputs(MSG, stderr); \
                                                        return(RETVAL); \
                                                    }

void ExitIfBad(int is_good_, const char *msg_, int exit_val_)
{
    if (is_good_)
    {
        return;
    }
    
    fputs(msg_, stderr);
    
    exit(exit_val_);
}

#endif  /*  __UTILS_H__  */