/*                       ____               ____________
 *                      |    |             |            |
 *                      |    |             |    ________|
 *                      |    |             |   |
 *                      |    |             |   |    
 *                      |    |             |   |    ____
 *                      |    |             |   |   |    |
 *                      |    |_______      |   |___|    |
 *                      |            |  _  |            |
 *                      |____________| |_| |____________|
 *                        
 *      Author(s):      limpygnome (Marcus Craske)              limpygnome@gmail.com
 * 
 *      License:        Creative Commons Attribution-ShareAlike 3.0 Unported
 *                      http://creativecommons.org/licenses/by-sa/3.0/
 * 
 *      File:           Definitions.h
 *      Path:           BC/Definitions.h
 * 
 *      Change-Log:
 *                      2013-06-25      Added header.
 * 
 * *****************************************************************************
 * Global definitions file; this file should only contain preprocessor macros.
 * *****************************************************************************
 */

// Shared definitions **********************************************************
#define GPIO_MIN                                0
#define GPIO_MAX                                17
#define IC_74HC595_MAX_REGISTERS                32
#define MCP3008_PIN_MIN                         0
#define MCP3008_PIN_MAX                         7
#define PORT_MIN                                1
#define PORT_MAX                                50000

// Configuration key names
#define CONFIG__ALARM__BUZZES                   "alarm.buzzes"
#define CONFIG__ALARM__BUZZLENGTH               "alarm.buzz_length"
#define CONFIG__ALARM__TIMES                    "alarm.times"
#define CONFIG__ALARM__BUZZER_PIN               "alarm.buzzer_pin"

#define CONFIG__DISPLAY__REGISTERS              "display.registers"
#define CONFIG__DISPLAY__PIN_DATA               "display.pin_data"
#define CONFIG__DISPLAY__PIN_LATCH              "display.pin_latch"
#define CONFIG__DISPLAY__PIN_CLOCK              "display.pin_clock"
#define CONFIG__DISPLAY__LIGHT_LOW              "display.light_low"
#define CONFIG__DISPLAY__LIGHT_HIGH             "display.light_high"
#define CONFIG__DISPLAY__LIGHT_BUZZ             "display.light_buzz"
#define CONFIG__DISPLAY__LIGHT_THRESHOLD        "display.light_threshold"

#define CONFIG__SENSORS__MCP_PIN_TMP36          "sensors.mcp_pin_tmp36"
#define CONFIG__SENSORS__MCP_PIN_PHOTORESISTOR  "sensors.mcp_pin_photoresistor"
#define CONFIG__SENSORS__PIN_DATA_IN            "sensors.pin_data_in"
#define CONFIG__SENSORS__PIN_DATA_OUT           "sensors.pin_data_out"
#define CONFIG__SENSORS__PIN_CLOCK              "sensors.pin_clock"
#define CONFIG__SENSORS__PIN_CHIP_SELECT        "sensors.pin_chip_select"
#define CONFIG__SENSORS__POLLRATE               "sensors.poll_rate"

#define CONFIG__WEBHTTP__PORT                   "web.port"
#define CONFIG__WEBHTTP__SOCKETS_BACKLOG        "web.sockets_backlog"
#define CONFIG__WEBHTTP__THREADS_MAX            "web.threads_max"

#define CONFIG__HTTPHANDLER_PAGE_DEFAULT        "httphandler.page_default"
#define CONFIG__HTTPHANDLER_PAGE_NOTFOUND       "httphandler.page_notfound"

#define CONFIG__HTTP_DISKCACHE_EXTENSIONS       "diskcache.extensions"
#define CONFIG__HTTP_DISKCACHE_ENABLED          "diskcache.enabled"

#define CONFIG__DATABASE_HOST                   "database.host"
#define CONFIG__DATABASE_PORT                   "database.port"
#define CONFIG__DATABASE_DB                     "database.db"
#define CONFIG__DATABASE_USER                   "database.user"
#define CONFIG__DATABASE_PASS                   "database.pass"
#define CONFIG__GUESTBOOK_POST_THRESHOLD        "guestbook.post_threshold"

