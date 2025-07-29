#if _WIN32
#define SAMPLELIBRARY_EXPORT extern "C" __declspec(dllexport)
#else
#define SAMPLELIBRARY_EXPORT extern "C" __attribute__((visibility("default")))
#endif

#include <cstdint>
#include <math.h>

#include <glm/glm.hpp> // for sample_library_vector_length
#include <curl/curl.h> // for sample_library_curl_current_version

SAMPLELIBRARY_EXPORT int32_t sample_library_add(const int32_t a, const int32_t b) {
    return a + b;
}

SAMPLELIBRARY_EXPORT int32_t sample_library_multiply(const int32_t a, const int32_t b) {
    return a * b;
}

SAMPLELIBRARY_EXPORT float_t sample_library_vector_length(const float_t x, const float_t y, const float_t z) {
    return glm::length(glm::vec3(x, y, z));
}

SAMPLELIBRARY_EXPORT char* sample_library_curl_current_version() {
    return curl_version();
}
