using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using static BanubaInterop;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            

        }

        private void btnCamera_Click(object sender, RoutedEventArgs e)
        {
            BanubaInterop.attachCamera();
        }

        private void btnImage_Click(object sender, RoutedEventArgs e)
        {
            int width = 0, height = 0;
            var buffer = ReadImageToRgbBuffer("face720x1280.jpg", ref width, ref height);
            Task.Run(async () =>
            {

                while (true)
                {
                    BanubaInterop.pushImageFromByteArray(buffer, width * 3, width, height, (int)BanubaInterop.pixel_buffer_format.bpc8_bgr);
                    await Task.Delay(30);
                }

            });
        }
        static byte[] ReadImageToRgbBuffer(string imagePath, ref int width, ref int height)
        {
            // Load the image
            using (Bitmap bitmap = new Bitmap(imagePath))
            {
                // Get width and height
                width = bitmap.Width;
                height = bitmap.Height;

                // Create an RGB buffer (3 bytes per pixel)
                byte[] rgbBuffer = new byte[width * height * 3];

                // Lock the bitmap's bits
                BitmapData bmpData = bitmap.LockBits(new System.Drawing.Rectangle(0, 0, width, height), ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format24bppRgb);

                try
                {
                    // Get the pointer to the bitmap's data
                    IntPtr ptr = bmpData.Scan0;

                    // Copy the RGB values into the buffer
                    Marshal.Copy(ptr, rgbBuffer, 0, rgbBuffer.Length);
                }
                finally
                {
                    // Unlock the bits
                    bitmap.UnlockBits(bmpData);
                }

                return rgbBuffer;
            }
        }
        private void btnPlay_Click(object sender, RoutedEventArgs e)
        {
            BanubaInterop.playRendering();
        }

        private void btnPause_Click(object sender, RoutedEventArgs e)
        {
            BanubaInterop.stopRendering();
        }

        private void btnOutputGLWF_Click(object sender, RoutedEventArgs e)
        {
            BanubaInterop.startRenderingGLFW();
        }
        // Callback method to be called from C++
        public void MyImageCallback(IntPtr imageData, int width, int height)
        {
            //// Convert the IntPtr to a byte array
            byte[] managedArray = new byte[width * height * 4]; // Assuming 4 bytes per pixel (e.g., RGBA)
            Marshal.Copy(imageData, managedArray, 0, managedArray.Length);

            BanubaInterop.ReleaseImage(imageData);
            //Console.WriteLine($"Callback called with image of size: {width}x{height}");
            //// Create a BitmapSource from the RGBA buffer
            BitmapSource bitmap = BitmapSource.Create(
                width,                        // Width of the image
                height,                       // Height of the image
                96,                           // DPI (Dots Per Inch) of the image
                96,                           // DPI (Dots Per Inch) of the image
                PixelFormats.Bgra32,          // Pixel format (Bgra32 for RGBA buffer)
                null,                         // Palette (null for RGBA)
                managedArray,                   // Buffer containing pixel data
                width * 4                     // Stride (width * bytes per pixel)
            );
            SaveBitmapSourceAsJpeg(bitmap, "output.jpg");
            //// Set the BitmapSource as the source for the Image control
            Dispatcher.BeginInvoke( new Action(() => { ImageControl.Source = bitmap; }));

            ////// Process the image data here
            ////// Example: Print out the first few bytes
            ////for (int i = 0; i < Math.Min(10, managedArray.Length); i++)
            ////{
            ////    Console.Write($"{managedArray[i]} ");
            ////}
            ////Console.WriteLine();
            //Task.Run(() =>
            //{
            //    try
            //    {
            //        //BanubaInterop.ReleaseImage(imageData);
            //    }catch(Exception ex)
            //    {

            //    }
            //});

        }
        private void SaveBitmapSourceAsJpeg(BitmapSource bitmapSource, string filePath)
        {
            // Create a JpegBitmapEncoder
            JpegBitmapEncoder encoder = new JpegBitmapEncoder();
            encoder.QualityLevel = 90; // Set the quality level (0 to 100)

            // Add the BitmapSource to the encoder's frames
            encoder.Frames.Add(BitmapFrame.Create(bitmapSource));

            // Save the encoded image to a file
            using (FileStream fileStream = new FileStream(filePath, FileMode.Create))
            {
                encoder.Save(fileStream);
            }
        }
        private void btnOutputBuffer_Click(object sender, RoutedEventArgs e)
        {
            // Create an instance of the delegate
            ImageCallbackDelegate callbackDelegate = new ImageCallbackDelegate(MyImageCallback);

            // Register the callback with the C++ library
            RegisterImageCallback(callbackDelegate);

            BanubaInterop.startRenderingBuffer((int)BanubaInterop.pixel_buffer_format.bpc8_bgra);
        }

        private void btnInit_Click(object sender, RoutedEventArgs e)
        {
            string resourcesFolder = "D:/projects/banuba/quickstart-desktop-cpp/resources";
            string clientToken = File.ReadAllText("banuba_token.txt");

            BanubaInterop.initializeBanuba(resourcesFolder, clientToken);
            BanubaInterop.loadEffect("effects/TrollGrandma");

        }
        private void btnRelease_Click(object sender, RoutedEventArgs e)
        {
            BanubaInterop.releaseBanuba();
        }
    }
}