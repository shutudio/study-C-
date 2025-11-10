/*    
    object_return.cpp
    목적: 객체 리턴을 학습하고 추가적으로 객체 치환에 대해 공부
    빌드: g++ -std=c++14 -o object_return.cpp
    실행: ./object_return
*/

#import <iostream>
using namespace std ;

class Circle {
    int radius ;
public :
    Circle() { radius = 1 ; }
    Circle(int radius){ this->radius = radius ;}
    void setRadius(int radius) { this->radius = radius ;}
    double getArea() {return 3.14 * radius * radius ;}
} ;

Circle getCricle(){
    Circle tmp(30) ;
    return tmp ; // 객체 tmp의 복사본을 리턴한다.
}

int main(){
    Circle c ; // 1로 초기화된 객체가 생성
    cout << c.getArea() << endl ;

    c = getCricle() ; // tmp 객체가 c에 복사된다.
    cout << c.getArea() << endl ;

    Circle c2(1) ;
    c = c2 ; //c2 객체를 c 객체에 비트 단위 복사
    cout << c.getArea() << endl ; 
}