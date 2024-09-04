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
        //std::shared_ptr<bnb::utility> utility;
        //std::shared_ptr<GLFWRenderer> renderer;
        //bnb::player_api::render_target_sptr render_target;
    };
    std::map<void*, std::shared_ptr<BanubaInstance>> playerInstances;
    //bnb::utility *bnbutil;
    //std::shared_ptr<bnb::utility> bnbutil;
}

extern "C" {

__declspec(dllexport) void* InitializeBanubaSDK(const char* resources_path, const char* client_token)
{
    // Initialize Banuba SDK utility
    //auto utility = std::make_shared<bnb::utility>(std::initializer_list<std::string>{resources_path, BNB_RESOURCES_FOLDER}, client_token);
    //bnbutil = new bnb::utility({resources_path, BNB_RESOURCES_FOLDER}, client_token);
    //bnbutil = std::make_shared<bnb::utility>(std::initializer_list<std::string>{resources_path, BNB_RESOURCES_FOLDER}, client_token);
    bnb::interfaces::utility_manager::initialize(std::initializer_list<std::string>{resources_path, BNB_RESOURCES_FOLDER}, client_token);
    auto renderer = std::make_shared<GLFWRenderer>(render_backend);
    auto render_target = bnb::player_api::opengl_render_target::create();
    
    // Create player using shared pointer and store it in a map
    auto player = bnb::player_api::player::create(30, render_target, renderer);

    // // Create frame output with callback t
    // // that is called when frames are received and saves it to a file.
    // auto frame_output = bnb::player_api::opengl_frame_output::create([player, result_path](const bnb::full_image_t& pb) {
    //     stbi_write_png(
    //         (result_path + ".png").c_str(),
    //         pb.get_width(),
    //         pb.get_height(),
    //         pb.get_bytes_per_pixel(),
    //         pb.get_base_ptr(),
    //         pb.get_bytes_per_row()
    //     );
    //     std::printf("Processing result was written to `%s`. \n", result_path.c_str());
    // }, bnb::pixel_buffer_format::bpc8_rgba);
    // // Sync effect loading
    player->load("effects/TrollGrandma");
    // player->use(input).use(frame_output);
    // // Switch to manual render mode
    // player->set_render_mode(bnb::player_api::player::render_mode::manual);

    void* playerHandle = static_cast<void*>(player.get());
    auto bnbMain = std::make_shared<BanubaInstance>();
    playerInstances[playerHandle] = bnbMain;  // Store the shared_ptr
    bnbMain->player = player;
    //bnbMain->utility = utility;
    //bnbMain->renderer = renderer;
    //bnbMain->render_target = render_target;
    return playerHandle;
}

__declspec(dllexport) int ProcessImage(void* player_handle, const unsigned char* input_image, 
int rgb_stride,
int width, int height, int channels, unsigned char** output_image, int* output_size)
{
    // Retrieve the shared_ptr from the map using the raw pointer
    auto player = playerInstances[static_cast<void*>(player_handle)]->player;
    if (!player) return -1; // Handle invalid player handle

    auto input = bnb::player_api::photo_input::create();
    // bnb::image_format format;
    // format.width = width;
    // format.height = height;
    // format.orientation = bnb::camera_orientation::deg_0;
    // bnb::bpc8_image_t::pixel_format_t pixel_format{bnb::bpc8_image_t::pixel_format_t::rgba};
    // auto color_plane = bnb::color_plane(const_cast<bnb::color_plane_data_t*>(get_pixel_data(frame)), [frame](bnb::color_plane_data_t*) {
    //     // Hold the copy of the frame here, unil deleter called
    // });
    
    // bnb::bpc8_image_t rgba_image(color_plane, pixel_format, format);
    // bnb::full_image_t image(std::move(rgba_image));

    auto image = bnb::full_image_t::create_bpc8(
            input_image,
            rgb_stride,
            width,
            height,
            bnb::pixel_buffer_format::bpc8_rgb,
            bnb::camera_orientation::deg_0,
            false,
            nullptr
        );
    // Load image data
    //input->load(input_image, width, height, channels);
    input->push(image);
    
    // Create frame output with callback
    auto frame_output = bnb::player_api::opengl_frame_output::create([output_image, output_size](const bnb::full_image_t& pb) {
        *output_size = pb.get_width() * pb.get_height() * pb.get_bytes_per_pixel();
        *output_image = new unsigned char[*output_size];
        std::memcpy(*output_image, pb.get_base_ptr(), *output_size);
        //std::printf("Output image ok \n");
    }, bnb::pixel_buffer_format::bpc8_rgba);
    
    player->use(input).use(frame_output);
    player->set_render_mode(bnb::player_api::player::render_mode::manual);
    
    
    player->render();
    
    player->pause();
    player->unuse(nullptr);
    return 0;
}

__declspec(dllexport) void ReleaseImage(unsigned char* image_data)
{
    delete[] image_data;
}

__declspec(dllexport) void Cleanup(void* player_handle)
{
    // Erase the stored shared_ptr to clean up resources
    playerInstances.erase(static_cast<void*>(player_handle));
    //delete bnbutil;
    bnb::interfaces::utility_manager::release();
}

}
