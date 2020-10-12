#pragma pack(1)

#include "BMPresizer.cpp"
#include <fstream>

int main() {
    BMPresizer a("..\\1.bmp", "..\\res.bmp");
    a.resize(2, 2);

    system("pause");
    return 0;
}