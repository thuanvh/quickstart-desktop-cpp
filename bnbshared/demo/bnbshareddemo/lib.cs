using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.IO;

public class ImageProcessor : IDisposable
{
    private const string DllName = "bnbshared.dll";  // Change this to the name of your compiled DLL

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern IntPtr InitializeBanubaSDK(string resourcesPath, string clientToken);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern int ProcessImage(IntPtr playerHandle, byte[] inputImage, int stride, int width, int height, int channels, out IntPtr outputImage, out int outputSize);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void ReleaseImage(IntPtr imageData);

    [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
    private static extern void Cleanup(IntPtr playerHandle);

    private IntPtr _playerHandle;

    public ImageProcessor(string resourcesPath, string clientToken)
    {
        _playerHandle = InitializeBanubaSDK(resourcesPath, clientToken);
    }

    public Bitmap ProcessBitmap(Bitmap inputBitmap)
    {
        // Convert Bitmap to byte array (24-bit RGB)
        byte[] inputBytes = BitmapToByteArray(inputBitmap, out int stride, out int width, out int height, out int channels);

        // Process the image using the C++ library
        IntPtr outputImagePtr;
        int outputSize;
        int result = ProcessImage(_playerHandle, inputBytes, stride, width, height, channels, out outputImagePtr, out outputSize);

        if (result != 0)
        {
            throw new Exception("Error processing image.");
        }

        // Convert output byte array back to Bitmap
        byte[] outputBytes = new byte[outputSize];
        Marshal.Copy(outputImagePtr, outputBytes, 0, outputSize);
        Bitmap outputBitmap = ByteArrayToBitmap(outputBytes, width, height);

        // Release the unmanaged memory
        ReleaseImage(outputImagePtr);

        return outputBitmap;
    }

    public void Dispose()
    {
        if (_playerHandle != IntPtr.Zero)
        {
            Cleanup(_playerHandle);
            _playerHandle = IntPtr.Zero;
        }
    }

    private byte[] BitmapToByteArray(Bitmap bitmap, out int stride, out int width, out int height, out int channels)
    {
        width = bitmap.Width;
        height = bitmap.Height;
        channels = 3; // RGB format for 24-bit

        Bitmap clone = new Bitmap(bitmap.Width, bitmap.Height, PixelFormat.Format24bppRgb); // Ensure 3-channel format
        using (Graphics g = Graphics.FromImage(clone))
        {
            g.DrawImage(bitmap, new Rectangle(0, 0, bitmap.Width, bitmap.Height));
        }

        BitmapData bitmapData = clone.LockBits(new Rectangle(0, 0, clone.Width, clone.Height), ImageLockMode.ReadOnly, clone.PixelFormat);
        stride = bitmapData.Stride;
        int bytes = stride * clone.Height;
        byte[] byteArray = new byte[bytes];
        Marshal.Copy(bitmapData.Scan0, byteArray, 0, bytes);
        clone.UnlockBits(bitmapData);

        return byteArray;
    }

    private Bitmap ByteArrayToBitmap(byte[] byteArray, int width, int height)
    {
        Bitmap bitmap = new Bitmap(width, height, PixelFormat.Format32bppArgb);
        BitmapData bitmapData = bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height), ImageLockMode.WriteOnly, bitmap.PixelFormat);

        Marshal.Copy(byteArray, 0, bitmapData.Scan0, byteArray.Length);
        bitmap.UnlockBits(bitmapData);

        return bitmap;
    }
}
