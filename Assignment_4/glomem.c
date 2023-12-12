/******************************************************************************
* SOURCE NAME  : glomem.h
* DESCRIPTION  : 공유 메모리 라이브러리
* DATE         :
* AUTHOR       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#include "kntable.h"
#include "glomem.h"

KEYNAME *g_KnList = NULL;

/*--- System V shared memory Check ---*/
int GlobalCreateShm()
{
	int shmid;

	shmid = shmget(SHM_KEY, MAX_ENTRIES * sizeof(KEYNAME), IPC_CREAT | PERM);
	if (shmid < 0)
	{
		printf("create error! [%d] : %s\n", errno, strerror(errno));
		return -1;
	}
	else
	{
		printf("Success Create Shared Memory [%d]\n", shmid);
	}

	g_KnList = shmat(shmid, NULL, 0);
	if (g_KnList < 0)
	{
		printf("Failed to attach shared memory: %s\n", strerror(errno));
		return -1;
	}

	memset(g_KnList, 0x00, MAX_ENTRIES * sizeof(KEYNAME));

	shmdt(g_KnList);

	return shmid;
}

/*--- System N shared memory Check ---*/
int GlobalCheckShm()
{
	int		shmid, shmin;
	struct	shmid_ds shminfo;

	shmid = shmget(SHM_KEY, 0, PERM);
	if (shmid < 0) return -1;

	// 상태정보 검색하여 공유 메모리 세그먼트 존재 확인
	shmin = shmctl(shmid, IPC_STAT, &shminfo);
	if (shmin == 0) return 1;

}

/*--- System V shared memory attach ---*/
#if 0
int GlobalAttachShm()
{
	int shmid;

	shmid = shmget(SHM_KEY, 0, PERM);

	g_KnList = shmat(shmid, NULL, 0);
	if (g_KnList == (KEYNAME*)-1)
	{
		printf("attach error! [%d] : %s\n", errno, strerror(errno));
		return -1;
	}

	return 1;
}
#endif

KEYNAME *GlobalAttachShm()
{
	int		 shmid;

	shmid = shmget(SHM_KEY, 0, PERM);
	if (shmid < 0)
	{
		printf("shmget error: %s\n", strerror(errno));
		return NULL;
	}

	g_KnList = shmat(shmid, NULL, 0);
	if (g_KnList == (void*)-1)
		return NULL;

	return g_KnList;
}

/*--- System V shared memory Detach ---*/
int GlobalDetachShm()
{
	if (shmdt(g_KnList) < 0)
	{
		printf("detach error! [%d] : %s\n", errno, strerror(errno));
		return -1;
	}

	return 1;
}

/*--- System V shared memory Remove ---*/
int GlobalRemoveShm()
{
	int shmid, ret;

	shmid = shmget(SHM_KEY, 0, PERM);
	if (shmid < 0)
	{
		printf("shmget error! [%d] : %s\n", errno, strerror(errno));
		return -1;
	}

	ret = shmctl(shmid, IPC_RMID, NULL);
	if (ret < 0)
	{
		printf("shmctl error! [%d] : %s\n", errno, strerror(errno));
		return -1;
	}
	else
	{
		printf("Remove Shared Memory\n");
	}

	return 1;
}


