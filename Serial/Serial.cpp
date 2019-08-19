#include "StdAfx.h"
#include "Serial.hpp"

namespace Serial{

    //////////////////////////////////////////////////////////////////////////
    Port::Port(void) : _handle(INVALID_HANDLE_VALUE), _isNameInitialized(false) {    }

    Port::~Port(void) {
        if( this->_handle )
            this->close();
    }
    //////////////////////////////////////////////////////////////////////////



    // ctors /////////////////////////////////////////////////////////////////
    Port::Port(const Port& port) : _handle(port._handle), _isNameInitialized(port._isNameInitialized), name(port.name) {}

    Port::Port(const char* name): _handle(INVALID_HANDLE_VALUE), _isNameInitialized(false)  {
        this->_setName(name);
        this->open();
    }

    Port::Port(const char* name, Config &cfg): _handle(INVALID_HANDLE_VALUE), _isNameInitialized(false)  {
        this->_setName(name);
        this->open();
        this->setup(cfg);
    }

    Port::Port(const std::string name): _handle(INVALID_HANDLE_VALUE), _isNameInitialized(false)  {
        this->_setName(name);
        this->open();
    }

    Port::Port(const std::string name, Config &cfg): _handle(INVALID_HANDLE_VALUE), _isNameInitialized(false)  {
        this->_setName(name);
        this->open();
        this->setup(cfg);
    }

    Port::Port(const wchar_t* name): _handle(INVALID_HANDLE_VALUE), _isNameInitialized(false)  {
        this->_setName(name);
        this->open();
    }

    Port::Port(const wchar_t* name, Config &cfg): _handle(INVALID_HANDLE_VALUE), _isNameInitialized(false)  {
        this->_setName(name);
        this->open();
        this->setup(cfg);

    }

    Port::Port(const std::wstring name): _handle(INVALID_HANDLE_VALUE), _isNameInitialized(false)  {
        this->_setName(name);
        this->open();
    }

    Port::Port(const std::wstring name, Config &cfg): _handle(INVALID_HANDLE_VALUE), _isNameInitialized(false)  {
        this->_setName(name); 
        this->open();
        this->setup(cfg);
    }
    //////////////////////////////////////////////////////////////////////////
    


    // _setName //////////////////////////////////////////////////////////////

    int Port::_setName(const char* name){
        this->name = std::string(name);
        this->__setCommName(this->name);
        this->_isNameInitialized = true;
        return 0;
    };

    int Port::_setName(const std::string name){
        this->name = name;
        this->__setCommName(this->name);
        this->_isNameInitialized = true;
        return 0;
    };

    int Port::_setName(const wchar_t* name){
        std::wstring tmp(name);
        this->name = std::string(tmp.begin(), tmp.end());
        this->__setCommName(this->name);
        this->_isNameInitialized = true;
        return 0;
    };

    int Port::_setName(const std::wstring name){
        this->name = std::string(name.begin(), name.end());
        this->__setCommName(this->name);
        this->_isNameInitialized = true;
        return 0;
    };

    //////////////////////////////////////////////////////////////////////////

    int Port::__setCommName(const std::string name){
        std::wstring wname(name.begin(), name.end());
        this->__commName = std::wstring(L"\\.\\\\") + wname;
        return 0;
    }

    //////////////////////////////////////////////////////////////////////////

    bool Port::is_open(){
        return ( this->_handle != INVALID_HANDLE_VALUE );
    };

    //////////////////////////////////////////////////////////////////////////


    // opens /////////////////////////////////////////////////////////////////
    int Port::open(const wchar_t* name){
        this->_setName(name);
        return this->open();
    };

    int Port::open(const wchar_t* name, Config &cfg){
        this->open(name);
        this->setup(cfg);
        return 0; 
    };

    int Port::open(const std::wstring name){
        this->_setName(name);
        return this->open();
    };

    int Port::open(const std::wstring name, Config &cfg){
        this->open(name);
        this->setup(cfg);
        return 0; 
    };

    int Port::open(const char* name){
        this->_setName(name);
        return this->open();
    };

