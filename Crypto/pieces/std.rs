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
    println!("{:?}", key);
    println!("{:?}", iv);

    let plaintext = *b"NOCTF{m7_r4nd0m_1s_s0_e4sy_t0_Cr4ck_r1gh7?}";
    println!("{:?}", plaintext);
    // let ciphertext = hex!("35fb6e44d3e49272c662e6fb4375181aaee9a54a32438ec7fd1807d9ae05d16af58f256cffd2567fcc0a04e67948add8");


    let mut buf = [0u8; 48];
    let ct = Aes128CbcEnc::new(&key.into(), &iv.into())
        .encrypt_padded_b2b_mut::<Pkcs7>(&plaintext, &mut buf)
        .unwrap();
    println!("{:?}", ct);
    // assert_eq!(ct,ciphertext);


    // let key = hex!("2677f8494d5720f9ce249e4e9378d692");
    // let iv=hex!("8d9e8f7f60c5c32c979e59ce68c7dfbc");
    // println!("{:?}", key);
    // println!("{:?}", iv);
    // let ct = hex!("35fb6e44d3e49272c662e6fb4375181aaee9a54a32438ec7fd1807d9ae05d16af58f256cffd2567fcc0a04e67948add8");

    let mut buf = [0u8; 48];
    let pt = Aes128CbcDec::new(&key.into(), &iv.into())
        .decrypt_padded_b2b_mut::<Pkcs7>(&ct, &mut buf)
        .unwrap();
    println!("{:?}", pt)
}
