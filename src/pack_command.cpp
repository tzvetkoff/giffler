/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <Latchezar Tzvetkoff> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Latchezar Tzvetkoff
 * ----------------------------------------------------------------------------
 */

/* pack_command.cpp */

#include "config.hpp"

#include <iostream>
#include <list>
#include <Magick++.h>
#include <getopt.h>

#include "ext.hpp"
#include "pack_command.hpp"

extern const char *program_name;
void
print_usage(std::ostream& out, int exit_code);

giffler::PackCommand::PackCommand(int argc, char *const *argv) {
    int ch;
    const struct option longopts[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"loop", no_argument, NULL, 'l'},
        {"delay", required_argument, NULL, 'd'},
        {"out", required_argument, NULL, 'o'},
        {NULL, 0, NULL, 0}
    };

    /* initialize defaults */
    _loop = false;
    _delay = 0.3f;
    _out = "./anim.gif";

    /* parse command line options */
    while ((ch = getopt_long(argc, argv, "hvld:o:", longopts, NULL)) != -1) {
        switch (ch) {
            case 'h':
                print_usage(std::cout, 0);
                break;
            case 'v':
                std::cout << GIFFLER_VERSION_STRING << std::endl;
                exit(0);

            case 'l':
                _loop = true;
                break;

            case 'd':
                _delay = atof(optarg);
                break;

            case 'o':
                _out = optarg;
                break;

            default:
                std::cerr << std::endl;
                print_usage(std::cerr, 1);
        }
    }

    /* check for at least 1 positional argument */
    if (argc - optind < 2) {
        std::cerr << PACKAGE_NAME << ": pack:"
            " missing required argument" << std::endl;
        std::cerr << std::endl;
        print_usage(std::cerr, 1);
    }

    /* ensure .gif extension */
    if (!ends_with(_out, std::string(".gif"))) {
        _out += ".gif";
    }

    /* store input images */
    for (ch = optind + 1; ch < argc; ++ch) {
        _in.push_back(argv[ch]);
    }
}

int
giffler::PackCommand::execute()
{
    /* read each frame and push it to the list */
    std::list<Magick::Image> frames;

    for (std::list<std::string>::iterator it = _in.begin(), end = _in.end();
        it != end; ++it) {
        Magick::Image frame;

        try {
            frame.read(*it);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }

        frames.push_back(frame);
    }

    /* set delay and loop */
    std::for_each(frames.begin(), frames.end(),
        Magick::animationDelayImage(_delay * 100));
    std::for_each(frames.begin(), frames.end(),
        Magick::animationIterationsImage(_loop ? 0 : 1));

    /* write output */
    try {
        Magick::writeImages(frames.begin(), frames.end(), _out);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    /* hooray! */
    return 0;
}
