/******************************************************************************
* SOURCE NAME  : glomem.h
* DESCRIPTION  : 공유 메모리 라이브러리
* DATE         :
* AUTHOR       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "kntable.h"
#include "glomem.h"

extern KEYNAME *g_KnList;

/******************************************************************************
FUNCTION    : GlobalCreateShm
DESCRIPTION : 공유 메모리 생성
PARAMETERS  :
RETURNED    :
******************************************************************************/
int GlobalCreateShm()
{
    int shm_id;

    //공유메모리 생성
    //이미 존재할 시 실패값 반환
    shm_id = shmget(SHM_KEY, MAX_ENTRIES * sizeof(KEYNAME), IPC_CREAT| IPC_EXCL | PERM);
    if (shm_id < 0)
    {
        perror("Error create shm");
        exit(EXIT_FAILURE);
    }
    else printf("Create Success\n");
    return shm_id;
}

/******************************************************************************
FUNCTION    : GlobalCheckShm
DESCRIPTION : 공유 메모리 존재 확인
PARAMETERS  :
RETURNED    :
******************************************************************************/
int GlobalCheckShm()
{
    int shm_id;

    //공유 메모리 id 값 가져옴
    shm_id = shmget(SHM_KEY, 0, PERM);

    //공유 메모리가 존재하지 않을 경우 에러 출력
    if (shm_id < 0)
    {
        perror("Check Error");
        exit(EXIT_FAILURE);
    }

    else printf("shm is exists\n");

    return 1;
}

/******************************************************************************
FUNCTION    : GlobalAttachShm
DESCRIPTION : 공유 메모리 attach
PARAMETERS  :
RETURNED    :
******************************************************************************/
void GlobalAttachShm()
{
    int shm_id;

    //공유 메모리 id 값 가져옴
    shm_id = shmget(SHM_KEY, 0, PERM);

    //공유 메모리와 Attach 
    g_KnList=(KEYNAME *) shmat(shm_id, NULL, 0);
}

/******************************************************************************
FUNCTION    : GlobalDetachShm
DESCRIPTION : 공유 메모리 detach
PARAMETERS  :
RETURNED    :
******************************************************************************/
int GlobalDetachShm()

{
    return(shmdt(g_KnList));
}

/******************************************************************************
FUNCTION    : GlobalRemoveShm
DESCRIPTION : 공유 메모리 삭제
PARAMETERS  :
RETURNED    :
******************************************************************************/
int GlobalRemoveShm()
{
    int shm_id;

    shm_id  = shmget(SHM_KEY, 0, PERM);
    return(shmctl(shm_id, IPC_RMID, NULL));
}
