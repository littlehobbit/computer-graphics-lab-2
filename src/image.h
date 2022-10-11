#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

struct ARGBColor {
  ARGBColor() = default;
  ARGBColor(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
      : alpha(a), red(r), green(g), blue(b) {}

  uint8_t alpha;
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

using Palette = std::vector<ARGBColor>;

using Pixels = std::vector<std::vector<ARGBColor>>;

struct ImageHeader {
  uint16_t width;
  uint16_t height;
  uint8_t bits_per_pixel;
  uint16_t palette_size;
};

struct Image {
  size_t width;
  size_t height;
  Pixels pixels;
};

Image read_image(const std::string &path);

#endif // __IMAGE_H__