#define CONFIG__COUNTRYLOOKUP_LAST_UPDATED      "countrylookup.last_updated_lists"
#define CONFIG__COUNTRYLOOKUP_UPDATE_INTERVAL   "countrylookup.update_interval"

#define CONFIG__RELAYBOARD_TOTAL_SOCKETS        "relayboard.total_sockets"

// Service titles
#define SERVICETITLE_CONFIGURATOR               "Configurator"
#define SERVICETITLE_ALARM                      "Alarm"
#define SERVICETITLE_DISPLAY                    "Display"
#define SERVICETITLE_SENSORS                    "Sensors"
#define SERVICETITLE_WEBHTTP                    "Web Server"
#define SERVICETITLE_COUNTRYLOOKUP              "IP Address Country Lookup"
#define SERVICETITLE_RELAYBOARD                 "Relay Board Controller"

// Service - Alarm *************************************************************
#define ALARM_DEFAULT_PIN_BUZZER                7
#define ALARM_DEFAULT_BUZZES                    20
#define ALARM_DEFAULT_BUZZ_LENGTH               1

// Service - Configurator ******************************************************
#define CONFIG_SAVE_WAIT                        1000LL                  // Time in milliseconds to wait to
                                                                        // save data to disk.

#define CONFIG_SAVE_MAXWAIT                     10000LL                 // The maximum time in milliseconds to
                                                                        // wait to save data to disk; this avoids
                                                                        // indefinite postponement.

#define CONFIG_FILE                             "binaryclock.config"    // The file location to store
                                                                        // configuration on disk.

#define CONFIG_NEWLINE_CHAR                     '$'                     // \n is replaced with this character.


// Service - Country Lookup ****************************************************
// Defintiions: the IP block lists of the five IP registrants.
// -- Heavy uses:
// -- http://www.apnic.net/publications/media-library/documents/resource-guidelines/rir-statistics-exchange-format
#define COUNTRYLOOKUP_LIST_RIPENCC              "iplist_ripencc"
#define COUNTRYLOOKUP_LIST_RIPENCC_URL          "ftp.arin.net/pub/stats/ripencc/delegated-ripencc-latest"
#define COUNTRYLOOKUP_LIST_LACNIC               "iplist_lacnic"
#define COUNTRYLOOKUP_LIST_LACNIC_URL           "ftp.arin.net/pub/stats/lacnic/delegated-lacnic-latest"
#define COUNTRYLOOKUO_LIST_ARIN                 "iplist_arin"
#define COUNTRYLOOKUP_LIST_ARIN_URL             "ftp.arin.net/pub/stats/arin/delegated-arin-latest"
#define COUNTRYLOOKUP_LIST_APNIC                "iplist_apnic"
#define COUNTRYLOOKUP_LIST_APNIC_URL            "ftp.arin.net/pub/stats/apnic/delegated-apnic-latest"
#define COUNTRYLOOKUP_LIST_AFRNIC               "iplist_afrinic"
#define COUNTRYLOOKUP_LIST_AFRINIC_URL          "ftp.arin.net/pub/stats/afrinic/delegated-afrinic-latest"

#define COUNTRYLOOKUP_LIST_UPDATE_INTERVAL      86400000LL // Default: 1 day

