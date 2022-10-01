/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef __SCRT_H__
#define __SCRT_H__



//#define SCRT_PRE_LINK
#define SCRT_ENABLE_UNLINK

/** Bit Define */
#define BIT_0   0x00000001U
#define BIT_1   0x00000002U
#define BIT_2   0x00000004U
#define BIT_3   0x00000008U
#define BIT_4   0x00000010U
#define BIT_5   0x00000020U
#define BIT_6   0x00000040U
#define BIT_7   0x00000080U
#define BIT_8   0x00000100U
#define BIT_9   0x00000200U
#define BIT_10  0x00000400U
#define BIT_11  0x00000800U
#define BIT_12  0x00001000U
#define BIT_13  0x00002000U
#define BIT_14  0x00004000U
#define BIT_15  0x00008000U
#define BIT_16  0x00010000U
#define BIT_17  0x00020000U
#define BIT_18  0x00040000U
#define BIT_19  0x00080000U
#define BIT_20  0x00100000U
#define BIT_21  0x00200000U
#define BIT_22  0x00400000U
#define BIT_23  0x00800000U
#define BIT_24  0x01000000U
#define BIT_25  0x02000000U
#define BIT_26  0x04000000U
#define BIT_27  0x08000000U
#define BIT_28  0x10000000U
#define BIT_29  0x20000000U
#define BIT_30  0x40000000U
#define BIT_31  0x80000000U

/** VaultIP Core Register Basic & Offset Address  */
#define EIP130_MAILBOX_IN_BASE                  0x0000
#define EIP130_MAILBOX_OUT_BASE                 0x0000
#define EIP130_REGISTEROFFSET_MAILBOX_STAT      0x3F00
#define EIP130_REGISTEROFFSET_MAILBOX_CTRL      0x3F00
#define EIP130_REGISTEROFFSET_MAILBOX_RAWSTAT   0x3F04
#define EIP130_REGISTEROFFSET_MAILBOX_RESET     0x3F04
#define EIP130_REGISTEROFFSET_MAILBOX_LINKID    0x3F08
#define EIP130_REGISTEROFFSET_MAILBOX_OUTID     0x3F0C
#define EIP130_REGISTEROFFSET_MAILBOX_LOCKOUT   0x3F10
#define EIP130_REGISTEROFFSET_MODULE_STATUS     0x3FE0
#define EIP130_REGISTEROFFSET_EIP_OPTIONS2      0x3FF4
#define EIP130_REGISTEROFFSET_EIP_OPTIONS       0x3FF8
#define EIP130_REGISTEROFFSET_EIP_VERSION       0x3FFC

/** ITP */
#define ITP_SCRT_SRAM_BASE_ADDR                 0x0041a000

/** SCRT0 Core Register Adddress */
#define SCRT0_BASE_ADDR                         0x60000000

/** SCRT1 User-Defined Register Adddress */
#define SCRT1_BASE_ADDR                         0x6e000000

/** Interval for One Checking */
#define INTERVAL_EACH_CHECKING                  10

/** Maximum Checking times */
#define MAX_TIMES_CHECKING                      ( 2000 / INTERVAL_EACH_CHECKING )

#define SCRT_AES_CMAC_OUTPUT_LEN        16

#define SCRT_SHA_1_OUTPUT_LEN           20
#define SCRT_SHA_224_OUTPUT_LEN         32
#define SCRT_SHA_256_OUTPUT_LEN         32
#define SCRT_SHA_384_OUTPUT_LEN         64
#define SCRT_SHA_512_OUTPUT_LEN         64

#define SCRT_SHA_1_STEP_SIZE            64  // must be multiple of 64-bytes
#define SCRT_SHA_224_STEP_SIZE          64  // must be multiple of 64-bytes
#define SCRT_SHA_256_STEP_SIZE          64  // must be multiple of 64-bytes
#define SCRT_SHA_384_STEP_SIZE          128 // must be multiple of 128-bytes
#define SCRT_SHA_512_STEP_SIZE          128 // must be multiple of 128-bytes


