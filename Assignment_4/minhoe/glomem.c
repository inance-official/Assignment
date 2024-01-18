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
    int fd;

    fd = shm_open(SHMNAME, O_CREAT | O_RDWR,0644);
    if (fd < 0)
    {
        perror("Error create shm");
        exit(EXIT_FAILURE);
    }
    else printf("Create Success\n");
    return fd;
}

/******************************************************************************
FUNCTION    : GlobalCheckShm
DESCRIPTION : 공유 메모리 존재 확인
PARAMETERS  :
RETURNED    :
******************************************************************************/
int GlobalCheckShm()
{
    int fd;

    fd = shm_open(SHMNAME, O_RDWR, PERM);

    if (fd < 0)
    {
        perror("Check Error");
        exit(EXIT_FAILURE);
    }
    else printf("shm is exists\n");

    close(fd);
    return 1;
}

/******************************************************************************
FUNCTION    : GlobalAttachShm
DESCRIPTION : 공유 메모리 attach
PARAMETERS  :
RETURNED    :
******************************************************************************/
void GlobalAttachShm(int fd)
{
    g_KnList = (KEYNAME *)mmap(NULL, sizeof(g_KnList), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if ( g_KnList == MAP_FAILED)
    {
        perror("Error mmaping the file");
        exit(EXIT_FAILURE);
    }
}

/******************************************************************************
FUNCTION    : GlobalDetachShm
DESCRIPTION : 공유 메모리 detach
PARAMETERS  :
RETURNED    :
******************************************************************************/
int GlobalDetachShm()
{
    return(munmap(g_KnList, sizeof(g_KnList)));
}

/******************************************************************************
FUNCTION    : GlobalRemoveShm
DESCRIPTION : 공유 메모리 삭제
PARAMETERS  :
RETURNED    :
******************************************************************************/
int GlobalRemoveShm()
{
    return(shm_unlink(SHMNAME));
}
