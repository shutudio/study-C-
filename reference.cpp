/*
    reference.cpp
    목적: 변수에 대한 참조
    실행: ./reference
*/

#include <iostream>
using namespace std ;

int main(){
    cout << "i" << '\t' << "n" << '\t' << "refn" << endl ;
    int i = 1 ;
    int n = 2;
    int &refn = n ; // 참조변수 refn 선언, n에 대한 별명
    n = 4 ;
    refn ++ ; // fefn는 n의 별명이므로 n도 증가
    cout << i << '\t' << n << '\t' << refn << endl ;

    refn = i ; // refn이 가리키는 n에 i의 값을 대입
    refn ++ ; // n도 증가
    cout << i << '\t' << n << '\t' << refn << endl ;

    int *p = &refn ; // p는 n의 주소를 가짐
    *p = 20 ; // refn도 20, n도 20으로 변경
    cout << i << '\t' << n << '\t' << refn << endl ;
}