#pragma once

#ifdef __cplusplus
extern "C" {
#endif

unsigned int compile_shader(const void * vp,
                            const int vp_length,
                            const void * fp,
                            const int fp_length);

int make_buffer(unsigned int target,
                const void * data,
                size_t size);

int make_texture(const void * data,
                 int internalformat,
                 int width,
                 int height,
                 int format,
                 int type);

#ifdef __cplusplus
}
#endif
