/******************************************************************************
* PROGRAM NAME : read
* SOURCE NAME  : read.c
* DESCRIPTION  : txt ���� �о����
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
DESCRIPTION : txt ���� �о����
PARAMETERS  : KEYNAME kn - ������ �ҷ��� �ӽ� ������ ����ü
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
DESCRIPTION : ���� �Լ�
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

        // �ð� ���ϱ�
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
