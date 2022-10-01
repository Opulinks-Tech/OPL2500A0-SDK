

#ifndef _SUPPLICANT_OPT_H
#define _SUPPLICANT_OPT_H

/**
 * wpa_printf - conditional printf
 * @level: priority level (MSG_*) of the message
 * @fmt: printf format string, followed by optional arguments
 *
 * This function is used to print conditional debugging and error messages. The
 * output may be directed to stdout, stderr, and/or syslog based on
 * configuration.
 *
 * Note: New line '\n' is added to the end of the text when printing to stdout.
 */
//#define DEBUG_PRINT

#define OPL_SUPPLICANT
#define OPL_PLATFORM
#define OPL_PLATFORM_HW_SECURITY
#define CONFIG_COMMON_FUN_NOT_IN_LIB
#define CONFIG_NO_RANDOM_POOL

//#define CONFIG_APS_HANDLE_AUTH_ASSOC

#define OPL_AP_INFO_TO_DRIVER

/**
 * This is for debug used.
 */
//#define OPL_SHOW_SYS_STATS

#define IEEE8021X_EAPOL
#define CONFIG_IEEE80211G
#define CONFIG_IEEE80211N
#define CONFIG_IEEE80211AX
#define CONFIG_IEEE80211W

/**
 * Use the crypto library from internal (wpa) or mbedtls.
 *
 * If defined, use the mbedtls library.
 */
#define USE_MBEDTLS_CRYPTO

#ifndef MBEDTLS_CONFIG_FILE
#define MBEDTLS_CONFIG_FILE "config-opl-default.h"
#endif

#ifdef USE_MBEDTLS_CRYPTO
    #define CONFIG_TLS_INTERNAL_CLIENT
    #define CONFIG_TLSV12
    #ifndef MBEDTLS_CONFIG_FILE
        #error "Should have a config for mbedtls crypto used"
    #endif
#else
    /**
     * By default use the software calculate for encrypt/decrypt function.
     * We assume that the FPGA board does not containthe hardware engine.
     *
     * If defined, use the hareware engine.
     */
    //#define OPL_HW_SHA_1

    #define CONFIG_CRYPTO_INTERNAL
    #define CONFIG_TLS_INTERNAL_CLIENT
    #define CONFIG_TLSV12
    #define CONFIG_SHA256
    //#define CONFIG_INTERNAL_SHA384
    //#define CONFIG_INTERNAL_SHA512
#endif

/**
 * Complier option of build in the realty of wpa enterprise functions.
 *
 * If defined, will include the all of about enterprise functions.
 */
//#define CONFIG_WPA_ENTERPRISE

#ifdef CONFIG_WPA_ENTERPRISE
    #ifndef IEEE8021X_EAPOL
        #error "Wpa enterprise based on ieee801x, must be defined."
    #endif

    #define EAP_PEER_METHOD
    #define EAP_PEAP
    #define EAP_TTLS
    #define EAP_TLS
    #define EAP_MSCHAPv2

    /**
     * Issued your certificate.
     */
    #define USED_DEFAULT_CERTS
#endif

/**
 * Needs include bignum.c, ecp.c, asn1write.c, ctr_drbg.c, ecdh.c,
 * ecdsa.c, ecp_curves.c, entropy.c ...
 * in mbedtls library.
 */
#define CONFIG_SAE

/**
 * SAE (Simultaneous Authentication of Equals)
 */
#ifdef CONFIG_SAE
    #define CONFIG_ECC
    #ifndef USE_MBEDTLS_CRYPTO
        #error "Needs include mbedtls library to supported WPA3 SAE authentication"
    #endif

    /**
     * SAE self test suite.
     */
    //#define CONFIG_SAE_TEST

#endif

#endif /* _SUPPLICANT_OPT_H */
