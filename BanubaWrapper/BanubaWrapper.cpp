// BanubaWrapper.cpp
#include "BanubaWrapper.h"
#include "BanubaClientToken.hpp"
#include <GlfwRenderer.hpp>
#include <Utils.hpp>
#include <bnb/player_api/interfaces/player.hpp>
#include <bnb/effect_player/utility.hpp>
#include <bnb/player_api/interfaces/render_target/opengl_render_target.hpp>
#include <bnb/player_api/interfaces/render_target/metal_render_target.hpp>
#include <bnb/player_api/interfaces/player/player.hpp>
#include <bnb/player_api/interfaces/input/live_input.hpp>
#include <bnb/spal/camera/base.hpp>
#include <bnb/player_api/interfaces/output/window_output.hpp>
#include <bnb/recognizer/interfaces/utility_manager.hpp>
#include <bnb/player_api/interfaces/output/opengl_frame_output.hpp>

using namespace bnb::interfaces;

namespace {
    render_backend_type render_backend = BNB_APPLE ? render_backend_type::metal : render_backend_type::opengl;
    std::shared_ptr<GLFWRenderer> renderer;
    bnb::player_api::player_sptr player;
    std::shared_ptr<bnb::player_api::live_input> input;
    std::shared_ptr<bnb::player_api::window_output> window_output;
    //std::shared_ptr<bnb::utility> utility;
    bnb::camera_sptr bnb_camera;
    std::shared_ptr<bnb::player_api::opengl_frame_output> frame_output;
    // Global variable to store the callback
    ImageCallbackType g_image_callback = nullptr;
    bnb::player_api::render_target_sptr render_target;
}

void initializeBanuba(const char* sdkPath, const char* clientToken) {
  // Initialize BanubaSDK with token and paths to resources
  bnb::interfaces::utility_manager::initialize(std::initializer_list<std::string>{bnb::sdk_resources_path(), sdkPath}, clientToken);
  // Create render delegate based on GLFW
  renderer = std::make_shared<GLFWRenderer>(render_backend);
  // Create render target
  
  if (render_backend == render_backend_type::opengl) {
    render_target = bnb::player_api::opengl_render_target::create();
  }
  else if (render_backend == render_backend_type::metal) {
    render_target = bnb::player_api::metal_render_target::create();
  }
  // Create player
  player = bnb::player_api::player::create(30, render_target, renderer);
  // Create live input, for realtime camera
  input = bnb::player_api::live_input::create();
}

void loadEffect(const char* effectName) 
{
  if (player) {
    player->load_async(effectName);
  }
}

void startRenderingGLFW() {
  window_output = bnb::player_api::window_output::create(renderer->get_native_surface());
  player->use(input).use(window_output);
    if (renderer) {
      // Setup callbacks for glfw window
      renderer->get_window()->set_callbacks([](uint32_t w, uint32_t h) {
        window_output->set_frame_layout(0, 0, w, h);
        }, nullptr);
        renderer->get_window()->show_window_and_run_events_loop();
    }
}



// Function to register the callback
void RegisterImageCallback(ImageCallbackType callback) {
    g_image_callback = callback;
}

void startRenderingBuffer() {
  // Create frame output with callback
  frame_output = bnb::player_api::opengl_frame_output::create([](const bnb::full_image_t& pb)
    {
      //std::printf("Output image ok \n");
      if (g_image_callback) {
      
        auto output_size = pb.get_width() * pb.get_height() * pb.get_bytes_per_pixel();
        auto output_image = new unsigned char[output_size];
        std::memcpy(output_image, pb.get_base_ptr(), output_size);
        g_image_callback(output_image, pb.get_width(), pb.get_height());
      }
    }, bnb::pixel_buffer_format::bpc8_rgba);

  player->use(input).use(frame_output);
}

void stopRendering() {
    if (player) {
      player->pause();
    }
}
void playRendering() {
  if (player) {
    player->play();
  }
}

void attachCamera() {
  bnb_camera = bnb::create_camera_device([](bnb::full_image_t image) {
    auto now_us = std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
    input->push(image, now_us);
    }, 0);
}

void pushImageFromByteArray(const unsigned char* imageData, int stride, int width, int height) {
    auto image = bnb::full_image_t::create_bpc8(
      imageData,
      stride,
      width,
      height,
      bnb::pixel_buffer_format::bpc8_rgb,
      bnb::camera_orientation::deg_0,
      false,
      [](uint8_t* data) { /* deleting the image data */ }
    );

    auto now_us = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();

    // Push the image to the input
    input->push(image, now_us);
}

void releaseBanuba() {
    g_image_callback = nullptr;
    if (player) player.reset();
    if (renderer) renderer.reset();
    if (input) input.reset();
    if (window_output) window_output.reset();
    if (bnb_camera) bnb_camera.reset();
    if (frame_output) frame_output.reset();
    if (render_target) render_target.reset();
    bnb::interfaces::utility_manager::release();
}
void ReleaseImage(const unsigned char* image_data)
{
  delete[] image_data;
}