// idx: T_ScrtMbIdx
#define SCRT_BASE_ADDR(idx)             (SCRT0_BASE_ADDR + ((idx & 0x03) << 10))

#define SCRT_CTRL_WRITE_MSK(idx)        (0x01 << (idx * 4))
#define SCRT_CTRL_READ_MSK(idx)         (0x02 << (idx * 4))
#define SCRT_CTRL_LINK_MSK(idx)         (0x04 << (idx * 4))
#define SCRT_CTRL_UNLINK_MSK(idx)       (0x08 << (idx * 4))
#define SCRT_STATUS_WRITE_MSK           SCRT_CTRL_WRITE_MSK
#define SCRT_STATUS_READ_MSK            SCRT_CTRL_READ_MSK
#define SCRT_STATUS_LINK_MSK            SCRT_CTRL_LINK_MSK
#define SCRT_STATUS_AVAIL_MSK           SCRT_CTRL_UNLINK_MSK

#define SCRT_STAT_CTRL_ADDR             (SCRT0_BASE_ADDR + EIP130_REGISTEROFFSET_MAILBOX_STAT)
#define SCRT_OTP_STATUS_ADDR            (0x30002400)

#define SCRT_ID                         0x4F5A3647
#define SCRT_PRIV_KEY_LEN               32
#define SCRT_PUB_KEY_VEC_LEN            32
#define SCRT_KEY_PARAM_FMT_LEN          224
#define SCRT_PRIV_KEY_FMT_LEN           36
#define SCRT_PUB_KEY_FMT_LEN            72
#define SCRT_SHARED_SRCRET_LEN          32

#define SCRT_WAIT_RES_SEM               osWaitForever
#define SCRT_WAIT_ECDH_SEM              osWaitForever

#define SCRT_WAIT_RSP_CNT               (20000000)


//#define SCRT_ACCESS_SEM

#ifdef SCRT_ACCESS_SEM
    #define SCRT_WAIT_ACCESS_SEM        (2000) // ms
#else
    #define SCRT_WAIT_RES               10 // ms
    #define SCRT_WAIT_RES_MAX_CNT       200
#endif

#define SCRT_MALLOC                 malloc
#define SCRT_FREE                   free

#define SCRT_DEBUG

#ifdef SCRT_DEBUG
    #define SCRT_LOGI(...)
    //#define SCRT_LOGI(args...)      tracer_log(LOG_LOW_LEVEL, args)
    #define SCRT_LOGE(args...)      tracer_log(LOG_HIGH_LEVEL, args)
#else
    #define SCRT_LOGI(...)
    #define SCRT_LOGE(...)
#endif

//#define SCRT_CHECK

