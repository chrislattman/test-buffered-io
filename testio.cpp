#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

void process_data(std::istream& source, std::ostream& destination) {
    const std::streamsize BUF_SIZE = 8192;
    char buffer[BUF_SIZE];
    std::streamsize bytes_read;
    while (true) {
        std::streamsize total_bytes_read = 0;
        while (total_bytes_read < BUF_SIZE) {
            source.read(buffer, BUF_SIZE);
            if (!source && !source.eof()) {
                throw std::runtime_error("Read error");
            }
            bytes_read = source.gcount();
            if (bytes_read == 0) {
                if (total_bytes_read > 0) {
                    for (std::streamsize i = 0; i < total_bytes_read; i++) {
                        buffer[i] ^= 0x5a;
                    }
                    destination.write(buffer, total_bytes_read);
                    if (!destination) {
                        throw std::runtime_error("Write error");
                    }
                    destination.flush();
                }
                return;
            }
            total_bytes_read += bytes_read;
        }
        for (std::streamsize i = 0; i < BUF_SIZE; i++) {
            buffer[i] ^= 0x5a;
        }
        destination.write(buffer, BUF_SIZE);
        if (!destination) {
            throw std::runtime_error("Write error");
        }
        destination.flush();
    }

    // If short reads are okay:
    // while (true) {
    //     source.read(buffer, BUF_SIZE);
    //     if (!source && !source.eof()) {
    //         throw std::runtime_error("Read error");
    //     }
    //     bytes_read = source.gcount();
    //     if (bytes_read == 0) {
    //         break;
    //     }
    //     std::transform(std::begin(buffer), std::begin(buffer) + bytes_read, std::begin(buffer), [](char c) { return c ^ 0x5a; });
    //     destination.write(buffer, bytes_read);
    //     if (!destination) {
    //         throw std::runtime_error("Write error");
    //     }
    //     destination.flush();
    // }
}

int main(void) {
    std::ifstream src("input.bin", std::ios::binary);
    if (!src) {
        std::cerr << "Error opening input file" << std::endl;
        return 1;
    }
    std::ofstream dst("output.bin", std::ios::binary);
    if (!dst) {
        std::cerr << "Error opening output file" << std::endl;
        return 1;
    }
    process_data(src, dst);
    std::cout << "File written to successfully." << std::endl;

    std::istringstream byte_input("Hello this is some random text!");
    std::ostringstream byte_output;
    process_data(byte_input, byte_output);
    std::cout << "Bytes copied successfully." << std::endl;
    std::string result = byte_output.str();
    std::for_each(result.begin(), result.end(), [](char c) { std::cout << std::hex << (int)c; });
    std::cout << "\n";

    // No straightforward built-in way to convert a socket descriptor to std::istream or std::ostream
    // However Boost.Asio offers socket stream wrappers https://www.boost.org/doc/libs/latest/doc/html/boost_asio.html
}
