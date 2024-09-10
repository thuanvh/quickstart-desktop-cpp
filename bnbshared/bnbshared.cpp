// ImageProcessor.cpp

#include "BanubaClientToken.hpp"
#include <GlfwRenderer.hpp>
#include <Utils.hpp>
#include <bnb/effect_player/utility.hpp>
#include <bnb/recognizer/interfaces/utility_manager.hpp>
#include <bnb/player_api/interfaces/player/player.hpp>
#include <bnb/player_api/interfaces/input/live_input.hpp>
#include <bnb/player_api/interfaces/input/photo_input.hpp>
#include <bnb/player_api/interfaces/output/opengl_frame_output.hpp>
#include <bnb/player_api/interfaces/render_target/opengl_render_target.hpp>
#include <bnb/player_api/interfaces/render_target/metal_render_target.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <filesystem>
#include <vector>
#include <map>
#include <bnb/player_api/interfaces/render_target.hpp>

using namespace bnb::interfaces;

// Global map to store shared pointers for player instances

namespace
{
    render_backend_type render_backend = render_backend_type::opengl;
    struct BanubaInstance{
        public:
        std::shared_ptr<bnb::player_api::player> player;
        std::shared_ptr<bnb::player_api::photo_input> input;
        //std::shared_ptr<bnb::utility> utility;
        //std::shared_ptr<GLFWRenderer> renderer;
        //bnb::player_api::render_target_sptr render_target;
        int width;
        int height;
        int channels;
        int stride;
        unsigned char* output_image;
        int output_size;
    };
    std::shared_ptr<BanubaInstance> bnbInstance;
    //bnb::utility *bnbutil;
    //std::shared_ptr<bnb::utility> bnbutil;
    
}

extern "C" {

__declspec(dllexport) void* InitializeBanubaSDK(const char* resources_path, const char* client_token)
{
    // Initialize Banuba SDK utility
    bnb::interfaces::utility_manager::initialize(std::initializer_list<std::string>{resources_path, BNB_RESOURCES_FOLDER}, client_token);
    auto renderer = std::make_shared<GLFWRenderer>(render_backend);
    auto render_target = bnb::player_api::opengl_render_target::create();
    
    // Create player using shared pointer and store it in a map
    auto player = bnb::player_api::player::create(30, render_target, renderer);
    
    auto bnbMain = std::make_shared<BanubaInstance>();
    bnbInstance = bnbMain;

    void* playerHandle = static_cast<void*>(bnbInstance.get());
    
    bnbMain->player = player;
    auto input = bnb::player_api::photo_input::create();
    bnbMain->input = input;
    return playerHandle;
}

__declspec(dllexport) void loadEffect(const char* effectName) 
{
  bnbInstance->player->load_async(effectName);
}

__declspec(dllexport) void SetProcessSize(int stride, int width, int height, int channels, unsigned char** output_image, int* output_size) {

  bnbInstance->stride = stride;
  bnbInstance->width = width;
  bnbInstance->height = height;
  bnbInstance->channels = channels;

  // Create frame output with callback
  auto frame_output = bnb::player_api::opengl_frame_output::create([output_image, output_size](const bnb::full_image_t& pb) 
  {
    if (bnbInstance->output_image == nullptr) {
      bnbInstance->output_size = pb.get_width() * pb.get_height() * pb.get_bytes_per_pixel();
      bnbInstance->output_image = new unsigned char[bnbInstance->output_size];
      *output_size = bnbInstance->output_size;
      *output_image = bnbInstance->output_image;
    }
    std::memcpy(bnbInstance->output_image, pb.get_base_ptr(), bnbInstance->output_size);
    //std::printf("Output image ok \n");
    }, bnb::pixel_buffer_format::bpc8_rgba);

  bnbInstance->player->use(bnbInstance->input).use(frame_output);
  bnbInstance->player->set_render_mode(bnb::player_api::player::render_mode::manual);

  const unsigned char* input_image = new unsigned char[height * stride];
  auto image = bnb::full_image_t::create_bpc8(
    input_image,
    stride,
    width,
    height,
    bnb::pixel_buffer_format::bpc8_rgb,
    bnb::camera_orientation::deg_0,
    false,
    [input_image](uint8_t* data) { delete[] input_image; }
  );
  // Load image data
  //input->load(input_image, width, height, channels);
  bnbInstance->input->push(image);
  bnbInstance->player->render();
}
__declspec(dllexport) int ProcessImage(void* player_handle, const unsigned char* input_image)
{
    auto image = bnb::full_image_t::create_bpc8(
            input_image,
            bnbInstance->stride,
            bnbInstance->width,
            bnbInstance->height,
            bnb::pixel_buffer_format::bpc8_rgb,
            bnb::camera_orientation::deg_0,
            false,
      [](uint8_t* data) { /* deleting the image data */ }
        );
    bnbInstance->input->push(image);
    bnbInstance->player->render();
    return 0;
}

__declspec(dllexport) void ReleaseImage(unsigned char* image_data)
{
    delete[] image_data;
}

__declspec(dllexport) void Cleanup(void* player_handle)
{
    // Erase the stored shared_ptr to clean up resources
    //playerInstances.erase(static_cast<void*>(player_handle));
    //delete bnbutil;
    //bnb::interfaces::utility_manager::release();
}

}
