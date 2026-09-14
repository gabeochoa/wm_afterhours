#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static uint64_t draws, binds, frames;
static FILE *output;
static double previous;

static double seconds(void) {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return now.tv_sec + now.tv_nsec / 1000000000.0;
}

static void probe_draw_elements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
  ++draws;
  glDrawElements(mode, count, type, indices);
}

static void probe_draw_arrays(GLenum mode, GLint first, GLsizei count) {
  ++draws;
  glDrawArrays(mode, first, count);
}

static void probe_bind_texture(GLenum target, GLuint texture) {
  ++binds;
  glBindTexture(target, texture);
}

static CGLError probe_present(CGLContextObj context) {
  if (!output) {
    const char *path = getenv("WM_GL_PROBE_OUTPUT");
    if (path) output = fopen(path, "w");
    if (output) fprintf(output, "frame,seconds,frame_ms,draw_calls,texture_bind_calls,swap_interval\n");
  }
  if (getenv("WM_GL_PROBE_VSYNC")) {
    GLint interval = 1;
    CGLSetParameter(context, kCGLCPSwapInterval, &interval);
  }
  const CGLError result = CGLFlushDrawable(context);
  const double now = seconds();
  GLint interval = 0;
  CGLGetParameter(context, kCGLCPSwapInterval, &interval);
  if (output) fprintf(output, "%llu,%.9f,%.6f,%llu,%llu,%d\n",
      (unsigned long long)frames, now, previous ? (now - previous) * 1000.0 : 0,
      (unsigned long long)draws, (unsigned long long)binds, interval);
  previous = now;
  draws = binds = 0;
  ++frames;
  return result;
}

__attribute__((destructor)) static void close_probe(void) {
  if (output) fclose(output);
}

#define INTERPOSE(replacement, original) \
  __attribute__((used)) static struct { const void *new_function; const void *old_function; } \
  interpose_##original __attribute__((section("__DATA,__interpose"))) = \
    { (const void *)&replacement, (const void *)&original }

INTERPOSE(probe_draw_elements, glDrawElements);
INTERPOSE(probe_draw_arrays, glDrawArrays);
INTERPOSE(probe_bind_texture, glBindTexture);
INTERPOSE(probe_present, CGLFlushDrawable);
