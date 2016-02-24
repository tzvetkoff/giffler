/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <Latchezar Tzvetkoff> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Latchezar Tzvetkoff
 * ----------------------------------------------------------------------------
 */

/* main.cpp */

#include "config.hpp"

#include <iostream>
#include <Magick++.h>

#include "pack_command.hpp"
#include "unpack_command.hpp"
#include "convert_command.hpp"

const char *program_name;

void
print_usage(std::ostream& out, int exit_code)
{
    out << PACKAGE_NAME << " " GIFFLER_VERSION_STRING << std::endl;
    out << std::endl;
    out << "Pack or unpack GIFs with ease" << std::endl;
    out << "Copyright (C) 2016 Latchezar Tzvetkoff" << std::endl;
    out << "Distributed under The Beerware License" << std::endl;
    out << std::endl;
    out << "Usage:" << std::endl;
    out << "  " << program_name << " <command> [options] <args>" << std::endl;
    out << std::endl;
    out << "Commands:" << std::endl;
    out << std::endl;

    out << "  help                              Print this message"
        << std::endl;
    out << "  version                           Print version information"
        << std::endl;
    out << std::endl;

    out << "  pack [options] <file0> [file1]    Pack multiple frames into a GIF"
        << std::endl;
    out << "    -l, --loop                        Loop the animation"
        " indefinitely" << std::endl;
    out << "    -d, --delay=D                     Set frame delay"
        " (default: 0.5s)" << std::endl;
    out << "    -o, --out=O                       Set output file name"
        " (default: anim.gif)" << std::endl;
    out << std::endl;

    out << "  unpack [options] <file>          Unpack a GIF to frames"
        << std::endl;
    out << "    -o, --out=O                      Set output directory"
        " (default: ./)" << std::endl;
    out << std::endl;

    out << "  convert [options] <file>         Convert an image to a multi-"
        "frame GIF with more than 256 colors" << std::endl;
    out << "    -x, --extract                    Extract frames to directory"
        " (saves memory, you can then use `pack' to do the hack)" << std::endl;
    out << "    -f, --full                       Write all colors, don't "
        " quantize to max-frames*255" << std::endl;
    out << "    -m, --max-frames=M               Quantize to M*255 colors" <<
        std::endl;
    out << "    -o, --out=O                      Set output file name or"
        " directory (default: frame.gif or ./)" << std::endl;
    out << std::endl;

    out << "Project homepage: " GIFFLER_PROJECT_URL << std::endl;
    out << "Report bugs to " GIFFLER_BUGREPORT_URL << std::endl;

    exit(exit_code);
}

/* mine */
int main(int argc, char *const *argv)
{
    /* set global program invocation name */
    program_name = argv[0];

    /* initialize magic */
    Magick::InitializeMagick(argv[0]);

    /* check sub-command */
    if (argc < 2) {
        print_usage(std::cerr, 1);
    }

    /* do what we gotta do */
    if (strcmp(argv[1], "help") == 0 ||
        strcmp(argv[1], "-help") == 0 ||
        strcmp(argv[1], "--help") == 0 ||
        strcmp(argv[1], "-h") == 0) {
        print_usage(std::cout, 0);
    }

    if (strcmp(argv[1], "version") == 0 ||
        strcmp(argv[1], "-version") == 0 ||
        strcmp(argv[1], "--version") == 0 ||
        strcmp(argv[1], "-v") == 0) {
        std::cout << GIFFLER_VERSION_STRING << std::endl;
        return 0;
    }

    if (strcmp(argv[1], "pack") == 0) {
        return giffler::PackCommand(argc, argv).execute();
    }

    if (strcmp(argv[1], "unpack") == 0) {
        return giffler::UnpackCommand(argc, argv).execute();
    }

    if (strcmp(argv[1], "convert") == 0) {
        return giffler::ConvertCommand(argc, argv).execute();
    }

    std::cerr << "unknown command: " << argv[1] << std::endl;
    std::cerr << std::endl;

    print_usage(std::cerr, 1);
    return 1;
}
