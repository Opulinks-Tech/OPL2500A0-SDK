/******************************************************************************
*  Copyright 2017 - 2021, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2021
******************************************************************************/
#ifndef _BLE_ADV_PARSER_H_
#define _BLE_ADV_PARSER_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define COLOR_RST        "\x1b[;0m"
#define COLOR_RED        "\x1b[;31;1m"
#define COLOR_GREEN      "\x1b[;32;1m"
#define COLOR_YELLOW     "\x1b[;33;1m"
#define COLOR_BLUE       "\x1b[;34;1m"
#define COLOR_MAGENTA    "\x1b[;35;1m"
#define COLOR_CYAN       "\x1b[;36;1m"


/**@brief Function for searching encoded Advertising or Scan Response data for specific data types.
 *
 * @param[in]    p_encoded_data  The data buffer containing the encoded Advertising data.
 * @param[in]    data_len        The length of the data buffer \p p_encoded_data.
 * @param[inout] p_offset        \c in: The offset to start searching from.
 *                               \c out: The offset the data type can be found at.
 *                                       This value is not changed if the call returns 0.
 * @param[in]    ad_type         The type of data to search for.
 *
 * @return The length of the found data, or 0 if no data was found with the the type \p ad_type,
 *         or if \p p_encoded_data or \p p_offset were NULL.
 */
uint16_t ble_advdata_search(uint8_t * p_encoded_data,
                            uint16_t  data_len,
                            uint16_t * p_offset,
                            uint8_t    ad_type);

/**@brief Function for getting specific data from encoded Advertising or Scan Response data.
 *
 * @details This function searches through encoded data e.g. the data produced by
 *          @ref ble_advdata_encode, or the data found in Advertising reports
 *          (@ref BLE_GAP_EVT_ADV_REPORT), and returns a pointer directly to the data within the
 *          data buffer.
 *
 *          Example code:
 *              ad_type_data = ble_advdata_parse(&data, len, AD_TYPE);
 *
 * @param[in]    p_encoded_data  Data buffer containing the encoded Advertising data.
 * @param[in]    data_len        Length of the data buffer \p p_encoded_data.
 * @param[in]    ad_type         Type of data to search for.
 *
 * @return Pointer to the found data, or NULL if no data was found with the type \p ad_type,
 *         or if \p p_encoded_data or \p p_data_len were NULL.
 */
uint8_t * ble_advdata_parse(uint8_t  * p_encoded_data,
                            uint16_t   data_len,
                            uint8_t    ad_type);


/**@brief Function for searching through encoded Advertising data for a complete local name.
 *
 * @param[in]    p_encoded_data Data buffer containing the encoded Advertising data.
 * @param[in]    data_len       Length of the data buffer \p p_encoded_data.
 * @param[in]    p_target_name  Name to search for.
 *
 * @retval true   If \p p_target_name was found among \p p_encoded_data, as a complete local name.
 * @retval false  If \p p_target_name was not found among \p p_encoded_data, or if \p p_encoded_data
 *                or \p p_target_name was NULL.
 */
bool ble_advdata_name_find(uint8_t  *p_encoded_data,
                           uint16_t  data_len,
                           const char *p_target_name);


/**@brief Function for searching through encoded Advertising data for a device shortened name.
 *
 * @param[in]    p_encoded_data     Data buffer containing the encoded Advertising data.
 * @param[in]    data_len           Length of the data buffer \p p_encoded_data.
 * @param[in]    p_target_name      Name to search for.
 * @param[in]    short_name_min_len Minimum length of the shortened name.
 *               For example, if the advertising data has a shortened name 'No' and this parameter is
 *               set to 4 with a target_name set to Nordic_XXX it will return false, but if
 *               the shortened name in the advertising data is 'Nord', it will return true.
 * @note: If the shortened name in the Advertising data has the same length as the target name,
 *        this function will return false, since this means that the complete name is actually
 *        longer, thus different than the target name.
 *
 * @retval true   If \p p_target_name was found among \p p_encoded_data, as short local name.
 * @retval false  If \p p_target_name was not found among \p p_encoded_data, or if \p p_encoded_data
 *                or \p p_target_name was NULL.
 */
bool ble_advdata_short_name_find(uint8_t * p_encoded_data,
                                 uint16_t  data_len,
                                 char    * p_target_name,
                                 uint8_t   short_name_min_len);

/**@brief Function for searching through encoded Advertising data for a UUID (16-bit or 128-bit).
 *
 * @param[in]    p_encoded_data  Data buffer containing the encoded Advertising data.
 * @param[in]    data_len        Length of the data buffer \p p_encoded_data.
 * @param[in]    p_target_uuid   UUID to search for.
 *
 * @retval true   If \p p_target_uuid was found among \p p_encoded_data.
 * @retval false  If \p p_target_uuid was not found among \p p_encoded_data, or if \p p_encoded_data
 *                or \p p_target_uuid was NULL.
 */
bool ble_advdata_uuid_find(uint8_t    * p_encoded_data,
                           uint16_t     data_len,
                           uint16_t * p_target_uuid);


void parse_advertising_data(uint8_t *data, size_t length);

void parse_hrs_value(uint8_t *data, size_t length);

#endif
