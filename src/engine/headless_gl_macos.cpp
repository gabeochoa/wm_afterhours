#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include "headless_gl_macos.h"
#include <OpenGL/gl3.h>
#include <cstdio>
#include <dlfcn.h>

// Function loader for GLAD - loads GL functions from OpenGL.framework
static void* macos_gl_get_proc_address(const char* name) {
    return dlsym(RTLD_DEFAULT, name);
}

bool HeadlessGLMacOS::init(int width, int height) {
    (void)width; (void)height;  // Context is resolution-independent

    // First try hardware-accelerated context
    CGLPixelFormatAttribute attrs_hw[] = {
        kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute)kCGLOGLPVersion_3_2_Core,
        kCGLPFAColorSize, (CGLPixelFormatAttribute)24,
        kCGLPFADepthSize, (CGLPixelFormatAttribute)24,
        kCGLPFAAccelerated,
        kCGLPFAAllowOfflineRenderers, // Allow headless GPUs
        (CGLPixelFormatAttribute)0
    };

    CGLPixelFormatObj pixelFormat = nullptr;
    GLint numFormats = 0;
    CGLError err = CGLChoosePixelFormat(attrs_hw, &pixelFormat, &numFormats);

    // If hardware acceleration fails, try software rendering
    if (!pixelFormat || err != kCGLNoError) {
        CGLPixelFormatAttribute attrs_sw[] = {
            kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute)kCGLOGLPVersion_3_2_Core,
            kCGLPFAColorSize, (CGLPixelFormatAttribute)24,
            kCGLPFADepthSize, (CGLPixelFormatAttribute)24,
            kCGLPFARendererID, (CGLPixelFormatAttribute)kCGLRendererGenericFloatID,
            (CGLPixelFormatAttribute)0
        };
        err = CGLChoosePixelFormat(attrs_sw, &pixelFormat, &numFormats);
    }

    // Last resort: minimal attributes
    if (!pixelFormat || err != kCGLNoError) {
        CGLPixelFormatAttribute attrs_min[] = {
            kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute)kCGLOGLPVersion_3_2_Core,
            (CGLPixelFormatAttribute)0
        };
        err = CGLChoosePixelFormat(attrs_min, &pixelFormat, &numFormats);
    }

    if (!pixelFormat) {
        fprintf(stderr, "HeadlessGL: CGLChoosePixelFormat failed with error %d\n", err);
        return false;
    }

    err = CGLCreateContext(pixelFormat, nullptr, &context);
    CGLDestroyPixelFormat(pixelFormat);

    if (!context || err != kCGLNoError) {
        fprintf(stderr, "HeadlessGL: CGLCreateContext failed with error %d\n", err);
        return false;
    }

    err = CGLSetCurrentContext(context);
    if (err != kCGLNoError) {
        fprintf(stderr, "HeadlessGL: CGLSetCurrentContext failed with error %d\n", err);
        CGLDestroyContext(context);
        context = nullptr;
        return false;
    }

    // Verify GL is working
    const GLubyte* version = glGetString(GL_VERSION);
    if (version) {
        fprintf(stderr, "HeadlessGL: OpenGL version: %s\n", version);
    } else {
        fprintf(stderr, "HeadlessGL: Warning - glGetString(GL_VERSION) returned NULL\n");
    }

    // Clear any GL errors
    while (glGetError() != GL_NO_ERROR) {}

    return true;
}

void* HeadlessGLMacOS::get_proc_address() {
    return (void*)macos_gl_get_proc_address;
}

void HeadlessGLMacOS::make_current() {
    CGLSetCurrentContext(context);
}

void HeadlessGLMacOS::shutdown() {
    if (context) {
        CGLDestroyContext(context);
        context = nullptr;
    }
}

#endif
