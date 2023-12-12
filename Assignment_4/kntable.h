/******************************************************************************
* SOURCE NAME  : kntable.h
* DESCRIPTION  : 헤더파일
* DATE         :
* AUTHOR       :
******************************************************************************/

/*--- define ---*/
#define KEYSIZE 9
#define NAMESIZE 40

#ifndef KNLIST_H
#define KNLIST_H

/*--- struct ---*/
typedef struct KEYNAME
{
    char key[KEYSIZE];
    char name[NAMESIZE];
} KEYNAME;

extern KEYNAME *g_KnList;

#endif /* KNLIST_H */

