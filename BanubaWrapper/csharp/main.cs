class Program
{
    static void Main(string[] args)
    {
        string resourcesFolder = "D:/projects/banuba/quickstart-desktop-cpp/resources";
        string clientToken = File.ReadAllText("banuba_token.txt");
        
        BanubaInterop.initializeBanuba(resourcesFolder, clientToken);
        BanubaInterop.loadEffect("effects/TrollGrandma");
        BanubaInterop.attachCamera();
        BanubaInterop.startRenderingGLFW();

        //// Example byte array (replace this with actual image data)
        //byte[] imageData = new byte[1920 * 1080 * 4]; // Assuming a 1920x1080 RGBA image
        //int width = 1920;
        //int height = 1080;
        //int format = 1; // 1 for RGBA, 2 for BGRA

        //// Push image to Banuba input
        //BanubaInterop.pushImageFromByteArray(imageData, width, height, format);

        Console.WriteLine("Press Enter to stop...");
        Console.ReadLine();

        BanubaInterop.stopRendering();
        BanubaInterop.releaseBanuba();  // Release resources
    }
}