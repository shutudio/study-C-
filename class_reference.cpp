/*    
    class_reference.cpp
    목적: 객체에 대한 참조
    실행: ./class_reference

*/

#include <iostream>
using namespace std;

class Circle {
    int radius ;
public :
    Circle() {radius = 1 ;}
    Circle(int radius){ this->radius = radius ;}
    void setRadius(int radius) {this->radius = radius ;}
    double getArea() {return 3.14 * radius * radius ;}
} ;

int main(){
    Circle circle ;
    Circle &refc = circle ; // circle 객체에 대한 참조 변수 refc 선언
    refc.setRadius(10) ; // refc를 통해 circle 객체의 반지름 설정
    cout << refc.getArea() << " " << circle.getArea() << endl ;
}