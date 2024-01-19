/******************************************************************************
* SOURCE NAME  : glomem.h
* DESCRIPTION  : ���� �޸� ���̺귯��
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
DESCRIPTION : ���� �޸� ����
PARAMETERS  :
RETURNED    :
******************************************************************************/
int GlobalCreateShm()
{
    int shm_id;

    //�����޸� ����
    //�̹� ������ �� ���а� ��ȯ
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
DESCRIPTION : ���� �޸� ���� Ȯ��
PARAMETERS  :
RETURNED    :
******************************************************************************/
int GlobalCheckShm()
{
    int shm_id;

    //���� �޸� id �� ������
    shm_id = shmget(SHM_KEY, 0, PERM);

    //���� �޸𸮰� �������� ���� ��� ���� ���
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
DESCRIPTION : ���� �޸� attach
PARAMETERS  :
RETURNED    :
******************************************************************************/
void GlobalAttachShm()
{
    int shm_id;

    //���� �޸� id �� ������
    shm_id = shmget(SHM_KEY, 0, PERM);

    //���� �޸𸮿� Attach 
    g_KnList=(KEYNAME *) shmat(shm_id, NULL, 0);
}

/******************************************************************************
FUNCTION    : GlobalDetachShm
DESCRIPTION : ���� �޸� detach
PARAMETERS  :
RETURNED    :
******************************************************************************/
int GlobalDetachShm()

{
    return(shmdt(g_KnList));
}

/******************************************************************************
FUNCTION    : GlobalRemoveShm
DESCRIPTION : ���� �޸� ����
PARAMETERS  :
RETURNED    :
******************************************************************************/
int GlobalRemoveShm()
{
    int shm_id;

    shm_id  = shmget(SHM_KEY, 0, PERM);
    return(shmctl(shm_id, IPC_RMID, NULL));
}
