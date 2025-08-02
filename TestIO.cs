using System.Text;

class TestIO
{
    private static void ProcessData(Stream source, Stream destination)
    {
        const int BUF_SIZE = 8192;
        byte[] buffer = new byte[BUF_SIZE];
        int bytesRead;
        while (true)
        {
            int totalBytesRead = 0;
            while (totalBytesRead < BUF_SIZE)
            {
                bytesRead = source.Read(buffer, totalBytesRead, BUF_SIZE - totalBytesRead);
                if (bytesRead == 0)
                {
                    if (totalBytesRead > 0)
                    {
                        for (int i = 0; i < totalBytesRead; i++)
                        {
                            buffer[i] ^= 0x5A;
                        }
                        destination.Write(buffer, 0, totalBytesRead);
                        destination.Flush();
                    }
                    return;
                }
                totalBytesRead += bytesRead;
            }
            for (int i = 0; i < BUF_SIZE; i++)
            {
                buffer[i] ^= 0x5A;
            }
            destination.Write(buffer);
            destination.Flush();
        }

        // If short reads are okay:
        // while ((bytesRead = source.Read(buffer)) > 0)
        // {
        //     Enumerable.Range(0, bytesRead).ToList().ForEach(i => buffer[i] ^= 0x5A);
        //     destination.Write(buffer, 0, bytesRead);
        //     destination.Flush();
        // }
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
