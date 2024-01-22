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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#include "kntable.h"

#define BUFFER_SIZE 9999

/******************************************************************************
FUNCTION    : ReadFile
DESCRIPTION : 파일 읽기
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int ReadFile()
{
    int         fd, rlen;
    char        rbuff[BUFFER_SIZE] = {0,};
    int         num                = 0;
    int         ii                 = 0;
    KEYNAME	   *knlist			   = NULL;

    char       tbuff[64]		   = {0,};
    time_t     now       		   = time(NULL);
    struct tm *tt        	       = NULL;

    fd = open("./file.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("file open error! error[%d]\n", errno);
        return -1;
    }

    tt = localtime(&now);

    strftime(tbuff, sizeof(tbuff), "%Y-%m-%d %H:%M:%S", tt);
    printf("Update key(5sec) : %s\n", tbuff);

    printf("\n");
    printf("%-9s%-50s\n","[Key]", "[Name]");

    ii = 0;

    while (rlen = read(fd, rbuff, sizeof(KEYNAME)) > 0)
    {
        knlist = (KEYNAME *)rbuff;
        memcpy(&knlist[ii], rbuff, sizeof(KEYNAME));

        printf("%-*.*s%-*.*s\n",
            sizeof(knlist[ii].key)-1, sizeof(knlist[ii].key)-1, knlist[ii].key,
               sizeof(knlist[ii].name)-1, sizeof(knlist[ii].name)-1, knlist[ii].name);

        ii++;
    }

    close(fd);

    return 1;
}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 메인함수
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

