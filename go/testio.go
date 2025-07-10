package main

import (
	"bytes"
	"encoding/hex"
	"fmt"
	"io"
	"os"
)

func processData(source io.Reader, destination io.Writer) error {
	var buffer [8192]byte
	for {
		bytesRead, err := source.Read(buffer[:])
		if bytesRead == 0 {
			break
		}
		if err != nil {
			return err
		}
		for i := range bytesRead {
			buffer[i] ^= 0x5a
		}
		_, err = destination.Write(buffer[:bytesRead])
		if err != nil {
			return err
		}
	}
	return nil
}

func main() {
	src, err := os.Open("input.bin"); if err != nil {
		panic(err)
	}
	dst, err := os.Create("output.bin"); if err != nil {
		panic(err)
	}
	defer src.Close()
	defer dst.Close()
	err = processData(src, dst); if err != nil {
		panic(err)
	}
	fmt.Println("File written to successfully.")

	in_memory := []byte("Hello this is some random text!")
	byteInput := bytes.NewReader(in_memory)
	var byteOutput bytes.Buffer
	err = processData(byteInput, &byteOutput); if err != nil {
		panic(err)
	}
	fmt.Println("Bytes copied successfully.")
	result := byteOutput.Bytes()
	fmt.Println(hex.EncodeToString(result))

	// net.Conn already implements io.Reader and io.Writer
}
