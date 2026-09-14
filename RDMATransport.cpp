#include "DTransport.cpp"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fstream>
#include <memory>
#include <chrono>

class RDMATransport : public DTransport {
    private:
        int fd = -1;
        struct stat sb;
        char* mapped = nullptr;
    
    public:
        void init() {
            fd = open(target, O_RDWR);
            if (fd == -1)
            {
                std::perror("open");
                return;
            }
            struct stat test;
            sb = test;

            if (fstat(fd, &sb) == -1)
            {
                std::perror("fstat");
                close(fd);
                return;
            }

            mapped = static_cast<char *>(mmap(nullptr, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
            if (mapped == MAP_FAILED)
            {
                std::perror("mmap");
                close(fd);
                return;
            }
        }

        void send() {
            std::ifstream fileToSend(source, std::ios::binary); 
            if (!fileToSend.is_open())
            {
                std::perror("fileToSend open");
                return;
            }

            fileToSend.read(mapped, sb.st_size);

            fileToSend.close();
        }

        int recieve(){
            
        }

        void transportClose()
        {
            munmap(mapped, sb.st_size);
            close(fd);
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