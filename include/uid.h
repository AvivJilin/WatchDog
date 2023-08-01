/*******************************************************************************
 * Author:  HRD28
 * Version: 1.0
*******************************************************************************/
#ifndef _UID_
#define _UID_

#include <sys/types.h>		/*	pid_t			*/
#include <time.h>			/*	time_t, size_t	*/

/* never access the fields of struct ilrd_uid, all fields may change */
typedef struct ilrd_uid
{
	pid_t pid;
	time_t time;
	size_t counter;
}ilrd_uid_ty;

/*BadID represents an invalid "ilrd_uid_ty"*/
extern const ilrd_uid_ty UIDBadID;

/*******************************************************************************
 * Creates a new "ilrd_uid_ty"
 * Returns "ilrd_uid_ty" if succeeded, otherwise "UIDBadID" 
 * Time Complexity: determined by the used system call complexity 
 ******************************************************************************/
ilrd_uid_ty UIDCreate(void);

/*******************************************************************************
 * Checks if "uid1" is equal to "uid2"
 * Returns 1 if they are the same, 0 otherwise
 * Time Complexity: O(1)
 ******************************************************************************/
int UIDIsSame(ilrd_uid_ty uid1, ilrd_uid_ty uid2);

 
#endif /*_UID_*/
