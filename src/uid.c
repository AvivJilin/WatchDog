#include <unistd.h>     /*  getpid                */
#include <time.h>       /*  time_t, size_t, time  */

#include "uid.h"

const ilrd_uid_ty UIDBadID = {(pid_t)0, (time_t)0, (size_t)0}; 

/*******************************************************************************
 * Creates a new "ilrd_uid_ty"
 * Returns "ilrd_uid_ty" if succeeded, otherwise "UIDBadID" 
 * Time Complexity: determined by the used system call complexity 
 ******************************************************************************/

ilrd_uid_ty UIDCreate(void)
{
   ilrd_uid_ty uid;
   static size_t cnt = 0;
   
   uid.pid = getpid();
   uid.time = time(NULL);
   if ((time_t)-1 == uid.time)
   {
        return UIDBadID;
   }
   ++cnt;
   uid.counter = cnt;

   return uid;
}

/*******************************************************************************
 * Checks if "uid1" is equal to "uid2"
 * Returns 1 if they are the same, 0 otherwise
 * Time Complexity: O(1)
 ******************************************************************************/
int UIDIsSame(ilrd_uid_ty uid1, ilrd_uid_ty uid2)
{
    return ((uid1.pid) == (uid2.pid) && (uid1.time) == (uid2.time)
        && (uid1.counter) == (uid2.counter));
}



 
