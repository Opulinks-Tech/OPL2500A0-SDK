#ifndef _IEEE80211_CRYPTO_H_
#error "Please include ieee80211_crypto.h. Don't directly include ieee80211_crypto_patch.h"
#endif

#ifndef _IEEE80211_CRYPTO_PATCH_H_
#define _IEEE80211_CRYPTO_PATCH_H_


typedef struct _ieee80211_igtk {
    u8 key[32]; 
    u8 cipher_alg;
    u8 key_len;
    u16 keyid;
    u8 pn[6];
} ieee80211_igtk;


int ieee80211_igtk_set_ext(uint32_t alg, uint32_t key_idx, uint32_t key_len, uint8_t *keys, uint8_t *pn);

s32 ieee80211_encrypt_frame_ext(ieee80211_key *key, u8 *frame, size_t len, size_t hdrlen, u8 *encrypt, size_t* crypt_len, u8 *input_frame_payload);
s32 ieee80211_decrypt_frame_ext(ieee80211_key *key, u8 *data, size_t data_len, size_t hdrlen, u8 *decrypted, size_t* decrypted_len);

void ieee80211_crypto_init_patch(void);

#endif /* _IEEE80211_CRYPTO_PATCH_H_ */
