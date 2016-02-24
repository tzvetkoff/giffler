/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <Latchezar Tzvetkoff> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Latchezar Tzvetkoff
 * ----------------------------------------------------------------------------
 */

/* pack_command.hpp */

#ifndef __giffler_pack_command__
#define __giffler_pack_command__

#include <iostream>
#include <list>

namespace giffler
{

class PackCommand
{

private:
    std::list<std::string> _in;
    bool _loop;
    float _delay;
    std::string _out;

public:
    PackCommand(int, char *const *);
    int execute();

};

}

#endif /* !defined(__giffler_pack_command__) */
