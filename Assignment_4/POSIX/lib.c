/******************************************************************************
 * FILE NAME   : lib.c
 * DESCRIPTION : ���� �޸� �Լ�
 * DATE        :
 * AUTHOR      :
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "kntable.h"
#include "glomem.h"
/******************************************************************************
FUNCTION    : Cmp
DESCRIPTION : ���Լ�
PARAMETERS  : const void *aa - ���� ������
              const void *bb - ���� ������
RETURNED    : strcmp()�� ��ȯ��
******************************************************************************/
int Cmp(const void *a, const void *b)
{

    return strcmp(((KEYNAME*)a)->key,((KEYNAME*)b)->key);
}


/******************************************************************************
FUNCTION    : CountShm
DESCRIPTION : ���� �޸� ���� ���ڵ� ����
PARAMETERS  :
RETURNED    : count - ���ڵ� ����
******************************************************************************/
int CountShm()
{
    char dum[1024] = {0,};
    int rec_cnt = 0;
    int fd = 0;

    fd = shm_open(SHMNAME, O_RDWR, PERM);

    //�����޸��� �����͸� �� �پ� ������ ī����
    while (read(fd,dum,sizeof(KEYNAME)) > 0)
    {
        rec_cnt++;
    }

    close(fd);

    return rec_cnt;
}

