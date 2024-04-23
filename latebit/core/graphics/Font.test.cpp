#include "latebit/core/graphics/Font.h"

#include "../../../test/lib/test.h"

using namespace lb;

auto main() -> int {
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
    assertEq("glyph width is set", font.getGlyphWidth(), 2);
    assertEq("glyph height is set", font.getGlyphHeight(), 3);
    assertEq("horizontal spacing is set", font.getHorizontalSpacing(), 1);

    font = Font(glyphs, 10, -1, -10);
    assertEq("glyph width is clamped to 8", font.getGlyphWidth(), 8);
    assertEq("glyph height is clamped to 1", font.getGlyphHeight(), 1);
    assertEq("spacing is clamped to 1", font.getHorizontalSpacing(), 1);
  });

  return report();
}