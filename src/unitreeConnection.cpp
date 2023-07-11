#include "unitreeConnection.h"
#include <iostream>
#include <cstring>
#include <thread>

namespace FREE_DOG_SDK {

UnitreeConnection::UnitreeConnection(const std::tuple<int, const char*, int, const char*>& settings) {
    listenPort = std::get<0>(settings);
    addr = std::get<1>(settings);
    sendPort = std::get<2>(settings);
    localIP = std::get<3>(settings);
    sock = connect();
    runRecv = false;
    recvThread = nullptr;
    std::cout << "Local IP: " << localIP << " listen Port: " << listenPort << std::endl;
    std::cout << "Address: " << addr << " send port: " << sendPort << std::endl;
}

void UnitreeConnection::startRecv() {
    runRecv = true;
    recvThread = new std::thread(&UnitreeConnection::recvThreadFunc, this);
}

void UnitreeConnection::stopRecv() {
    runRecv = false;
    recvThread->join();
    delete recvThread;
    recvThread = nullptr;
}

void UnitreeConnection::send(LowCmd* cmd) {
  cmd->buildCmd(&cmd_buff, false);
    
  int res = sendto(sock, cmd_buff.data(), cmd_buff.size(), 0, (struct sockaddr*)&robot_address, sizeof(robot_address));
    if(res == -1) {
        std::cerr << "Failed sending command: " << strerror(errno) << std::endl;
    }
}

std::vector<std::vector<uint8_t>> UnitreeConnection::getData() {
    std::lock_guard<std::mutex> lock(dataMutex);

    std::vector<std::vector<uint8_t>> ret = data;
    // Clear data buffer after handing it out
    data.clear();
    return ret;
}

int UnitreeConnection::connect() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(localIP);
    server_address.sin_port = htons(listenPort);

    if (bind(sock, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Failed to bind socket: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    robot_address.sin_family = AF_INET;
    robot_address.sin_addr.s_addr = inet_addr(addr);
    robot_address.sin_port = htons(sendPort);
 
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
        std::cerr << "Failed to set socket receive timeout" << std::endl;
        exit(EXIT_FAILURE);
    }

    return sock;
}

void UnitreeConnection::recvThreadFunc() {
    std::vector<uint8_t> buffer(2048);
    while (runRecv) {
        struct sockaddr_in client_address;
        socklen_t addrlen = sizeof(client_address);
        ssize_t receivedBytes = recvfrom(sock, buffer.data(), buffer.size(), 0, (struct sockaddr*)&client_address, &addrlen);
        if (receivedBytes > 0) {
            std::lock_guard<std::mutex> lock(dataMutex);
            data.push_back(std::vector<uint8_t>(buffer.begin(), buffer.begin() + receivedBytes));
        }
    }
}

}
