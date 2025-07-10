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
        byte[] buffer = new byte[8192];
        int bytesRead;
        while ((bytesRead = source.read(buffer)) != -1) {
            IntStream.range(0, bytesRead).forEach(i -> buffer[i] ^= 0x5a);
            destination.write(buffer, 0, bytesRead);
            destination.flush();
        }
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
