import io
from typing import BinaryIO

def process_data(source: BinaryIO, destination: BinaryIO) -> None:
    BUF_SIZE = 8192
    buffer = bytearray(BUF_SIZE)
    while True:
        total_bytes_read = 0
        while total_bytes_read < BUF_SIZE:
            curr_buffer = source.read(BUF_SIZE - total_bytes_read)
            if len(curr_buffer) == 0:
                if total_bytes_read > 0:
                    tmp_buffer = bytes(b ^ 0x5a for b in buffer[:total_bytes_read])
                    destination.write(tmp_buffer)
                    destination.flush()
                return
            buffer[total_bytes_read:total_bytes_read + len(curr_buffer)] = curr_buffer
            total_bytes_read += len(curr_buffer)
        tmp_buffer = bytes(b ^ 0x5a for b in buffer)
        destination.write(tmp_buffer)
        destination.flush()

    # If short reads are okay:
    # while True:
    #     curr_buffer = source.read(BUF_SIZE)
    #     if len(curr_buffer) == 0:
    #         break
    #     curr_buffer = bytes(b ^ 0x5a for b in curr_buffer)
    #     destination.write(curr_buffer)
    #     destination.flush()

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
