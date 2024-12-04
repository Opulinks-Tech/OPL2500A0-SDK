#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ble.h"
#include "ble_gap_if.h"
#include "ble_adv_parser.h"


#define BLE_GAP_AD_TYPE_FLAGS                               0x01 //< Flags for discoverability.
#define BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE   0x02 //< Partial list of 16 bit service UUIDs.
#define BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE         0x03 //< Complete list of 16 bit service UUIDs.
#define BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_MORE_AVAILABLE   0x04 //< Partial list of 32 bit service UUIDs.
#define BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_COMPLETE         0x05 //< Complete list of 32 bit service UUIDs.
#define BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE  0x06 //< Partial list of 128 bit service UUIDs.
#define BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE        0x07 //< Complete list of 128 bit service UUIDs.
#define BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME                    0x08 //< Short local device name.
#define BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME                 0x09 //< Complete local device name.
#define BLE_GAP_AD_TYPE_TX_POWER_LEVEL                      0x0A //< Transmit power level.
#define BLE_GAP_AD_TYPE_CLASS_OF_DEVICE                     0x0D //< Class of device.
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_HASH_C               0x0E //< Simple Pairing Hash C.
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_RANDOMIZER_R         0x0F //< Simple Pairing Randomizer R.
#define BLE_GAP_AD_TYPE_SECURITY_MANAGER_TK_VALUE           0x10 //< Security Manager TK Value.
#define BLE_GAP_AD_TYPE_SECURITY_MANAGER_OOB_FLAGS          0x11 //< Security Manager Out Of Band Flags.
#define BLE_GAP_AD_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE     0x12 //< Slave Connection Interval Range.
#define BLE_GAP_AD_TYPE_SOLICITED_SERVICE_UUIDS_16BIT       0x14 //< List of 16-bit Service Solicitation UUIDs.
#define BLE_GAP_AD_TYPE_SOLICITED_SERVICE_UUIDS_128BIT      0x15 //< List of 128-bit Service Solicitation UUIDs.
#define BLE_GAP_AD_TYPE_SERVICE_DATA                        0x16 //< Service Data - 16-bit UUID.
#define BLE_GAP_AD_TYPE_PUBLIC_TARGET_ADDRESS               0x17 //< Public Target Address.
#define BLE_GAP_AD_TYPE_RANDOM_TARGET_ADDRESS               0x18 //< Random Target Address.
#define BLE_GAP_AD_TYPE_APPEARANCE                          0x19 //< Appearance.
#define BLE_GAP_AD_TYPE_ADVERTISING_INTERVAL                0x1A //< Advertising Interval.
#define BLE_GAP_AD_TYPE_LE_BLUETOOTH_DEVICE_ADDRESS         0x1B //< LE Bluetooth Device Address.
#define BLE_GAP_AD_TYPE_LE_ROLE                             0x1C //< LE Role.
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_HASH_C256            0x1D //< Simple Pairing Hash C-256.
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_RANDOMIZER_R256      0x1E //< Simple Pairing Randomizer R-256.
#define BLE_GAP_AD_TYPE_SERVICE_DATA_32BIT_UUID             0x20 //< Service Data - 32-bit UUID.
#define BLE_GAP_AD_TYPE_SERVICE_DATA_128BIT_UUID            0x21 //< Service Data - 128-bit UUID.
#define BLE_GAP_AD_TYPE_3D_INFORMATION_DATA                 0x3D //< 3D Information Data.
#define BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA          0xFF //< Manufacturer Specific Data.

#define UUID16_SIZE     2   /**< Size of 16 bit UUID. */
#define UUID32_SIZE     4   /**< Size of 32 bit UUID. */
#define UUID128_SIZE    16  /**< Size of 128 bit UUID. */
#define N_AD_TYPES      2


