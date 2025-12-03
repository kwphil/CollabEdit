#include "crypto_wrapper.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(CryptoIntegration, BasicEncryption) {
    std::vector<uint8_t> key(32, 0x42);  // 32-byte key
    collaboration::CryptoWrapper crypto(key);
    
    std::string plaintext = "Hello, collaborative world!";
    std::vector<uint8_t> plaintext_vec(plaintext.begin(), plaintext.end());
    
    auto ciphertext = crypto.encrypt(plaintext_vec);
    
    EXPECT_FALSE(ciphertext.empty());
    EXPECT_NE(ciphertext, plaintext_vec);
}

TEST(CryptoIntegration, EmptyInput) {
    std::vector<uint8_t> key(32, 0x42);
    collaboration::CryptoWrapper crypto(key);
    
    std::vector<uint8_t> empty;
    auto result = crypto.encrypt(empty);
    EXPECT_TRUE(result.empty());
}
