#include "core/graphics/Font.h"

#include "../lib/test.h"

using namespace lb;

void Font_test() {
  test("getGlyph", []() {
    Font font = DEFAULT_FONT;

    const Glyph& AGlyph = font.getGlyph('A');
    assert("glyph is retrieved", AGlyph == font.getGlyphs()[33]);

    const Glyph& lowGlyph = font.getGlyph(0);
    assert("returns empty glyph when out of bounds (lower)",
           lowGlyph == font.getGlyphs()[0]);

    const Glyph& hiGlyph = font.getGlyph(127);
    assert("returns empty glyph when out of bounds (lower)",
           hiGlyph == font.getGlyphs()[0]);
  });

  test("constructor", []() {
    GlyphList glyphs = {};
    Font font = Font(glyphs, 2, 3, 1);
    assert("font is created", font.getGlyphs() == glyphs);
    assert_int("glyph width is set", font.getGlyphWidth(), 2);
    assert_int("glyph height is set", font.getGlyphHeight(), 3);
    assert_int("horizontal spacing is set", font.getHorizontalSpacing(), 1);

    font = Font(glyphs, 10, -1, -10);
    assert_int("glyph width is clamped to 8", font.getGlyphWidth(), 8);
    assert_int("glyph height is clamped to 1", font.getGlyphHeight(), 1);
    assert_int("spacing is clamped to 1", font.getHorizontalSpacing(), 1);
  });
}