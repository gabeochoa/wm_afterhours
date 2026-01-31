#pragma once
#ifdef __APPLE__

#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>

struct HeadlessGLMacOS {
    CGLContextObj context = nullptr;

    bool init(int width, int height);
    void* get_proc_address();  // Returns function loader for rlLoadExtensions
    void make_current();
    void shutdown();
};

#endif
