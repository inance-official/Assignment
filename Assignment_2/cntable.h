/******************************************************************************
* SOURCE NAME  : cntable.h
* DESCRIPTION  : 헤더파일
* DATE         :
* AUTHOR       :
******************************************************************************/

/*--- define ---*/
#define CODESIZE 9
#define NAMESIZE 40

/*--- struct ---*/
typedef struct tm NOWTIME;

typedef struct _CODENAME
{
    char code[CODESIZE + 1];
    char name[NAMESIZE + 1];
} CODENAME;

