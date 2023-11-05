use rand_mt::Mt;
use std::fs::File;
use std::io::{Read, Write};
use rand::RngCore;
use aes::cipher::{
    block_padding::Pkcs7,KeyInit, generic_array::GenericArray, KeyIvInit
};
use hex_literal::hex;
use cbc::cipher;
use cbc::cipher::{BlockDecryptMut, BlockEncryptMut};


fn main() {
    let mut safe_rng = rand::rngs::OsRng;
    let mut rng = Mt::new(safe_rng.next_u32());

    let mut list1: Vec<u8> = vec![0; 1248];
    let mut list2: Vec<u32> = Vec::new();
    let mut list3: Vec<u64> = Vec::new();

    rng.fill_bytes(&mut list1);
    for _ in 0..156{
        let rand32 = rng.next_u32();
        list2.push(rand32);
    }
    for _ in 0..78{
        let rand64 = rng.next_u64();
        list3.push(rand64);
    }

    let mut file = File::create("task.txt").expect("Error creating file");
    for rand8 in list1 {
        writeln!(file, "{:x}", rand8).expect("Error writing to file");
    }
    for rand32 in list2 {
        writeln!(file, "{:x}", rand32).expect("Error writing to file");
    }
    for rand64 in &list3 {
        let _mask_a:u64 = 0xf0f0f0f0f0f0f0f0;
        writeln!(file, "{:x}", rand64 & _mask_a).expect("Error writing to file");
    }
    for rand64 in &list3 {
        let _mask_b:u64 = 0x0f0f0f0f0f0f0f0f;
        writeln!(file, "{:x}", rand64 & _mask_b).expect("Error writing to file");
    }



    type Aes128CbcEnc = cbc::Encryptor<aes::Aes128>;
    type Aes128CbcDec = cbc::Decryptor<aes::Aes128>;

    let mut buf = [0; 16];
    rng.fill_bytes(&mut buf);
    let key = GenericArray::from(buf);
    let mut buf = [0; 16];
    rng.fill_bytes(&mut buf);
    let iv = GenericArray::from(buf);

    let plaintext = *b"NOCTF{FAKE_FLAG}";
    let ciphertext = hex!("d37a3dbf0a224a489f9ceb9f3777f510a14072bbb0a663865c3e1cd5361a839bdf2d54b9bf03fceccdf247abb8a08a1a");


    let mut buf = [0u8; 48];
    let ct = Aes128CbcEnc::new(&key.into(), &iv.into())
        .encrypt_padded_b2b_mut::<Pkcs7>(&plaintext, &mut buf)
        .unwrap();
    assert_eq!(ct,ciphertext);

}
