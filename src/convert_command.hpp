/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <Latchezar Tzvetkoff> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Latchezar Tzvetkoff
 * ----------------------------------------------------------------------------
 */

/* convert_command.hpp */

#ifndef __giffler_convert_command__
#define __giffler_convert_command__

#include <iostream>

namespace giffler
{

class ConvertCommand
{

private:
    std::string _in;
    bool _extract;
    bool _full;
    int _max_frames;
    std::string _out;

    int _execute_full(Magick::Image&, int, int);
    int _execute_fast(Magick::Image&, int, int);

public:
    ConvertCommand(int, char *const *);
    int execute();

};

}

#endif /* !defined(__giffler_convert_command__) */