// Service - Display ***********************************************************
// -- Definitions of shift-register pins for the LEDs
// -- -- Hours
#define DISPLAY_LED_H_16                        1
#define DISPLAY_LED_H_8                         2
#define DISPLAY_LED_H_4                         4
#define DISPLAY_LED_H_2                         8
#define DISPLAY_LED_H_1                         16
// -- -- Minutes
#define DISPLAY_LED_M_32                        32
#define DISPLAY_LED_M_16                        64
#define DISPLAY_LED_M_8                         128
#define DISPLAY_LED_M_4                         256
#define DISPLAY_LED_M_2                         512
#define DISPLAY_LED_M_1                         1024
// -- -- Seconds
#define DISPLAY_LED_S_32                        2048
#define DISPLAY_LED_S_16                        4096
#define DISPLAY_LED_S_8                         8192
#define DISPLAY_LED_S_4                         16384
#define DISPLAY_LED_S_2                         32768
#define DISPLAY_LED_S_1                         65536
// -- -- Error LEDs
#define DISPLAY_LED_ERROR_1                     131072
#define DISPLAY_LED_ERROR_2                     262144
#define DISPLAY_LED_ERROR_3                     524288
// -- Definitions of pin info for interfacing with the shift-registers
#define DISPLAY_DEFAULT_REGISTERS               3
#define DISPLAY_DEFAULT_PIN_DATA                14
#define DISPLAY_DEFAULT_PIN_LATCH               18
#define DISPLAY_DEFAULT_PIN_CLOCK               15
// -- Definitions of default light sequences
#define DISPLAY_DEFAULT_SEQUENCE_LIGHT_LOW      Sequence::Offline
#define DISPLAY_DEFAULT_SEQUENCE_LIGHT_HIGH     Sequence::Time
#define DISPLAY_DEFAULT_SEQUENCE_LIGHT_BUZZ     Sequence::AllLeds
#define DISPLAY_DEFAULT_LIGHT_THRESHOLD         35

// Service - Relay Board *******************************************************
#define RELAYBOARD_LOGIC_INTERVAL               1000    // The delay in m/s between checking the logic for sockets.
#define RELAYBOARD_DEFAULT_SOCKETS              6       // The default number of relay-board relays.

// Service - Sensors ***********************************************************
#define SENSORS_DEFAULT_MCP_TMP36                IC_MCP3008::Pin1
#define SENSORS_DEFAULT_MCP_PHOTORESISTOR        IC_MCP3008::Pin0
#define SENSORS_DEFAULT_PIN_CLOCK                23
#define SENSORS_DEFAULT_PIN_DATA_OUT             24
#define SENSORS_DEFAULT_PIN_DATA_IN              25
#define SENSORS_DEFAULT_PIN_CHIP_SELECT          8
#define SENSORS_DEFAULT_POLL_RATE_MS             500
#define SENSORS_VOLTAGE                         3.3
#define SENSORS_MCP_TMP36_ERROR                 4
#define SENSORS_MCP_PHOTORESISTOR_ERROR         7

// Service - Web ***************************************************************
#define WEBHTTP_DEFAULT_PORT                    5000
#define WEBHTTP_DEFAULT_SOCKETS_BACKLOG         64
#define WEBHTTP_DEFAULT_THREADS_MAX             64
#define WEBHTTP_SOCKET_POLLRATE_MS              500

// Disk Cache ******************************************************************
#define HTTP_DISKCACHE_DEFAULT_EXTENSIONS       ".bc,.bct,.css,.png,.jpg"

// HTTP Handler ****************************************************************
#define HTTPHANDLER_DEFAULT_PAGE_DEFAULT        "home.bc"
#define HTTPHANDLER_DEFAULT_PAGE_NOTFOUND       "404.bct"

// HTTP Request ****************************************************************
#define HTTP_HEADER_LIMIT_BYTES 8190

// Pages ***********************************************************************
#define MODULES_PAGES_DEFAULT_ADMIN_IP_START            "10.0."
#define MODULES_PAGES_DEFAULT_HOST                      "127.0.0.1"
#define MODULES_PAGES_DEFAULT_PORT                      5432
#define MODULES_PAGES_DEFAULT_DATABASE                  "main"
#define MODULES_PAGES_DEFAULT_USER                      "postgres"
#define MODULES_PAGES_DEFAULT_PASS                      ""

#define MODULES_PAGES_GUESTBOOK_POSTSPERPAGE            4
#define MODULES_PAGES_GUESTBOOK_NAME_MIN                0
#define MODULES_PAGES_GUESTBOOK_NAME_MAX                24
#define MODULES_PAGES_GUESTBOOK_MESSAGE_MIN             3
#define MODULES_PAGES_GUESTBOOK_MESSAGE_MAX             512
#define MODULES_PAGES_GUESTBOOK_DEFAULT_POST_THRESHOLD  3
