#include "crypto_wrapper.hpp"
#include <stdexcept>
#include <cstring>

namespace collaboration {

CryptoWrapper::CryptoWrapper(const std::vector<uint8_t>& key) {
    if (key.empty()) {
        throw std::invalid_argument("Key cannot be empty");
    }
    
    ctx_ = crypto_context_new(key.data(), key.size());
    if (!ctx_) {
        throw std::runtime_error("Failed to create crypto context");
    }
}

CryptoWrapper::~CryptoWrapper() {
    if (ctx_) {
        crypto_context_free(ctx_);
    }
}

CryptoWrapper::CryptoWrapper(CryptoWrapper&& other) noexcept 
    : ctx_(other.ctx_) {
    other.ctx_ = nullptr;
}

CryptoWrapper& CryptoWrapper::operator=(CryptoWrapper&& other) noexcept {
    if (this != &other) {
        if (ctx_) {
            crypto_context_free(ctx_);
        }
        ctx_ = other.ctx_;
        other.ctx_ = nullptr;
    }
    return *this;
}

std::vector<uint8_t> CryptoWrapper::encrypt(const std::vector<uint8_t>& plaintext) {
    if (!ctx_ || plaintext.empty()) {
        return {};
    }
    
    uint8_t* ciphertext_raw = nullptr;
    size_t ciphertext_len = 0;
    
    CryptoError err = crypto_encrypt(
        ctx_,
        plaintext.data(),
        plaintext.size(),
        &ciphertext_raw,
        &ciphertext_len
    );
    
    if (err != 0) {  // Success is 0
        throw std::runtime_error("Encryption failed");
    }
    
    // Take ownership of the buffer
    ScopedCBuffer scoped_buffer(ciphertext_raw);
    
    // Copy to vector
    std::vector<uint8_t> result(ciphertext_len);
    std::memcpy(result.data(), scoped_buffer.get(), ciphertext_len);
    
    return result;
}

} // namespace collaboration
