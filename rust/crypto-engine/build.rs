use std::env;
use std::path::PathBuf;
use cbindgen::Config;

fn main() {
    let crate_dir = env::var("CARGO_MANIFEST_DIR").unwrap();
    let out_dir = env::var("OUT_DIR").unwrap();
    
    // Generate C bindings
    let config = Config::from_file("cbindgen.toml").unwrap_or_default();
    
    cbindgen::generate_with_config(&crate_dir, config)
        .expect("Unable to generate bindings")
        .write_to_file(PathBuf::from(&out_dir).join("crypto_engine.h"));
    
    // Tell Cargo to rerun if source changes
    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=src/ffi.rs");
    println!("cargo:rerun-if-changed=cbindgen.toml");
}
