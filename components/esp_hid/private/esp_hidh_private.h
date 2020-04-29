// Copyright 2017-2019 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _ESP_HIDH_PRIVATE_H_
#define _ESP_HIDH_PRIVATE_H_

#include "esp_hidh.h"
#if CONFIG_BLUEDROID_ENABLED
#include "esp_gap_bt_api.h"
#endif /* CONFIG_BLUEDROID_ENABLED */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_event.h"
#include "sys/queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief HIDH device report data
 */
typedef struct esp_hidh_dev_report_s {
    struct esp_hidh_dev_report_s *next;
    uint8_t map_index;      //the index of the report map
    uint8_t report_id;      //the id of the report
    uint8_t report_type;    //input, output or feature
    uint8_t protocol_mode;  //boot or report
    size_t value_len;       //maximum len of value by report map
    esp_hid_usage_t usage;  //generic, keyboard or mouse
    //BLE properties
    uint16_t handle;        //handle to the value
    uint16_t ccc_handle;    //handle to client config
    uint8_t permissions;    //report permissions
} esp_hidh_dev_report_t;

/**
 * @brief HIDH device data
 */
struct esp_hidh_dev_s {
    struct esp_hidh_dev_s   *next;

    esp_hid_device_config_t config;
    esp_hid_usage_t         usage;
    esp_hid_transport_t     transport; //BT, BLE or USB
    bool                    connected; //we have all required data to communicate
    bool                    opened;    //we opened the device manually, else the device connected to us
    int                     status;    //status of the last command

    size_t                  reports_len;
    esp_hidh_dev_report_t   *reports;

    void                    *tmp;
    size_t                  tmp_len;

    xSemaphoreHandle        semaphore;

    esp_err_t               (*close)        (esp_hidh_dev_t *dev);
    esp_err_t               (*report_write) (esp_hidh_dev_t *dev, size_t map_index, size_t report_id, int report_type, uint8_t *data, size_t len);
    esp_err_t               (*report_read)  (esp_hidh_dev_t *dev, size_t map_index, size_t report_id, int report_type, size_t max_length, uint8_t *value, size_t *value_len);
    void                    (*dump)         (esp_hidh_dev_t *dev, FILE *fp);

#if CONFIG_BLUEDROID_ENABLED
    esp_bd_addr_t bda;
#endif /* CONFIG_BLUEDROID_ENABLED */

    union {
#if CONFIG_BT_HID_HOST_ENABLED
        struct {
            esp_bt_cod_t cod;
            int handle;
            uint8_t sub_class;
            uint8_t app_id;
            uint16_t attr_mask;
        } bt;
#endif /* CONFIG_BT_HID_HOST_ENABLED */
#if CONFIG_GATTC_ENABLE
        struct {
            esp_ble_addr_type_t address_type;
            int conn_id;
            uint16_t appearance;
            uint16_t battery_handle;
            uint16_t battery_ccc_handle;
        } ble;
#endif /* CONFIG_GATTC_ENABLE */
    };
    TAILQ_ENTRY(esp_hidh_dev_s) devices;
};

typedef TAILQ_HEAD(esp_hidh_dev_head_s, esp_hidh_dev_s) esp_hidh_dev_head_t;

esp_hidh_dev_t *esp_hidh_dev_malloc(void);

#if CONFIG_BLUEDROID_ENABLED
esp_hidh_dev_t *esp_hidh_dev_get_by_bda(esp_bd_addr_t bda); //BT/BLE
esp_hidh_dev_t *esp_hidh_dev_get_by_handle(int handle); //BT Only
esp_hidh_dev_t *esp_hidh_dev_get_by_conn_id(uint16_t conn_id); //BLE Only
#endif /* CONFIG_BLUEDROID_ENABLED */

esp_hidh_dev_report_t *esp_hidh_dev_get_report_by_id_and_type(esp_hidh_dev_t *dev, size_t map_index, size_t report_id, int report_type);
esp_hidh_dev_report_t *esp_hidh_dev_get_input_report_by_id_and_proto(esp_hidh_dev_t *dev, size_t report_id, int protocol_mode);
esp_hidh_dev_report_t *esp_hidh_dev_get_report_by_handle(esp_hidh_dev_t *dev, uint16_t handle);  //BLE Only


#ifdef __cplusplus
}
#endif

#endif /* _ESP_HIDH_PRIVATE_H_ */
