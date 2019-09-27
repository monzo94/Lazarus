#pragma once

#include <stdexcept>

#ifndef NDEBUG
#include <cstdio>
#define DEBUG(...) do { printf(__VA_ARGS__); printf("\n"); } while (0)
#else
#define DEBUG(...)
#endif

/**
 * Class for exceptions related to the Lazarus engine.
 * 
 * Meant only for internal use.
 */
namespace __lz
{    
class LazarusException : public std::exception
{
public:
    LazarusException(const std::string &msg)
        : msg(msg)
    {
    }

    virtual const char* what() const throw()
    {
        return msg.c_str();
    }

private:
    std::string msg;
};
}
