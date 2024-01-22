/******************************************************************************
* FILE NAME   : main.c
* DESCRIPTION : 메인 함수
* DATE        :
* AUTHOR      :
******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include"kntable.h"

KEYNAME *g_KnList;
/******************************************************************************
FUNCTION    : InsertShmU
DESCRIPTION : 공유 메모리에 데이터 추가
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int InsertShmU()
{
    printf("\n**********************************\n");
    printf("|                                |\n");
    printf("|      Shared Memory Insert      |\n");
    printf("|                                |\n");
    printf("**********************************\n");
    printf("* finish insert -> insert 'q'\n");

    GlobalCheckShm();
    lInsertShm();

    return 1;
}

/******************************************************************************
FUNCTION    : ReadShmU
DESCRIPTION : 공유 메모리에 데이터 출력
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int ReadShmU()
{
    printf("\n**********************************\n");
    printf("|                                |\n");
    printf("|       Shared Memory Read       |\n");
    printf("|                                |\n");
    printf("**********************************\n\n");

    GlobalCheckShm();
    lReadShm();

    return 1;

}

/******************************************************************************
FUNCTION    : SearchShmU
DESCRIPTION : 공유 메모리에서 데이터 검색
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int SearchShmU()
{
    printf("\n**********************************\n");
    printf("|                                |\n");
    printf("|      Shared Memory Search      |\n");
    printf("|                                |\n");
    printf("**********************************\n\n");

    GlobalCheckShm();
    lSearchShm();

    return 1;

}
/******************************************************************************
FUNCTION    : DeleteShmU
DESCRIPTION : 공유 메모리에서 데이터 삭제
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int DeleteShmU()
{
    printf("\n**********************************\n");
    printf("|                                |\n");
    printf("|      Shared Memory Delete      |\n");
    printf("|                                |\n");
    printf("**********************************\n\n");

    GlobalCheckShm();
    lDeleteShm();

    return 1;
}

/******************************************************************************
FUNCTION    : CheckShmU
DESCRIPTION : 공유 메모리 존재 여부 확인
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int CheckShmU()
{
    printf("\n**********************************\n");
    printf("|                                |\n");
    printf("|       Shared Memory Check      |\n");
    printf("|                                |\n");
    printf("**********************************\n\n");

    GlobalCheckShm();

    return 1;
}

/******************************************************************************
FUNCTIONS   : Usage
DESCRIPTION : Usage display
PARAMETERS  :
RETURNED    :
******************************************************************************/
void Usage()
{
    printf("\n\n");
    printf("**********************************\n");
    printf("|                                |\n");
    printf("|     Shared Memory Control      |\n");
    printf("|                                |\n");
    printf("*================================*\n");
    printf("|                                |\n");
    printf("|                                |\n");
    printf("|   [1] Check Shared Memory      |\n");
    printf("|   [2] Create Shared Memory     |\n");
    printf("|   [3] Add                      |\n");
    printf("|   [4] Read                     |\n");
    printf("|   [5] Search                   |\n");
    printf("|   [6] Delete                   |\n");
    printf("|   [7] Remove Shared Memory     |\n");
    printf("|                                |\n");
    printf("*================================*\n");
    printf("\n");
    printf("Use : ./utilM [ Control Number ]\n\n");
}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 메인 함수
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main(int argc, char* argv[])
{
    int fd;
    int rec_cnt = 0;

    if (argc != 2)
    {
        Usage();
        return -1;
    }
    switch (atoi(argv[1]))
    {
        case 1 :
            CheckShmU();
            break;
        case 2 :
            fd = GlobalCreateShm();
            break;
        case 3 :
            InsertShmU();
            break;
        case 4 :
            ReadShmU();
            break;
        case 5 :
            SearchShmU();
            break;
        case 6 :
            DeleteShmU();
            break;
        case 7 :
            if (GlobalRemoveShm() <0)
            {
                printf("Remove fail\n");
            }
            else printf("Remove Success\n");

            break;
        default :
            Usage();
            break;
    }

    return 0;
}
