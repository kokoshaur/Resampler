#pragma pack(1)

#include "BMPresizer.cpp"
#include <fstream>

int main() {
    BMPresizer a("..\\123.bmp", "..\\res.bmp");
    a.resize(2, 2);

    BMPresizer a1("..\\123.bmp", "..\\res2.bmp");
    a1.resize(0.5, 0.5);

    BMPresizer a2("..\\123.bmp", "..\\res4.bmp");
    a2.resize(0.5, 2);

    BMPresizer a3("..\\123.bmp", "..\\res5.bmp");
    a3.resize(2, 0.5);

    system("pause");
    return 0;
}