#include "fastfetch.h"
#include "users.h"

#if FF_HAVE_UTMPX_H
    #include <utmpx.h>
#else
    //for Android compatibility
    #include <utmp.h>
    #define utmpx utmp
    #define setutxent setutent
    #define getutxent getutent
#endif

void ffDetectUsers(FFlist* users, FFstrbuf* error)
{
    struct utmpx* n = NULL;
    setutxent();

next:
    while((n = getutxent()))
    {
        if(n->ut_type != USER_PROCESS)
            continue;

        for(uint32_t i = 0; i < users->length; ++i)
        {
            if(ffStrbufCompS((FFstrbuf*)ffListGet(users, i), n->ut_user) == 0)
                goto next;
        }

        ffStrbufInitS((FFstrbuf*)ffListAdd(users), n->ut_user);
    }

    if(users->length == 0)
        ffStrbufAppendS(error, "Unable to detect users");
}
