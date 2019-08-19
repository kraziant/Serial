#ifndef SERIALLIB_SERIAL_HPP
#define SERIALLIB_SERIAL_HPP

#include <string>
#include <vector>

#include <Windows.h>

#include "SerialConfig.hpp"


namespace Serial
{

    typedef struct COMMQUEUESIZE{
        DWORD download;
        DWORD upload;
    }COMMQUEUESIZE;

    class Config
    {

        public:

            DCB dcb;
            COMMTIMEOUTS cto;
            COMMQUEUESIZE cqs;


        public:
            Config(void);
            ~Config(void);
    };


    class Port{

        // PROPERTIES ////////////////////////////////////////////////////////

        public:
            std::string name;

        protected:
            HANDLE _handle;
            bool _isNameInitialized;
            Serial::Config _config;

        private:
            std::wstring __commName;


        // METHODS ///////////////////////////////////////////////////////////

        public:
            Port();
            Port(const Port& port);
            Port(const char* name);
            Port(const char* name, Config &cfg);
            Port(const wchar_t* name);
            Port(const wchar_t* name, Config &cfg);
            Port(const std::string name);
            Port(const std::string name, Config &cfg);
            Port(const std::wstring name);
            Port(const std::wstring name, Config &cfg);
            
            ~Port();

            bool is_open();

            int open();
            int open(const wchar_t* name);
            int open(const wchar_t* name, Config &cfg);
            int open(const std::wstring name);
            int open(const std::wstring name, Config &cfg);
            int open(const char* name);
            int open(const char* name, Config &cfg);
            int open(const std::string name);
            int open(const std::string name, Config &cfg);

            int close();
            
            int setup(Config &cfg);

            unsigned int write(std::vector<char>& buf);
            std::vector<char> read();

            Port& operator<<(Config& cfg);
            Port& operator>>(DCB& dcb);
            Port& operator<<(DCB& dcb);
            Port& operator<<(COMMTIMEOUTS& cto);
            Port& operator<<(COMMQUEUESIZE& cqs);

            Port& operator<<(std::vector<char>& buf);
            Port& operator<<(char value);
            Port& operator>>(std::vector<char>& buf);
            

        protected:
            int _setName(const char* name);
            int _setName(const std::string name);
            int _setName(const wchar_t* name);
            int _setName(const std::wstring name);

        private:
            int __setCommName(const std::string name);
            

        
    };

};


#endif
