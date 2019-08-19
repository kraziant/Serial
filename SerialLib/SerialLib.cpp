// SerialLib.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


#include <iostream>
#include <vector>

#include "Serial.hpp"

#include "helpers.hpp"


// main //////////////////////////////////////////////////////////////////////
int
real_main()
{

    // Тестовые данные ///////////////////////////////////////////////////////
    char dump[] = {1,2,3,4,5,6,7,8,9,10};
    std::vector<char> buf(dump, dump+(sizeof(dump)/sizeof(char)));


    // Порт //////////////////////////////////////////////////////////////////
    // Специальный тип Serial::Port. Имеет ряд конструкторов, позволяющих 
    // копировать объект, создавать объект и открывать порт (наподобии ifstream)
    //    Port();
    //    Port(const Port& port);
    //    Port(const char* name);
    //    Port(const char* name, Config &cfg);
    //    Port(const wchar_t* name);
    //    Port(const wchar_t* name, Config &cfg);
    //    Port(const std::string name);
    //    Port(const std::string name, Config &cfg);
    //    Port(const std::wstring name);
    //    Port(const std::wstring name, Config &cfg);
    // понимает в качестве аргументов std::string, std::wstring, char*, wchar*
    // объект можно создать сразу с конфигурацие, тогда он будет создан, открыт
    // и сконфигурирован конструктором.

    Serial::Port p("COM3");
       
    if( !p.is_open() ){
        say(p.name << " port is not opened");
        std::cout << "Port " << p.name << " is not opened" << std::endl;
        return 0;
    }


    // Конфигурирование интерфейса ///////////////////////////////////////////
    // Специальный тип Serial::Config включает в себя структуры Win32Api:
    //   - DCB -- baudRate, Parity, StopBit, ...
    //   - COMMTIMEOUTS -- задержки между байтами при чтении/записи
    // и структуру для описания размеров очередей
    //   - COMMQUEUESIZE -- размеры очередей приема и отправики данных
    //
    // Можно конфигурировать скопом и поотдельности.
    //    Port& operator>>(DCB& dcb); -- можно прочитать текущую DCB (GetCommState)
    //
    //    Port& operator<<(Config& cfg);    
    //    Port& operator<<(DCB& dcb);
    //    Port& operator<<(COMMTIMEOUTS& cto);
    //    Port& operator<<(COMMQUEUESIZE& cqs);
    //

    Serial::Config cfg;
    p >> cfg.dcb;

    cfg.dcb.BaudRate=BAUD_115200;
    cfg.dcb.StopBits=ONESTOPBIT;
    cfg.dcb.Parity=NOPARITY;
    cfg.dcb.ByteSize=8;

    p << cfg;       // можно через оператор << можно через метод p.setup(cfg);

    // Можно переконфигурировать///////////////////////////////////////////////
    // cfg.cto.ReadIntervalTimeout=10;
    // cfg.cto.ReadTotalTimeoutConstant=10;        
    // p << cfg;

    // Запись и чтение данных ////////////////////////////////////////////////
    // Можем передавать std::vector<char> и char.
    //    unsigned int write(std::vector<char>& buf);
    //    Port& operator<<(std::vector<char>& buf);
    //    Port& operator<<(char value);
    //
    // Читаем все в std::vector<char>
    //    std::vector<char> read();
    //    Port& operator>>(std::vector<char>& buf);
    // Количество читаемых байт определяется cfg.cqs.download;
    

    p << buf;                           //p.write(buf)    
	p << 0xCA << 0xFE << 0xBA << 0xBE;  // warning C4309
    p << (char)0xCA << (char)0xFE << (char)0xBA << (char)0xBE;
    

    // ПРИМЕРЫ ///////////////////////////////////////////////////////////////
    // echo //////////////////////////////////////////////////////////////////
    // while(1){
    //     p << (char)getch();
    // }

    // loop back /////////////////////////////////////////////////////////////
    buf.clear();
    while(1){
        
        p >> buf;

        if(buf.size()){
            
            p << buf;

            std::string msg(buf.begin(), buf.end());
            std::cout <<  msg;
            
            if(msg.find("quit") != std::string::npos) break;

        }
        else
            Sleep(200);
    }

    return 0;
}
//////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
   
    setlocale(LC_ALL, "Russian");

    real_main();

    //anykey();
    
	return 0;
}

