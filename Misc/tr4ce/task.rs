use std::hash::Hash;
use chacha20::ChaCha20;
use chacha20::cipher::{KeyIvInit, StreamCipher, StreamCipherSeek};
use hex_literal::hex;


let key = [0x11; 32];
let nonce = [0x22; 12];
let ciphertext = hex!("096f2d1b6427e90856bbf62d38dc4c52b206d858554d02082f9c053538396335b13ac06fd03df798");

let mut cipher = ChaCha20::new(&key.into(), &nonce.into());

let mut buffer = ciphertext.clone();

cipher.seek(114514);
for chunk in buffer.chunks_mut(3) {
    cipher.apply_keystream(chunk);
}
assert_eq!(buffer, topSecret)

// TODO further more auth
