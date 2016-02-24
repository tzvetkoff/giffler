/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <Latchezar Tzvetkoff> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Latchezar Tzvetkoff
 * ----------------------------------------------------------------------------
 */

/* ext.hpp */

#ifndef __giffler_ext__
#define __giffler_ext__

namespace giffler
{

template<typename T>
bool starts_with(const T& something, const T& prefix)
{
    return something.compare(0, prefix.length(), prefix) == 0;
}

template<typename T>
bool ends_with(const T& something, const T& suffix)
{
    return something.length() >= suffix.length() &&
        something.compare(something.length() - suffix.length(), suffix.length(),
            suffix) == 0;
}

}


#endif /* !defined(__giffler_ext__) */