    int Port::open(const char* name, Config &cfg){
        this->open(name);
        this->setup(cfg);
        return 0; 
    };

    int Port::open(const std::string name){
        this->_setName(name);
        return this->open();
    };

    int Port::open(const std::string name, Config &cfg){
        this->open(name);
        this->setup(cfg);
        return 0; 
    };
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    // OPEN //////////////////////////////////////////////////////////////////
    int 
    Port::open(){

        if( !this->_isNameInitialized )
            return 1; 

        this->_handle = CreateFile((LPWSTR)(LPCTSTR)this->__commName.c_str(), 
		    GENERIC_READ | GENERIC_WRITE,
		    0,
		    NULL,
		    OPEN_EXISTING,
		    FILE_ATTRIBUTE_NORMAL,
		    NULL);

        DWORD err = GetLastError();

	    if( this->_handle == INVALID_HANDLE_VALUE ){
		    return 1;
	    }

        return 0;
    };

    // SETUP //////////////////////////////////////////////////////////////////
    int 
    Port::setup(Config &cfg){

        this->_config = cfg;

        *this << this->_config.dcb;
        *this << this->_config.cto;
        *this << this->_config.cqs;

        return 0;
    };

    // CLOSE //////////////////////////////////////////////////////////////////
    int 
    Port::close(){

        PurgeComm(this->_handle, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);

        if( this->is_open() ){
            CloseHandle(this->_handle);            
        }

        this->_handle = INVALID_HANDLE_VALUE;

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////


    // WRITE /////////////////////////////////////////////////////////////////
    unsigned int
    Port::write(std::vector<char>& buf) {

        unsigned int written=0;

	    if( this->is_open() ) 
            WriteFile(this->_handle, &buf[0], buf.size(), (LPDWORD)&written, NULL);
	
	    return written;

    }

    // READ //////////////////////////////////////////////////////////////////
    // Чтение производится буферами указанными при конифигурировании интерфейса;
    std::vector<char> 
    Port::read() {

        unsigned int readed=0;

        char *buffer = new char[this->_config.cqs.download];

        if( this->is_open() ) 
            ReadFile(this->_handle, buffer, this->_config.cqs.download, (LPDWORD)&readed, NULL);

        std::vector<char> result(buffer, buffer+readed);

        delete buffer;
	    
        return result;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    // OPERATOR << ///////////////////////////////////////////////////////////
    Port&
    Port::operator<<(Config& cfg) {

        this->setup(cfg);
        return *this;

    }

    Port&
    Port::operator>>(DCB& dcb) {

        //FIXME: Не возвращается статус выполнения операции 
        // Что если мы не проинициализируем значения;
        if( this->is_open() )
	        GetCommState(this->_handle,  &dcb);

        return *this;
    }

    Port&
    Port::operator<<(DCB& dcb) {

        //FIXME: Не возвращается статус выполнения операции 
        // Что если мы не проинициализируем значения;
        if( this->is_open() )
            this->_config.dcb = dcb;
	        SetCommState(this->_handle,  &dcb);

        return *this;
    }

    Port&
    Port::operator<<(COMMTIMEOUTS& cto) {

        //FIXME: Не возвращается статус выполнения операции 
        // Что если мы не проинициализируем значения;
        if( this->is_open() )
            this->_config.cto = cto;
            SetCommTimeouts(this->_handle,  &cto);

        return *this;
    }

    Port&
    Port::operator<<(COMMQUEUESIZE& cqs) {

        //FIXME: Не возвращается статус выполнения операции 
        // Что если мы не проинициализируем значения;
        if( this->is_open() )
            this->_config.cqs = cqs;
            SetupComm(this->_handle,  cqs.download, cqs.upload);

        return *this;
    }


    Port&
    Port::operator<<(char value) {

        std::vector<char> buf;
        buf.push_back(value);
        this->write(buf);
        return *this;

    }

    Port& 
    Port::operator<<(std::vector<char>& buf) {

        this->write(buf);
        return *this;

    }

    Port& 
    Port::operator>>(std::vector<char>& buf) {

        buf = this->read();
        return *this;

    }

}