/*    
    1call_by_value_exam.cpp
    목적: "값에 의한 호출(call by value)"의 문제점(원본 변경 불가)과
    매개변수에 대해 '일반 생성자'가 아니라 '복사 생성자''가 호출된다는 점을 시연
    빌드: g++ -std=c++14 -o 1call_by_value_exam.cpp
    실행: ./1call_by_value_exam

    핵심 포인트
    - increase(Circle c)에서 c는 '값에 의한 복사'로 생성된다.
        이때 호출되는 것은 '일반 생성자'가 아니라 복사 생성자'이다.
        복사 생성자를 명시하지 않으면, 컴파일러가 자동으로 만들어 둔 복사 생성자가
        동작하므로 "생성자 호출 메시지"가 출력되지 않는다.
    - 함수 내부에서 c를 변경해도 c1에는 영향이 없다.(깊은/얕은 복사 이슈와 무관하게
        기본 타입 하나(Radius)만 가지므로 여기서는 '원본 불변'이 더 본질)
    - 함수가 끝나면, 매개변수로 복사된 임시 객체 c가 소멸되며 소멸자 메시지가 먼저 찍힌다.
*/

#include <iostream>
using namespace std ; 

class Circle {
private:
    int radius ;    // 반지름 값(정수 하나만 보관)
public:
    Circle() ;      // 기본 생성자
    Circle(int r);  // 반지름을 매개변수로 받는 생성자
    ~Circle() ;     // 소멸자
    double getArea() {return 3.14*radius*radius ;}
    int getRadius() { return radius ; }
    void setRadius(int radius) {this->radius = radius ;}
}; 

/*
    기본 생성자: '위임 생성자(delegating constructor)' 문법을 사용하야
    Circle(1) 생성자를 재사용하여 radius를 1로 초기화
*/
Circle::Circle():Circle(1){
    cout << "Circle() 기본 생성자 호출 = "<<radius << endl ;
}

// 반지름을 직접 지정하는 생성자
Circle::Circle(int r){
    radius = r ;
    cout << "Circle(int) 생성자 호출 = " << radius << endl ;
}

// 소멸자: 객체가 사라질 때 반지름 정보를 출력
Circle::~Circle(){
    cout << "원 반지름 " << radius << "인 Circle 객체 소멸" << endl ;
}

/*
    값에 의한 호출(call by value)
    - 인자로 넘어온 c1이 '복사되어' 지역변수 c가 만들어진다.
    -여기서 호출되는 것은 '일반 생성자'가 아니라 '복사 생성자'.
    - 현재 복사 생성자를 정의하지 않았으므로, 컴파일러가 만든
        암시적 복사 생성자가 동작(=아무 메시지 없음)
    - 따라서 "생성자 호출" 메시지는 찍히지 않는다.
    - 함수 끝에서 지역 변수 c가 소멸되어 소멸자 메시지가 한 번 더 찍힌다.
*/
void increase(Circle c){
    int r = c.getRadius() ; //여기서 c는 원본 c1의 복사본
    c.setRadius(r+1) ;      // 복사본의 반지름만 +1 (원본 c1에는 영향 없음)
}

// 주소에 의한 호출로 반지름을 증가시키는 함수
void increase2(Circle *p){
    int r = p->getRadius() ; //포인터 p가 가리키는 객체의 반지름을 얻음
    p->setRadius(r+1) ;      // 포인터 p가 가리키는 객체의 반지름을 +1
}

int main(){
    Circle c1(30) ;
    Circle c2(40) ;
    increase(c1) ;
    cout << c1.getRadius() << endl ;
    increase2(&c2) ; // 주소 전달
    cout << c2.getRadius() << endl ;
}