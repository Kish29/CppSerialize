#include <utility>

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

void listTest() {
    DataStream ds;

    std::vector<int> v{1, 2, 3};
    ds << v;

    std::list<std::string> m{"jar", "lzy", "zyf"};
    ds << m;

    std::vector<int> v2;
    ds >> v2;
    for (auto item: v2) {
        std::cout << item << std::endl;
    }

    std::list<std::string> v3;
    ds >> v3;
    for (auto &item: v3) {
        std::cout << item << std::endl;
    }
}

void mapTest() {
    DataStream ds;
    std::map<std::string, int> m;
    m["a"] = 1;
    m["b"] = 2;
    m["c"] = 3;
    ds << m;

    std::map<std::string, int> m2;
    ds >> m2;
    for (auto &it: m2) {
        std::cout << it.first << "=" << it.second << std::endl;
    }

    std::set<std::string> s;
    s.insert("jar");
    s.insert("lzy");
    s.insert("zyf");
    ds << s;
    std::set<std::string> s2;
    ds >> s2;
    for (auto &it: s2) {
        std::cout << it << std::endl;
    }
}

class User : public Serializable {
public:
    User() = default;
    ~User() = default;

    User(std::string name, int age) : m_name(std::move(name)), m_age(age) {}

    void profile() const {
        std::string ps;
        if (!m_passwords.empty()) {
            ps.append("[");
            for (auto &p: m_passwords) {
                ps.append(p).append(", ");
            }
            ps = ps.substr(0, ps.size() - 2);
            ps.append("]");
        }
        std::cout << "{name=" << m_name << ", age=" << m_age << ", passwords=" << ps << "}" << std::endl;
    }

    void add_password(const std::string &p) {
        m_passwords.push_back(p);
    }

    SERIALIZE(m_name, m_age, m_passwords)

private:
    std::string m_name{};
    int m_age{};
    std::list<std::string> m_passwords{};
};

void objectTest() {
    DataStream ds;

    User a("jar", 23);
    a.add_password("hhh");
    a.add_password("uuu");
    ds << a;

    User b;
    b.profile();

    ds >> b;
    b.profile();
}

int main() {
    DataStream ds;
    int a = 123;
    ds << a;

    int b;
    ds >> b;
    std::cout << b;
    return 0;
}
