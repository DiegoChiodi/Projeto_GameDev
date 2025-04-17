#include <iostream>

int sign (float valor) {
    bool ver = valor;
    int result = (valor < 0) ? -1 : ver;
    
    return result;
}

int main ()
{
    std::cout << sign(-1.0f);
    return 0;
}