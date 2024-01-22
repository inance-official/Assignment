/******************************************************************************
* PROGRAM NAME : read
* SOURCE NAME  : read.c
* DESCRIPTION  : txt 파일 읽어오기
* DATE         :
* AUTHOR       :
******************************************************************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<fcntl.h>
#include<unistd.h>


#include"kntable.h"

/******************************************************************************
FUNCTION    : ReadFile
DESCRIPTION : txt 파일 읽어오기
PARAMETERS  : KEYNAME kn - 파일을 불러와 임시 저장할 구조체
RETURNED    : 1(SUCCESS)
******************************************************************************/
int ReadFile(KEYNAME kn)
{
    int fd = 0;

    if ((fd = open("file.txt", O_RDONLY | O_CREAT,0666)) <0)
    {
        printf("file open fail!\n");
        return -1;
    }

    while (read(fd, kn.code, sizeof(kn.code)) > 0)
    {
        printf("%-10s\t", kn.code);

        read(fd, kn.name, sizeof(kn.name));
        printf("%-40s\n", kn.name);
    }

    close(fd);
}

/******************************************************************************
FUNCTION    : main
DESCRIPTION : 메인 함수
PARAMETERS  :
RETURNED    : 0(SUCCESS)
******************************************************************************/
int main()
{
    KEYNAME kn;

    time_t now;
    struct tm* t;
    while (1)
    {
        system("clear");

        // 시간 구하기
        now = time(NULL);
        t = localtime(&now);

        printf("Update Key (2sec)\n");
        printf("Date : %d-%02d-%02d %02d:%02d:%02d\n\n",t -> tm_year + 1900, t -> tm_mon + 1, t -> tm_mday, t -> tm_hour, t -> tm_min, t -> tm_sec);
        printf("%-10s\t%-40s\n", "[Key]", "[NAME]");
        ReadFile(kn);
        sleep(2);
    }

    return 0;
}
