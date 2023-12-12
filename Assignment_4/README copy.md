# Table

- [Question](#question)
- [Formatting](#formatting)
- [Input & Output example](#input--output-example)

# Question

- 이 프로젝트는 file.txt 파일에 대해 쓰기, 읽기, 검색, 삭제 기능을 수행하는 프로그램을 만들어야 한다. 각 기능은 별도의 main 함수를 가진 소스 파일에 구현되야하며 구조체와 define은 헤더 파일에 포함되어야 한다.

- Assignment_2에서 f serise(`fopen()`, `fclose()` ...)에서 universal I/O model(`open()`, `close()` ...)로 교체
- key에 대한 중복값 예외

- insert.c
    - `qsort()`를 사용하여 데이터를 삽입하고, 'q'를 입력하면 프로그램이 종료
    - code에 대한 중복 값 예외
- read.c
    - `system()`을 사용하여 화면을 지우고, `sleep()`을 사용하여 2초마다 화면을 갱신.
    - 현재 날짜와 시간도 출력
- search.c
    - `bsearch()`를 사용하여 키를 검색하고, 중복된 키가 있으면 모두 출력.
    -  검색이 끝나면 추가 검색 여부를 확인하고, 검색이 실패하면 다시 검색 여부를 확인
- delete.c
    - 데이터를 삭제하고 `qsort()`를 사용하여 재배열.
    - 키가 중복된 경우에만 목록을 출력하고, 중복된 키 중에서 이름을 입력하여 삭제. 삭제 확인 여부를 출력
- kntable.h
    - 구조체와 define이 포함된 헤더 파일
- file.txt
    - 데이터만 저장된 파일

프로젝트에 필수적으로 사용해야할 함수는 `open()`, `close()`, `qsort()`, `bsearch()`이다. 또한, 동적 할당 값을 입력으로 받지 않아야 하며, 파일에 데이터를 쓸 때는 반드시 정렬해야 한다. 텍스트 파일에는 데이터만 있어야 하며, 각 함수에 대한 주석을 작성해야 하며 컴파일은 Makefile로 수행한다.

# Formatting
- 기본 형식
- 필요에 따라 사용자 지정 함수 변경 및 추가 삭제 가능



# Input & Output example

