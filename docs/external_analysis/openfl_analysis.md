# OpenFL Feature Analysis for afterhours

## Overview

[OpenFL](https://www.openfl.org/learn/features/) is an open-source implementation of
the Flash/ActionScript 3.0 display API, targeting native (C++/OpenGL), HTML5, and other
backends. It provides a comprehensive 2D graphics pipeline, rich text system, display
list (scene graph), event model, filters, blend modes, and bitmap manipulation — all
originally designed for interactive multimedia and game development.

This document inventories OpenFL's features, compares them with `afterhours`' current
drawing and text APIs, and identifies specific gaps and adoptable patterns.

---

## 1. OpenFL 2D Graphics (Drawing API)

OpenFL's `Graphics` class is a **stateful vector drawing API** following the Flash
model: you set a fill/line style, then draw shapes, and the system fills/strokes them
when you call `endFill()` or start a new fill.

### 1.1 Fill Styles

| Feature | OpenFL | afterhours |
|---------|--------|------------|
| Solid color fill | `beginFill(color, alpha)` | `draw_rectangle(rect, color)` — solid only |
| **Gradient fill (linear)** | `beginGradientFill(LINEAR, colors[], alphas[], ratios[], matrix)` | **Missing** |
| **Gradient fill (radial)** | `beginGradientFill(RADIAL, ...)` with focal point control | **Missing** |
| **Bitmap/texture fill** | `beginBitmapFill(bitmap, matrix, repeat, smooth)` | **Missing** — only NPatch/sprite fills |
| **Shader fill** | `beginShaderFill(shader, matrix)` — custom GPU shader as fill | **Missing** |
| Gradient spread modes | PAD, REFLECT, REPEAT | **Missing** |
| Gradient interpolation | LINEAR_RGB vs RGB color space | **Missing** |
| Fill applies to all shapes | Yes — fill persists until `endFill()` or new `begin*Fill()` | No — each draw call takes its own color |

### 1.2 Line/Stroke Styles

| Feature | OpenFL | afterhours |
|---------|--------|------------|
| Line thickness | `lineStyle(thickness, ...)` 0–255 px | `draw_line_ex(start, end, thickness, color)` |
| Line color + alpha | `lineStyle(thickness, color, alpha)` | Color only (alpha via color.a) |
| **Line caps** | None, Round, Square (`CapsStyle`) | **Missing** |
| **Line joints** | Bevel, Miter, Round (`JointStyle`) | **Missing** |
| **Miter limit** | Configurable (1–255) | **Missing** |
| **Pixel hinting** | Snap strokes to pixel grid | **Missing** |
| **Line scale mode** | NORMAL, NONE, VERTICAL, HORIZONTAL | **Missing** |
| **Gradient line style** | `lineGradientStyle(...)` | **Missing** |
| **Bitmap line style** | `lineBitmapStyle(bitmap, ...)` | **Missing** |
| Dashed/dotted lines | Not built-in (but achievable via drawPath) | Planned in `docs/42_drawing_tools.md` |

### 1.3 Path & Curve Drawing

| Feature | OpenFL | afterhours |
|---------|--------|------------|
| Move to point | `moveTo(x, y)` | **Missing** — no path state |
| Line to point | `lineTo(x, y)` | **Missing** — only `draw_line(x1,y1,x2,y2)` |
| **Quadratic bezier curve** | `curveTo(cx, cy, ax, ay)` | **Missing** |
| **Cubic bezier curve** | `cubicCurveTo(cx1, cy1, cx2, cy2, ax, ay)` | **Missing** |
| **Batch draw path** | `drawPath(commands[], data[], winding)` | **Missing** |
| **IGraphicsData vector** | `drawGraphicsData(vector)` — batch draw fills+strokes+paths | **Missing** |
| **Read back graphics** | `readGraphicsData()` — query vector data from display object | **Missing** |
| Path winding rules | EVEN_ODD, NON_ZERO | **Missing** |
| Clear all graphics | `clear()` | Implicit per-frame (immediate mode) |
| Copy graphics | `copyFrom(source)` | **Missing** |

### 1.4 Shape Primitives

| Feature | OpenFL | afterhours |
|---------|--------|------------|
| Rectangle | `drawRect(x, y, w, h)` | `draw_rectangle(rect, color)` ✓ |
| Rounded rectangle | `drawRoundRect(x, y, w, h, ellipseW, ellipseH)` | `draw_rectangle_rounded(...)` ✓ |
| Per-corner radius | `drawRoundRectComplex(x, y, w, h, tl, tr, bl, br)` | `DrawRectangleCustom(...)` ✓ |
| Circle | `drawCircle(x, y, radius)` | `draw_circle(x, y, radius, color)` ✓ |
| Ellipse | `drawEllipse(x, y, w, h)` | `draw_ellipse(cx, cy, rh, rv, color)` ✓ |
| **Triangles with UV** | `drawTriangles(vertices, indices, uvtData, culling)` | Triangles exist, but **no UV mapping** |
| **Batch quads** | `drawQuads(rects, indices, transforms)` — batch with per-quad transform | **Missing** |
| Ring/arc | Not built-in (achievable via curves) | `draw_ring()`, `draw_ring_segment()` ✓ (ahead of OpenFL) |
| Regular polygon | Not built-in | `draw_poly(center, sides, radius, rotation, color)` ✓ |

### 1.5 Filters (Post-Processing Effects)

| Filter | OpenFL | afterhours |
|--------|--------|------------|
| **Blur** | `BlurFilter(blurX, blurY, quality)` | **Missing** |
| **Drop shadow** | `DropShadowFilter(distance, angle, color, alpha, blur, strength, inner, knockout)` | `TextShadow` only (text-level, not general) |
| **Glow** | `GlowFilter(color, alpha, blur, strength, inner, knockout)` | **Missing** |
| **Bevel** | `BevelFilter(distance, angle, highlightColor, shadowColor, ...)` | **Missing** |
| **Color matrix** | `ColorMatrixFilter(4×5 matrix)` — hue rotation, saturation, luminance-to-alpha | **Missing** |
| **Convolution** | `ConvolutionFilter(matrix, divisor, bias)` — edge detect, sharpen, emboss | **Missing** |
| **Displacement map** | `DisplacementMapFilter(mapBitmap, ...)` — warping/distortion | **Missing** |
| **Shader filter** | `ShaderFilter(shader)` — custom GPU filter | **Missing** |

### 1.6 Blend Modes

| Mode | OpenFL | afterhours |
|------|--------|------------|
| Normal | `BlendMode.NORMAL` | Default rendering ✓ |
| **Add** | Additive blending (lightening dissolve) | **Missing** |
| **Multiply** | Darken by multiplying color channels | **Missing** |
| **Screen** | Lighten (inverse multiply) | **Missing** |
| **Overlay** | Combines multiply and screen | **Missing** |
| **Darken** | Min of each channel | **Missing** |
| **Lighten** | Max of each channel | **Missing** |
| **Difference** | Absolute difference of channels | **Missing** |
| **Hardlight** | Screen or multiply based on source brightness | **Missing** |
| **Subtract** | Subtract source from destination | **Missing** |
| **Invert** | Invert background | **Missing** |
| **Alpha** | Apply source alpha to background | **Missing** |
| **Erase** | Erase background based on source alpha | **Missing** |
| **Layer** | Force transparency group / pre-composition | **Missing** |

### 1.7 Bitmap/Pixel Manipulation

| Feature | OpenFL | afterhours |
|---------|--------|------------|
| **Get/set individual pixel** | `getPixel(x,y)`, `setPixel(x,y,color)` | **Missing** |
| **Get/set pixel with alpha** | `getPixel32()`, `setPixel32()` | **Missing** |
| **Copy pixels** | `copyPixels(source, sourceRect, destPoint)` | **Missing** |
| **Copy channel** | `copyChannel(source, rect, dest, srcChannel, dstChannel)` | **Missing** |
| **Color transform** | `colorTransform(rect, transform)` | **Missing** |
| **Flood fill** | `floodFill(x, y, color)` | **Missing** |
| **Noise generation** | `noise(seed, low, high, channels, grayscale)` | **Missing** |
| **Perlin noise** | `perlinNoise(baseX, baseY, octaves, seed, ...)` | **Missing** |
| **Threshold** | `threshold(source, rect, dest, operation, threshold, color)` | **Missing** |
| **Scroll** | `scroll(x, y)` — shift pixel contents | **Missing** |
| **Merge** | `merge(source, rect, dest, redMult, greenMult, blueMult, alphaMult)` | **Missing** |
| **Draw display object to bitmap** | `draw(source, matrix, colorTransform, blendMode, clipRect)` | **Missing** (no render-to-texture) |
| **Hit test** | `hitTest(point, alphaThreshold, secondBitmap)` | **Missing** |
| Create from file/bytes/base64 | Yes | Texture loading via raylib ✓ |

### 1.8 Masking

| Feature | OpenFL | afterhours |
|---------|--------|------------|
| **Shape masking** | `displayObject.mask = maskShape` — arbitrary shape clips children | **Missing** |
| Rectangular clipping | Via mask or scroll rect | `begin_scissor_mode(x, y, w, h)` ✓ |
| **Alpha masking** | Bitmap alpha used as mask | **Missing** |

### 1.9 Matrix Transforms

| Feature | OpenFL | afterhours |
|---------|--------|------------|
| Scale | `scaleX`, `scaleY` on any display object | No per-object scale (only via rotation matrix push) |
| Rotation | `rotation` property (degrees) | `push_rotation()` / `pop_rotation()` ✓ |
| Translation | `x`, `y` positioning | Rect position ✓ |
| **Full affine matrix** | `Matrix(a, b, c, d, tx, ty)` on fills and transforms | **Missing** — no skew, no arbitrary affine |
| **Transform point** | `matrix.transformPoint(point)` | **Missing** |
| **Gradient box** | `matrix.createGradientBox(w, h, rotation, tx, ty)` | **Missing** |
| Hierarchical transforms | Parent transform automatically applies to children | **Missing** — flat ECS, no transform hierarchy |

---

## 2. OpenFL Text System

OpenFL provides a full `TextField` class modeled after Flash's text system, with
rich formatting, input, selection, and layout capabilities.

### 2.1 Text Formatting (TextFormat)

| Feature | OpenFL | afterhours |
|---------|--------|------------|
| Font family | `font` property (string name) | `font_name` in RenderPrimitive ✓ |
| Font size | `size` (integer, pixels) | `font_size` ✓ |
| Text color | `color` (0xRRGGBB) | `color` ✓ |
| **Bold** | `bold` — requires bold font variant embedded | **Missing** — no font weight switching |
| **Italic** | `italic` — requires italic font variant | **Missing** — no font style switching |
| **Underline** | `underline` — renders underline decoration | **Missing** |
| **Strikethrough** | `strikethrough` — renders strikethrough line | **Missing** |
| Text alignment | `align` (LEFT, CENTER, RIGHT, JUSTIFY) | `TextAlignment` (Left, Center, Right) — **no Justify** |
| **Letter spacing** | `letterSpacing` (float, pixels between chars) | **Missing** — fixed 1.0 spacing |
| **Leading (line spacing)** | `leading` (integer, vertical space between lines) | **Missing** |
| **Kerning** | `kerning` toggle (boolean) | **Missing** — no kerning control |
| **Left margin** | `leftMargin` (pixels) | **Missing** — handled by UI layout padding |
| **Right margin** | `rightMargin` (pixels) | **Missing** |
| **Indent** | `indent` (first line indent) | **Missing** |
| **Block indent** | `blockIndent` (full paragraph indent) | **Missing** |
| **Bullet/list** | `bullet` boolean for bulleted paragraphs | **Missing** |
| **Tab stops** | `tabStops` (array of pixel positions) | **Missing** |
| **URL/hyperlink** | `url` + `target` properties | **Missing** |

### 2.2 Per-Range Formatting

| Feature | OpenFL | afterhours |
|---------|--------|------------|
| **Apply format to range** | `setTextFormat(format, beginIndex, endIndex)` | **Missing** — entire text is one style |
| **Default format** | `defaultTextFormat` — applied before text is added | One style per RenderPrimitive ✓ |
| **Get format at index** | `getTextFormat(beginIndex, endIndex)` | **Missing** |

This is one of the most significant text gaps. OpenFL can render a single text field
with multiple fonts, sizes, colors, and styles within the same string. afterhours
currently applies a single style to the entire text of a RenderPrimitive.

### 2.3 Text Layout & Wrapping

| Feature | OpenFL | afterhours |
|---------|--------|------------|
| **Word wrapping** | `wordWrap` boolean — automatic line breaks at field width | **Missing** — single line only |
| **Multi-line** | `multiline` boolean — allows newlines | **Missing** at render level |
| **Auto-size** | `autoSize` (NONE, LEFT, CENTER, RIGHT) — resize field to fit text | **Missing** |
| **Scrolling** | `scrollH`, `scrollV`, `maxScrollH`, `maxScrollV` | **Missing** (scroll containers exist at UI level) |
| **Bottom scroll line** | `bottomScrollV` — visible line range query | **Missing** |
| **Line metrics** | `getLineMetrics(lineIndex)` — ascent, descent, width, height, leading, x | **Missing** |
| **Num lines** | `numLines` | **Missing** |
| **Line offsets** | `getLineOffset(lineIndex)`, `getLineLength(lineIndex)` | **Missing** |
| **Line text** | `getLineText(lineIndex)` | **Missing** |
| **Char boundaries** | `getCharBoundaries(charIndex)` — Rectangle for a character | **Missing** |
| **Char at point** | `getCharIndexAtPoint(x, y)` | **Missing** |
| **Line at point** | `getLineIndexAtPoint(x, y)` | **Missing** |

### 2.4 Text Input & Selection

| Feature | OpenFL | afterhours |
|---------|--------|------------|
| **Input type** | `type = INPUT` — editable text field | Separate text input widgets at UI level |
| **Selection range** | `setSelection(begin, end)`, `selectionBeginIndex`, `selectionEndIndex` | **Missing** at render level |
| **Caret index** | `caretIndex` — cursor position | **Missing** |
| **Select all** | Programmatic and keyboard (Ctrl+A) | **Missing** |
| **Copy/paste** | Keyboard shortcuts (Ctrl+C, Ctrl+V) | **Missing** |
| **Restrict input** | `restrict` — character whitelist/blacklist pattern | **Missing** |
| **Max chars** | `maxChars` — character limit | **Missing** |
| **Editable** | `selectable`, `mouseEnabled` | **Missing** |
| **Password mode** | `displayAsPassword` — mask characters | **Missing** |

### 2.5 Text Rendering Features

| Feature | OpenFL | afterhours |
|---------|--------|------------|
| **Anti-aliasing** | `antiAliasType` (NORMAL, ADVANCED) | Bilinear filter on font texture ✓ |
| **Grid fit** | `gridFitType` (NONE, PIXEL, SUBPIXEL) | **Missing** |
| **Sharpness** | `sharpness` (-400 to 400) | **Missing** |
| **Thickness** | `thickness` (-200 to 200) | **Missing** |
| **Embedded fonts** | `embedFonts` boolean — use embedded vs system fonts | TTF/OTF loading ✓ |
| Text stroke (outline) | Not built-in (achievable via filters) | `TextStroke` (8-direction) ✓ (ahead of OpenFL) |
| Text shadow | `DropShadowFilter` on text | `TextShadow` ✓ |
| **HTML text** | `htmlText` — inline HTML tags (`<b>`, `<i>`, `<font>`, `<a>`, `<img>`, etc.) | **Missing** (BBCode planned in `docs/15_rich_text_label.md`) |
| **CSS styles** | `styleSheet` — apply CSS to HTML text | **Missing** |
| **Background fill** | `background` + `backgroundColor` on TextField | **Missing** at text level (UI component handles it) |
| **Border** | `border` + `borderColor` on TextField | **Missing** at text level |

---

## 3. OpenFL Display List (Scene Graph)

OpenFL uses a retained-mode scene graph with a hierarchy of display objects.
afterhours uses an immediate-mode ECS architecture, so direct adoption isn't
appropriate, but some concepts are useful.

| Feature | OpenFL | afterhours |
|---------|--------|------------|
| Scene graph hierarchy | DisplayObject → Sprite → Stage | Flat ECS with parent pointers |
| Transform inheritance | Automatic parent→child | **Missing** — each entity positions itself |
| **Cache as bitmap** | `cacheAsBitmap` — snapshot for faster rendering | **Missing** |
| **Scroll rect** | `scrollRect` — visible region with scrolling | Scissor mode ✓ |
| **Alpha (opacity)** | `alpha` (0.0–1.0) on any node, inherited | Color.a only, not inherited |
| **Visible** | `visible` boolean | Component-level flags |
| **Mouse enabled** | `mouseEnabled`, `mouseChildren` | **Missing** — input is per-entity |
| **Hit area** | `hitArea` — custom interactive region | **Missing** |

### 3.1 Event System

| Feature | OpenFL | afterhours |
|---------|--------|------------|
| **Event bubbling** | Events climb from target → parent → stage | **Missing** — no event hierarchy |
| **Capture phase** | Events descend from stage → target before bubbling | **Missing** |
| **preventDefault()** | Cancel default behavior | **Missing** |
| **Event priorities** | Listener priority ordering | **Missing** |
| **W3C DOM Level 3 compliant** | Full spec compliance | N/A |

---

## 4. Other OpenFL Features (Less Relevant)

These are included for completeness but are less applicable to afterhours' scope:

- **Stage3D** — GPU 3D pipeline (vertex/fragment shaders, draw triangles, textures,
  mipmapping, depth testing). afterhours uses raylib's GPU pipeline directly.
- **Sound/Audio** — MP3/OGG/WAV with volume, panning, seek. Game audio is typically
  handled at the engine level, not the UI library.
- **Network** — HTTP, sockets. Out of scope.
- **Accelerometer** — Mobile sensor. Out of scope.
- **File system** — File I/O. Out of scope.
- **Window management** — Multiple windows. Out of scope.
- **MovieClip** — Frame-based animation timeline. afterhours has its own animation system.

---

## 5. Gap Analysis: Drawing APIs

### 5.1 Critical Gaps (High Priority)

These are features that would significantly expand afterhours' drawing capabilities:

#### 1. Bezier Curves (Quadratic + Cubic)

OpenFL provides `curveTo()` (quadratic) and `cubicCurveTo()` (cubic). afterhours has
no curve drawing at all. This is fundamental for:
- Smooth paths and connectors
- Rounded UI decorations beyond simple corner radius
- Chart/graph line rendering
- Arrow connectors in node-based UIs
- Custom shape outlines

**Recommendation:** Add to `drawing_helpers.h`:

```cpp
// Quadratic bezier: current → control → anchor
void draw_bezier_quad(Vector2Type start, Vector2Type control,
                      Vector2Type end, float thickness, Color color,
                      int segments = 20);

// Cubic bezier: current → control1 → control2 → anchor
void draw_bezier_cubic(Vector2Type start, Vector2Type control1,
                       Vector2Type control2, Vector2Type end,
                       float thickness, Color color, int segments = 20);
```

Raylib has `DrawLineBezierQuad()` and `DrawLineBezierCubic()` that can back these.

#### 2. Gradient Fills (Linear + Radial)

OpenFL supports linear and radial gradients with multiple color stops, alpha per stop,
ratio-based positioning, spread modes (pad/reflect/repeat), and matrix transforms.
afterhours has solid color fills only.

Gradients are essential for:
- Button/panel background effects
- Progress bar fills
- Sky/environment backgrounds
- Health bar color transitions
- Visual polish and depth

**Recommendation:** Add a gradient render primitive:

```cpp
enum class GradientType { Linear, Radial };
enum class GradientSpread { Pad, Reflect, Repeat };

struct GradientStop {
    float position;  // 0.0 to 1.0
    Color color;
};

// Add to RenderPrimitiveType: Gradient
// Add to drawing_helpers.h:
void draw_gradient_rect(RectangleType rect, GradientType type,
                        const GradientStop* stops, int stop_count,
                        float angle = 0.0f);  // angle for linear direction
```

Implementation: Generate gradient via pixel shader or by tessellating into colored
triangles. Raylib doesn't have built-in gradient rects, so this requires either
a shader or vertex-colored quad strips.

#### 3. Line Caps and Joints

OpenFL provides `CapsStyle` (None, Round, Square) and `JointStyle` (Bevel, Miter,
Round) for controlling how lines begin/end and how connected segments meet.

**Recommendation:** Add cap/joint enums:

```cpp
enum class LineCap { Butt, Round, Square };
enum class LineJoin { Miter, Round, Bevel };

void draw_line_styled(Vector2Type start, Vector2Type end, float thickness,
                      Color color, LineCap cap = LineCap::Butt);

void draw_polyline(Vector2Type* points, int count, float thickness,
                   Color color, LineJoin join = LineJoin::Miter,
                   LineCap cap = LineCap::Butt);
```

#### 4. Blend Modes

OpenFL supports 14 blend modes. afterhours has none (everything is default alpha
blending). At minimum, these are useful for game UIs:

| Mode | Use Case |
|------|----------|
| ADD | Glow/light effects, particle systems |
| MULTIPLY | Darkening overlays, shadow tinting |
| SCREEN | Lightening overlays, highlights |
| OVERLAY | Combining textures with contrast |

**Recommendation:** Expose raylib's `BeginBlendMode()` / `EndBlendMode()`:

```cpp
enum class BlendMode { Normal, Additive, Multiply, Screen };

void begin_blend_mode(BlendMode mode);
void end_blend_mode();
```

Raylib supports `RL_BLEND_ADDITIVE`, `RL_BLEND_MULTIPLIED`, `RL_BLEND_ADD_COLORS`,
and custom blend modes via `rlSetBlendFactors()`.

### 5.2 Important Gaps (Medium Priority)

#### 5. Path Builder API

OpenFL's `moveTo()` / `lineTo()` / `curveTo()` stateful path model is ergonomic for
complex shapes. afterhours only has individual shape calls.

**Recommendation:** A lightweight path builder:

```cpp
class PathBuilder {
    std::vector<Vector2Type> points_;
    std::vector<uint8_t> commands_;  // MoveTo=0, LineTo=1, QuadTo=2, CubicTo=3
public:
    PathBuilder& move_to(float x, float y);
    PathBuilder& line_to(float x, float y);
    PathBuilder& quad_to(float cx, float cy, float ax, float ay);
    PathBuilder& cubic_to(float cx1, float cy1, float cx2, float cy2,
                          float ax, float ay);
    PathBuilder& close();

    void stroke(float thickness, Color color, LineCap cap, LineJoin join);
    void fill(Color color);
};
```

#### 6. Bitmap/Texture Fill

OpenFL's `beginBitmapFill()` tiles or stretches a texture across a shape. Currently
afterhours can only draw textures as rectangles (`draw_texture_pro`, `draw_texture_npatch`).

**Recommendation:** Add texture-filled rectangles with repeat/mirror modes:

```cpp
enum class TextureWrap { Clamp, Repeat, Mirror };

void draw_texture_filled_rect(TextureType texture, RectangleType rect,
                              TextureWrap wrap_x, TextureWrap wrap_y,
                              Color tint = {255,255,255,255});
```

#### 7. Color Matrix Transform

OpenFL's `ColorMatrixFilter` applies a 4×5 matrix to RGBA. This enables:
- Grayscale conversion (desaturation)
- Hue rotation
- Brightness/contrast adjustment
- Sepia tone
- Color channel swapping

afterhours has `darken()`, `lighten()`, `lerp()` etc. at the color utility level,
but no per-pixel or per-region color transforms.

**Recommendation:** Add a color matrix utility for batch color transformation:

```cpp
struct ColorMatrix {
    float m[20];  // 4x5 matrix (4 rows × 5 cols, last col is offset)

    static ColorMatrix identity();
    static ColorMatrix grayscale();
    static ColorMatrix sepia();
    static ColorMatrix brightness(float amount);
    static ColorMatrix contrast(float amount);
    static ColorMatrix hue_rotate(float degrees);
    static ColorMatrix saturate(float amount);

    Color apply(Color input) const;
};
```

#### 8. Shape Masking

OpenFL allows any display object to act as a mask for another. afterhours only has
rectangular scissor clipping.

**Recommendation:** For immediate-mode rendering, stencil-buffer masking is the
typical approach:

```cpp
void begin_stencil_mask();   // Start drawing the mask shape
void apply_stencil_mask();   // Switch to rendering masked content
void end_stencil_mask();     // Remove mask
```

Raylib provides `rlEnableStencilTest()` / `rlStencilMask()` for this.

### 5.3 Nice-to-Have Gaps (Lower Priority)

| Gap | Description | Notes |
|-----|-------------|-------|
| Render-to-texture | Draw scene to offscreen texture | Useful for effects, minimaps, caching |
| Triangle UV mapping | Map texture coordinates to triangles | For 3D-like 2D distortion effects |
| Batch quad rendering | Draw many quads with per-quad transforms efficiently | Performance optimization for particles |
| Perlin/simplex noise | Procedural texture generation | Useful for terrain, particle effects |
| Pixel-level read/write | Get/set individual pixels on textures | Useful for painting tools |
| Flood fill | Fill connected pixels of same color | Useful for paint tools |
| Convolution filters | Edge detection, sharpen, emboss | Post-processing effects |
| Displacement mapping | Warp/distort based on map texture | Water/heat effects |

---

## 6. Gap Analysis: Text APIs

### 6.1 Critical Gaps (High Priority)

#### 1. Word Wrapping / Multi-line Text

OpenFL's `wordWrap` + `multiline` properties enable automatic line breaking. afterhours
renders text as a single line that overflows its container.

**Recommendation:** Add word-wrap logic to the text rendering pipeline:

```cpp
struct TextLayoutOptions {
    bool word_wrap = false;
    bool multiline = false;    // Allow explicit \n
    float max_width = 0.0f;    // Wrap width (0 = no wrap)
    float line_spacing = 0.0f; // Extra space between lines (leading)
};

// Returns laid-out lines with positions
struct TextLine {
    const char* start;
    int length;
    float x, y;
    float width, height;
};

std::vector<TextLine> layout_text(Font font, const char* text,
                                  float font_size, float spacing,
                                  const TextLayoutOptions& options);
```

This is the single most impactful text improvement. It enables:
- Paragraph text in dialogs
- Wrapping labels
- Multi-line tooltips
- Chat messages
- Description text that adapts to container width

#### 2. Per-Range Text Formatting

OpenFL's `setTextFormat(format, begin, end)` applies different styles to character
ranges. afterhours applies one style to all text in a primitive.

**Recommendation:** A styled-spans model for the render pipeline:

```cpp
struct TextSpan {
    int start_index;
    int end_index;
    std::optional<Color> color;
    std::optional<float> font_size;
    std::optional<std::string> font_name;
    // Future: bold, italic, underline flags
};

// Add to RenderCommandBuffer:
void add_styled_text(const RectangleType& rect,
                     const std::string& text,
                     const std::string& default_font,
                     float default_font_size,
                     Color default_color,
                     const std::vector<TextSpan>& spans,
                     TextAlignment alignment,
                     int layer, EntityID entity_id = -1);
```

This dovetails with the planned Rich Text Label (`docs/15_rich_text_label.md`) and
would provide the render-level support for BBCode or any markup system.

#### 3. Letter Spacing

OpenFL's `letterSpacing` controls space between characters. afterhours uses a fixed
`spacing = 1.0f` in the text renderer.

**Recommendation:** Expose letter spacing as a text property:

```cpp
// In text rendering:
float letter_spacing = 1.0f;  // Currently hardcoded

// Should become configurable per-text-element
```

This is a small change but affects visual polish significantly — tighter spacing for
headings, wider spacing for labels, etc.

#### 4. Leading (Line Spacing)

OpenFL's `leading` controls vertical space between lines. afterhours has no multi-line
text, so this doesn't exist yet, but it should be part of the word-wrap implementation.

**Recommendation:** Include in `TextLayoutOptions` (see word wrap above).

### 6.2 Important Gaps (Medium Priority)

#### 5. Text Justify Alignment

OpenFL supports `JUSTIFY` alignment (distribute words to fill width). afterhours has
Left, Center, Right only.

**Recommendation:** Add `TextAlignment::Justify` and implement word-spacing adjustment
in the layout algorithm.

#### 6. Bold/Italic Font Switching

OpenFL's `bold` and `italic` properties switch to the bold/italic variant of the
embedded font. afterhours loads fonts by name but has no concept of font families
with weight/style variants.

**Recommendation:** Extend FontManager with family support:

```cpp
struct FontFamily {
    Font regular;
    Font bold;
    Font italic;
    Font bold_italic;
};

// In FontManager:
auto& load_font_family(const std::string& family_name,
                       const char* regular_file,
                       const char* bold_file = nullptr,
                       const char* italic_file = nullptr,
                       const char* bold_italic_file = nullptr);

Font get_font(const std::string& family, bool bold, bool italic);
```

#### 7. Underline and Strikethrough

OpenFL renders these as text decorations. afterhours has neither.

**Recommendation:** Add as post-processing in the text renderer:

```cpp
struct TextDecoration {
    bool underline = false;
    bool strikethrough = false;
    Color decoration_color;  // Defaults to text color
    float thickness = 1.0f;
};
```

Underline: draw a line at baseline + descent. Strikethrough: draw at baseline -
(ascent * 0.35).

#### 8. Character Boundary Queries

OpenFL's `getCharBoundaries(index)` returns the rectangle for any character. This is
essential for:
- Text cursor positioning
- Selection highlight rendering
- Click-to-position in text input
- Tooltip anchoring to specific words

**Recommendation:** Add character measurement API:

```cpp
RectangleType get_char_bounds(Font font, const char* text,
                              int char_index, float font_size,
                              float spacing);

int get_char_at_point(Font font, const char* text,
                      float font_size, float spacing,
                      float point_x, float point_y);
```

### 6.3 Nice-to-Have Gaps (Lower Priority)

| Gap | Description | Notes |
|-----|-------------|-------|
| Tab stops | Custom tab positions | For data tables, code display |
| Block indent | Paragraph indentation | For document editing |
| Bullet/list mode | Automatic bullet point rendering | For menus, item lists |
| Restrict input | Character whitelist/blacklist | For numeric-only, no-spaces, etc. |
| Password display | Mask characters with dots/asterisks | For auth UIs |
| Embedded HTML | Render inline HTML tags | BBCode planned instead |
| CSS text styling | Apply stylesheets to text | Not relevant for game UI |
| Text sharpness/thickness | Fine control over font rendering | Font-level hint tuning |

---

## 7. Summary: Prioritized Recommendations

### Phase 1 — Core Drawing Enhancements (Highest Impact)

| # | Feature | Effort | Impact |
|---|---------|--------|--------|
| 1 | **Bezier curves** (quad + cubic) | Medium | High — enables smooth paths, connectors |
| 2 | **Word wrap / multi-line text** | Medium | High — single most requested text feature |
| 3 | **Gradient fills** (linear + radial) | Medium | High — visual polish for every UI |
| 4 | **Blend modes** (add, multiply, screen) | Low | Medium — enables glow, overlay effects |
| 5 | **Letter spacing** | Low | Medium — text polish |

### Phase 2 — Extended Drawing

| # | Feature | Effort | Impact |
|---|---------|--------|--------|
| 6 | **Line caps + joints** | Low | Medium — polyline quality |
| 7 | **Path builder API** | Medium | Medium — ergonomic complex shapes |
| 8 | **Per-range text formatting** | High | High — rich text foundation |
| 9 | **Bold/italic font families** | Medium | Medium — text expressiveness |
| 10 | **Color matrix transforms** | Medium | Medium — visual effects |

### Phase 3 — Advanced Features

| # | Feature | Effort | Impact |
|---|---------|--------|--------|
| 11 | **Shape masking (stencil)** | Medium | Medium — advanced clipping |
| 12 | **Underline / strikethrough** | Low | Low-Medium — text decoration |
| 13 | **Character boundary queries** | Medium | High — text input quality |
| 14 | **Render-to-texture** | High | Medium — effects, caching |
| 15 | **Text justify alignment** | Low | Low — document-style text |

### What NOT to Adopt

| OpenFL Feature | Reason to Skip |
|----------------|----------------|
| Retained-mode display list / scene graph | afterhours is ECS + immediate-mode; display list is architecturally incompatible |
| HTML text / CSS styles | BBCode planned instead; HTML is web-centric |
| W3C DOM event model | afterhours' ECS event model is simpler and sufficient |
| Stage3D GPU pipeline | afterhours uses raylib's GPU layer directly |
| MovieClip timeline animation | afterhours has its own animation system |
| Sound/Audio/Network/File | Out of scope for UI library |
| Shader fills | Requires custom shader pipeline; defer until needed |
| Displacement mapping | Very niche effect; defer |

---

## 8. Cross-Reference with Existing Plans

| OpenFL Feature | Existing afterhours Plan | Status |
|----------------|-------------------------|--------|
| Bezier curves | `docs/42_drawing_tools.md` mentions freeform drawing, but not curves | **Gap** — curves not planned yet |
| Gradient fills | `docs/23_visual_effects.md` | Check if gradients are mentioned |
| Blend modes | `docs/23_visual_effects.md` | Check if blend modes are mentioned |
| Line styles (dash/dot) | `docs/42_drawing_tools.md` — `LineStyle` enum planned | Planned ✓ |
| Rich text / per-range format | `docs/15_rich_text_label.md` — BBCode planned | Planned ✓ |
| Word wrapping | `docs/40_text_editing_widget.md` — text editing widget planned | Partially planned |
| Color matrix | Not mentioned anywhere | **Gap** |
| Shape masking | Not mentioned anywhere | **Gap** |
| Character boundaries | Not mentioned anywhere | **Gap** — critical for text editing |
| Font families (bold/italic) | `docs/17_font_loading.md` | Check if families are mentioned |
| Underline/strikethrough | `docs/15_rich_text_label.md` — BBCode `[u]`/`[s]` planned | Planned ✓ |

---

## 9. Key Architectural Lesson from OpenFL

OpenFL's drawing API is **stateful** (set fill → draw shapes → end fill). afterhours'
is **stateless** (each draw call includes all its parameters). Both are valid, but
OpenFL's approach has ergonomic advantages for complex drawings:

```
// OpenFL: draw a gradient-filled rounded rect with a thick dashed border
graphics.beginGradientFill(LINEAR, [red, blue], [1, 1], [0, 255], matrix);
graphics.lineStyle(3, black, 1.0, false, NORMAL, ROUND, ROUND);
graphics.drawRoundRect(10, 10, 200, 100, 15);
graphics.endFill();

// afterhours equivalent would require:
// 1. A new gradient-rect primitive (doesn't exist)
// 2. A separate call for the rounded outline
// 3. No way to combine fill + stroke in one call
```

**Recommendation:** For the path builder API, consider a mini stateful API where fill
and stroke style are set once, then multiple shapes are drawn with those styles. This
doesn't replace the immediate-mode draw calls but provides a higher-level layer for
complex vector graphics (diagrams, charts, decorated panels).

---

## 10. afterhours Strengths Over OpenFL

afterhours already has features that OpenFL lacks or handles differently:

| Feature | afterhours | OpenFL |
|---------|------------|--------|
| **Ring / arc segment** | `draw_ring()`, `draw_ring_segment()` — first-class | Must be approximated with curves |
| **Per-corner radius** | `DrawRectangleCustom()` with 4 independent radii | Only uniform or ellipse-based |
| **Text stroke (8-dir outline)** | `TextStroke` with configurable thickness + color | Requires DropShadowFilter or manual |
| **Text shadow** | `TextShadow` with offset + color | Requires DropShadowFilter |
| **Regular polygon** | `draw_poly(center, sides, radius, rotation)` | Not built-in |
| **9-slice / NPatch** | `draw_texture_npatch()` | Supported but at display object level |
| **Rotated rounded rect** | `draw_rectangle_rounded_rotated()` | Must manually apply Matrix |
| **WCAG contrast utilities** | Full contrast ratio, compliance checking, auto-text-color | Not built-in |
| **Arena-allocated render commands** | Zero-allocation rendering pipeline | GC-based (Haxe/JS) |
| **ECS architecture** | Composable, data-oriented | Rigid display object hierarchy |
