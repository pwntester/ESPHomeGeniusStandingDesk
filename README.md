# ESPHome Component for Genius Control Box

Heavily inspired in https://github.com/TillFleisch/ESPHomeGenericSitStandDesk

# Usage

```yaml
substitutions:
  desk_id: "standingdesk"

esphome:
  name: standing-desk

external_components:
  #- source: github://pwntester/ESPHomeGeniusStandingDesk@master
  - source:
      type: local
      path: components

esp8266:
  board: d1_mini

# Enable logging
logger:
  baud_rate: 0

# Enable Home Assistant API
api:
  encryption:
    key: !secret api_encryption_key

ota:
  password: !secret ota_password

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "Standingdesk Fallback Hotspot"
    password: !secret ap_password

captive_portal:

uart:
  id: uart_bus
  rx_pin: RX
  tx_pin: TX
  baud_rate: 9600

genius_desk:
  id: ${desk_id}
  uart_id: uart_bus
  min_height: 80
  max_height: 120

sensor:
  - platform: genius_desk
    desk_id: ${desk_id}
    id: desk_height
    name: "Desk Height"

binary_sensor:
  - platform: genius_desk
    desk_id: ${desk_id}
    calibration_needed:
      name: "Calibration Needed"
    desk_is_moving:
      name: "Desk is Moving"

switch:
  - platform: genius_desk
    desk_id: ${desk_id}
    up:
      name: "Up"
    down:
      name: "Down"

button:
  - platform: genius_desk
    desk_id: ${desk_id}
    M1:
      name: "M1"
    M2:
      name: "M2"
    M3:
      name: "M3"
    M4:
      name: "M4"
    calibration:
      name: "Calibrate"
```

```
esphome run standing-desk.yaml
```
