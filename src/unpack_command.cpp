/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <Latchezar Tzvetkoff> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Latchezar Tzvetkoff
 * ----------------------------------------------------------------------------
 */

/* unpack_command.cpp */

#include "config.hpp"

#include <iostream>
#include <list>
#include <Magick++.h>
#include <getopt.h>

#include "ext.hpp"
#include "unpack_command.hpp"

extern const char *program_name;
void
print_usage(std::ostream& out, int exit_code);

giffler::UnpackCommand::UnpackCommand(int argc, char *const *argv) {
    int ch;
    const struct option longopts[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"outdir", required_argument, NULL, 'o'},
        {NULL, 0, NULL, 0}
    };

    /* initialize defaults */
    _out = "./";

    /* parse command line options */
    while ((ch = getopt_long(argc, argv, "hvo:", longopts, NULL)) != -1) {
        switch (ch) {
            case 'h':
                print_usage(std::cout, 0);
                break;
            case 'v':
                std::cout << GIFFLER_VERSION_STRING << std::endl;
                exit(0);

            case 'o':
                _out = std::string(optarg);
                break;

            default:
                std::cerr << std::endl;
                print_usage(std::cerr, 1);
        }
    }

    /* check for 1 positional argument */
    if (argc - optind != 2) {
        std::cerr << PACKAGE_NAME << ": unpack:"
            " missing required argument" << std::endl;
        std::cerr << std::endl;
        print_usage(std::cerr, 1);
    }

    _in = argv[optind + 1];
}

int
giffler::UnpackCommand::execute()
{
    std::list<Magick::Image> _frames;

    /* load the image */
    try {
        Magick::readImages(&_frames, _in);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    /* iterate over each frame and write it */
    int frame_index = 0;
    for (std::list<Magick::Image>::iterator it = _frames.begin(),
        end = _frames.end(); it != end; ++it) {
        Magick::Image& frame = *it;
        char *out;
        asprintf(&out, "%s/frame_%04d.gif", _out.c_str(), frame_index++);
        frame.write(out);
    }

    /* hooray! */
    return 0;
}
