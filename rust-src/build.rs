fn main() {
    println!("cargo:rerun-if-changed=../src/bittendef.h:src/bittenSave.rs");
}