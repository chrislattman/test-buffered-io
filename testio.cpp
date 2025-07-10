#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

void process_data(std::istream& source, std::ostream& destination) {
    char buffer[8192];
    while (true) {
        source.read(buffer, sizeof(buffer));
        std::streamsize bytes_read = source.gcount();
        if (bytes_read == 0) {
            break;
        }
        if (!source && !source.eof()) {
            throw std::runtime_error("Read error");
        }
        std::transform(std::begin(buffer), std::begin(buffer) + bytes_read, std::begin(buffer), [](char c) { return c ^ 0x5a; });
        destination.write(buffer, bytes_read);
        if (!destination) {
            throw std::runtime_error("Write error");
        }
        destination.flush();
    }
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
