#include "DTransport.cpp"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class TCPTransportClient : public DTransport{
    private:
        int clientSocket = -1;

    public:
        void Init()
        {
            clientSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (clientSocket < 0)
            {
                std::cerr << "Socket creation failed\n";
                return;
            }

            sockaddr_in serverAddress{};
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_port = htons(8080);

            if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0)
            {
                std::cerr << "Invalid address / Address not supported\n";
                close(clientSocket);
                clientSocket = -1;
                return;
            }

            if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
            {
                std::cerr << "Connection failed\n";
                close(clientSocket);
                clientSocket = -1;
            }
        }

        void Send()
        {
            if (clientSocket < 0)
                return;

            int file_fd = open("test_file.txt", O_RDONLY);
            if (file_fd < 0)
            {
                std::cerr << "Failed to open file for sending\n";
                return;
            }

            struct stat file_stat;
            if (fstat(file_fd, &file_stat) < 0)
            {
                std::cerr << "Failed to get file stats\n";
                close(file_fd);
                return;
            }

            off_t offset = 0;
            size_t remaining = file_stat.st_size;

            while (remaining > 0)
            {
                ssize_t sent = sendfile(clientSocket, file_fd, &offset, remaining);
                if (sent <= 0)
                {
                    break;
                }
                remaining -= sent;
            }

            close(file_fd);
        }

        int Recieve()
        {
            if (clientSocket < 0)
                return -1;

            std::ofstream file("tcp_res.txt", std::ios::binary);
            if (!file.is_open())
            {
                return -1;
            }

            char buffer[4096];
            ssize_t bytesRead;

            while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0)
            {
                file.write(buffer, bytesRead);
            }

            file.close();
            return bytesRead < 0 ? -1 : 1;
        }

        void Close()
        {
            if (clientSocket >= 0)
            {
                close(clientSocket);
                clientSocket = -1;
            }
        }
};

int main(){
    TCPTransportClient client;
    client.Init();
    client.Recieve();
    client.Close();

    return 1;

}