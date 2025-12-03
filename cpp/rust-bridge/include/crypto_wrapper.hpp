#pragma once

#include <memory>
#include <vector>
#include <string>

// Forward declaration of C types
extern "C" {
    struct CryptoContext;
    enum CryptoError : int;
    CryptoContext* crypto_context_new(const uint8_t* key_data, size_t key_len);
    void crypto_context_free(CryptoContext* ctx);
    CryptoError crypto_encrypt(CryptoContext* ctx,
                              const uint8_t* plaintext,
                              size_t plaintext_len,
                              uint8_t** ciphertext_out,
                              size_t* ciphertext_len_out);
    void crypto_free_buffer(uint8_t* buffer);
}

namespace collaboration {

class CryptoWrapper {
public:
    explicit CryptoWrapper(const std::vector<uint8_t>& key);
    ~CryptoWrapper();
    
    // Delete copy constructor/assignment
    CryptoWrapper(const CryptoWrapper&) = delete;
    CryptoWrapper& operator=(const CryptoWrapper&) = delete;
    
    // Allow move
    CryptoWrapper(CryptoWrapper&& other) noexcept;
    CryptoWrapper& operator=(CryptoWrapper&& other) noexcept;
    
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext);
    
private:
    CryptoContext* ctx_{nullptr};
};

// RAII wrapper for C buffer
class ScopedCBuffer {
public:
    ScopedCBuffer(uint8_t* ptr) : ptr_(ptr) {}
    ~ScopedCBuffer() {
        if (ptr_) {
            crypto_free_buffer(ptr_);
        }
    }
    
    // Disallow copy
    ScopedCBuffer(const ScopedCBuffer&) = delete;
    ScopedCBuffer& operator=(const ScopedCBuffer&) = delete;
    
    // Allow move
    ScopedCBuffer(ScopedCBuffer&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }
    
    uint8_t* get() const { return ptr_; }
    
private:
    uint8_t* ptr_{nullptr};
};

} // namespace collaboration
