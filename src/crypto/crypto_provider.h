#pragma once

#include <array>
#include <utility>

#include <openssl/evp.h>
#include <openssl/sha.h>


namespace crypto
{

struct diffie_hellman_X25519_traits_t
{
    diffie_hellman_X25519_traits_t() = delete;
    static constexpr uint8_t ret_size_b = 32;
    using ret_pub_key_t = std::array<uint8_t, ret_size_b>;
    using ret_priv_key_t = ret_pub_key_t;
    using ret_val_t = std::pair<ret_pub_key_t, ret_priv_key_t>;
};

struct hash_sha256_hmac_traits_t
{
    hash_sha256_hmac_traits_t() = delete;
    static constexpr uint8_t ret_size_b = 32;
    using ret_val_t = std::array<uint8_t, ret_size_b>;
};


class CryptoProvider
{
public:
    template <typename DH_ALGO>
    static auto get_pub_priv_keys() -> typename DH_ALGO::ret_val_t
    {
        /* Return value */
        typename DH_ALGO::ret_val_t ret_val {};

        /* Allocates public key algorithm context */
        EVP_PKEY_CTX* ctx = nullptr;
        if constexpr (std::is_same_v<DH_ALGO, diffie_hellman_X25519_traits_t>) {
            ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_X25519, nullptr);
        } else {
            // error
        }

        /* Initializes the public key algorithn context for a key generation operation */
        if (ctx && EVP_PKEY_keygen_init(ctx) <= 0) {
            EVP_PKEY_CTX_free(ctx);
        }

        /* Generate the public key + private */
        EVP_PKEY* pkey = nullptr;
        if (ctx && EVP_PKEY_keygen(ctx, &pkey) <= 0) {
            EVP_PKEY_CTX_free(ctx);
        }

        /* Get public key */
        size_t len_pub = sizeof(ret_val.first);
        if (ctx && EVP_PKEY_get_raw_public_key(pkey,
                                               ret_val.first.data(),
                                               &len_pub) <= 0) {
            EVP_PKEY_CTX_free(ctx);
        }

        /* Get private key */
        size_t len_priv = sizeof(ret_val.second);
        if (ctx && EVP_PKEY_get_raw_public_key(pkey,
                                               ret_val.second.data(),
                                               &len_priv) <= 0) {
            EVP_PKEY_CTX_free(ctx);
        }

        static_assert(ret_val.first.size() == DH_ALGO::ret_size_b &&
                      ret_val.second.size() == DH_ALGO::ret_size_b);
        return ret_val;
    }

    template <typename HASH_ALGO>
    static auto get_hash(const uint8_t* plaintext_p, size_t len) -> typename HASH_ALGO::ret_val_t
    {
        /* Return value */
        typename HASH_ALGO::ret_val_t ret_val {};
        uint32_t hash_len {};

        std::invoke_result_t<decltype(EVP_sha256)> md = nullptr;
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();

        if constexpr (std::is_same_v<HASH_ALGO, hash_sha256_hmac_traits_t>) {
            md = EVP_sha256();
        } else {
            // error
        }

        /* Sets up digest context ctx to use a digest type */
        if (ctx && EVP_DigestInit_ex(ctx, md, nullptr) <= 0) {
            EVP_MD_CTX_free(ctx);
        }

        /* Hashes cnt bytes of data at d into the digest context ctx */
        if (ctx && EVP_DigestUpdate(ctx, plaintext_p, len) <= 0) {
            EVP_MD_CTX_free(ctx);
        }

        /* Retrieves the digest value from ctx and places it in md */
        if (ctx && EVP_DigestFinal_ex(ctx,
                                      ret_val.data(),
                                      &hash_len) <= 0) {
            EVP_MD_CTX_free(ctx);
        }

        /* Cleans up digest context ctx and frees up the space allocated to it */
        if (ctx) {
            EVP_MD_CTX_free(ctx);
        }

        static_assert(ret_val.size() == HASH_ALGO::ret_size_b);
        return ret_val;
    }
};
}
