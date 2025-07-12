using System.Text;

class TestIO
{
    private static void ProcessData(Stream source, Stream destination)
    {
        byte[] buffer = new byte[8192];
        int bytesRead;
        while ((bytesRead = source.Read(buffer, 0, buffer.Length)) > 0)
        {
            for (int i = 0; i < bytesRead; i++)
            {
                buffer[i] ^= 0x5A;
            }
            destination.Write(buffer, 0, bytesRead);
            destination.Flush();
        }
    }

    static void Main(string[] args)
    {
        using (FileStream src = new("input.bin", FileMode.Open, FileAccess.Read))
        using (FileStream dst = new("output.bin", FileMode.Create, FileAccess.Write))
        {
            ProcessData(src, dst);
            Console.WriteLine("File written to successfully.");
        }

        byte[] inMemory = Encoding.UTF8.GetBytes("Hello this is some random text!");
        using MemoryStream byteInput = new(inMemory);
        using MemoryStream byteOutput = new();
        ProcessData(byteInput, byteOutput);
        Console.WriteLine("Bytes copied successfully.");
        byte[] result = byteOutput.ToArray();
        Console.WriteLine(string.Join("", result.Select(b => $"{b:x2}")));

        // For a network socket, use client.getStream()
    }
}
