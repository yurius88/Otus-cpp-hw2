#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <sstream>

struct IpAddr
{
    IpAddr()
			: Addr1(0),
		    Addr2(0),
		    Addr3(0),
		    Addr4(0)
    {
	    
    }

    IpAddr(unsigned char addr1, unsigned char addr2, unsigned char addr3, unsigned char addr4)
	    : Addr1(addr1), Addr2(addr2), Addr3(addr3), Addr4(addr4)
    {
	    
    }

    IpAddr(std::vector<std::string> ipAddrs)
    {
        assert(ipAddrs.size() == 4);

        Addr1 = static_cast<char>(strtoul(ipAddrs[0].c_str(), NULL, 10));
        Addr2 = static_cast<char>(strtoul(ipAddrs[1].c_str(), NULL, 10));
        Addr3 = static_cast<char>(strtoul(ipAddrs[2].c_str(), NULL, 10));
        Addr4 = static_cast<char>(strtoul(ipAddrs[3].c_str(), NULL, 10));
    }

    unsigned char Addr1, Addr2, Addr3, Addr4;

    bool operator <(const IpAddr& b) const
    {
	    if (this->Addr1 == b.Addr1)
	    {
            if (this->Addr2 == b.Addr2)
            {
                if (this->Addr3 == b.Addr3)
                {
                    return this->Addr4 > b.Addr4;
                }
                return this->Addr3 > b.Addr3;
            }
            return this->Addr2 > b.Addr2;
	    }
	    return this->Addr1 > b.Addr1;
    }

    std::string ToString() const
    {
        std::stringstream ss;
        ss << std::to_string(Addr1) << '.' << std::to_string(Addr2)
    		<< '.' << std::to_string(Addr3) << '.' << std::to_string(Addr4);
        return ss.str();
    }
};

std::vector<std::string> split(const std::string& str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while (stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

void print(const std::vector<IpAddr>& ip_pool,  std::ostream& stream)
{
    for (const IpAddr& ipAddr : ip_pool)
    {
        stream << ipAddr.ToString() << std::endl;
    }
}

void printInFile(const std::vector<IpAddr>& ip_pool, const std::string& fileName)
{
    std::ofstream stream(fileName);
    if (!stream.is_open())
        return;
    print(ip_pool, stream);
}

std::vector<IpAddr> filter(const std::vector<IpAddr>& ip_pool, const std::vector<unsigned char>& addrs, bool filter_any)
{
    assert(!addrs.empty());
    assert(addrs.size() < 4);

    std::vector<IpAddr> ip_pool_filtered;
    std::vector<unsigned char>::size_type addrs_size = addrs.size();

    for (const IpAddr& ipAddr : ip_pool)
    {
        if (filter_any)
        {
	       for(unsigned char addr : addrs)
	       {
		       if (ipAddr.Addr1 == addr
                   || ipAddr.Addr2 == addr
                   || ipAddr.Addr3 == addr
                   || ipAddr.Addr4 == addr
                   )
		       {
                   ip_pool_filtered.push_back(ipAddr);
		       }
	       }
        }
        else
        {
            if (addrs_size == 1)
            {
                if (ipAddr.Addr1 == addrs[0])
                {
                    ip_pool_filtered.push_back(ipAddr);
                }
            }
            else if (addrs_size == 2)
            {
                if (ipAddr.Addr1 == addrs[0]
                    && ipAddr.Addr2 == addrs[1])
                {
                    ip_pool_filtered.push_back(ipAddr);
                }
            }
            else if (addrs_size == 3)
            {
                if (ipAddr.Addr1 == addrs[0]
                    && ipAddr.Addr2 == addrs[1]
                    && ipAddr.Addr3 == addrs[2]
                    )
                {
                    ip_pool_filtered.push_back(ipAddr);
                }
            }
            else
            {
                if (ipAddr.Addr1 == addrs[0]
                    && ipAddr.Addr2 == addrs[1]
                    && ipAddr.Addr3 == addrs[2]
                    && ipAddr.Addr4 == addrs[3]
                    )
                {
                    ip_pool_filtered.push_back(ipAddr);
                }
            }
        }
    }

    return ip_pool_filtered;
}

#define UNREFERENCED_PARAMETER(P)          (void)(P)

int main(int argc, char const* argv[])
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);
    
    try
    {
        std::ifstream ifs{ "ip_filter.tsv" };
        if (!ifs.is_open())
            return 0;

        std::vector<IpAddr> ip_pool;

        for (std::string line; std::getline(ifs, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

        // reverse lexicographically sort
        std::sort(ip_pool.begin(), ip_pool.end());
        print(ip_pool, std::cout);

        // filter by first byte and output
        print(filter(ip_pool, { 1 }, false), std::cout);

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // filter by first and second bytes and output
        print(filter(ip_pool, { 46,70 }, false), std::cout);

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // filter by any byte and output
        print(filter(ip_pool, { 46 }, true), std::cout);

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
