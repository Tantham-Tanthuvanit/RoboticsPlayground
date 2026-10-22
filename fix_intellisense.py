Import("env")
import os

# Expand the ESP32 3.x @includes file so VS Code IntelliSense can see FreeRTOS headers
libs_pkg = env.PioPlatform().get_package_dir("framework-arduinoespressif32-esp32-libs")
if libs_pkg:
    freertos_inc = os.path.join(libs_pkg, "include", "freertos", "FreeRTOS-Kernel", "include")
    if os.path.exists(freertos_inc):
        env.Append(CPPPATH=[freertos_inc])