#include "image.h"

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>

const ARGBColor NO_COLOR = {0, 0, 0, 0};

Image read_image(const std::string &path) {
  std::ifstream in_file(path, std::ios::binary);

  if (!in_file.is_open()) {
    throw std::runtime_error("No such file");
  }

  // read header
  ImageHeader header;

  in_file.read((char *)(&header.width), sizeof(uint16_t));
  in_file.read((char *)(&header.height), sizeof(uint16_t));
  in_file.read((char *)(&header.bits_per_pixel), sizeof(uint8_t));
  in_file.read((char *)(&header.palette_size), sizeof(uint16_t));

  std::cout << "width = " << header.width << " height = " << header.height
            << " bpp = " << (int)header.bits_per_pixel
            << " p size = " << header.palette_size << std::endl;

  // TODO: little-big

  // read palette
  Palette palette;
  for (int i = 0; i < header.palette_size; i++) {
    ARGBColor color;
    in_file >> color.alpha >> color.red >> color.green >> color.blue;
    palette.push_back(color);
  }

  Pixels pixels;
  // read pixels by bits
  for (int y = 0; y < header.height; y++) {
    std::vector<ARGBColor> row;

    //
    for (int x = 0; x < header.width; x++) {
      uint8_t byte;
      in_file.read((char*)&byte, 1);

      if (byte & 0x80) {
        auto index = byte & 0x7F;

        if (index > palette.size()) {
          throw std::runtime_error("Pixel index is out from range");
        }

        row.push_back(palette[index - 1]);
      } else {
        /* byte
           |01101010|
           |-regrebl|
        */
        uint8_t red = (byte >> 5) & 0x3;
        uint8_t green = (byte >> 2) & 0x7;
        uint8_t blue = byte & 0x3;

        const uint8_t multiplier_2b = 255 / 3;
        const uint8_t multiplier_3b = 255 / 7;

        row.emplace_back(0xff, red * multiplier_2b, green * multiplier_3b,
                         blue * multiplier_2b);
      }
    }
    std::reverse(row.begin(), row.end());
    pixels.push_back(std::move(row));
  }

  return {.width = header.width,
          .height = header.height,
          .pixels = std::move(pixels)};
}
