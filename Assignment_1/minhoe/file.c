/*******************************************************************************
* 프로그램 이름 : file
* 소스파일 이름 : file.c
* 마지막 작성일 : 2023년 12월 06일
* 작 성 자      : INANCE
* 설 명         : 과제 1 파일 입출력 처리 
*******************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define CODESIZE 9
#define NAMESIZE 40

int g_cnt   = 0;
int g_ii    = 0;

typedef struct _STOCK
{
    char code[CODESIZE+1];
    char name[NAMESIZE+1];
} STOCKS;


/*******************************************************************************
함수명   : Add()
파라미터 : STOCKS* stock - STOCKS 구조체
           FILE* fd      - File Description
리턴값   : 정상   - 양수 or 0
           비정상 - 음수
설명     : 파일 입력
*******************************************************************************/

int Add(STOCKS* stock, FILE* fd)
{
    char code_buf[10] = {0,};
    char name_buf[41] = {0,};

    fseek(fd,0,SEEK_END);

    //code 입력 후 stock 구조체에 복사
    printf("\n[\"CODE\"를 입력해주세요]\n");
    printf("CODE (1~9) : ");
    scanf("%9s", code_buf);
    memcpy(stock[g_ii].code, code_buf, CODESIZE);

    // 입력 버퍼 비우기
    while (getchar() != '\n'){}

    //name 입력 후 stock 구조체에 복사
    printf("[\"NAME\"을 입력해주세요]\n");
    printf("NAME (1~40) : ");
    scanf("%40s", name_buf);
    memcpy(stock[g_ii].name, name_buf, NAMESIZE);

    // 입력 버퍼 비우기
    while (getchar() != '\n'){}

    //stock 구조체에 복사한 파일 내용을 file.txt에 입력
    fprintf(fd, "%-10s\t%s\n", stock[g_ii].code, stock[g_ii].name);
    printf("[파일 입력을 성공했습니다]\n");

    //전역변수 인덱스인 g_ii를 증가시켜, 할당된 메모리만큼만 입력받음
    g_ii++;

    return 1;
}

/*******************************************************************************
** 함수명   : Read()
** 파라미터 : FILE* fd - File Description
** 리턴값   : 정상   - 양수 or 0
**            비정상 - 음수
** 설명     : file.txt로부터 파일을 읽어와 화면 상 출력
*******************************************************************************/
int Read(FILE* fd)
{
    STOCKS st;

    fseek(fd,0,SEEK_SET);

    printf("\n---------------------------------------\n");
    printf("%-10s\t%-40s\n", "CODE", "NAME");
    printf("---------------------------------------\n");

    //tab으로 구분 된 파일 내용을 불러와 st 구조체의 code에 삽입 후 출력
    while (fscanf(fd,"%s",st.code) != EOF)
    {
        printf("%-10s\t", st.code);

        //개행으로 구분 된 파일 내용을 불러와 st 구조체의 name에 삽입 후 출력
        fscanf(fd,"%s", st.name);
        printf("%-40s\n", st.name);
    }
    printf("---------------------------------------\n\n");

    return 1;
}
/*******************************************************************************
** 함수명   : Search()
** 파라미터 : FILE* fd - File Description
** 리턴값   : 정상   - 양수 or 0
**            비정상 - 음수
** 설명     : code를 입력 받아, code와 일치하는 name 출력
*******************************************************************************/
int Search(FILE* fd)
{
    STOCKS st;
    char buf[10]={0,};

    fseek(fd,0,SEEK_SET);

    //검색하고자 하는 code 입력받아 buf에 입력
    printf("\n[\"CODE\"를 입력해주세요]\n");
    printf("code : ");
    scanf("%s", buf);

    while (fscanf(fd,"%s", st.code) != EOF)
    {
        fscanf(fd,"%s", st.name);
        //입력받은 code 값과 파일에서 불러온 code 값을 비교해 일치 할 경우 출력 후 탈출
        if (strncmp(st.code, buf, sizeof(buf)) == 0)
        {
            printf("\n---------------------------------------\n");
            printf("%-10s\t%-40s\n", "code", "name");
            printf("---------------------------------------\n");
            printf("%-10s\t", st.code);
            printf("%-40s\n", st.name);
            printf("---------------------------------------\n");
            break;
            }
    }

    return 1;
}

/*******************************************************************************
** 함수명   : main()
** 파라미터 : int  argc    - 실행인자 개수
**           char argv**  - 실행인자
** 리턴값   : 정상   - 0(EXIT_SUCCESS)
**            비정상 - 1(EXIT_FAILURE)
** 설명     : main 함수
*******************************************************************************/
int main()
{
    int mode;
    STOCKS* stock;
    FILE* fd;

    //받고자하는 데이터의 수를 입력받아 동적으로 메모리 할당
    printf("\n[데이터의 수를 입력해주세요]\n");
    scanf("%d", &g_cnt);

    stock = (STOCKS *)calloc(g_cnt, sizeof(STOCKS));
//  memset(stock, 0x00, sizeof(stock)); 

    //파일 open, 에러시 리턴값 음수
    if ((fd = fopen("file.txt", "w+")) == NULL)
        {
            printf("file open fail!\n");
            return -1;
        }

    while (1)
    {
        printf("\n[메뉴를 선택해주세요]\n");
        printf("1.ADD\n2.READ\n3.SEARCH\n4.EXIT\n");
        printf("INSERT MENU : ");

        scanf("%d", &mode);
        switch (mode)
        {
            case 1 :
                //전역 index가 받고자하는 데이터 수를 초과할 경우 add 기능을 작동하지 않도록 함
                if (g_ii >= g_cnt)
                {
                    printf("!입력 가능한 데이터 수를 초과하였습니다!\n");
                    break;
                }
                Add(stock,fd);
                break;
            case 2 :
                Read(fd);
                break;
            case 3 :
                Search(fd);
                break;
            case 4 :
                free(stock);
                fclose(fd);
                return 1;
        }
    }
}
