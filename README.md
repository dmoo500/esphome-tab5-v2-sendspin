# ESPHome Tab5 V2 with Sendspin

ESPHome configuration for M5Stack Tab5 with Sendspin media player integration.

## Features

- 🎨 **LVGL UI** with navigation sidebar
  - Dashboard with clock, temperature, and humidity
  - Light control with brightness and color palette
  - Climate control (HVAC)
  - Media Player controls

- 🎵 **Media Player**
  - Sendspin integration
  - Album art display
  - Play/Pause, Next/Previous controls
  - Volume control with visual feedback
  - Swipe gestures support

- 🔋 **Hardware Support**
  - M5Stack Tab5 V2 display (1280x720)
  - ST7123 touchscreen
  - Battery monitoring with charging status
  - ES8388 audio DAC
  - WiFi with internal/external antenna selection

## Hardware

- **Device**: M5Stack Tab5 (ESP32-P4)
- **Display**: MIPI DSI 1280x720
- **Audio**: ES8388 DAC, ES7210 ADC
- **Connectivity**: WiFi (ESP32-C6 hosted)

## Configuration Files

- `tab5-v2-ma-hdm.yaml` - Main configuration
- `common/tab5-v2-globals.yaml` - Global variables
- `common/tab5-v2-fonts.yaml` - Font definitions
- `common/tab5-v2-images.yaml` - Image resources
- `common/tab5-v2-lvgl.yaml` - LVGL UI layout
- `common/tab5-v2-scripts.yaml` - Automation scripts

## Setup

1. Copy `secrets.yaml.example` to `secrets.yaml` and configure:
   ```yaml
   wifi_ssid: "YourWiFiSSID"
   wifi_password: "YourWiFiPassword"
   ```

2. Compile and upload with ESPHome:
   ```bash
   esphome run tab5-v2-ma-hdm.yaml
   ```

## Home Assistant Integration

The configuration integrates with Home Assistant for:
- Light control (Wohnzimmer)
- Climate control (HVAC)
- Temperature and humidity sensors
- Media player state synchronization

## Requirements

- ESPHome 2025.12.0 or newer
- Home Assistant (for full functionality)

## License

MIT

## Credits

Based on ESPHome and leverages the Sendspin media player component.
