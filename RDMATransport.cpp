#include "DTransport.cpp"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fstream>
#include <memory>
#include <chrono>

class RDMATransport : public DTransport
{
private:
    int fd = -1;
    struct stat sb;
    char *mapped = nullptr;

public:
    void init()
    {
        struct stat source_sb;
        if (stat(source, &source_sb) == -1)
        {
            std::perror("stat source");
            return;
        }
        sb = source_sb;

        fd = open(target, O_RDWR | O_CREAT, 0644);
        if (fd == -1)
        {
            std::perror("open target");
            return;
        }

        if (ftruncate(fd, sb.st_size) == -1)
        {
            std::perror("ftruncate");
            close(fd);
            fd = -1;
            return;
        }

        mapped = static_cast<char *>(mmap(nullptr, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
        if (mapped == MAP_FAILED)
        {
            std::perror("mmap");
            close(fd);
            fd = -1;
            return;
        }
    }

    void send()
    {
        std::ifstream fileToSend(source, std::ios::binary);
        if (!fileToSend.is_open())
        {
            std::perror("fileToSend open");
            return;
        }

        fileToSend.read(mapped, sb.st_size);
        fileToSend.close();
    }

    int recieve()
    {
        return 0;
    }

    void transportClose()
    {
        if (mapped != MAP_FAILED && mapped != nullptr)
        {
            munmap(mapped, sb.st_size);
        }
        if (fd != -1)
        {
            close(fd);
        }
    }
};

int main()
{
    char *file = "test_file.txt";
    char *target = "target.txt";
    std::unique_ptr<DTransport> transportStrategy = std::make_unique<RDMATransport>();

    transportStrategy->target = target;
    transportStrategy->source = file;

    transportStrategy->init();

    auto start = std::chrono::high_resolution_clock::now();

    transportStrategy->send();

    auto end = std::chrono::high_resolution_clock::now();

    transportStrategy->transportClose();

    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout << "RDMA clock time: " << elapsed.count() << " ms\n";
    return 0;
}