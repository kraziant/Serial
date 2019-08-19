#include "StdAfx.h"
#include "Serial.hpp"

namespace Serial
{

    Config::Config(void)
    {
	    this->dcb.BaudRate = 115200;
	    this->dcb.Parity   = 0;
	    this->dcb.StopBits = 0;
	    this->dcb.ByteSize = 8;

	    
	    this->cto.ReadIntervalTimeout = 0xFFFFFFFF;
	    this->cto.ReadTotalTimeoutMultiplier = 0;
	    this->cto.ReadTotalTimeoutConstant = 0;
	    this->cto.WriteTotalTimeoutConstant = 1500;
	    this->cto.WriteTotalTimeoutMultiplier = 0;

	    this->cqs.download = 1024;
		this->cqs.upload   = 1024;
    }


    Config::~Config(void)
    {
    }

}