#include "image.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

struct PixelReader {
  PixelReader(std::ifstream &stream, uint8_t bits_per_pixel)
      : stream(stream), pixel_mask(~(0x1 << bits_per_pixel)),
        bits_per_pixel(bits_per_pixel) {}

  uint8_t read() {
    if (total_bits_readed % 8 == 0) {
      // read new byte
      stream >> total_byte;
      byte_end += 8;
    }

    if (total_bits_readed + bits_per_pixel > byte_end) {

    } else {
      auto shift = 8 - (total_bits_readed + bits_per_pixel) % 8;
    }
  }

  std::ifstream &stream;
  uint8_t total_byte = 0;
  uint8_t pixel_mask = 0;
  uint8_t bits_per_pixel = 0;
  uint8_t byte_end = 8;
  uint8_t total_bits_readed = 0;
};

const ARGBColor NO_COLOR = {0, 0, 0, 0};

Image read_image(const std::string &path) {
  std::ifstream in_file(path, std::ios::binary);

  if (!in_file.is_open()) {
    throw std::runtime_error("No such file");
  }

  // read header
  ImageHeader header;
  in_file >> header.width >> header.height >> header.bits_per_pixel >>
      header.palette_size;

  // TODO: check pixel size < 4bit

  // read palette
  Palette palette;
  for (int i = 0; i < header.palette_size; i++) {
    ARGBColor color;
    in_file >> color.alpha >> color.red >> color.green >> color.blue;
  }

  Pixels pixels;
  // read pixels by bits
  for (int y = 0; y < header.height; y++) {
    std::vector<ARGBColor> row;

    //
    for (int x = 0; x < header.width; x++) {
      // variant 3
      // if (byte & 0x80) {
      // do index pixel
      // } else {
      // do color pixel
      // }
    }
    std::reverse(row.begin(), row.end());
    pixels.push_back(std::move(row));
  }

  return {.header = header, .pixels = std::move(pixels)};
}
