#ifndef DNS_CHECKER_DNSPOOLER_HPP
#define DNS_CHECKER_DNSPOOLER_HPP

#include <atomic>
#include <thread>
#include <vector>

class DNSPooler
{
    int interval;
    void pool();
    void refreshDomains();
    const std::vector<std::string> rootServers
        { "198.41.0.4",
        "192.228.79.201",
        "192.33.4.12",
        "199.7.91.13",
        "192.203.230.10",
        "192.5.5.241",
        "192.112.36.4",
        "198.97.190.53",
        "192.36.148.17",
        "192.58.128.30",
        "193.0.14.129",
        "199.7.83.42",
        "202.12.27.33" };

    public:
    DNSPooler(int interval);
    void run();
    void setInterval(int interval);

    void stop();

    std::thread thread;
    std::atomic_bool stopThread;
};

#endif // DNS_CHECKER_DNSPOOLER_HPP
