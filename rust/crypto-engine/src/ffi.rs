//! C-compatible FFI interface for crypto operations

use std::os::raw::{c_char, c_void};
use std::ffi::{CStr, CString};
use std::ptr;
use std::slice;

/// Opaque handle to encryption context
#[repr(C)]
pub struct CryptoContext {
    _private: [u8; 0],
}

/// Error codes for FFI
#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub enum CryptoError {
    Success = 0,
    InvalidKey = 1,
    InvalidData = 2,
    MemoryAllocationFailed = 3,
    UnknownError = 4,
}

/// Result type for FFI functions
#[repr(C)]
pub struct CryptoResult<T> {
    error: CryptoError,
    value: T,
}

// FFI-safe functions (no panics, no exceptions)
#[no_mangle]
pub extern "C" fn crypto_context_new(
    key_data: *const u8,
    key_len: usize,
) -> *mut CryptoContext {
    // SAFETY: Must validate all inputs
    if key_data.is_null() || key_len == 0 {
        return ptr::null_mut();
    }
    
    // Create context (simplified example)
    let context = Box::new(CryptoContext { _private: [] });
    Box::into_raw(context)
}

#[no_mangle]
pub extern "C" fn crypto_encrypt(
    ctx: *mut CryptoContext,
    plaintext: *const u8,
    plaintext_len: usize,
    ciphertext_out: *mut *mut u8,
    ciphertext_len_out: *mut usize,
) -> CryptoError {
    // SAFETY: Validate pointers
    if ctx.is_null() || plaintext.is_null() || ciphertext_out.is_null() || ciphertext_len_out.is_null() {
        return CryptoError::InvalidData;
    }
    
    let plaintext_slice = unsafe { slice::from_raw_parts(plaintext, plaintext_len) };
    
    // Actual encryption (simplified)
    let ciphertext: Vec<u8> = plaintext_slice.iter().map(|b| b ^ 0xFF).collect();
    
    // Allocate memory for output (caller must free with crypto_free_buffer)
    let output_ptr = Box::into_raw(ciphertext.into_boxed_slice()) as *mut u8;
    unsafe {
        *ciphertext_out = output_ptr;
        *ciphertext_len_out = ciphertext.len();
    }
    
    CryptoError::Success
}

#[no_mangle]
pub extern "C" fn crypto_free_buffer(buffer: *mut u8) {
    if !buffer.is_null() {
        unsafe {
            // Reconstruct Box to properly deallocate
            let _ = Box::from_raw(buffer);
        }
    }
}

#[no_mangle]
pub extern "C" fn crypto_context_free(ctx: *mut CryptoContext) {
    if !ctx.is_null() {
        unsafe {
            let _ = Box::from_raw(ctx);
        }
    }
}
