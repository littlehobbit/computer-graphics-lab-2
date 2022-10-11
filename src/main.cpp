#include "CLI/App.hpp"
#include "CLI/Validators.hpp"
#include "image.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Window.hpp>
#include <fstream>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Window.hpp>

#include <CLI/CLI.hpp>

#include <iterator>
#include <unordered_set>

void write_lab_file(const sf::Image &image, const std::string path);

int main(int argc, char *argv[]) {

  CLI::App app("Image shower & converter", "lab-2");

  std::string image_path;
  bool convert_to_bmp = false;
  bool convert_to_lab = false;

  app.add_flag("--to-lab", convert_to_lab);
  app.add_flag("--to-bmp", convert_to_bmp);
  app.add_option("-i", image_path)->check(CLI::ExistingPath);

  CLI11_PARSE(app, argc, argv);

  sf::Image output_image;

  if (image_path.substr(image_path.size() - 3, 3) == "lab") {
    // read lab file
    auto image = read_image(image_path);
    std::cout << "height = " << image.height << " width = " << image.width
              << std::endl;

    // load lab file to image
    output_image.create(image.width, image.height);

    for (int y = 0; y < image.height; y++) {
      for (int x = 0; x < image.width; x++) {
        const auto &pixel = image.pixels[y][x];
        sf::Color color(pixel.red, pixel.blue, pixel.green, pixel.alpha);
        output_image.setPixel(x, y, color);
      }
    }

  } else {
    // read by Image
    output_image.loadFromFile(image_path);
  }

  if (convert_to_bmp) {
    // convert and write bmp
    auto bmp_format = image_path.substr(0, image_path.size() - 3) + "bmp";
    output_image.saveToFile(bmp_format);
  }

  if (convert_to_lab) {
    // convert to lab format
    write_lab_file(output_image, image_path);
  }

  sf::RenderWindow window(
      sf::VideoMode(output_image.getSize().x, output_image.getSize().y),
      "lab-2");

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    sf::Texture texture;
    texture.loadFromImage(output_image);
    sf::Sprite sprite;
    sprite.setTexture(texture, true);

    window.draw(sprite);
    window.display();
  }
}

namespace std {
bool operator<(const sf::Color &lhs, const sf::Color &rhs) {
  return lhs.toInteger() < rhs.toInteger();
}
} // namespace std

void write_lab_file(const sf::Image &image, const std::string path) {
  std::vector<std::vector<uint8_t>> pixels;

  auto size = image.getSize();
  for (int y = 1; y <= size.y; y++) {
    std::vector<uint8_t> row;
    for (int x = 1; x <= size.x; x++) {
      auto color = image.getPixel(x, y);

      uint8_t new_pixel = 0;
      new_pixel |= ((color.r / 64) & 0x3) << 5;
      new_pixel |= ((color.g / 32) & 0x7) << 2;
      new_pixel |= (color.b / 64) & 0x3;

      row.push_back(new_pixel);
    }
    std::reverse(row.begin(), row.end());
    pixels.push_back(std::move(row));
  }

  ImageHeader header;
  header.width = size.x;
  header.height = size.y;
  header.bits_per_pixel = 8;
  header.palette_size = 0;

  auto new_path = path.substr(0, path.size() - 3) + "lab";
  std::ofstream out(new_path, std::ios::binary | std::ios::out);

  out.write((char *)&header.width, 2);
  out.write((char *)&header.height, 2);
  out.write((char *)&header.bits_per_pixel, 1);
  out.write((char *)&header.palette_size, 2);

  for (auto &row : pixels) {
    for (auto &pixel : row) {
      out.write((char *)&pixel, 1);
    }
  }

  std::cout << "write file width=" << header.width
            << " height=" << header.height << std::endl;
}