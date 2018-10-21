#include<stdio.h>

void a(){
    ;
}

void b(){
    a();
}

void c(){
    ;
}

void d(){
    c();
}

void e(){
    d();
}

int main(){
    b();
    e();
}