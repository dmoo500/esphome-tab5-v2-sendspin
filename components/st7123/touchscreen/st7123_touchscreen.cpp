#include "st7123_touchscreen.h"
#include "esphome/core/log.h"

namespace esphome {
namespace st7123 {

static const char *const TAG = "st7123.touchscreen";

static const uint8_t ST7123_ADDR_1 = 0x40;
static const uint8_t ST7123_ADDR_2 = 0x41;

void ST7123Touchscreen::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ST7123 Touchscreen...");
  
  if (this->reset_pin_ != nullptr) {
    this->reset_pin_->setup();
    this->reset_pin_->digital_write(false);
    delay(10);
    this->reset_pin_->digital_write(true);
    this->set_timeout(50, [this] { this->setup_internal_(); });
  } else {
    this->setup_internal_();
  }
}

void ST7123Touchscreen::setup_internal_() {
  if (this->interrupt_pin_ != nullptr) {
    this->interrupt_pin_->setup();
    this->interrupt_pin_->pin_mode(gpio::FLAG_INPUT | gpio::FLAG_PULLUP);
  }

  // Try both possible I2C addresses
  if (!this->write_bytes(0, nullptr, 0)) {
    ESP_LOGD(TAG, "Found ST7123 at address 0x%02X", this->address_);
  } else {
    // Try alternate address
    uint8_t addr = (this->address_ == ST7123_ADDR_1) ? ST7123_ADDR_2 : ST7123_ADDR_1;
    this->set_i2c_address(addr);
    if (!this->write_bytes(0, nullptr, 0)) {
      ESP_LOGD(TAG, "Found ST7123 at alternate address 0x%02X", addr);
    } else {
      ESP_LOGE(TAG, "Failed to communicate with ST7123");
      this->mark_failed();
      return;
    }
  }

  this->setup_done_ = true;
  ESP_LOGCONFIG(TAG, "ST7123 setup complete");
}

void ST7123Touchscreen::dump_config() {
  ESP_LOGCONFIG(TAG, "ST7123 Touchscreen:");
  LOG_I2C_DEVICE(this);
  LOG_PIN("  Interrupt Pin: ", this->interrupt_pin_);
  LOG_PIN("  Reset Pin: ", this->reset_pin_);
  
  if (this->is_failed()) {
    ESP_LOGE(TAG, "  Setup failed!");
  }
}

void ST7123Touchscreen::update_touches() {
  uint8_t data[16];
  
  // Read touch data from device
  if (!this->read_bytes_raw(data, 16)) {
    ESP_LOGW(TAG, "Failed to read touch data");
    return;
  }

  // Parse touch points
  uint8_t num_touches = data[0] & 0x0F;
  if (num_touches > 5) {
    num_touches = 0;
  }

  for (uint8_t i = 0; i < num_touches; i++) {
    uint8_t offset = 2 + (i * 6);
    if (offset + 5 >= 16) break;

    uint16_t x = ((data[offset] & 0x0F) << 8) | data[offset + 1];
    uint16_t y = ((data[offset + 2] & 0x0F) << 8) | data[offset + 3];
    uint8_t id = (data[offset + 4] >> 4) & 0x0F;

    this->add_raw_touch_position_(id, x, y);
  }

  // Check for button state changes
  if (data[1] != this->button_state_) {
    for (auto *listener : this->button_listeners_) {
      for (uint8_t i = 0; i < 8; i++) {
        bool old_state = (this->button_state_ >> i) & 0x01;
        bool new_state = (data[1] >> i) & 0x01;
        if (old_state != new_state) {
          listener->update_button(i, new_state);
        }
      }
    }
    this->button_state_ = data[1];
  }
}

}  // namespace st7123
}  // namespace esphome