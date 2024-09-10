using System;
using System.Runtime.InteropServices;

public static class BanubaInterop
{
    // Define the callback delegate type that matches the C++ function pointer signature
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void ImageCallbackDelegate(IntPtr imageData, int width, int height);

    // Import the C++ functions
    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void RegisterImageCallback(ImageCallbackDelegate callback);

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void initializeBanuba(string sdkPath, string resourcesFolder, string clientToken);

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void loadEffect(string effectName);

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void startRenderingGLFW();
    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void startRenderingBuffer();

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void stopRendering();

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void playRendering();

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void pushImageFromByteArray(byte[] imageData, int stride, int width, int height);

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void releaseBanuba();  // New release function
    
    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void attachCamera();  // New release function

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void ReleaseImage(IntPtr imageData);
}
