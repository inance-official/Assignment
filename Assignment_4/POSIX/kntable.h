/******************************************************************************
* SOURCE NAME  : kntable.h
* DESCRIPTION  :
* DATE         :
* AUTHOR       :
******************************************************************************/

/*--- define ---*/
#define KEYSIZE 9
#define NAMESIZE 40

#define SHMNAME "/shm.txt"

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
