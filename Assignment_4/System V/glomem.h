/******************************************************************************
* SOURCE NAME  : glomem.h
* DESCRIPTION  : SHM ���� ��� ����
* DATE         :
* AUTHOR       :
******************************************************************************//**


/*--- define ---*/
#define SHM_KEY		0x00012345
#define PERM		0666
#define MAX_ENTRIES 1000

/*--- protype ---*/
int GlobalCreateShm();
int GlobalCheckShm();
KEYNAME *GlobalAttachShm();
int GlobalDetachShm();
int GlobalRemoveShm();

