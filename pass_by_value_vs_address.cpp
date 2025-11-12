/*    
    pass_by_value_vs_address.cpp
    목적: "값에 의한 호출(call by value)"과 "주소에 의한 호출(call by address-포인터 사용)"의 차이를 시연
    실행: ./pass_by_value_vs_address

*/

#include <iostream> 
using namespace std;

/*
    1) 값에 의한 호출 (Call by Value)
        - 매개변수 a, b는 '원본의 복사본'입니다.
        - 함수 안에서 a, b를 바꿔도, 원본 (m,n)은 바뀌지 않습니다.
*/
void swapByValue(int a, int b) {
    int tmp1 = a ; // 지역 변수 tmp1에 a 복사
    a = b ;       // 지역 변수 a <- 지역 변수 b
    b = tmp1 ;    // 지역 변수 b <- 지역 변수 tmp1
    // 함수가 끝나면 a, b, tmp1는 사라집니다. 호출자 m, n에는 영향 없음

}

/*
    2) 주소에 의한 호출 (Call by Address using Pointers)
        - 매개변수 a, b는 '원본의 주소'를 받습니다.
        - *a, *b는 각각 원본 정수(p, q) 그 자체를 가리키므로
          *a, *b를 바꾸면 p, q가 실제로 바뀝니다.

*/
void swapByReference(int *a, int *b) {
    int tmp2 ;  // 임시 저장용 지역 변수
    tmp2 = *a ; // tmp2 <- (a가 가리키는 값: 원본 p)
    *a = *b ;   //( a가 가리키는 값: p) <- (b가 가리키는 값: q)
    *b = tmp2 ; // (b가 가리키는 값: q) <- tmp2(원래 p)
    // 함수가 끝나면 a, b, tmp2는 사라지지만, 결과적으로 p와 q의 값이 교환됩니다.
    
}

int main() {
    // m,n : 값 전달 테스트용
    int m = 2 , n = 9 ;
    // p,q : 포인터(주소) 전달 테스트용
    int p = 2, q= 9 ;

    // 값 전달
    swapByValue(m,n) ;

    // 주소(포인터) 전달
    swapByReference(&p, &q) ; // p, q의 주소를 넘김
    cout << "After swapByValue: m = " << m << ", n = " << n << endl;
    cout << "After swapByReference: p = " << p << ", q = " << q << endl ;
}