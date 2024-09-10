using System;
using System.Runtime.InteropServices;

public static class BanubaInterop
{
    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void initializeBanuba(string sdkPath, string resourcesFolder, string clientToken);

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void loadEffect(string effectName);

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void startRendering();

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void stopRendering();

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void pushImageFromByteArray(byte[] imageData, int width, int height, int format);

    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void releaseBanuba();  // New release function
    [DllImport("BanubaWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void attachCamera();  // New release function
}