void parse_advertising_data(uint8_t *data, size_t length)
{
    size_t index = 0;
    int uuid16_flag = 0;
    int uuid128_flag = 0;

    while (index < length) {
        uint8_t field_length = data[index];
        if (field_length == 0) break; // No more fields

        uint8_t field_type = data[index + 1];
        uint8_t *field_data = &data[index + 2];
        uuid16_flag = 0;
        uuid128_flag = 0;

        switch (field_type) {
            case BLE_GAP_AD_TYPE_FLAGS:
                printf("Flags: 0x%02X\n", field_data[0]);
                break;
            case BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME:
                printf("Complete Local Name: %.*s\n", field_length - 1, field_data);
                break;
            case BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA:
                printf("Manufacturer Specific Data: ");
                for (size_t i = 0; i < field_length - 1; i++) {
                    printf("%02X ", field_data[i]);
                }
                printf("\n");
                break;
            case BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE:
                uuid16_flag = 1;
            case BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE:
                printf("%s list of 16-bit Service UUIDs: ", uuid16_flag ? "Imcomplete":"Complete");
                for (size_t i = 0; i < field_length - 1; i += 2) {
                    uint16_t uuid = field_data[i] | (field_data[i + 1] << 8);
                    printf("%04X ", uuid);
                }
                printf("\n");
                break;
            case BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE:
                uuid128_flag = 1;
            case BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE:
                printf("%s list of 128-bit Service UUIDs: ", uuid128_flag ? "Imcomplete":"Complete" );
                for (size_t i = 0; i < field_length - 1; i += 16) {
                    printf("UUID: ");
                    for (int j = 0; j < 16; j++) {
                        printf("%02X", field_data[i + j]);
                    }
                    printf(" ");
                }
                printf("\n");
                break;
            case BLE_GAP_AD_TYPE_TX_POWER_LEVEL:
                printf("Tx Power Level: %d dBm\n", (int8_t)field_data[0]);
                break;
            case BLE_GAP_AD_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE:
                if (field_length - 1 >= 4) {
                    uint16_t min_interval = field_data[0] | (field_data[1] << 8);
                    uint16_t max_interval = field_data[2] | (field_data[3] << 8);
                    printf("Slave Connection Interval Range: Min=%d, Max=%d\n", min_interval, max_interval);
                } else {
                    printf("Invalid Slave Connection Interval Range\n");
                }
                break;
            default:
                printf("Unknown field type: 0x%02X\n", field_type);
                break;
        }

        index += field_length + 1;
    }
}

uint8_t *ble_advdata_parse(uint8_t *p_encoded_data, uint16_t data_len, uint8_t ad_type)
{
    uint16_t offset = 0;
    uint16_t len    = ble_advdata_search(p_encoded_data, data_len, &offset, ad_type);

    if (len == 0)
    {
        return NULL;
    }
    else
    {
        return &p_encoded_data[offset];
    }
}

uint16_t ble_advdata_search(uint8_t *p_encoded_data,
                            uint16_t        data_len,
                            uint16_t      * p_offset,
                            uint8_t         ad_type)
{
    if ((p_encoded_data == NULL) || (p_offset == NULL))
    {
        return 0;
    }

    uint16_t i = 0;

    while ((i + 1 < data_len) && ((i < *p_offset) || (p_encoded_data[i + 1] != ad_type)))
    {
        // Jump to next data.
        i += (p_encoded_data[i] + 1);
    }

    if (i >= data_len)
    {
        return 0;
    }
    else
    {
        uint16_t offset = i + 2;
        uint16_t len    = p_encoded_data[i] ? (p_encoded_data[i] - 1) : 0;
        if (!len || ((offset + len) > data_len))
        {
            // Malformed. Zero length or extends beyond provided data.
            return 0;
        }
        *p_offset = offset;
        return len;
    }
}

bool ble_advdata_name_find(uint8_t *p_encoded_data,
                           uint16_t data_len,
                           const char *p_target_name)
{
    uint16_t parsed_name_len;
    uint8_t const * p_parsed_name;
    uint16_t data_offset  = 0;

    if (p_target_name == NULL)
    {
        return false;
    }

    parsed_name_len = ble_advdata_search(p_encoded_data,
                                         data_len,
                                         &data_offset,
                                         GAP_ADTYPE_LOCAL_NAME_COMPLETE);

    p_parsed_name = &p_encoded_data[data_offset];

    if ((data_offset != 0)
        && (parsed_name_len != 0)
        && (strlen(p_target_name) == parsed_name_len)
        && (memcmp(p_target_name, p_parsed_name, parsed_name_len) == 0))
    {
        return true;
    }

    return false;
}


bool ble_advdata_short_name_find(uint8_t *p_encoded_data,
                                 uint16_t  data_len,
                                 char    *p_target_name,
                                 uint8_t short_name_min_len)
{
    uint16_t parsed_name_len;
    uint8_t const * p_parsed_name;
    uint16_t data_offset = 0;

    if (p_target_name == NULL)
    {
        return false;
    }

    parsed_name_len = ble_advdata_search(p_encoded_data,
                                         data_len,
                                         &data_offset,
                                         GAP_ADTYPE_LOCAL_NAME_SHORT);

    p_parsed_name = &p_encoded_data[data_offset];

    if (   (data_offset != 0)
        && (parsed_name_len != 0)
        && (parsed_name_len >= short_name_min_len)
        && (parsed_name_len < strlen(p_target_name))
        && (memcmp(p_target_name, p_parsed_name, parsed_name_len) == 0))
    {
        return true;
    }

    return false;
}

bool ble_advdata_uuid_find(uint8_t *p_encoded_data,
                               uint16_t data_len,
                               uint16_t *p_target_uuid)

