#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "BanubaWrapper.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>

// Function to read an image file into a byte array
std::vector<unsigned char> ReadImageFile(const std::string& filePath, int& width, int& height, int& channels)
{
    // Use a library like stb_image.h to load the image (assuming it is available in your project)
    unsigned char* image = stbi_load(filePath.c_str(), &width, &height, &channels, 3);  // Load as 24-bit RGB

    if (!image)
    {
        std::cerr << "Failed to load image: " << filePath << std::endl;
        return std::vector<unsigned char>();
    }

    std::vector<unsigned char> imageData(image, image + (width * height * 3));
    stbi_image_free(image);  // Free the image memory

    return imageData;
}

// Function to write a byte array to an image file
void WriteImageFile(const std::string& filePath, const unsigned char* imageData, int width, int height, int channels)
{
    // Use a library like stb_image_write.h to save the image (assuming it is available in your project)
    stbi_write_png(filePath.c_str(), width, height, channels, imageData, width * channels);
}
std::string readFileToString(const std::string& filePath) {
  std::ifstream file(filePath);
  if (!file) {
    std::cerr << "Unable to open file: " << filePath << std::endl;
    return "";
  }

  std::ostringstream oss;
  oss << file.rdbuf();  // Read the entire file buffer into the string stream
  return oss.str();     // Convert the string stream to a string
}
void ImageCallback(const unsigned char* imageData, int width, int height) {
  std::cout << "ImageCallback " << width << " x " << height << std::endl;
  ReleaseImage(imageData);
}
int main()
{
    std::cout<<"Initialize the Banuba SDK" << std::endl;

    const char* resourcesPath = "D:\\projects\\banuba\\quickstart-desktop-cpp\\resources";
    auto token = readFileToString("banuba_token.txt");
    const char* clientToken = token.c_str();
    std::cout<<"InitializeBanubaSDK" << std::endl;

    //void* playerHandle = initializeBanuba(resourcesPath, resourcesFolder, clientToken);
    initializeBanuba(resourcesPath, clientToken);
    std::cout<<"InitializeBanubaSDK Done" << std::endl;
 
    /*if (!playerHandle)
    {
        std::cerr << "Failed to initialize Banuba SDK." << std::endl;
        return -1;
    }*/

    //// Load input image
    //int width, height, channels;
    //    std::cout<<"ReadImageFile" << std::endl;

    //std::vector<unsigned char> inputImage = ReadImageFile("face720x1280.jpg", width, height, channels);
    //    std::cout<<"ReadImageFile Done" << std::endl;

    ////WriteImageFile("testinput.jpg", inputImage.data(), width, height, channels);
    ////if (inputImage.empty())
    ////{
    ////    std::cerr << "Failed to read input image." << std::endl;
    ////    //Cleanup(playerHandle);
    ////    return -1;
    ////}

    //// Process the image
    //unsigned char* outputImage = nullptr;
    //int outputSize = 0;
    //int stride = width * channels;
    loadEffect("effects/TrollGrandma");

    attachCamera();
    
    //startRenderingGLFW();
    RegisterImageCallback(ImageCallback);
    startRenderingBuffer();
    //SetProcessSize(stride, width, height, channels, &outputImage, &outputSize);

    //using std::chrono::high_resolution_clock;
    //using std::chrono::duration_cast;
    //using std::chrono::duration;
    //using std::chrono::milliseconds;
    //int result = 0;
    //{
    //  auto t1 = high_resolution_clock::now();

    //  result = ProcessImage(playerHandle, inputImage.data());

    //  auto t2 = high_resolution_clock::now();

    //  /* Getting number of milliseconds as an integer. */
    //  auto ms_int = duration_cast<milliseconds>(t2 - t1);
    //  std::cout << "Processing time: " << ms_int.count() << " ms " << std::endl;
    //}
    //if (result != 0)
    //{
    //    std::cerr << "Failed to process image." << std::endl;
    //    Cleanup(playerHandle);
    //    return -1;
    //}

    //// Save the output image
    //WriteImageFile("output_image.png", outputImage, width, height, 4);  // 4 channels for RGBA output

    //std::cout<< "Release the output image memory" << std::endl;
    //ReleaseImage(outputImage);

    //std::cout<< "Clean up the Banuba SDK resources" << std::endl;
    //Cleanup(playerHandle);
    do
    {
      std::cout << '\n' << "Press a key to continue...";
    } while (std::cin.get() != '\n');
    std::cout << "Image processing completed successfully." << std::endl;
    return 0;
}
