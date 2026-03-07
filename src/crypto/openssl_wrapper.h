#pragma once

#include <array>
#include <utility>
#include <memory>

#include <openssl/evp.h>
#include <openssl/sha.h>


namespace shadow
{

namespace network
{

namespace crypto
{

struct diffie_hellman_X25519_traits_t
{
    diffie_hellman_X25519_traits_t() = delete;
    static constexpr uint8_t ret_size_b = 32;
    using ret_pub_key_t = std::array<std::byte, ret_size_b>;
    using ret_priv_key_t = ret_pub_key_t;
    using ret_val_t = std::pair<ret_pub_key_t, ret_priv_key_t>;
};

struct hash_sha256_hmac_traits_t
{
    /* SHA256 + HMAC-SHA256 section*/
    hash_sha256_hmac_traits_t() = delete;
    static constexpr uint8_t ret_size_b = 32;
    static constexpr uint8_t HMAC_SHA256_ret_size_b = ret_size_b;
    using ret_val_t = std::array<std::byte, ret_size_b>;
};


class CryptoProvider
{

/**
 * TODO: add SHA-256, HMAC based on sha-256
 *
 */
public:
    template <typename DH_ALGO>
    static auto get_pub_priv_keys() -> typename DH_ALGO::ret_val_t
    {
        /* Return value */
        typename DH_ALGO::ret_val_t ret_val {};

        /* Allocates public key algorithm context */
        EVP_PKEY_CTX* ctx = nullptr;
        if constexpr (std::is_same_v<DH_ALGO, diffie_hellman_X25519_traits_t>()) {
            ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_X25519, nullptr);
        } else {
            // error
        }

        /* Initializes the public key algorithn context for a key generation operation */
        EVP_PKEY_keygen_init(ctx);

        /* Generate the public key + private */
        EVP_PKEY* pkey = nullptr;
        EVP_PKEY_keygen(ctx, &pkey);

        /* Get public key */
        size_t len_pub = sizeof(ret_val.first);
        EVP_PKEY_get_raw_public_key(pkey, reinterpret_cast<uint8_t*>(ret_val.first.data()), &len_pub);

        /* Get private key */
        size_t len_priv = sizeof(ret_val.second);
        EVP_PKEY_get_raw_public_key(pkey, reinterpret_cast<uint8_t*>(ret_val.second.data()), &len_priv);

        return ret_val;
    }

    template <typename HASH_ALGO>
    static auto get_hash(const uint8_t* plaintext_p, size_t len) -> typename HASH_ALGO::ret_val_t
    {
        /* Return value */
        typename HASH_ALGO::ret_val_t ret_val {};
        uint32_t hash_len {};

        /* Allocates public key algorithm context */
        EVP_MD_CTX* ctx = nullptr;
        if constexpr (std::is_same_v<HASH_ALGO, hash_sha256_hmac_traits_t>()) {
            EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
        } else {
            // error
        }

        EVP_DigestUpdate(ctx, plaintext_p, len);
        EVP_DigestFinal_ex(ctx, ret_val.data(), &hash_len);

        EVP_MD_CTX_free(ctx);
        ret_val.resize(hash_len);
        return ret_val;
    }
};
}
}
}