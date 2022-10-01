#ifndef _IEEE80211_CRYPTO_H_
#define _IEEE80211_CRYPTO_H_

#include <stdint.h>

#include "common/wifi_ieee802_11_defs.h"


/*
 * 802.11 protocol crypto-related definitions.
 */

/*
 * 802.11 ciphers.
 */
enum ieee80211_cipher{
    IEEE80211_CIPHER_NONE       = 0x00000000,
    IEEE80211_CIPHER_USEGROUP   = 0x00000001,
    IEEE80211_CIPHER_WEP40      = 0x00000002,
    IEEE80211_CIPHER_TKIP       = 0x00000004,
    IEEE80211_CIPHER_CCMP       = 0x00000008,
    IEEE80211_CIPHER_WEP104     = 0x00000010,
    IEEE80211_CIPHER_BIP        = 0x00000020    /* 11w */
};


#define IEEE80211_CIPHER_WEP        0
#define IEEE80211_CIPHER_TKIP       1
#define IEEE80211_CIPHER_AES_OCB    2
#define IEEE80211_CIPHER_AES_CCM    3
#define IEEE80211_CIPHER_CKIP       5
#define IEEE80211_CIPHER_NONE       6   /* pseudo value */


/*
 * 802.11 Authentication and Key Management Protocols.
 */
enum ieee80211_akm {
    IEEE80211_AKM_NONE          = 0x00000000,
    IEEE80211_AKM_8021X         = 0x00000001,
    IEEE80211_AKM_PSK           = 0x00000002,
    IEEE80211_AKM_SHA256_8021X  = 0x00000004,   /* 11w */
    IEEE80211_AKM_SHA256_PSK    = 0x00000008    /* 11w */
};

#define IEEE80211_TKIP_HDRLEN   8
#define IEEE80211_TKIP_MICLEN   8
#define IEEE80211_TKIP_ICVLEN   4
#define IEEE80211_CCMP_HDRLEN   8
#define IEEE80211_CCMP_MICLEN   8

#define IEEE80211_PMK_LEN       32

#define LEN_TKIP_IV_HDR         8
#define LEN_CCMP_HDR            8
#define LEN_CCMP_MIC            8


/* Required encryption head and tailroom */
#define IEEE80211_ENCRYPT_HEADROOM 8
#define IEEE80211_ENCRYPT_TAILROOM 18

#define IEEE80211_KEYBUF_SIZE   16


#define IEEE80211_KEY_GROUP 0x00000001  /* group data key */
#define IEEE80211_KEY_TX    0x00000002  /* Tx+Rx */
#define IEEE80211_KEY_IGTK  0x00000004  /* integrity group key */

typedef struct _ieee80211_key {
    u8 key[16];      /* 128 bits max */
    u8 tx_mic[8];
    u8 rx_mic[8];
    u8 tsc[6];       /* TSC value. 48bit */
    u8 rsc[6];       /* TSC value. 48bit */
    u8 cipher_alg;   /* 0:none, 1:WEP64, 2:WEP128, 3:TKIP, 4:AES, 5:CKIP64, 6:CKIP128 */
    u8 key_len;      /* Key length for each key, 0: entry is invalid */
    u8 keyid;
    u8 type;         /* Indicate Pairwise/Group when reporting MIC error */
} ieee80211_key;

/*
u8* ccmp_decrypt(const u8 *tk, const struct ieee80211_hdr *hdr, u8 *data, size_t data_len, size_t *decrypted_len);
u8* ccmp_encrypt(const u8 *tk, u8 *frame, size_t len, size_t hdrlen, u8 *qos, u8 *pn, int keyid, size_t *encrypted_len);

u8* tkip_decrypt(const u8 *tk, const struct ieee80211_hdr *hdr, const u8 *data, size_t data_len, size_t *decrypted_len);
u8* tkip_encrypt(const u8 *tk, u8 *frame, size_t len, size_t hdrlen, u8 *qos, u8 *pn, int keyid, size_t *encrypted_len);

u8* ieee80211_ccmp_encrypt(const u8 *tk, u8 *frame, size_t len, size_t hdrlen, u8 *qos, u8 *pn, int keyid, u8 *cdata, size_t *encrypted_len);
*/
s32 ieee80211_encrypt_frame_impl(ieee80211_key *key, u8 *frame, size_t len, size_t hdrlen, u8 *encrypt, size_t* crypt_len);
s32 ieee80211_decrypt_frame_impl(ieee80211_key *key, u8 *data, size_t data_len, size_t hdrlen, u8 *decrypted, size_t* decrypted_len);
s32 ieee80211_encrypt_mgmt_frame_impl(u8 *frame, size_t len, size_t hdrlen, u8 *encrypt, size_t* crypt_len);
s32 ieee80211_decrypt_mgmt_frame_impl(u8 *data, size_t data_len, size_t hdrlen, u8 *decrypted, size_t* decrypted_len);
int ieee80211_ptk_set_impl(uint32_t alg, uint32_t key_idx, 
                           uint32_t tk1_len, uint8_t *tk1, 
                           uint32_t tk2_len, uint8_t *tk2);
