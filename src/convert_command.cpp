/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <Latchezar Tzvetkoff> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Latchezar Tzvetkoff
 * ----------------------------------------------------------------------------
 */

/* convert_command.cpp */

#include "config.hpp"

#include <iostream>
#include <iomanip>
#include <list>
#include <map>
#include <Magick++.h>
#include <getopt.h>

#include "ext.hpp"
#include "convert_command.hpp"

extern const char *program_name;
void
print_usage(std::ostream& out, int exit_code);

giffler::ConvertCommand::ConvertCommand(int argc, char *const *argv) {
    int ch;
    const struct option longopts[] = {
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'v'},
        {"extract", no_argument, NULL, 'x'},
        {"full", no_argument, NULL, 'f'},
        {"max-frames", required_argument, NULL, 'm'},
        {"out", required_argument, NULL, 'o'},
        {NULL, 0, NULL, 0}
    };

    /* initialize defaults */
    _extract = false;
    _full = false;
    _max_frames = 16;
    _out = "./frame.gif";

    /* parse command line options */
    while ((ch = getopt_long(argc, argv, "hvxfm:o:", longopts, NULL)) != -1) {
        switch (ch) {
            case 'h':
                print_usage(std::cout, 0);
                break;
            case 'v':
                std::cout << GIFFLER_VERSION_STRING << std::endl;
                exit(0);

            case 'x':
                _extract = true;
                break;

            case 'f':
                _full = true;
                break;

            case 'm':
                _max_frames = atoi(optarg);
                break;

            case 'o':
                _out = optarg;
                break;

            default:
                std::cerr << std::endl;
                print_usage(std::cerr, 1);
        }
    }

    /* if we're in extract mode, make _out a directory */
    if (_extract) {
        if (_out == "./frame.gif") {
            _out = "./";
        }
    } else if (!ends_with(_out, std::string(".gif"))) {
        _out += ".gif";
    }

    /* check for 1 positional argument */
    if (argc - optind != 2) {
        std::cerr << PACKAGE_NAME << ": convert:"
            " missing required argument" << std::endl;
        std::cerr << std::endl;
        print_usage(std::cerr, 1);
    }

    _in = argv[optind + 1];
}

int
giffler::ConvertCommand::execute()
{
    Magick::Image image;

    try {
        image.read(_in);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    image.matte(false);
    image.modifyImage();
    int width = image.columns();
    int height = image.rows();

    if (_full) {
        return _execute_full(image, width, height);
    } else {
        return _execute_fast(image, width, height);
    }
}

int
giffler::ConvertCommand::_execute_full(Magick::Image& image,
    int width, int height)
{
    if (_max_frames < 2) {
        /* well, nothing to do here... */
        image.write(_out);
        return 0;
    }

    /* extract unique colors */
    std::list<Magick::Color> colors;
    std::map<Magick::Color, int> color_map;
    Magick::PixelPacket *pixels = image.getPixels(0, 0, width, height);

    std::list<std::pair<Magick::Color, size_t> > histogram;
    Magick::colorHistogram(&histogram, image);

    for (std::list<std::pair<Magick::Color, size_t> >::iterator
        it = histogram.begin(), end = histogram.end(); it != end; ++it) {
        Magick::Color& color = it->first;
        colors.push_back(it->first);
        color_map[it->first] = it->second;
    }

    if (colors.empty()) {
        std::cerr << "!?" << std::endl;
        return 1;
    }

    /* generate first frame - no matte, quantized to 256 colors */
    int frame_index = 0;
    std::list<Magick::Image> frames;
    Magick::Image frame(image, "white");
    frame.magick("GIF");
    frame.quantizeColors(256);
    frame.quantize();
    frame.matte(false);

    /* remove the first frame colors from the other */
    Magick::colorHistogram(&histogram, frame);

    for (std::list<std::pair<Magick::Color, size_t> >::iterator
        it = histogram.begin(), end = histogram.end(); it != end; ++it) {
        Magick::Color& color = it->first;
        colors.remove(it->first);
    }

    /* print some info */
    std::cerr << "INFO: Frame #"
        << std::setw(4) << std::setfill('0') << frame_index
        << std::setw(0) << " (" << frame.totalColors()
        << " colors)" << std::endl;

    if (_extract) {
        /* extract the frame */
        char *out;
        asprintf(&out, "%s/frame_%04d.gif", _out.c_str(), frame_index);
        frame.write(out);
        free(out);
    } else {
        /* push the first frame */
        frames.push_back(frame);
    }
    ++frame_index;

    /* do other frames */
    color_map.clear();
    Magick::Color transparent("transparent");

    do {
        if (colors.empty() && color_map.empty()) {
            /* nothing more to do */
            break;
        }

        if (!colors.empty()) {
            /* move a color to the palette map */
            Magick::Color& color = colors.front();
            color_map[color] = true;
            colors.pop_front();
        }

        if (color_map.size() == 255 || colors.empty()) {
            /* remove all colors outside the palette */
            frame = Magick::Image(image, "transparent");
            frame.matte(true);
            frame.backgroundColor(transparent);
            pixels = frame.getPixels(0, 0, width, height);

            for (int i = 0; i < width * height; ++i) {
                const Magick::Color& color = pixels[i];
                std::map<Magick::Color, int>::iterator it =
                    color_map.find(color);

                if (it == color_map.end()) {
                    pixels[i] = transparent;
                }
            }

            /* print some info */
            std::cerr << "INFO: Frame #"
                << std::setw(4) << std::setfill('0') << frame_index
                << std::setw(0) << " (" << frame.totalColors()
                << " colors)" << std::endl;

            if (_extract) {
                /* extract the frame */
                char *out;
                asprintf(&out, "%s/frame_%04d.gif", _out.c_str(), frame_index);
                frame.write(out);
                free(out);
            } else {
                /* push the frame */
                frames.push_back(frame);
            }

            ++frame_index;
            color_map.clear();
        }
    } while (true);

    if (!_extract) {
        /* kill delay */
        std::for_each(frames.begin(), frames.end(),
            Magick::animationDelayImage(2));
        /* kill loop */
        std::for_each(frames.begin(), frames.end(),
            Magick::animationIterationsImage(1));

        /* write the resulting gif */
        Magick::writeImages(frames.begin(), frames.end(), _out);
    }

    /* hooray! */
    return 0;
}

int
giffler::ConvertCommand::_execute_fast(Magick::Image& image,
    int width, int height)
{
    if (_max_frames < 2) {
        /* well, nothing to do here... */
        image.write(_out);
        return 0;
    }

    /* reduce the colors */
    int max_colors = _max_frames * 255;

    if (max_colors > image.totalColors()) {
        max_colors = (image.totalColors() / 256) * 256;
    }

    image.quantizeDither(false);
    image.quantizeColors(max_colors);
    image.quantize();
    std::cerr << "INFO: Quantized to " << image.totalColors() <<
        " colors (requested max: " << max_colors << ")" << std::endl;

    /* reduce normally */
    return _execute_full(image, width, height);
}
