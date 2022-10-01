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


#endif /* _IEEE80211_CRYPTO_PATCH_H_ */