int ieee80211_ptk_get_impl(ieee80211_key **key);
int ieee80211_gtk_set_impl(uint32_t alg, uint32_t key_idx, uint32_t key_len, uint8_t *keys);
int ieee80211_gtk_get_impl(ieee80211_key **key);
int ieee80211_igtk_set_impl(uint32_t alg, uint32_t key_idx, uint32_t key_len, uint8_t *keys);
int ieee80211_igtk_get_impl(ieee80211_key **key);
int ieee80211_ptk_gtk_key_reset_impl(void);

/* Rom patch interface */
typedef s32 (*ieee80211_encrypt_frame_fp_t)(ieee80211_key *key, u8 *frame, size_t len, size_t hdrlen, u8 *encrypt, size_t* crypt_len);
typedef s32 (*ieee80211_decrypt_frame_fp_t)(ieee80211_key *key, u8 *data, size_t data_len, size_t hdrlen, u8 *decrypted, size_t* decrypted_len);
typedef s32 (*ieee80211_encrypt_mgmt_frame_fp_t)(u8 *frame, size_t len, size_t hdrlen, u8 *encrypt, size_t* crypt_len);
typedef s32 (*ieee80211_decrypt_mgmt_frame_fp_t)(u8 *data, size_t data_len, size_t hdrlen, u8 *decrypted, size_t* decrypted_len);
typedef int (*ieee80211_ptk_set_fp_t)(uint32_t alg, uint32_t key_idx, 
                           uint32_t tk1_len, uint8_t *tk1, 
                           uint32_t tk2_len, uint8_t *tk2);
typedef int (*ieee80211_ptk_get_fp_t)(ieee80211_key **key);
typedef int (*ieee80211_gtk_set_fp_t)(uint32_t alg, uint32_t key_idx, uint32_t key_len, uint8_t *keys);
typedef int (*ieee80211_igtk_set_fp_t)(uint32_t alg, uint32_t key_idx, uint32_t key_len, uint8_t *keys);
typedef int (*ieee80211_igtk_get_fp_t)(ieee80211_key **key);
typedef int (*ieee80211_gtk_get_fp_t)(ieee80211_key **key);
typedef int (*ieee80211_ptk_gtk_key_reset_fp_t)(void);

/* Export interface funtion pointer */
extern ieee80211_encrypt_frame_fp_t ieee80211_encrypt_frame;
extern ieee80211_decrypt_frame_fp_t ieee80211_decrypt_frame;
extern ieee80211_encrypt_mgmt_frame_fp_t ieee80211_encrypt_mgmt_frame;
extern ieee80211_decrypt_mgmt_frame_fp_t ieee80211_decrypt_mgmt_frame;
extern ieee80211_ptk_set_fp_t ieee80211_ptk_set;
extern ieee80211_ptk_get_fp_t ieee80211_ptk_get;
extern ieee80211_gtk_set_fp_t ieee80211_gtk_set;
extern ieee80211_gtk_get_fp_t ieee80211_gtk_get;
extern ieee80211_igtk_set_fp_t ieee80211_igtk_set;
extern ieee80211_igtk_get_fp_t ieee80211_igtk_get;
extern ieee80211_ptk_gtk_key_reset_fp_t ieee80211_ptk_gtk_key_reset;

#include "ieee80211_crypto_patch.h"
#endif /* _IEEE80211_CRYPTO_H_ */
