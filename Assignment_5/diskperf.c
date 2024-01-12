#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <memory.h>
#include <strings.h>

int main(int argc, char **argv)
{
	FILE		   *fp   = NULL;
    char            buff[1024*1024];    // 1MB
    char            filepath[256];
    int             blen;

    int             ii, jj;
    int             lcnt;

    struct timespec sttm;    			//start time
    struct timespec edtm;    			//end time
    long            diff = 0;

    if (argc < 3)
    {
        printf("Use: diskperf 파일경로 반복횟수\n");
        exit(0);
    }
									//argv[0] : 프로그램 이름
    strcpy(filepath, argv[1]);		//argv[1] : filepath
    lcnt = atoi(argv[2]);			//argv[3] : ASCII to Integer

    printf("-----------------------------------------------------------------------------------\n");
    printf("반복횟수                  [%d]\n", lcnt);
    printf("-----------------------------------------------------------------------------------\n");

    /*--- opened write test ---*/
    fp = fopen(filepath, "w+"); 
	//텍스트 쓰기, 갱신, 기존 내용 지우고 열기, 없으면 새로 만듦
    if (fp == NULL)
    {
        printf("open Error! [%s]errno[%d]\n", filepath, errno);
        return -1;
    }

    for (ii = 1024; ii <= sizeof(buff); ii += 10240)
    {
		//start time
        clock_gettime(CLOCK_MONOTONIC, &sttm);

        for (jj = 0; jj < lcnt; jj++)
        {
			//fwrite
            blen = fwrite(buff, 1, ii, fp);
            if (blen != ii)
            {
                printf("write Error! size[%d:%d]errno[%d]\n", blen, ii, errno);
            }
			fflush(fp);
        }
		//end time
        clock_gettime(CLOCK_MONOTONIC, &edtm);
		//end time - start time
        diff = ((edtm.tv_sec * 1000000000) + edtm.tv_nsec) -
               ((sttm.tv_sec * 1000000000) + sttm.tv_nsec);
        printf("T1) opened write                 시간[%3ld.%09ld 초] size[%5d]\n",
            diff / 1000000000, diff % 1000000000, ii);
    }
    fclose(fp);
    printf("-----------------------------------------------------------------------------------\n");


   /*--- opened read test ---*/
    fp = fopen(filepath, "r");
	//읽기, 파일이 없으면 에러
    if (fp == NULL)
    {
        printf("open Error! [%s]errno[%d]\n", filepath, errno);
        return -1;
    }
  
    for (ii = 1024; ii <= sizeof(buff); ii += 10240)
    {
		//start time
        clock_gettime(CLOCK_MONOTONIC, &sttm);

        for (jj = 0; jj < lcnt; jj++)
        {
			//fread
            blen = fread(buff, 1, ii, fp);
            if (blen != ii)
            {
                printf("read Error! size[%d:%d]errno[%d]\n", blen, ii, errno);
            }
        }
		//end time
        clock_gettime(CLOCK_MONOTONIC, &edtm);
		//end time - start time
        diff = ((edtm.tv_sec * 1000000000) + edtm.tv_nsec) -
               ((sttm.tv_sec * 1000000000) + sttm.tv_nsec);
        printf("T2) opened read                  시간[%3ld.%09ld 초] size[%5d]\n",
            diff / 1000000000, diff % 1000000000, ii);
    }

    fclose(fp);
    printf("-----------------------------------------------------------------------------------\n");

    /*--- open/close write test ---*/
	fp = fopen(filepath, "w+");
	//텍스트 쓰기, 갱신, 기존 내용 지우고 열기, 없으면 새로 만듦
    if (fp == NULL)
    {
        printf("open Error! [%s]errno[%d]\n", filepath, errno);
        return -1;
    }
	fclose(fp);

    for (ii = 1024; ii <= sizeof(buff); ii += 10240)
    {
		//start time
        clock_gettime(CLOCK_MONOTONIC, &sttm);

        for (jj = 0; jj < lcnt; jj++)
        {
			fp = fopen(filepath, "r+");
			//텍스트 쓰기, 갱신, 기존 내용 지우고 열기, 없으면 새로 만듦
            if (fp == NULL)
            {
                printf("open Error! [%s]errno[%d]\n", filepath, errno);
                return -1;
            }

            blen = fwrite(buff, 1, ii, fp);
            if (blen != ii)
            {
                printf("write Error! size[%d:%d]errno[%d]\n", blen, ii, errno);
            }
			fflush(fp);

			fclose(fp);
        }

		//end time
        clock_gettime(CLOCK_MONOTONIC, &edtm);
		//end time - start time
        diff = ((edtm.tv_sec * 1000000000) + edtm.tv_nsec) -
               ((sttm.tv_sec * 1000000000) + sttm.tv_nsec);
        printf("T3) open/close write             시간[%3ld.%09ld 초] size[%5d]\n",
            diff / 1000000000, diff % 1000000000, ii);
    }
    printf("-----------------------------------------------------------------------------------\n");

	/*--- open/close read test ---*/
    for (ii = 1024; ii <= sizeof(buff); ii += 10240)
    {
		//start time
        clock_gettime(CLOCK_MONOTONIC, &sttm);

        for (jj = 0; jj < lcnt; jj++)
        {
            fp = fopen(filepath, "r");
			// 읽기 모드, 파일이 없으면 에러
            if (fp == NULL)
            {
                printf("open Error! [%s]errno[%d]\n", filepath, errno);
                return -1;
            }

			//fread
            blen = fread(buff, 1, ii, fp);
            if (blen != ii)
            {
                printf("read Error! size[%d:%d]errno[%d]\n", blen, ii, errno);
            }

			fclose(fp);
        }
		//end time
        clock_gettime(CLOCK_MONOTONIC, &edtm);
		//end time -  start time
        diff = ((edtm.tv_sec * 1000000000) + edtm.tv_nsec) -
               ((sttm.tv_sec * 1000000000) + sttm.tv_nsec);
        printf("T4) open/close read              시간[%3ld.%09ld 초] size[%5d]\n",
            diff / 1000000000, diff % 1000000000, ii);
    }
    printf("-----------------------------------------------------------------------------------\n");

	exit(0);
}