/******************************************************************************
FUNCTION    : lInsertShm
DESCRIPTION : ���� �޸𸮿� ������ �Է�
PARAMETERS  :
RETURNED    :  1(SUCCESS)
******************************************************************************/
int lInsertShm()
{
    char key_buf[KEYSIZE] = {0,};
    char name_buf[NAMESIZE] = {0,};

    int ii = 0;
    int fd = 0;
    int rec_cnt;

    //fd���� ������
    fd = shm_open(SHMNAME, O_RDWR, 0644);

    while(1)
    {
        //�����͸� �߰��ϱ� ���� ������ ������ �̵�
        lseek(fd, 0, SEEK_END);

        printf("\nInsert Key & Name\n");
        printf("Input Key[9] : ");
        scanf("%9s", key_buf);

        while (getchar() != '\n'){}

        //q �Է½� ���α׷� ����    
        if (strcmp(key_buf,"q") == 0) break;
       //�ߺ� Ű �� ����� �Է� ����
        if (DupKeyChk(fd, key_buf) < 0) break;

        printf("Input name[40] : ");
        scanf("%40s", name_buf);

        while (getchar() != '\n'){}

        //������ �Է�
        write(fd, key_buf, sizeof(key_buf));
        write(fd, name_buf, sizeof(name_buf));
    }

        rec_cnt = CountShm();

        GlobalAttachShm(fd);

        //�Էµ� ������ ����
        qsort(g_KnList, rec_cnt, sizeof(KEYNAME), Cmp);

        GlobalDetachShm();

        close(fd);


    return 1;
}
/******************************************************************************
FUNCTION    : DupKeyChk
DESCRIPTION : ���� �޸𸮿� ������ �Է½� �ߺ� Ű �� Ȯ��
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int DupKeyChk(int fd, char *key_buf)
{
    int ii = 0;
    int rec_cnt = 0;

    //���� ���ڵ� ���� ������
    rec_cnt = CountShm();

    GlobalAttachShm(fd);
    //��� ���ڵ� Ž��
    for( ii=0; ii < rec_cnt; ii++)
    {
        //�Է��ϰ��� �ϴ� key ���� ���� ���� ���� �� ���
        if (strncmp((g_KnList+ii) -> key, key_buf, sizeof(key_buf)) == 0)
        {
            printf("A duplicate key exists.\n");
            printf("%-9s    %-s\n","[Key]","[Name]");
            printf("%-9s    %-s\n", (g_KnList + ii) -> key, (g_KnList + ii) -> name);

            GlobalDetachShm();

            return -1;
        }
    }
    GlobalDetachShm();
    return 1;
}

/******************************************************************************
FUNCTION    : ReadShm
DESCRIPTION : ���� �޸𸮿� ������ ���
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int lReadShm()
{
    int fd = 0;
    char key_buf[KEYSIZE] = {0,};
    char name_buf[NAMESIZE] = {0,};
    time_t now;
    struct tm* t;
    int rec_cnt;

    while(1)
    {
        fd = shm_open(SHMNAME, O_RDWR, 0644);
        system("clear");

        // �ð� ���ϱ�
        now = time(NULL);
        t = localtime(&now);

        rec_cnt = CountShm();

        printf("Update Key (2sec)\n");
        printf("Date : %d-%02d-%02d %02d:%02d:%02d\n\n",t -> tm_year + 1900, t -> tm_mon + 1, t -> tm_mday, t -> tm_hour, t -> tm_min, t -> tm_sec);
        printf("%-10s\t%-40s\n", "[Key]", "[NAME]");

        //���� �޸� ���� ������ ���
        while (read(fd, key_buf,sizeof(key_buf)) > 0)
        {
            printf("%-10s\t", key_buf);

            read(fd, name_buf, sizeof(name_buf));
            printf("%-40s\n", name_buf);
        }

        sleep(2);

        close(fd);
    }

    return 1;
}

/******************************************************************************
FUNCTION    : SearchShm
DESCRIPTION : ���� �޸𸮿� ������ �˻�
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int lSearchShm()
{
    int rec_cnt;
    int fd = 0;
    KEYNAME find_kn;
    KEYNAME* kn_res = NULL;

    printf("---------------------------------");
    printf("\n\n    Enter a key to find name\n\n");
    printf("---------------------------------\n\n");

    //ã���� �ϴ� key �� �Է�
    printf("Input key(9) : ");
    scanf("%9s", find_kn.key);

    fd = shm_open(SHMNAME, O_RDWR,0644);
    if (fd == -1)
    {
        perror("shm_open");
        return -1;
    }

    //���� ���ڵ� ���� ������
    rec_cnt = CountShm();

    GlobalAttachShm(fd);

    //bsearch�� �̿��Ͽ� ã�����ϴ� ���� kn_res�� ����
    kn_res = bsearch(&find_kn.key, g_KnList, rec_cnt, sizeof(KEYNAME), Cmp);

    //���� ���� ���
    if (kn_res == NULL)
    {
        printf("\n---------------------------------");
        printf("\nNot Found. key[%s]\n",find_kn.key);
        printf("Search failed. Please try again.\n");
        printf("---------------------------------\n");
    }
    //���� ���� �� ���
    else printf("Key : %s\t Name : %s\n", kn_res -> key, kn_res -> name);
    
    GlobalDetachShm();
    close(fd);

    return 0;
}   

/******************************************************************************
FUNCTION    : DeleteShm
DESCRIPTION : ���� �޸𸮿� ������ ����
PARAMETERS  :
RETURNED    : 1(SUCCESS)
******************************************************************************/
int lDeleteShm()
{
    int fd;
    int fin_sig;
    int ii;
    int rec_cnt;
    KEYNAME del_kn;
    KEYNAME *kn_res;

    printf("---------------------------------");
    printf("\n\n    Enter a key to delete\n\n");
    printf("---------------------------------\n\n");

    //������� �ϴ� key �� �Է�
    printf("Input key(9) : ");
    scanf("%9s", del_kn.key);

    fd = shm_open(SHMNAME, O_RDWR,0644);
    if (fd == -1)
    {
        perror("shm_open");
        return -1;
    }

    //���� ���ڵ� ���� ������
    rec_cnt = CountShm();

    GlobalAttachShm(fd);

    //������� �ϴ� key �� �˻�
    kn_res = bsearch(&del_kn.key, g_KnList, rec_cnt, sizeof(KEYNAME), Cmp);

    // ���� ���� ���
    if (kn_res == NULL)
    {
        printf("\n---------------------------------");
        printf("\nNot Found. key[%s]\n",del_kn.key);
        printf("Search failed. Please try again.\n");
        printf("---------------------------------\n");
        return -1;
    }
    //���� ���� �� ��� ���� ��Ȯ��
    printf("\n---------------------------------\n");
    printf("Key : %s\t Name : %s\n", kn_res -> key, kn_res -> name);

    printf("\nAre you sure? (1)yes (2)no : ");
    scanf("%d",&fin_sig);
    while (getchar() != '\n'){}

    //���� Ȯ���� ����
    if (fin_sig == 1)
    {
        //������ �ش�Ǵ� ���ڵ带 �����ϰ� �����
        lseek(fd, 0, SEEK_SET);
        for(ii=0; ii < rec_cnt; ii++)
        {
            if ((g_KnList+ii) -> key != kn_res -> key)
            {
                write(fd, (g_KnList+ii) -> key, sizeof((g_KnList+ii) -> key));
                write(fd, (g_KnList+ii) -> name, sizeof((g_KnList+ii) -> name));
            }
         }

        //���� ũ�� ����
        ftruncate(fd,lseek(fd, 0, SEEK_CUR));

        printf("Delete Complete\n\n");
        printf("Save Complete\n\n");
    }

    GlobalDetachShm();
    close(fd);