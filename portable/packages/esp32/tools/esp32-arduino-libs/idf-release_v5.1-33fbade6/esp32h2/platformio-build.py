# Copyright 2014-present PlatformIO <contact@platformio.org>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Arduino

Arduino Wiring-based Framework allows writing cross-platform software to
control devices attached to a wide range of Arduino boards to create all
kinds of creative coding, interactive objects, spaces or physical experiences.

http://arduino.cc/en/Reference/HomePage
"""

# Extends: https://github.com/platformio/platform-espressif32/blob/develop/builder/main.py

from os.path import basename, join

from SCons.Script import DefaultEnvironment

env = DefaultEnvironment()

FRAMEWORK_DIR = env.PioPlatform().get_package_dir("framework-arduinoespressif32")
FRAMEWORK_SDK_DIR = env.PioPlatform().get_package_dir(
    "framework-arduinoespressif32-libs"
)

board_config = env.BoardConfig()

flatten_cppdefines = env.Flatten(env['CPPDEFINES'])

#
# zigbee libs
#
if "ZIGBEE_MODE_ZCZR" in flatten_cppdefines:
    env.Append(
        LIBS=[
            "-lesp_zb_api_zczr",
            "-lesp_zb_cli_command",
            "-lzboss_stack.zczr",
            "-lzboss_port"
        ]
    )
if "ZIGBEE_MODE_ED" in flatten_cppdefines:
    env.Append(
        LIBS=[
            "-lesp_zb_api_ed",
            "-lesp_zb_cli_command",
            "-lzboss_stack.ed",
            "-lzboss_port"
        ]
    )
if "ZIGBEE_MODE_RCP" in flatten_cppdefines:
    env.Append(
        LIBS=[
            "-lesp_zb_api_rcp",
            "-lesp_zb_cli_command",
            "-lzboss_stack.rcp",
            "-lzboss_port"
        ]
    )

env.Append(
    ASFLAGS=[
        "-march=rv32imc"
    ],

    ASPPFLAGS=[
        "-x", "assembler-with-cpp"
    ],

    CFLAGS=[
        "-march=rv32imac_zicsr_zifencei",
        "-std=gnu17",
        "-Wno-old-style-declaration"
    ],

    CXXFLAGS=[
        "-march=rv32imac_zicsr_zifencei",
        "-std=gnu++2b",
        "-fexceptions",
        "-fno-rtti"
    ],

    CCFLAGS=[
        "-Os",
        "-ffunction-sections",
        "-fdata-sections",
        "-Wno-error=unused-function",
        "-Wno-error=unused-variable",
        "-Wno-error=unused-but-set-variable",
        "-Wno-error=deprecated-declarations",
        "-Wno-unused-parameter",
        "-Wno-sign-compare",
        "-Wno-enum-conversion",
        "-gdwarf-4",
        "-ggdb",
        "-nostartfiles",
        "-freorder-blocks",
        "-Wwrite-strings",
        "-fstack-protector",
        "-fstrict-volatile-bitfields",
        "-fno-jump-tables",
        "-fno-tree-switch-conversion",
        "-MMD"
    ],

    LINKFLAGS=[
        "-nostartfiles",
        "-march=rv32imac_zicsr_zifencei",
        "--specs=nosys.specs",
        "-Wl,--cref",
        "-Wl,--defsym=IDF_TARGET_ESP32H2=0",
        "-Wl,--no-warn-rwx-segments",
        "-fno-rtti",
        "-fno-lto",
        "-Wl,--gc-sections",
        "-Wl,--warn-common",
        "-Wl,--wrap=esp_log_write",
        "-Wl,--wrap=esp_log_writev",
        "-Wl,--wrap=log_printf",
        "-Wl,--wrap=mbedtls_ecdsa_verify",
        "-Wl,--undefined=uxTopUsedPriority",
        "-Wl,--undefined=FreeRTOS_openocd_params",
        "-T", "esp32h2.peripherals.ld",
        "-T", "esp32h2.rom.ld",
        "-T", "esp32h2.rom.api.ld",
        "-T", "esp32h2.rom.libgcc.ld",
        "-T", "esp32h2.rom.newlib.ld",
        "-T", "esp32h2.rom.version.ld",
        "-T", "esp32h2.rom.wdt.ld",
        "-T", "esp32h2.rom.heap.ld",
        "-T", "memory.ld",
        "-T", "sections.ld",
        "-u", "_Z5setupv",
        "-u", "_Z4loopv",
        "-u", "esp_app_desc",
        "-u", "pthread_include_pthread_impl",
        "-u", "pthread_include_pthread_cond_var_impl",
        "-u", "pthread_include_pthread_local_storage_impl",
        "-u", "pthread_include_pthread_rwlock_impl",
        "-u", "pthread_include_pthread_semaphore_impl",
        "-u", "start_app",
        "-u", "__ubsan_include",
        "-u", "tlsf_set_rom_patches",
        "-u", "__assert_func",
        "-u", "esp_crypto_dpa_prot_include_impl",
        "-u", "app_main",
        "-u", "newlib_include_heap_impl",
        "-u", "newlib_include_syscalls_impl",
        "-u", "newlib_include_pthread_impl",
        "-u", "newlib_include_assert_impl",
        "-u", "__cxa_guard_dummy",
        "-u", "include_esp_phy_override",
        "-u", "vfs_include_syscalls_impl",
        '-Wl,-Map="%s"' % join("${BUILD_DIR}", "${PROGNAME}.map")
    ],

    CPPPATH=[
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "newlib", "platform_include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "freertos", "FreeRTOS-Kernel", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "freertos", "FreeRTOS-Kernel", "portable", "riscv", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "freertos", "esp_additions", "include", "freertos"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "freertos", "esp_additions", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "freertos", "esp_additions", "arch", "riscv", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_hw_support", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_hw_support", "include", "soc"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_hw_support", "include", "soc", "esp32h2"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_hw_support", "port", "esp32h2"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_hw_support", "port", "esp32h2", "private_include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "heap", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "log", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "soc", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "soc", "esp32h2"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "soc", "esp32h2", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "hal", "esp32h2", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "hal", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "hal", "platform_port", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_rom", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_rom", "include", "esp32h2"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_rom", "esp32h2"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_common", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_system", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_system", "port", "soc"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_system", "port", "include", "riscv"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_system", "port", "include", "private"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "riscv", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "lwip", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "lwip", "include", "apps"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "lwip", "include", "apps", "sntp"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "lwip", "lwip", "src", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "lwip", "port", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "lwip", "port", "freertos", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "lwip", "port", "esp32xx", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "lwip", "port", "esp32xx", "include", "arch"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-tflite-micro"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-tflite-micro", "third_party", "gemmlowp"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-tflite-micro", "third_party", "flatbuffers", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-tflite-micro", "third_party", "ruy"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-tflite-micro", "third_party", "kissfft"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-tflite-micro", "signal", "micro", "kernels"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-tflite-micro", "signal", "src"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-tflite-micro", "signal", "src", "kiss_fft_wrappers"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_ringbuf", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "efuse", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "efuse", "esp32h2", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "deprecated"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "analog_comparator", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "dac", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "gpio", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "gptimer", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "i2c", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "i2s", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "ledc", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "mcpwm", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "parlio", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "pcnt", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "rmt", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "sdio_slave", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "sdmmc", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "sigma_delta", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "spi", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "temperature_sensor", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "touch_sensor", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "twai", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "uart", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "driver", "usb_serial_jtag", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_pm", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "mbedtls", "port", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "mbedtls", "mbedtls", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "mbedtls", "mbedtls", "library"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "mbedtls", "esp_crt_bundle", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "mbedtls", "mbedtls", "3rdparty", "everest", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "mbedtls", "mbedtls", "3rdparty", "p256-m"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "mbedtls", "mbedtls", "3rdparty", "p256-m", "p256-m"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_app_format", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bootloader_support", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bootloader_support", "bootloader_flash", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_partition", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "app_update", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_mm", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "spi_flash", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "pthread", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_timer", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "app_trace", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_event", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "nvs_flash", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_phy", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_phy", "esp32h2", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "vfs", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_netif", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "wpa_supplicant", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "wpa_supplicant", "port", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "wpa_supplicant", "esp_supplicant", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_coex", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_wifi", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_wifi", "wifi_apps", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "include", "esp32h2", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "common", "osi", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "common", "api", "include", "api"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "common", "btc", "profile", "esp", "blufi", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "common", "btc", "profile", "esp", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "common", "hci_log", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "host", "bluedroid", "api", "include", "api"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "esp_ble_mesh", "mesh_common", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "esp_ble_mesh", "mesh_common", "tinycrypt", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "esp_ble_mesh", "mesh_core"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "esp_ble_mesh", "mesh_core", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "esp_ble_mesh", "mesh_core", "storage"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "esp_ble_mesh", "btc", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "esp_ble_mesh", "mesh_models", "common", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "esp_ble_mesh", "mesh_models", "client", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "esp_ble_mesh", "mesh_models", "server", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "esp_ble_mesh", "api", "core", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "esp_ble_mesh", "api", "models", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "esp_ble_mesh", "api"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "porting", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "porting", "npl", "freertos", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "bt", "porting", "transport", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "unity", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "unity", "unity", "src"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "cmock", "CMock", "src"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "console"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "http_parser"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp-tls"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp-tls", "esp-tls-crypto"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_adc", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_adc", "interface"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_adc", "esp32h2", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_adc", "deprecated", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_eth", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_gdbstub", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_hid", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "tcp_transport", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_http_client", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_http_server", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_https_ota", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_https_server", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_psram", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_lcd", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_lcd", "interface"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "protobuf-c", "protobuf-c"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "protocomm", "include", "common"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "protocomm", "include", "security"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "protocomm", "include", "transports"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "protocomm", "include", "crypto", "srp6a"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "protocomm", "proto-c"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "esp_local_ctrl", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espcoredump", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espcoredump", "include", "port", "riscv"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "wear_levelling", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "sdmmc", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "fatfs", "diskio"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "fatfs", "vfs"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "fatfs", "src"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "idf_test", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "idf_test", "include", "esp32h2"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "ieee802154", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "json", "cJSON"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "mqtt", "esp-mqtt", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "openthread", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "openthread", "openthread", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "spiffs", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "wifi_provisioning", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-nn", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-nn", "src", "common"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__network_provisioning", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "chmorgan__esp-libhelix-mp3", "libhelix-mp3", "pub"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-modbus", "freemodbus", "common", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__libsodium", "libsodium", "src", "libsodium", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__libsodium", "port_include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__mdns", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "dotprod", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "support", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "support", "mem", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "windows", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "windows", "hann", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "windows", "blackman", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "windows", "blackman_harris", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "windows", "blackman_nuttall", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "windows", "nuttall", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "windows", "flat_top", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "iir", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "fir", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "math", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "math", "add", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "math", "sub", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "math", "mul", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "math", "addc", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "math", "mulc", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "math", "sqrt", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "matrix", "mul", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "matrix", "add", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "matrix", "addc", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "matrix", "mulc", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "matrix", "sub", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "matrix", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "fft", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "dct", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "conv", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "common", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "matrix", "mul", "test", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "kalman", "ekf", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-dsp", "modules", "kalman", "ekf_imu13states", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-zboss-lib", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-zigbee-lib", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__rmaker_common", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp_diagnostics", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__cbor", "port", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp_diag_data_store", "src", "rtc_store"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp_diag_data_store", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp_insights", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp_modem", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__jsmn", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__json_parser", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__json_generator", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp_schedule", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-serial-flasher", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp-serial-flasher", "port"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp_rcp_update", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp_secure_cert_mgr", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__esp_rainmaker", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "espressif__qrcode", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "joltwallet__littlefs", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "include", "fb_gfx", "include"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", board_config.get("build.arduino.memory_type", (board_config.get("build.flash_mode", "dio") + "_qspi")), "include"),
        join(FRAMEWORK_DIR, "cores", board_config.get("build.core"))
    ],

    LIBPATH=[
        join(FRAMEWORK_SDK_DIR, "esp32h2", "lib"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", "ld"),
        join(FRAMEWORK_SDK_DIR, "esp32h2", board_config.get("build.arduino.memory_type", (board_config.get("build.flash_mode", "dio") + "_qspi")))
    ],

    LIBS=[
        "-lriscv", "-lesp_ringbuf", "-lefuse", "-ldriver", "-lesp_pm", "-lmbedtls", "-lesp_app_format", "-lbootloader_support", "-lesp_partition", "-lapp_update", "-lesp_mm", "-lspi_flash", "-lpthread", "-lesp_system", "-lesp_rom", "-lhal", "-llog", "-lheap", "-lsoc", "-lesp_hw_support", "-lfreertos", "-lnewlib", "-lcxx", "-lesp_common", "-lesp_timer", "-lapp_trace", "-lesp_event", "-lnvs_flash", "-lesp_phy", "-lvfs", "-llwip", "-lesp_netif", "-lwpa_supplicant", "-lesp_coex", "-lbt", "-lunity", "-lcmock", "-lconsole", "-lhttp_parser", "-lesp-tls", "-lesp_adc", "-lesp_eth", "-lesp_gdbstub", "-lesp_hid", "-ltcp_transport", "-lesp_http_client", "-lesp_http_server", "-lesp_https_ota", "-lesp_https_server", "-lesp_lcd", "-lprotobuf-c", "-lprotocomm", "-lesp_local_ctrl", "-lespcoredump", "-lwear_levelling", "-lsdmmc", "-lfatfs", "-lieee802154", "-ljson", "-lmqtt", "-lopenthread", "-lspiffs", "-lwifi_provisioning", "-lespressif__esp-nn", "-lespressif__esp-tflite-micro", "-lespressif__network_provisioning", "-lchmorgan__esp-libhelix-mp3", "-lespressif__esp-modbus", "-lespressif__libsodium", "-lespressif__mdns", "-lespressif__esp-dsp", "-lespressif__rmaker_common", "-lespressif__esp_diagnostics", "-lespressif__cbor", "-lespressif__esp_diag_data_store", "-lespressif__esp_insights", "-lespressif__esp_modem", "-lespressif__json_parser", "-lespressif__json_generator", "-lespressif__esp_schedule", "-lespressif__esp-serial-flasher", "-lespressif__esp_rcp_update", "-lespressif__esp_secure_cert_mgr", "-lespressif__esp_rainmaker", "-lespressif__qrcode", "-ljoltwallet__littlefs", "-lfb_gfx", "-lapp_trace", "-lapp_trace", "-lcmock", "-lunity", "-lesp_lcd", "-lespressif__esp-tflite-micro", "-lespressif__esp-nn", "-lm", "-lesp_hid", "-lfatfs", "-lwear_levelling", "-lspiffs", "-lchmorgan__esp-libhelix-mp3", "-lespressif__esp-modbus", "-lespressif__libsodium", "-lespressif__esp-dsp", "-lespressif__esp_insights", "-lespressif__esp_diagnostics", "-lespressif__cbor", "-lespressif__esp_diag_data_store", "-lespressif__esp_modem", "-lespressif__esp_rainmaker", "-lesp_local_ctrl", "-lesp_https_server", "-lwifi_provisioning", "-lespressif__network_provisioning", "-lprotocomm", "-lbt", "-lble_app", "-lprotobuf-c", "-ljson", "-lespressif__mdns", "-lespressif__json_parser", "-lespressif__json_generator", "-lespressif__esp_schedule", "-lespressif__rmaker_common", "-lmqtt", "-lespressif__esp_rcp_update", "-lespressif__esp-serial-flasher", "-lespressif__esp_secure_cert_mgr", "-lespressif__qrcode", "-ljoltwallet__littlefs", "-lsdmmc", "-lriscv", "-lesp_ringbuf", "-lefuse", "-ldriver", "-lesp_pm", "-lmbedtls", "-lesp_app_format", "-lbootloader_support", "-lesp_partition", "-lapp_update", "-lesp_mm", "-lspi_flash", "-lpthread", "-lesp_system", "-lesp_rom", "-lhal", "-llog", "-lheap", "-lsoc", "-lesp_hw_support", "-lfreertos", "-lnewlib", "-lcxx", "-lesp_common", "-lesp_timer", "-lesp_event", "-lnvs_flash", "-lesp_phy", "-lvfs", "-llwip", "-lesp_netif", "-lwpa_supplicant", "-lesp_coex", "-lconsole", "-lhttp_parser", "-lesp-tls", "-lesp_adc", "-lesp_eth", "-lesp_gdbstub", "-ltcp_transport", "-lesp_http_client", "-lesp_http_server", "-lesp_https_ota", "-lespcoredump", "-lieee802154", "-lopenthread", "-lmbedtls_2", "-lmbedcrypto", "-lmbedx509", "-leverest", "-lp256m", "-lcoexist", "-lriscv", "-lesp_ringbuf", "-lefuse", "-ldriver", "-lesp_pm", "-lmbedtls", "-lesp_app_format", "-lbootloader_support", "-lesp_partition", "-lapp_update", "-lesp_mm", "-lspi_flash", "-lpthread", "-lesp_system", "-lesp_rom", "-lhal", "-llog", "-lheap", "-lsoc", "-lesp_hw_support", "-lfreertos", "-lnewlib", "-lcxx", "-lesp_common", "-lesp_timer", "-lesp_event", "-lnvs_flash", "-lesp_phy", "-lvfs", "-llwip", "-lesp_netif", "-lwpa_supplicant", "-lesp_coex", "-lconsole", "-lhttp_parser", "-lesp-tls", "-lesp_adc", "-lesp_eth", "-lesp_gdbstub", "-ltcp_transport", "-lesp_http_client", "-lesp_http_server", "-lesp_https_ota", "-lespcoredump", "-lieee802154", "-lopenthread", "-lmbedtls_2", "-lmbedcrypto", "-lmbedx509", "-leverest", "-lp256m", "-lcoexist", "-lriscv", "-lesp_ringbuf", "-lefuse", "-ldriver", "-lesp_pm", "-lmbedtls", "-lesp_app_format", "-lbootloader_support", "-lesp_partition", "-lapp_update", "-lesp_mm", "-lspi_flash", "-lpthread", "-lesp_system", "-lesp_rom", "-lhal", "-llog", "-lheap", "-lsoc", "-lesp_hw_support", "-lfreertos", "-lnewlib", "-lcxx", "-lesp_common", "-lesp_timer", "-lesp_event", "-lnvs_flash", "-lesp_phy", "-lvfs", "-llwip", "-lesp_netif", "-lwpa_supplicant", "-lesp_coex", "-lconsole", "-lhttp_parser", "-lesp-tls", "-lesp_adc", "-lesp_eth", "-lesp_gdbstub", "-ltcp_transport", "-lesp_http_client", "-lesp_http_server", "-lesp_https_ota", "-lespcoredump", "-lieee802154", "-lopenthread", "-lmbedtls_2", "-lmbedcrypto", "-lmbedx509", "-leverest", "-lp256m", "-lcoexist", "-lriscv", "-lesp_ringbuf", "-lefuse", "-ldriver", "-lesp_pm", "-lmbedtls", "-lesp_app_format", "-lbootloader_support", "-lesp_partition", "-lapp_update", "-lesp_mm", "-lspi_flash", "-lpthread", "-lesp_system", "-lesp_rom", "-lhal", "-llog", "-lheap", "-lsoc", "-lesp_hw_support", "-lfreertos", "-lnewlib", "-lcxx", "-lesp_common", "-lesp_timer", "-lesp_event", "-lnvs_flash", "-lesp_phy", "-lvfs", "-llwip", "-lesp_netif", "-lwpa_supplicant", "-lesp_coex", "-lconsole", "-lhttp_parser", "-lesp-tls", "-lesp_adc", "-lesp_eth", "-lesp_gdbstub", "-ltcp_transport", "-lesp_http_client", "-lesp_http_server", "-lesp_https_ota", "-lespcoredump", "-lieee802154", "-lopenthread", "-lmbedtls_2", "-lmbedcrypto", "-lmbedx509", "-leverest", "-lp256m", "-lcoexist", "-lriscv", "-lesp_ringbuf", "-lefuse", "-ldriver", "-lesp_pm", "-lmbedtls", "-lesp_app_format", "-lbootloader_support", "-lesp_partition", "-lapp_update", "-lesp_mm", "-lspi_flash", "-lpthread", "-lesp_system", "-lesp_rom", "-lhal", "-llog", "-lheap", "-lsoc", "-lesp_hw_support", "-lfreertos", "-lnewlib", "-lcxx", "-lesp_common", "-lesp_timer", "-lesp_event", "-lnvs_flash", "-lesp_phy", "-lvfs", "-llwip", "-lesp_netif", "-lwpa_supplicant", "-lesp_coex", "-lconsole", "-lhttp_parser", "-lesp-tls", "-lesp_adc", "-lesp_eth", "-lesp_gdbstub", "-ltcp_transport", "-lesp_http_client", "-lesp_http_server", "-lesp_https_ota", "-lespcoredump", "-lieee802154", "-lopenthread", "-lmbedtls_2", "-lmbedcrypto", "-lmbedx509", "-leverest", "-lp256m", "-lcoexist", "-lriscv", "-lesp_ringbuf", "-lefuse", "-ldriver", "-lesp_pm", "-lmbedtls", "-lesp_app_format", "-lbootloader_support", "-lesp_partition", "-lapp_update", "-lesp_mm", "-lspi_flash", "-lpthread", "-lesp_system", "-lesp_rom", "-lhal", "-llog", "-lheap", "-lsoc", "-lesp_hw_support", "-lfreertos", "-lnewlib", "-lcxx", "-lesp_common", "-lesp_timer", "-lesp_event", "-lnvs_flash", "-lesp_phy", "-lvfs", "-llwip", "-lesp_netif", "-lwpa_supplicant", "-lesp_coex", "-lconsole", "-lhttp_parser", "-lesp-tls", "-lesp_adc", "-lesp_eth", "-lesp_gdbstub", "-ltcp_transport", "-lesp_http_client", "-lesp_http_server", "-lesp_https_ota", "-lespcoredump", "-lieee802154", "-lopenthread", "-lmbedtls_2", "-lmbedcrypto", "-lmbedx509", "-leverest", "-lp256m", "-lcoexist", "-lc", "-lm", "-lnewlib", "-lstdc++", "-lpthread", "-lgcc", "-lcxx", "-lphy", "-lbtbb", "-lesp_phy", "-lphy", "-lbtbb", "-lesp_phy", "-lphy", "-lbtbb"
    ],

    CPPDEFINES=[
        "ESP_PLATFORM",
        ("IDF_VER", '\\"v5.1.4-828-g33fbade6b8-dirty\\"'),
        ("MBEDTLS_CONFIG_FILE", '\\"mbedtls/esp_config.h\\"'),
        ("MD5_ENABLED", 1),
        ("OPENTHREAD_CONFIG_FILE", '\\"openthread-core-esp32x-ftd-config.h\\"'),
        ("OPENTHREAD_FTD", 1),
        ("OPENTHREAD_PROJECT_LIB_CONFIG_FILE", '\\"openthread-core-esp32x-ftd-config.h\\"'),
        ("SERIAL_FLASHER_BOOT_HOLD_TIME_MS", 50),
        ("SERIAL_FLASHER_RESET_HOLD_TIME_MS", 100),
        ("SOC_MMU_PAGE_SIZE", 'CONFIG_MMU_PAGE_SIZE'),
        "UNITY_INCLUDE_CONFIG_H",
        "_GNU_SOURCE",
        "_POSIX_READER_WRITER_LOCKS",
        ("configENABLE_FREERTOS_DEBUG_OCDAWARE", 1),
        "TF_LITE_STATIC_MEMORY",
        "ARDUINO_ARCH_ESP32",
        ("ESP32", "ESP32"),
        ("F_CPU", "$BOARD_F_CPU"),
        ("ARDUINO", 10812),
        ("ARDUINO_VARIANT", '\\"%s\\"' % board_config.get("build.variant").replace('"', "")),
        ("ARDUINO_BOARD", '\\"%s\\"' % board_config.get("name").replace('"', "")),
        "ARDUINO_PARTITION_%s" % basename(board_config.get(
            "build.partitions", "default.csv")).replace(".csv", "").replace("-", "_")
    ]
)
