#ifndef UNITREE_CONNECTION_H
#define UNITREE_CONNECTION_H

#include <vector>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <mutex>
#include "lowCmd.h"

namespace FREE_DOG_SDK {

class UnitreeConnection {
public:
    UnitreeConnection(const std::tuple<int, const char*, int, const char*>& settings);
    void startRecv();
    void stopRecv();
    void send(LowCmd* cmd);
    std::vector<std::vector<uint8_t>> getData();

private:
    int listenPort;
    struct sockaddr_in server_address;
    struct sockaddr_in robot_address;
    const char* addr;
    int sendPort;
    const char* localIP;
    int sock;
    bool runRecv;
    std::vector<std::vector<uint8_t>> data;
    std::thread* recvThread;
    std::mutex dataMutex;  

    int connect();
    void recvThreadFunc();
    std::array<uint8_t, 614> cmd_buff;
};

}
#endif  // UNITREE_CONNECTION_H