{
    uint16_t        data_offset = 0;
    uint8_t         raw_uuid_len    = UUID16_SIZE;
    uint8_t const * p_parsed_uuid;
    uint16_t        parsed_uuid_len = data_len;
    //uint8_t         raw_uuid[UUID128_SIZE];
    uint8_t         ad_types[N_AD_TYPES];

    switch (raw_uuid_len)
    {
        case UUID16_SIZE:
            ad_types[0] = BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE;
            ad_types[1] = BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE;
            break;

        case UUID128_SIZE:
            ad_types[0] = BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE;
            ad_types[1] = BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE;
            break;

        default:
            return false;
    }

    for (uint8_t i = 0; (i < N_AD_TYPES) && (data_offset == 0); i++)
    {
        parsed_uuid_len = ble_advdata_search(p_encoded_data, data_len, &data_offset, ad_types[i]);
    }

    if (data_offset == 0)
    {
        // Could not find any relevant UUIDs in the encoded data.
        return false;
    }

    p_parsed_uuid = &p_encoded_data[data_offset];

    // Verify if any UUID matches the given UUID.
    for (uint16_t list_offset = 0; list_offset < parsed_uuid_len; list_offset += raw_uuid_len)
    {
        if (memcmp(&p_parsed_uuid[list_offset], p_target_uuid, parsed_uuid_len) == 0)
        {
            return true;
        }
    }

    // Could not find the UUID among the encoded data.
    return false;
}

void parse_hrs_value(uint8_t *data, size_t length)
{
    if (length < 1) {
        printf("Invalid HRS value length\n");
        return;
    }

    uint8_t flags = data[0];
    size_t index = 1;

    // Parse heart rate measurement value
    uint16_t heart_rate = 0;
    if (flags & 0x01) { // Heart Rate Value Format (UINT16)
        if (index + 2 > length) {
            printf("Invalid HRS value length for UINT16 heart rate\n");
            return;
        }
        heart_rate = data[index] | (data[index + 1] << 8);
        index += 2;
    } else { // Heart Rate Value Format (UINT8)
        if (index + 1 > length) {
            printf("Invalid HRS value length for UINT8 heart rate\n");
            return;
        }
        heart_rate = data[index];
        index += 1;
    }
    printf(COLOR_RED"Heart Rate: %u bpm\n"COLOR_RST, heart_rate);

    // Parse sensor contact status
    uint8_t contact_detected = (flags & 0x06) >> 1;
    switch (contact_detected) {
        case 0:
            printf("Sensor Contact: Not supported\n");
            break;
        case 2:
            printf("Sensor Contact: Supported but not detected\n");
            break;
        case 3:
            printf("Sensor Contact: Supported and detected\n");
            break;
        case 1:
            printf("Sensor Contact: Not supported\n");
            break;
        default:
            printf("Sensor Contact: Unknown state\n");
            break;
    }

    // Parse energy expended if present
    if (flags & 0x08) { // Energy Expended Status
        if (index + 2 > length) {
            printf("Invalid HRS value length for energy expended\n");
            return;
        }
        uint16_t energy_expended = data[index] | (data[index + 1] << 8);
        index += 2;
        printf("Energy Expended: %u kJ\n", energy_expended);
    }

    // Parse RR intervals if present
    if (flags & 0x10) { // RR-Interval Status
        printf("RR-Intervals: ");
        while (index + 2 <= length) {
            uint16_t rr_interval = data[index] | (data[index + 1] << 8);
            index += 2;
            printf("%u ms ", rr_interval);
        }
        printf("\n");
    }
}

#if 0
int test() {
    // Example advertising data (mock data for demonstration)
    uint8_t advertising_data[] = {
        0x02, 0x01, 0x06,  // Flags
        0x0A, 0x09, 'E', 'x', 'a', 'm', 'p', 'l', 'e', 'B', 'L', 'E',  // Complete Local Name
        0x07, 0xFF, 0x4C, 0x00, 0x02, 0x15, 0xA0, 0x00, 0x00, 0x00,  // Manufacturer Specific Data
        0x05, 0x03, 0x0F, 0x18, 0x0A, 0x18,  // Complete List of 16-bit Service UUIDs
        0x11, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB  // Complete List of 128-bit Service UUIDs
    };

    size_t advertising_data_length = sizeof(advertising_data);

    printf("Parsing advertising data:\n");
    parse_advertising_data(advertising_data, advertising_data_length);

    uint8_t adv_data[] = {
        0x03, BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE, 0x0D, 0x18,
        0x11, BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE,
        0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xAF, 0x6B, 0xE0, 0xE1
    };

    uint16_t target_uuid16 = 0x180D;
    uint16_t target_uuid128[UUID128_SIZE/2] = {0xE1E0, 0x6BAF, 0x0000, 0x1000, 0x8000, 0x0080, 0x5F9B, 0x34FB};

    // test 16bi tUUID search
    bool found16 = ble_advdata_uuid_find(adv_data, sizeof(adv_data), &target_uuid16);
    printf("16位UUID %04X search result: %s\n", target_uuid16, found16 ? "found" : "not found");

    // test 1128bit UUID search
    bool found128 = ble_advdata_uuid_find(adv_data, sizeof(adv_data), (uint16_t*)target_uuid128);
    printf("128位UUID search result: %s\n", found128 ? "found" : "not found");

    return 0;


}
#endif
