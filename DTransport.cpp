#include <string>

class DTransport {
    
    public:
        char* source;
        char* target;

        virtual void init() = 0;

        virtual void send() = 0;

        virtual int recieve() = 0; 

        virtual void transportClose() = 0;

        virtual ~DTransport() = default;
};