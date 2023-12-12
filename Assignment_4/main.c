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
FUNCTION    : InsertShmU
DESCRIPTION : ���� �޸𸮿� ������ �߰�
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int InsertShmU()
{
    /*--- Insert ---*/
    printf("\n");
    printf("Insert Key & Name\n");
    printf("If you want to quit input key [q]\n");

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
    system("clear");

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

    return 0;
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

    return 0;
}

/******************************************************************************
FUNCTION    : CheckShmU
DESCRIPTION : ���� �޸� ���� ���� Ȯ��
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int CheckShmU()
{
    int rtn;

    rtn = lCheckShm();
    if (rtn < 0)
        printf("None exist Shared Memory\n");
    else
        printf("Exist Shared Memory\n");

    return 0;
}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : ���� �Լ�
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main()
{
    int			uopt	= 0;		//Menu ����
    bool		escape	= true;		//loop escape
    int			num		= 0;
    int			rtn		= 0;
    int			shmin	= 0;

    /*--- Menu ---*/
    while (escape)
    {
        printf("                                  \n");
        printf("**********************************\n");
        printf("*							  	 *\n");
        printf("*     Shared Memory Control      *\n");
        printf("*						 		 *\n");
        printf("*================================*\n");
        printf("*                                *\n");
        printf("*                                *\n");
        printf("*   [1] Check Shared Memory      *\n");
        printf("*   [2] Create Shared Memory     *\n");
        printf("*   [3] Add                      *\n");
        printf("*   [4] Read                     *\n");
        printf("*   [5] Search                   *\n");
        printf("*   [6] Delete                   *\n");
        printf("*   [7] Remove Shared Memory     *\n");
        printf("*   [8] Program Exit             *\n");
        printf("*                                *\n");
        printf("*================================*\n");
        printf("                                  \n");
        printf("Choose Menu : ");
        scanf("%d", &uopt);
        printf("                                  \n");
        printf("----------------------------------\n");
        printf("                                  \n");

        switch (uopt)
        {
            /*--- Check Shared Memory ---*/
            case 1:
                printf("\n");
                CheckShmU();
                printf("\n");
                break;

            /*--- Create Shared Memory ---*/
            case 2:
                printf("\n");
                CreateShm();
                printf("\n");
                break;

            /*--- Insert ---*/
            case 3:
                InsertShmU();
                break;

            /*--- Read ---*/
            case 4:
                ReadShmU();
                break;

            /*--- Search ---*/
            case 5:
                SearchShmU();
                break;

            /*--- Delete ---*/
            case 6:
                DeleteShmU();
                break;

            /*--- Remove Shared Memory ---*/
            case 7:
                printf("                                  \n");
                RemoveShm();
                printf("                                  \n");
                break;

            /*--- Exit ---*/
            case 8:
                printf("                                  \n");
                printf("Exit the Program.                 \n");
                printf("                                  \n");
                escape = false;
                break;

            default :
                printf("                                  \n");
                printf("This is an unknown command.       \n");
                printf("                                  \n");
        }
    }

    return 0;
}


