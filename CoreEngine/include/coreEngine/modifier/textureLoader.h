#ifndef TEXTURE_LOADER
#define TEXTURE_LOADER

#include "stb_image.h"

namespace cl {
    char *result_string_pointer = "SOIL initialized";

    enum {
        CVR_LOAD_AUTO = 0,
        CVR_LOAD_L = 1,
        CVR_LOAD_LA = 2,
        CVR_LOAD_RGB = 3,
        CVR_LOAD_RGBA = 4
    };

    unsigned char *
    cvr_load_image(
            const char *filename,
            int *width, int *height, int *channels,
            int force_channels) {
        unsigned char *result = stbi_load(filename,
                                          width, height, channels, force_channels);
        if (result == NULL) {
            result_string_pointer = (char *) stbi_failure_reason();
        } else {
            result_string_pointer = "Image loaded";
        }
        return result;
    }
}
#endif /* TEXTURE_LOADER	*/