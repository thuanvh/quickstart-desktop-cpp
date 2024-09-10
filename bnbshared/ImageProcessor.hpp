// ImageProcessor.hpp

#ifndef IMAGE_PROCESSOR_HPP
#define IMAGE_PROCESSOR_HPP

#include <map>
#include <memory>

// namespace bnb {
//     namespace player_api {
//         class player;  // Forward declaration for player class.
//     }
// }

// Declare the global map to store shared pointers for player instances.
//extern std::map<void*, std::shared_ptr<bnb::player_api::player>> playerInstances;

extern "C" {

/**
 * @brief Initializes the Banuba SDK and returns a handle to the created player instance.
 *
 * @param resources_path Path to the SDK resources.
 * @param client_token   The client token for SDK authentication.
 * @return A handle (void*) to the created player instance.
 */
__declspec(dllexport) void* InitializeBanubaSDK(const char* resources_path, const char* client_token);

/**
 * @brief Processes an image using the player instance.
 *
 * @param player_handle Handle to the player instance.
 * @param input_image   Pointer to the input image data (RGB format).
 * @param rgb_stride    The stride of the input image (number of bytes per row).
 * @param width         Width of the input image.
 * @param height        Height of the input image.
 * @param channels      Number of channels in the input image (typically 3 for RGB).
 * @param output_image  Pointer to store the processed output image data.
 * @param output_size   Pointer to store the size of the processed output image data.
 * @return 0 on success, -1 on failure.
 */
__declspec(dllexport) int ProcessImage(void* player_handle, const unsigned char* input_image);

__declspec(dllexport) void SetProcessSize(int stride, int width, int height, int channels, unsigned char** output_image, int* output_size);
__declspec(dllexport) void loadEffect(const char* effectName);
/**
 * @brief Releases the memory allocated for an image.
 *
 * @param image_data Pointer to the image data to be released.
 */
__declspec(dllexport) void ReleaseImage(unsigned char* image_data);

/**
 * @brief Cleans up the resources associated with a player instance.
 *
 * @param player_handle Handle to the player instance to be cleaned up.
 */
__declspec(dllexport) void Cleanup(void* player_handle);

} // extern "C"

#endif // IMAGE_PROCESSOR_HPP