#ifdef SCRT_CHECK
    #define SCRT_ASSERT(a)    \
    if(!(a))    \
    {     \
        tracer_drct_printf("SCRT_ASSERT FAILED '" #a "' %s %s:%d\n", __func__, __FILE__, __LINE__);   \
        while(1);    \
    }
#else
    #define SCRT_ASSERT(...)
#endif

#define SCRT_IP_CLK_MSK     (0x00000001)
#define SCRT_OTP_CLK_MSK    (0x00000002)


typedef enum
{
    SCRT_MB_IDX_0 = 0,
    SCRT_MB_IDX_1,
    SCRT_MB_IDX_2,
    SCRT_MB_IDX_3,

    SCRT_MB_IDX_MAX
} T_ScrtMbIdx;

typedef enum
{
    SCRT_TOKEN_ID_RESET = 0xE000,
    SCRT_TOKEN_ID_TRNG_CFG = 0xE010,
    SCRT_TOKEN_ID_ASSET_LOAD = 0xE020,
    SCRT_TOKEN_ID_ASSET_CREATE_KEY_PARAM = 0xE030,
    SCRT_TOKEN_ID_ASSET_CREATE_PRIV_KEY = 0xE040,
    SCRT_TOKEN_ID_ASSET_CREATE_PUB_KEY = 0xE050,
    SCRT_TOKEN_ID_ASSET_CREATE_SHARED_SECRET = 0xE060,
    SCRT_TOKEN_ID_SHARED_SECRET_GEN = 0xE070,
    SCRT_TOKEN_ID_PUB_DATA_READ = 0xE080,
    SCRT_TOKEN_ID_ASSET_DELETE = 0xE090,
    SCRT_TOKEN_ID_KEY_PAIR_GEN = 0xE0A0,
    SCRT_TOKEN_ID_AES_CCM_ENCRYPT = 0xE0B0,
    SCRT_TOKEN_ID_AES_CCM_DECRYPT = 0xE0C0,
    SCRT_TOKEN_ID_HMAC_SHA_1 = 0xE0D0,
    SCRT_TOKEN_ID_AES_ECB = 0xE0E0,
    SCRT_TOKEN_ID_AES_CMAC = 0xE0F0,
    SCRT_TOKEN_ID_SHA = 0xE100,

    SCRT_TOKEN_ID_MAX = 0xEFFF
} T_ScrtTokenId;

typedef struct
{
    uint8_t u8Used;
    uint8_t u8MbIdx;
} T_ScrtRes;


typedef enum
{
    SCRT_STEP_NEW = 0,
    SCRT_STEP_CONTINUE,
    SCRT_STEP_FINAL,

    SCRT_STEP_MAX
} T_ScrtStep;

typedef enum
{
    SCRT_TYPE_SHA_1 = 0,
    SCRT_TYPE_SHA_224,
    SCRT_TYPE_SHA_256,
    SCRT_TYPE_SHA_384,
    SCRT_TYPE_SHA_512,

    SCRT_TYPE_MAX
} T_ScrtShaType;


/** SCRT Initialization */
int nl_scrt_Init_impl(void);

/** AES CCM [deprecated] */
int nl_aes_ccm_impl (bool bEncrypt, uint8_t *sk, int sk_len, uint8_t *nonce, int nonce_len, unsigned int adata, int adata_len, unsigned int plain_text, unsigned int encrypted_text, int text_len, uint8_t *tag, int tag_len);

/** AES CCM */
int nl_scrt_aes_ccm_impl (int bEncrypt, unsigned char *sk, int sk_len, unsigned char *nonce, int nonce_len, unsigned char *adata, int adata_len, unsigned char *plain_text, unsigned char *encrypted_text, int text_len, unsigned char *tag, int tag_len);

/** AES CCM Sample */
void aes_ccm_sample(void);

/** HMAC SHA-1 */
int nl_hmac_sha_1_impl(uint8_t *sk, int sk_len, uint8_t *in_data, int data_len, uint8_t *mac);

/** HMAC SHA-1 Sample */
void hmac_sha_1_sample(void);

/** AES ECB */
int nl_scrt_aes_ecb_impl(int bEncrypt, unsigned char *sk, int sk_len, unsigned char *data_in, unsigned char *data_out, unsigned data_len);

/** AES ECB Sample */
void aes_ecb_sample(void);

/** AES CMAC */
int nl_scrt_aes_cmac_impl(uint8_t *u8aKey, uint8_t u8KeyLen, uint8_t *u8aInputBuf, uint32_t u32BufSize, uint32_t u32InputLen, uint8_t *u8aMac);
void aes_cmac_sample(void);

/** TRNG */
int nl_scrt_trng(int size, unsigned char *trng_out);
void trng_sample(void);

typedef int (*scrt_status_chk_fp_t)(uint32_t u32Mask, uint32_t u32Value);
typedef int (*scrt_asset_create_fp_t)(uint8_t u8Idx, uint32_t *pu32AsId);
typedef int (*scrt_asset_delete_fp_t)(uint8_t u8Idx, uint32_t u32AsId);
typedef int (*scrt_asset_load_fp_t)(uint8_t u8Idx, uint32_t u32AsId, uint8_t *u8aAddr, uint16_t u16Len);
typedef int (*scrt_asset_get_fp_t)(uint8_t u8Idx, uint32_t u32AsId, uint8_t *u8aBuf, uint32_t u32BufLen, uint32_t u32OutputLen);
typedef int (*scrt_cmd_fp_t)(uint8_t u8Idx);
typedef uint8_t (*scrt_res_alloc_fp_t)(void);
typedef void (*scrt_res_free_fp_t)(uint8_t u8Idx);
typedef void (*scrt_clk_enable_fp_t)(uint8_t u8Enable, uint32_t u32Msk);

typedef int (*nl_scrt_common_fp_t)(void);
typedef int (*nl_aes_ccm_fp_t)(bool bEncrypt, uint8_t *sk, int sk_len, uint8_t *nonce, int nonce_len, unsigned int adata, int adata_len, unsigned int plain_text, unsigned int encrypted_text, int text_len, uint8_t *tag, int tag_len);
typedef int (*nl_scrt_aes_ccm_fp_t)(int bEncrypt, unsigned char *sk, int sk_len, unsigned char *nonce, int nonce_len, unsigned char *adata, int adata_len, unsigned char *plain_text, unsigned char *encrypted_text, int text_len, unsigned char *tag, int tag_len);
typedef int (*nl_hmac_sha_1_fp_t)(uint8_t *sk, int sk_len, uint8_t *in_data, int data_len, uint8_t *mac);
typedef int (*nl_scrt_aes_ecb_fp_t)(int bEncrypt, unsigned char *sk, int sk_len, unsigned char *data_in, unsigned char *data_out, unsigned data_len);
typedef int (*nl_scrt_ecdh_key_pair_gen_fp_t)(void *pPubKey, uint32_t *u32aPrivKey, uint32_t *pu32PrivKeyId);
typedef int (*nl_scrt_ecdh_dhkey_gen_fp_t)(uint8_t *u8aPubKeyX, uint8_t *u8aPubKeyY, uint32_t *u32aPrivKey, void *pDhKey, uint32_t u32PrivKeyId);
typedef int (*nl_scrt_key_delete_fp_t)(uint32_t u32KeyId);
typedef int (*nl_scrt_aes_cmac_fp_t)(uint8_t *u8aKey, uint8_t u8KeyLen, uint8_t *u8aInputBuf, uint32_t u32BufSize, uint32_t u32InputLen, uint8_t *u8aMac);
typedef int (*nl_scrt_hmac_sha_1_step_fp_t)(uint8_t type, uint32_t total_len, uint8_t *sk, int sk_len, uint8_t *in_data, int in_data_len, uint8_t *mac);
typedef int (*nl_scrt_sha_fp_t)(uint8_t u8Type, uint8_t u8Step, uint32_t u32TotalLen, uint8_t *u8aData, uint32_t u32DataLen, uint8_t u8HasInterMac, uint8_t *u8aMac);


// internal
extern nl_scrt_common_fp_t scrt_param_init;
extern nl_scrt_common_fp_t scrt_mb_init;
extern scrt_cmd_fp_t scrt_trng_init;
extern scrt_cmd_fp_t scrt_eng_init;
extern scrt_status_chk_fp_t scrt_status_chk;
extern scrt_asset_create_fp_t scrt_ecdh_key_param_init;
extern scrt_asset_create_fp_t scrt_ecdh_priv_key_asset_create;
extern scrt_asset_create_fp_t scrt_ecdh_pub_key_asset_create;
extern scrt_asset_create_fp_t scrt_ecdh_shared_secret_asset_create;
extern scrt_asset_delete_fp_t scrt_asset_delete;
extern scrt_asset_load_fp_t scrt_asset_load;
extern scrt_asset_get_fp_t scrt_asset_get;
extern scrt_cmd_fp_t scrt_reset;
//extern nl_scrt_common_fp_t scrt_internal_reset;
extern scrt_cmd_fp_t scrt_link;
extern scrt_cmd_fp_t scrt_unlink;
extern nl_scrt_common_fp_t scrt_access_lock;
extern nl_scrt_common_fp_t scrt_access_unlock;
extern nl_scrt_common_fp_t scrt_res_lock;
extern nl_scrt_common_fp_t scrt_res_unlock;
extern nl_scrt_common_fp_t scrt_ecdh_lock;
extern nl_scrt_common_fp_t scrt_ecdh_unlock;
extern nl_scrt_common_fp_t scrt_sem_create;
extern scrt_res_alloc_fp_t scrt_res_alloc;
extern scrt_res_free_fp_t scrt_res_free;

// external
extern nl_scrt_common_fp_t nl_scrt_Init;
extern nl_aes_ccm_fp_t nl_aes_ccm; //degraded
extern nl_scrt_aes_ccm_fp_t nl_scrt_aes_ccm;
extern nl_hmac_sha_1_fp_t nl_hmac_sha_1;
extern nl_scrt_aes_ecb_fp_t nl_scrt_aes_ecb;
extern nl_scrt_common_fp_t nl_scrt_otp_status_get;
extern nl_scrt_ecdh_key_pair_gen_fp_t nl_scrt_ecdh_key_pair_gen;
extern nl_scrt_ecdh_dhkey_gen_fp_t nl_scrt_ecdh_dhkey_gen;
extern nl_scrt_key_delete_fp_t nl_scrt_key_delete;
extern nl_scrt_aes_cmac_fp_t nl_scrt_aes_cmac;
extern nl_scrt_sha_fp_t nl_scrt_sha;


#if 0
#include "clib.h"
#include "api_val_sym.h"
#include "api_val_asset.h"
#include "api_val_result.h"
#include "api_val_buffers.h"
#include "testvectors_aes_basic.h"
#include "sfzutf.h"

#define SYM_CRYPT_ENCRYPT true
#define SYM_CRYPT_DECRYPT false
#define SYM_CRYPT_MAX_BUFLEN       2048 /** Maximum buffer length. */
#define SYM_CRYPT_IN_PLACE        0x0001
#define SYM_CRYPT_MULTIPART       0x0002
#define SYM_CRYPT_UNALIGNED       0x0004

typedef struct
{
    //int Index;
    ValSymContextPtr_t SymContext_p;
    ValAssetId_t KeyAssetId;
    uint8_t * ptx_p;
    uint8_t * ctx_p;
    uint32_t txt_len;
    uint32_t BlockSize;
    bool fPadding;
    uint8_t PaddingByte;
    uint32_t options;
    int misalign;
} SymCryptCtx_t;

/* Beware that the following mode definitions are not guaranteed to be
   numerically identical to the corresponding SFZCRYPTO_MODE_xxx macros. */
typedef enum
{
    SFZCRYPTO_MODE_ECB = 0,
    SFZCRYPTO_MODE_CBC,
    SFZCRYPTO_MODE_CTR,
    SFZCRYPTO_MODE_ICM,
    SFZCRYPTO_MODE_F8,
    SFZCRYPTO_MODE_XTS_AES,
    SFZCRYPTO_MODE_NUM        // must be last
} SfzCrypto_Mode_t;

int nl_scrt_Init(void);
int nl_scrt_aes_cbc(Nl_Scrt_Usr_t user, bool fDecrypt, const uint8_t *key, uint32_t keySize, const uint8_t *iv, uint8_t *data, size_t data_len, uint8_t *out_data);
int nl_scrt_aes_ctr(Nl_Scrt_Usr_t user, bool fDecrypt, const uint8_t *key, uint32_t keySize, const uint8_t *iv, uint8_t *data, size_t data_len, uint8_t *out_data);
int nl_scrt_aes_cmac(Nl_Scrt_Usr_t user, uint8_t * Key_p, uint32_t KeySize, uint8_t * Msg_p, uint32_t MsgLen, uint8_t * Mac_p, uint32_t MacLen);
int nl_scrt_aes_cbcmac(Nl_Scrt_Usr_t user, uint8_t * Key_p, uint32_t KeySize, uint8_t * Msg_p, uint32_t MsgLen, uint8_t * Mac_p, uint32_t MacLen, uint8_t * Iv_p);
ValStatus_t nl_scrt_trng_gen(Nl_Scrt_Usr_t user, const ValSize_t DataSize, ValOctetsOut_t * const  Data_p);
ValStatus_t nl_scrt_asym_ecc_gen_keypair(uint32_t KeySize, uint8_t * PubKey_p, uint8_t * PrivKey_p);
void nl_se_sample_test(void);
#endif

int scrt_param_init_impl(void);
int scrt_mb_init_impl(void);
int scrt_trng_init_impl(uint8_t u8Idx);
int scrt_eng_init_impl(uint8_t u8Idx);
int scrt_status_chk_impl(uint32_t u32Mask, uint32_t u32Value);
int scrt_ecdh_key_param_init_impl(uint8_t u8Idx, uint32_t *pu32KeyParamAsId);
int scrt_ecdh_priv_key_asset_create_impl(uint8_t u8Idx, uint32_t *pu32PrivKeyAsId);
int scrt_ecdh_pub_key_asset_create_impl(uint8_t u8Idx, uint32_t *pu32PubKeyAsId);
int scrt_ecdh_shared_secret_asset_create_impl(uint8_t u8Idx, uint32_t *pu32SharedSecretAsId);
int scrt_asset_delete_impl(uint8_t u8Idx, uint32_t u32AsId);
int scrt_asset_load_impl(uint8_t u8Idx, uint32_t u32AsId, uint8_t *u8aAddr, uint16_t u16Len);
int scrt_asset_get_impl(uint8_t u8Idx, uint32_t u32AsId, uint8_t *u8aBuf, uint32_t u32BufLen, uint32_t u32OutputLen);
int scrt_link_impl(uint8_t u8Idx);

#ifdef SCRT_ENABLE_UNLINK
int scrt_unlink_impl(uint8_t u8Idx);
#endif

#ifdef SCRT_ACCESS_SEM
int scrt_access_lock_impl(void);
int scrt_access_unlock_impl(void);
#endif

int scrt_res_lock_impl(void);
int scrt_res_unlock_impl(void);
int scrt_ecdh_lock_impl(void);
int scrt_ecdh_unlock_impl(void);
int scrt_sem_create_impl(void);
uint8_t scrt_res_alloc_impl(void);
void scrt_res_free_impl(uint8_t u8Idx);
void scrt_clk_enable_impl(uint8_t u8Enable, uint32_t u32Msk);

int nl_scrt_init_impl(void);
int nl_scrt_otp_status_get_impl(void);
int nl_scrt_ecdh_key_pair_gen_impl(void *pPubKey, uint32_t *u32aPrivKey, uint32_t *pu32PrivKeyId);
int nl_scrt_ecdh_dhkey_gen_impl(uint8_t *u8aPubKeyX, uint8_t *u8aPubKeyY, uint32_t *u32aPrivKey, void *pDhKey, uint32_t u32PrivKeyId);
int nl_scrt_key_delete_impl(uint32_t u32KeyId);
int nl_scrt_sha_impl(uint8_t u8Type, uint8_t u8Step, uint32_t u32TotalLen, uint8_t *u8aData, uint32_t u32DataLen, uint8_t u8HasInterMac, uint8_t *u8aMac);
#endif
