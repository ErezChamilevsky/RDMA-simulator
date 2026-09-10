#include <string>

class DTransport {
    
    public:
        std::string destination;
        std::string target;

        virtual void Init() = 0;

        virtual void Send() = 0;

        virtual int Recieve() = 0; 

        virtual void Close() = 0;
    

};