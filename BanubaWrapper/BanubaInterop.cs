using System;
using System.Runtime.InteropServices;

public static class BanubaInterop
{
    public enum pixel_buffer_format
    {
        bpc8_rgb = 0x0101,  // one plane: RGB
        bpc8_bgr = 0x0102,  // one plane: BGR
        bpc8_rgba = 0x0103, // one plane: RGBA
        bpc8_bgra = 0x0104, // one plane: BGRA
        bpc8_argb = 0x0105, // one plane: ARGB
        nv12 = 0x0201,      // two planes: first RED, second RG
        i420 = 0x0401       // three planes: first RED, second RED, third RED
    }; // enum class pixel_buffer_format

    // Define the callback delegate type that matches the C++ function pointer signature
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void ImageCallbackDelegate(IntPtr imageData, int width, int height);

    // Import the C++ functions
    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void RegisterImageCallback(ImageCallbackDelegate callback);

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void initializeBanuba(string resourcesFolder, string clientToken);

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void loadEffect(string effectName);

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void startRenderingGLFW();
    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void startRenderingBuffer(int pixelformat = (int)pixel_buffer_format.bpc8_rgba);

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void stopRendering();

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void playRendering();

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void pushImageFromByteArray(byte[] imageData, int stride, int width, int height, int pixelformat = (int)pixel_buffer_format.bpc8_rgb);

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void releaseBanuba();  // New release function
    
    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void attachCamera();  // New release function

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void ReleaseImage(IntPtr imageData);
}
