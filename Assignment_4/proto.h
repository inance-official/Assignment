/******************************************************************************
* SOURCE NAME  : glomem.h
* DESCRIPTION  : SHM key¿Í prototype
* DATE         :
* AUTHOR       :
******************************************************************************/

/*--- define ---*/
#define SHM_KEY		0x00012345
#define PERM		0666
#define MAX_ENTRIES 1000

/*--- prototype ---*/
int CreateShm();
int CheckShm();
KEYNAME *AttachShm();
int DetachShm();
int RemoveShm();

