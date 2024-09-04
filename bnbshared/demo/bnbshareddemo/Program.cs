// See https://aka.ms/new-console-template for more information
using System;
using System.Drawing;
using System.Drawing.Imaging;
class Program
{
    static void Main()
    {
        string resourcesPath = @"D:\projects\banuba\quickstart-desktop-cpp\resources";
        string clientToken = @"";

        using (ImageProcessor processor = new ImageProcessor(resourcesPath, clientToken))
        {
            // Load input Bitmap (make sure it's 24-bit or will be converted to 24-bit)
            Bitmap inputBitmap = new Bitmap("face720x1280.jpg");

            // Process the image
            Bitmap outputBitmap = processor.ProcessBitmap(inputBitmap);

            // Save the output Bitmap
            outputBitmap.Save("output_image_csharp.png", ImageFormat.Png);

            Console.WriteLine("Image processing completed successfully.");
        }
    }
}
