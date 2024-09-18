// BanubaWrapper.h
#ifdef _WIN32
#define BANUBA_API __declspec(dllexport)
#else
#define BANUBA_API
#endif

extern "C" {
    // Define a callback type that takes a byte array, width, and height
    typedef void (*ImageCallbackType)(const unsigned char* imageData, int width, int height);

    // Function to register the callback
    BANUBA_API void RegisterImageCallback(ImageCallbackType callback);

    BANUBA_API void initializeBanuba(const char* resourcesFolder, const char* clientToken);
    BANUBA_API void loadEffect(const char* effectName);
    BANUBA_API void startRenderingGLFW();
    BANUBA_API void startRenderingBuffer(int pixelFormat);

    BANUBA_API void stopRendering();
    BANUBA_API void playRendering();
    BANUBA_API void pushImageFromByteArray(const unsigned char* imageData, int stride, int width, int height, int pixelformat);
    BANUBA_API void releaseBanuba();  // New release function
    BANUBA_API void attachCamera();

    BANUBA_API void ReleaseImage(const unsigned char* image_data);
}
