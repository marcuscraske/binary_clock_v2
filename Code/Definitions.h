// Shared definitions
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