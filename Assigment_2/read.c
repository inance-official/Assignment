/******************************************************************************
* PROGRAM NAME : read
* SOURCE NAME  : read.c
* DESCRIPTION  : txt 파일 읽어오기
* DATE         :
* AUTHOR       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include "kntable.h"

/******************************************************************************
FUNCTION    : ReadFile
DESCRIPTION : txt 파일 읽어오기
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int ReadFile()
{
    FILE      *fp         = NULL;

    char 	   rbuff[64]  = {0,};

    char       tbuff[64]  = {0,};
    time_t     now        = time(NULL);
    struct tm *tt         = NULL;
    KEYNAME   *knlist     = NULL;


    fp = fopen("file.txt", "r");

    if(fp == NULL)
    {
        printf("file error[%d]\n", errno);
        return -1;
    }

    tt = localtime(&now);

    strftime(tbuff, sizeof(tbuff), "%Y-%m-%d %H:%M:%S", tt);
    printf("Update key(2sec)\n");
    printf("Date : %s\n", tbuff);

    printf("\n");
    printf("%-9s%-50s\n","[Key]", "[Name]");

    while (fgets(rbuff, sizeof(rbuff), fp))
    {
        knlist = (KEYNAME *)rbuff;

        printf("%-*.*s%-*.*s\n",
                sizeof(knlist->key)-1, sizeof(knlist->key)-1, knlist->key,
                sizeof(knlist->name)-1, sizeof(knlist->name)-1, knlist->name);

        if(feof(fp))
            break;
    }

    fclose(fp);

    return 1;
}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 메인 함수
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main()
{
    while (1)
    {
        system("clear");
        ReadFile();
        sleep(2);
    }

    return 0;
}

