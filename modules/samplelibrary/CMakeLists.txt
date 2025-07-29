add_library(samplelibrary
    src/entrypoint.cpp
)

find_package(glm REQUIRED) # for sample_library_vector_length
find_package(CURL REQUIRED) # for sample_library_curl_current_version

target_include_directories(samplelibrary
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    samplelibrary
    PRIVATE
    glm::glm # for sample_library_vector_length
    CURL::libcurl # for sample_library_curl_current_version
)
