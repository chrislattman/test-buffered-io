package main

import (
	"bytes"
	"encoding/hex"
	"fmt"
	"io"
	"os"
)

func processData(source io.Reader, destination io.Writer) error {
	const BUF_SIZE int = 8192
	var buffer [BUF_SIZE]byte
	var bytesRead int
	var err error
	for {
		totalBytesRead := 0
		for totalBytesRead < BUF_SIZE {
			bytesRead, err = source.Read(buffer[totalBytesRead:])
			if bytesRead == 0 {
				if totalBytesRead > 0 {
					for i := range totalBytesRead {
						buffer[i] ^= 0x5a
					}
					_, err = destination.Write(buffer[:totalBytesRead])
					if err != nil {
						return err
					}
					return nil
				}
			}
			totalBytesRead += bytesRead
		}
		for i := range BUF_SIZE {
			buffer[i] ^= 0x5a
		}
		_, err = destination.Write(buffer[:])
		if err != nil {
			return err
		}
	}

	// If short reads are okay:
	// for {
	// 	bytesRead, err := source.Read(buffer[:])
	// 	if bytesRead == 0 {
	// 		break
	// 	}
	// 	if err != nil {
	// 		return err
	// 	}
	// 	for i := range bytesRead {
	// 		buffer[i] ^= 0x5a
	// 	}
	// 	_, err = destination.Write(buffer[:bytesRead])
	// 	if err != nil {
	// 		return err
	// 	}
	// }
	// return nil
}

func main() {
	src, err := os.Open("input.bin")
	if err != nil {
		panic(err)
	}
	dst, err := os.Create("output.bin")
	if err != nil {
		panic(err)
	}
	defer src.Close()
	defer dst.Close()
	err = processData(src, dst)
	if err != nil {
		panic(err)
	}
	fmt.Println("File written to successfully.")

	in_memory := []byte("Hello this is some random text!")
	byteInput := bytes.NewReader(in_memory)
	var byteOutput bytes.Buffer
	err = processData(byteInput, &byteOutput)
	if err != nil {
		panic(err)
	}
	fmt.Println("Bytes copied successfully.")
	result := byteOutput.Bytes()
	fmt.Println(hex.EncodeToString(result))

	// net.Conn already implements io.Reader and io.Writer
}
