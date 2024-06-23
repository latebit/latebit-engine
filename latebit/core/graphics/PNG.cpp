#include <png.h>

#include <array>
#include <cstdio>
#include <string>

#include "Colors.h"

using namespace std;

namespace lb {

class PNGDecoder {
 public:
  PNGDecoder(string filename) {
    this->file = std::fopen(filename.c_str(), "rb");
    this->read =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    this->info = png_create_info_struct(read);
  }

  // Returns false if the file could not be opened
  auto canOpenFile() -> bool { return !!file; }

  // Reads the header of the file and check if it's really a PNG
  auto isPNGFile() -> bool {
    array<char, 8> header;
    size_t size = fread(header.data(), 1, 8, file);
    if (size != 8) return false;
    return !png_sig_cmp(reinterpret_cast<png_const_bytep>(header.data()), 0,
                        size);
  }

  // Returns false if memory for this PNG could not be allocated
  auto canAllocateMemory() -> bool { return !!read && !!info; }

  // Decodes the PNG file by populating the read and info structs
  auto decode() -> int {
    if (!read || !file || !info) {
      return -1;
    }

    // Load the PNG file into the read struct
    png_init_io(read, file);
    // Offset 8 bytes to skip the header (since we have read it before)
    png_set_sig_bytes(read, 8);
    // Read the PNG info from the PNG struct
    png_read_info(read, info);

    // Set width and height of the image
    width = png_get_image_width(read, info);
    height = png_get_image_height(read, info);

    return 0;
  }

  // Checks if the PNG file is a 16-colors PNG
  auto is16ColorsPNG() -> bool {
    if (!read || !info) return false;

    if (png_get_color_type(read, info) != PNG_COLOR_TYPE_PALETTE) return false;

    int colorsInPalette;

    // Cannot extract palette info, we cannot assume it's correct
    if (png_get_PLTE(read, info, &palette, &colorsInPalette) != PNG_INFO_PLTE)
      return false;

    return colorsInPalette == 16;
  }

  // Reads the image data and the transparency data from the PNG file
  // and stores it locally
  auto readImage() -> int {
    if (!read || !info) return -1;

    // Allocate and read all the colored pixels on the grid
    image = (png_bytep*)malloc(sizeof(png_bytep) * height);

    if (!image) return -1;

    for (int y = 0; y < height; y++) {
      image[y] = (png_byte*)malloc(png_get_rowbytes(read, info));
      if (!image[y]) return -1;
    }

    png_read_image(read, image);

    // Allocate and read all the transparent entries on the grid
    png_get_tRNS(read, info, &transparentEntries, &transparentEntriesCount,
                 &transparencyColor);

    if (!transparentEntries) return -1;

    return 0;
  }

  // Returns the color of the cell at (x, y) position
  auto getCellColor(int x, int y) -> Color::Color {
    if (y >= height) return Color::UNDEFINED_COLOR;
    if (x >= width) return Color::UNDEFINED_COLOR;

    auto index = image[y][x];

    if (index < transparentEntriesCount && transparentEntries[index] == 0) {
      return Color::UNDEFINED_COLOR;
    }

    return (Color::Color)index;
  }

  // Returns the width of the image
  auto getWidth() -> int { return width; }

  // Returns the height of the image
  auto getHeight() -> int { return height; }

  ~PNGDecoder() {
    if (this->image != nullptr) {
      for (int y = 0; y < this->height; y++) {
        free(this->image[y]);
      }
      free(this->image);
    }

    png_destroy_read_struct(&this->read, &this->info, nullptr);
    if (this->file != nullptr) {
      std::fclose(file);
      this->file = nullptr;
    }
  }

 private:
  FILE* file = nullptr;
  png_structp read = nullptr;
  png_infop info = nullptr;
  png_colorp palette = nullptr;  // managed by libpng

  int width = 0;
  int height = 0;

  png_bytep* image = nullptr;
  png_bytep transparentEntries = nullptr;  // managed by libpng
  int transparentEntriesCount = 0;
  png_color_16p transparencyColor = nullptr;  // managed by libpng
};

}  // namespace lb