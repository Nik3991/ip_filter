#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

namespace {
    static const char dot = '.';
}

struct Ip
{
#ifdef WIN32
    using ip_t = __int32;
#else
    using ip_t = __int32_t;
#endif

    ip_t ip;

    int oct3() const {return (ip >> 24) & 0x000000FF;}
    int oct2() const {return (ip >> 16) & 0x000000FF;}
    int oct1() const {return (ip >> 8) & 0x000000FF;}
    int oct0() const {return ip & 0x000000FF;}

    bool operator<(const Ip& _ip) const
    {
        bool result = false;
        auto is_equal = [&](int _o1, int _o2)
        {
           result = _o1 < _o2;
           return _o1 == _o2;
        };

        if (is_equal(oct3(), _ip.oct3()))
        {
            if (is_equal(oct2(), _ip.oct2()))
            {
                if (is_equal(oct1(), _ip.oct1()))
                {
                    is_equal(oct0(), _ip.oct0());
                }
            }
        }

        return !result;
    }

    bool equal(const Ip& _ip, int mask) const
    {
        return (ip & mask) == (_ip.ip & mask);
    }

    bool any_oct_equal(int _oct) const
    {
        return oct0() == _oct || oct1() == _oct || oct2() == _oct || oct3() == _oct;
    }
};

Ip ip_from_string(const string &str)
{
    int bias = 3;
    int result = 0;

    string::size_type start = 0;
    string::size_type stop = str.find_first_of(dot);
    while(stop != string::npos)
    {
        result |= (stoi(str.substr(start, stop - start)) << 8*bias);
        start = stop + 1;
        stop = str.find_first_of(dot, start);
        --bias;
    }

    result |= (stoi(str.substr(start)) << 8*bias);

    return Ip{result};
}

ostream& operator<<(ostream& _cout, const Ip& _ip)
{
    _cout << _ip.oct3() << dot <<
             _ip.oct2() << dot <<
             _ip.oct1() << dot <<
             _ip.oct0() << endl;
    return _cout;
}

void print(const Ip& _target_ip, int mask, vector<Ip>& _v)
{
    for (const Ip& ip : _v)
    {
        if (_target_ip.equal(ip, mask))
        {
            cout << ip;
        }
    }
}

void print(int _oct, vector<Ip>& _v)
{
    for (const Ip& ip : _v)
    {
        if(ip.any_oct_equal(_oct))
        {
            cout << ip;
        };
    }
}

int main()
{
    vector<Ip> ip_pool;
    for(string line; getline(cin, line) && !line.empty();)
    {
        ip_pool.push_back(ip_from_string(line.substr(0, line.find_first_of('\t'))));
    }

    sort(ip_pool.begin(), ip_pool.end());

    for (Ip& ip : ip_pool)
    {
        cout << ip;
    }

    print(ip_from_string("1.0.0.0"),   0xFF000000, ip_pool);
    print(ip_from_string("46.70.0.0"), 0xFFFF0000, ip_pool);
    print(46, ip_pool);

    return 0;
}
