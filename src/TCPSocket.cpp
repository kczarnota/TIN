#include "TCPSocket.hpp"

/**
 * Default constructor for TCPSocket
 */
TCPSocket::TCPSocket()
{
    this->isConnected = 0;
    this->socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
}

TCPSocket::TCPSocket(int socketFileDescriptor)
{
    this->socketFileDescriptor = socketFileDescriptor;
}

TCPSocket::~TCPSocket()
{
    close(this->socketFileDescriptor);
}

int TCPSocket::connect(SocketAddress & address)
{
    int status =
        ::connect(this->socketFileDescriptor,
                  address.toInternalAddressStructPointer(), address.getSize());
    return status;
}

int TCPSocket::bind(const SocketAddress & address)
{
    int result =
        ::bind(this->socketFileDescriptor,
               address.toInternalAddressStructPointer(), address.getSize());
    return result;
}

int TCPSocket::listen(int backlog)
{
    ::listen(this->socketFileDescriptor, backlog);
}

TCPSocket TCPSocket::accept()
{
    int newSocket = ::accept(this->socketFileDescriptor, 0, 0);
    TCPSocket client;
    client = *this;
    client.socketFileDescriptor = newSocket;
    return client;
}

TCPSocket & TCPSocket::operator<<(const std::string & data)
{
    send(this->socketFileDescriptor, data.c_str(), data.size(), 0);
    return *this;
}

TCPSocket & TCPSocket::operator>>(std::string & data)
{
    fd_set rfds;
    int retval;
    ssize_t recive_size = 0;
    int bufferSize = 1 << 10;
    char buffer[bufferSize];
    int numAttempts = 2;
    while (1)
    {
        FD_ZERO(&rfds);
        FD_SET(this->socketFileDescriptor, &rfds);
        retval = select(this->socketFileDescriptor + 1, &rfds, nullptr, nullptr,
                        &this->timeout);
        if (retval > 0)
        {
            recive_size =
                recv(this->socketFileDescriptor, buffer, bufferSize - 1, 0);
            buffer[recive_size] = '\0';
            data += buffer;
            numAttempts = 2;
        }
        else if (numAttempts > 0)
        {
            numAttempts--;
        }
        else
        {
            /*Timeout or error*/
            std::cout << "Timeout\n";
            break;
        }
    }
    return *this;
}

void TCPSocket::serveForever(int port)
{
    this->bindAll(port);
    this->listen();
}

void TCPSocket::bindAll(int port)
{
    SocketAddress address;
    address.setPort(port);
    this->bind(address);
}