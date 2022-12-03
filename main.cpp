#include <iostream>
#include "DataStream.h"

using namespace yazi::serialize;

int main() {
    DataStream ds;
    bool b = true;
    char c = 'j';
    int32_t i32 = 123;
    int64_t i64 = 456;
    float f = 1.23;
    double d = 4.56;
    ds.write(b);
    ds.write(c);
    ds.write(i32);
    ds.write(i64);
    ds.write(f);
    ds.write(d);
    ds.display();
    return 0;
}
