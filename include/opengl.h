#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int uint;

uint compile_shader(const void * vp,
                    const int vp_length,
                    const void * fp,
                    const int fp_length);

uint make_buffer(unsigned int target,
                 const void * data,
                 size_t size);

uint make_texture(const void * data,
                  int internalformat,
                  int width,
                  int height,
                  int format,
                  int type);

uint make_framebuffer(uint * texture, int length);

#ifdef __cplusplus
}
#endif
