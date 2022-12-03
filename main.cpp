#include "DataStream.h"

using namespace yazi::serialize;

void writeTest() {
    DataStream ds;
    bool b = true;
    char c = 'j';
    int32_t i32 = 123;
    int64_t i64 = 456;
    float f = 1.23;
    double d = 4.56;
    std::string s = "jiangaoran";
    ds << b;
    ds.display();
    ds << c;
    ds.display();
    ds << i32;
    ds.display();
    ds << i64;
    ds.display();
    ds << f;
    ds.display();
    ds << d;
    ds.display();
    ds << s;
    ds.display();
}

void readTest() {
    DataStream ds;
    ds << false;
    bool b;
    ds >> b;
    std::cout << std::boolalpha << b;

    ds << 'x';
    char c;
    ds >> c;
    std::cout << c;

    int i32 = 123;
    ds << i32;
    int i32r;
    ds >> i32r;
    std::cout << i32r;

    int64_t i64 = 456;
    ds << i64;
    int64_t i64r;
    ds >> i64r;
    std::cout << i64r;

    float f = 1.23;
    ds << f;
    float fr;
    ds >> fr;
    std::cout << fr;

    double d = 4.56;
    ds << d;
    double dr;
    ds >> dr;
    std::cout << dr;

    ds << "safsadf";
    std::string sr;
    ds >> sr;
    std::cout << sr;
}

int main() {
    readTest();
    return 0;
}
