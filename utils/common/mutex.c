
#include "cmdlib.h"
#include "threads.h"
#include "mutex.h"

/*
===================================================================

WIN32

===================================================================
*/
#ifdef WIN32

#define	USED

#include <windows.h>

void MutexLock (mutex_t *m)
{
	CRITICAL_SECTION *crit;

	if (!m)
		return;
	crit = (CRITICAL_SECTION *) m;
	EnterCriticalSection (crit);
}

void MutexUnlock (mutex_t *m)
{
	CRITICAL_SECTION *crit;

	if (!m)
		return;
	crit = (CRITICAL_SECTION *) m;
	LeaveCriticalSection (crit);
}

mutex_t *MutexAlloc(void)
{
	CRITICAL_SECTION *crit;

	if (numthreads == 1)
		return NULL;
	crit = (CRITICAL_SECTION *) malloc(sizeof(CRITICAL_SECTION));
	InitializeCriticalSection (crit);
	return (void *) crit;
}

#endif

/*
===================================================================

OSF1

===================================================================
*/

#ifdef __osf__
#define	USED

#include <pthread.h>

void MutexLock (mutex_t *m)
{
	pthread_mutex_t	*my_mutex;

	if (!m)
		return;
	my_mutex = (pthread_mutex_t *) m;
	pthread_mutex_lock (my_mutex);
}

void MutexUnlock (mutex_t *m)
{
	pthread_mutex_t	*my_mutex;

	if (!m)
		return;
	my_mutex = (pthread_mutex_t *) m;
	pthread_mutex_unlock (my_mutex);
}

mutex_t *MutexAlloc(void)
{
	pthread_mutex_t	*my_mutex;
	pthread_mutexattr_t	mattrib;

	if (numthreads == 1)
		return NULL;
	my_mutex = malloc (sizeof(*my_mutex));
	if (pthread_mutexattr_create (&mattrib) == -1)
		Error ("pthread_mutex_attr_create failed");
	if (pthread_mutexattr_setkind_np (&mattrib, MUTEX_FAST_NP) == -1)
		Error ("pthread_mutexattr_setkind_np failed");
	if (pthread_mutex_init (my_mutex, mattrib) == -1)
		Error ("pthread_mutex_init failed");
	return (void *) my_mutex;
}

#endif

/*
===================================================================

IRIX

===================================================================
*/

#ifdef _MIPS_ISA 
#define	USED

#include <task.h>
#include <abi_mutex.h>
#include <sys/types.h>
#include <sys/prctl.h>

void MutexLock (mutex_t *m)
{
	abilock_t *lck;

	if (!m)
		return;
	lck = (abilock_t *) m;
	spin_lock (lck);
}

void MutexUnlock (mutex_t *m)
{
	abilock_t *lck;

	if (!m)
		return;
	lck = (abilock_t *) m;
	release_lock (lck);
}

mutex_t *MutexAlloc(void)
{
	abilock_t *lck;

	if (numthreads == 1)
		return NULL;
	lck = (abilock_t *) malloc(sizeof(abilock_t));
	init_lock (lck);
	return (void *) lck;
}

#endif

/*
=======================================================================

  SINGLE THREAD

=======================================================================
*/

#ifndef USED

void MutexLock (mutex_t *m)
{
}

void MutexUnlock (mutex_t *m)
{
}

mutex_t *MutexAlloc(void)
{
	return NULL;
}

#endif
