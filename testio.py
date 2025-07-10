import io
from typing import BinaryIO

def process_data(source: BinaryIO, destination: BinaryIO) -> None:
    while True:
        buffer = source.read(8192)
        if len(buffer) == 0:
            break
        buffer = bytes(b ^ 0x5a for b in buffer)
        destination.write(buffer)
        destination.flush()

with open("input.bin", "rb") as src, open("output.bin", "wb") as dst:
    process_data(src, dst)
    print("File written to successfully.")

in_memory = b"Hello this is some random text!"
byte_input = io.BytesIO(in_memory)
byte_output = io.BytesIO()
process_data(byte_input, byte_output)
print("Bytes copied successfully.")
byte_output.seek(0)
result = byte_output.read()
print(result.hex())

# For a network socket, use client_socket.makefile("rwb")
