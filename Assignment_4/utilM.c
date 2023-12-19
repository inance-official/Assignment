/******************************************************************************
* FILE NAME   : main.c
* DESCRIPTION : ���� �Լ�
* DATE        :
* AUTHOR      :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdbool.h>

#include "kntable.h"
#include "glomem.h"

/******************************************************************************
FUNCTION    : CheckShmU
DESCRIPTION : ���� �޸� ���� ���� Ȯ��
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int CheckShmU()
{
    int rtn;

    rtn = GlobalCheckShm();
    if (rtn < 0)
	{
        printf("                                  \n");
        printf("None exist Shared Memory\n");
        printf("                                  \n");
	}
    else
	{
        printf("                                  \n");
        printf("Exist Shared Memory\n");
        printf("                                  \n");
	}

    return 1;
}


/******************************************************************************
FUNCTION    : CreateShmU
DESCRIPTION : ���� �޸� ����
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int CreateShmU()
{

	int rtn;

    rtn = GlobalCheckShm();
    if (rtn < 0)
	{
        printf("                                  \n");
        printf("Already Exist Shared Memory\n");
        printf("Shmid : ID : %d\n", rtn);
        printf("                                  \n");

        return 1;
	}

    rtn = GlobalCreateShm();
    if (rtn < 0)
    {
        printf("                                  \n");
        printf("GlobalCreateShm() error!!\n");
        printf("                                  \n");
    }
    else
    {
        printf("                                  \n");
        printf("Create SHM [ID : %d]\n", rtn);
        printf("                                  \n");
    }

    return 1;
}

/******************************************************************************
FUNCTION    : InsertShmU
DESCRIPTION : ���� �޸𸮿� ������ �߰�
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int InsertShmU()
{

    /*--- Insert ---*/
    printf("                                  \n");
    printf("*--------------------------------*\n");
    printf("                                  \n");
    printf("        Insert Key & Name         \n");
    printf("        Quit input key [q]        \n");
    printf("                                  \n");
    printf("*--------------------------------*\n");
    printf("                                  \n");

    lInsertShm();

    return 1;
}

/******************************************************************************
FUNCTION    : ReadShmU
DESCRIPTION : ���� �޸𸮿� ������ ���
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int ReadShmU()
{
    /*--- ���� �޸𸮿� �����Ͱ� �����ϸ� ��� ---*/
    lReadShm(g_KnList);

    return 1;
}

/******************************************************************************
FUNCTION    : SearchShmU
DESCRIPTION : ���� �޸𸮿��� ������ �˻�
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int SearchShmU()
{
    int     rtn;

    /*--- �˻� ---*/
    rtn = lSearchShm();
    if(rtn < 0)
    {
        printf("SearchShm error!\n");
        return -1;
    }

    return 1;
}

/******************************************************************************
FUNCTION    : DeleteShmU
DESCRIPTION : ���� �޸𸮿��� ������ ����
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int DeleteShmU()
{
    int     rtn;

    /*--- ã�Ƽ� ���� ---*/
    rtn = lDeleteShm();
    if(rtn < 0)
    {
        printf("DeleteShm error!\n");
        return -1;
    }

    return 1;
}

/******************************************************************************
FUNCTIONS   : Usage
DESCRIPTION : Usage display
PARAMETERS  : int argc    - Excution argument number
              char **argv - Excution argument
RETURNED    :
******************************************************************************/
void Usage(argc, argv)
int     argc;
char  **argv;
{
    printf("                                  \n");
    printf("**********************************\n");
    printf("|                                |\n");
    printf("|     Shared Memory Control      |\n");
    printf("|                                |\n");
    printf("*================================*\n");
    printf("|                                |\n");
    printf("|                                |\n");
    printf("|   [1] Check Shared Memory      |\n");
    printf("|   [2] Create Shared Memory     |\n");
    printf("|   [3] Insert                   |\n");
    printf("|   [4] Read                     |\n");
    printf("|   [5] Search                   |\n");
    printf("|   [6] Delete                   |\n");
    printf("|   [7] Remove Shared Memory     |\n");
    printf("|                                |\n");
    printf("*================================*\n");
    printf("                                  \n");
    printf("                                  \n");
    printf(" Use : ./utilM [ Control Number ] \n");
    printf("                                  \n");
    printf("                                  \n");
}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : ���� �Լ�
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main(int argc, char **argv)
{
    int			uopt	= 0;		//Menu ����
    bool		escape	= true;		//loop escape
    int			num		= 0;
    int			rtn		= 0;
    int			shmin	= 0;

    char    server[256];

    if (argc < 2) {
        Usage(argc,argv);
        exit(1);
    }

    if      (argv[1][0]=='1') CheckShmU();
    else if (argv[1][0]=='2') CreateShmU();
    else if (argv[1][0]=='3') InsertShmU();
    else if (argv[1][0]=='4') ReadShmU();
    else if (argv[1][0]=='5') SearchShmU();
    else if (argv[1][0]=='6') DeleteShmU();
    else if (argv[1][0]=='7') GlobalRemoveShm();
	else
	{
        Usage(argc,argv);
        exit(1);
    }

    return 0;
}


