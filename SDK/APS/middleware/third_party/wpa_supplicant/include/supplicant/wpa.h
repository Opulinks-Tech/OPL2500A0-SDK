
#ifndef __WPA_H__
#define __WPA_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup WiFi_APIs WiFi Related APIs
  * @brief WiFi APIs
  */

/** @addtogroup WiFi_APIs
  * @{
  */

/** \defgroup WPA_APIs  WPS APIs
  * @brief Wpa Supplicant APIs
  *
  */

/** @addtogroup WPA_APIs
  * @{
  */
/* Crypto callback functions */
//const wpa_crypto_funcs_t g_wifi_default_wpa_crypto_funcs;
/* Mesh crypto callback functions */
//const mesh_crypto_funcs_t g_wifi_default_mesh_crypto_funcs;

/**
  * @brief     Supplicant initialization
  *
  * @return
  *          - OPL_OK : succeed
  *          - OPL_ERR_NO_MEM : out of memory
  */
int supplicant_init(void);

/**
  * @brief     Supplicant deinitialization
  *
  * @return
  *          - OPL_OK : succeed
  *          - others: failed
  */
int supplicant_deinit(void);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __WPA_H__ */
