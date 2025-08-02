import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.util.stream.IntStream;

public class TestIO {
    private static void processData(InputStream source, OutputStream destination) throws IOException {
        final int BUF_SIZE = 8192;
        byte[] buffer = new byte[BUF_SIZE];
        int bytesRead;
        // Use source.read() if short reads are okay (need to check if bytesRead == -1)
        while ((bytesRead = source.readNBytes(buffer, 0, BUF_SIZE)) != 0) {
            for (int i = 0; i < bytesRead; i++) {
                buffer[i] ^= 0x5a;
            }
            destination.write(buffer, 0, bytesRead);
            destination.flush();
        }

        // Manual way to prevent short reads until EOF:
        // while (true) {
        //     int totalBytesRead = 0;
        //     while (totalBytesRead < BUF_SIZE) {
        //         bytesRead = source.read(buffer, totalBytesRead, BUF_SIZE - totalBytesRead);
        //         if (bytesRead == -1) {
        //             if (totalBytesRead > 0) {
        //                 IntStream.range(0, totalBytesRead).forEach(i -> buffer[i] ^= 0x5a);
        //                 destination.write(buffer, 0, totalBytesRead);
        //                 destination.flush();
        //             }
        //             return;
        //         }
        //         totalBytesRead += bytesRead;
        //     }
        //     IntStream.range(0, BUF_SIZE).forEach(i -> buffer[i] ^= 0x5a);
        //     destination.write(buffer);
        //     destination.flush();
        // }
    }

    public static void main(String[] args) throws IOException {
        try(
            FileInputStream src = new FileInputStream("input.bin");
            FileOutputStream dst = new FileOutputStream("output.bin");
        ) {
            processData(src, dst);
            System.out.println("File written to successfully.");
        }

        byte[] inMemory = "Hello this is some random text!".getBytes(StandardCharsets.UTF_8);
        ByteArrayInputStream byteInput = new ByteArrayInputStream(inMemory);
        ByteArrayOutputStream byteOutput = new ByteArrayOutputStream();
        processData(byteInput, byteOutput);
        System.out.println("Bytes copied successfully.");
        byte[] result = byteOutput.toByteArray();
        IntStream.range(0, result.length).forEach(i -> System.out.printf("%02x", result[i]));
        System.out.println();

        // For a network socket, use clientSocket.getInputStream() and clientSocket.getOutputStream()
    }
}
