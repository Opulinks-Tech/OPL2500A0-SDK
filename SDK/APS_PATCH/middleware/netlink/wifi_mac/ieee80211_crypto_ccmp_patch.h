#ifndef _IEEE80211_CRYPTO_CCMP_H_
#error "Please include ieee80211_crypto_ccmp.h. Don't directly include ieee80211_crypto_ccmp_patch.h"
#endif

#ifndef _IEEE80211_CRYPTO_CCMP_PATCH_H_
#define _IEEE80211_CRYPTO_CCMP_PATCH_H_

extern u8 * ccmp_encrypt_ext(const u8 *tk, u8 *frame, size_t len, size_t hdrlen, u8 *qos,
          u8 *pn, int keyid, size_t *encrypted_len, u8 *input_frame_payload, u8 *output);

extern u8 * ccmp_decrypt_ext(const u8 *tk, const struct ieee80211_hdr *hdr,
          u8 *data, size_t data_len, size_t *decrypted_len, u8 *output);

#endif /* _IEEE80211_CRYPTO_CCMP_PATCH_H_ */
