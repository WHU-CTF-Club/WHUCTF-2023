use std::hash::Hash;
use chacha20::ChaCha20;
use chacha20::cipher::{KeyIvInit, StreamCipher, StreamCipherSeek};
use hex_literal::hex;

fn main() {
    let key = [0x42; 32];
    let nonce = [0x24; 12];
    let plaintext = hex!("4e4f4354467b7430705f7365633372745f666f525f616c315f316e5f4d69344043306d70616e597d");
    println!("{:?}", plaintext);
    let ciphertext = hex!("aa4b23390d6c44841f589d465c10de757d2d5dec1ed5cf1ba0fdbb236c22be9de7c3e0b67e6d80e8");
    println!("{:?}", ciphertext);

// Key and IV must be references to the `GenericArray` type.
// Here we use the `Into` trait to convert arrays into it.
    let mut cipher = ChaCha20::new(&key.into(), &nonce.into());

    let mut buffer = plaintext.clone();
    cipher.seek(114514);
// apply keystream (encrypt)
    cipher.apply_keystream(&mut buffer);
    println!("{:?}", buffer);
    // assert_eq!(buffer, ciphertext);

    let ciphertext = buffer.clone();

// ChaCha ciphers support seeking
    cipher.seek(114514);

// decrypt ciphertext by applying keystream again
    cipher.apply_keystream(&mut buffer);
    println!("{:?}", buffer);
    assert_eq!(buffer, plaintext);

// stream ciphers can be used with streaming messages
    cipher.seek(114514);
    for chunk in buffer.chunks_mut(3) {
        cipher.apply_keystream(chunk);
    }
    println!("{:?}", buffer);
    assert_eq!(buffer, ciphertext);
}
