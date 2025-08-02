use std::{
    fs::File,
    io::{Cursor, Read, Result, Write},
};

fn process_data<R: Read, W: Write>(source: &mut R, destination: &mut W) -> Result<()> {
    const BUF_SIZE: usize = 8192;
    let mut buffer = [0u8; BUF_SIZE];
    let mut bytes_read;
    loop {
        let mut total_bytes_read = 0;
        while total_bytes_read < BUF_SIZE {
            bytes_read = source.read(&mut buffer[total_bytes_read..])?;
            if bytes_read == 0 {
                if total_bytes_read > 0 {
                    for x in &mut buffer[..total_bytes_read] {
                        *x ^= 0x5a;
                    }
                    destination.write_all(&buffer[..total_bytes_read])?;
                    destination.flush()?;
                }
                return Ok(());
            }
            total_bytes_read += bytes_read;
        }
        for x in &mut buffer {
            *x ^= 0x5a;
        }
        destination.write_all(&buffer)?;
        destination.flush()?;
    }

    // If short reads are okay:
    // loop {
    //     bytes_read = source.read(&mut buffer)?;
    //     if bytes_read == 0 {
    //         break;
    //     }
    //     buffer[..bytes_read].iter_mut().for_each(|x| *x ^= 0x5a);
    //     destination.write_all(&buffer[..bytes_read])?;
    //     destination.flush()?;
    // }
    // Ok(())
}

fn main() {
    let mut src = File::open("input.bin").unwrap();
    let mut dst = File::create("output.bin").unwrap();
    process_data(&mut src, &mut dst).unwrap();
    println!("File written to successfully.");

    let in_memory = b"Hello this is some random text!";
    let mut byte_input = Cursor::new(in_memory);
    let mut byte_output = Cursor::new(Vec::<u8>::new());
    process_data(&mut byte_input, &mut byte_output).unwrap();
    println!("Bytes copied successfully.");
    let result = byte_output.into_inner();
    println!(
        "{}",
        result
            .iter()
            .map(|b| format!("{b:02x}"))
            .collect::<String>()
    );

    // TcpStream already implements std::io::Read and std::io::Write
}
