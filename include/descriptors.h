#include "common/tusb_common.h"
#include "device/usbd.h"

// Gamepad Report Descriptor Template
// with 2 buttons and 1 joysticks with following layout
// | Button Map (2 bytes) |  X | Y 
#define GAMECON_REPORT_DESC_GAMEPAD(...)                   \
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),                \
        HID_USAGE(HID_USAGE_DESKTOP_GAMEPAD),              \
        HID_COLLECTION(HID_COLLECTION_APPLICATION),        \
        __VA_ARGS__                                        \
            HID_USAGE_PAGE(HID_USAGE_PAGE_BUTTON),         \
        HID_USAGE_MIN(1),                                  \
        HID_USAGE_MAX(2),                                  \
        HID_LOGICAL_MIN(0),                                \
        HID_LOGICAL_MAX(1),                                \
        HID_REPORT_COUNT(8),                               \
        HID_REPORT_SIZE(1),                                \
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
        HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),            \
        HID_LOGICAL_MIN(0x00),                             \
        HID_LOGICAL_MAX_N(0x00ff, 2),                      \
        HID_USAGE(HID_USAGE_DESKTOP_X),                    \
        HID_USAGE(HID_USAGE_DESKTOP_Y),                    \
        HID_REPORT_COUNT(2),                               \
        HID_REPORT_SIZE(8),                                \
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
        HID_COLLECTION_END

