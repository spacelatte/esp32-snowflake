
# 2019 (special) Snowflake thing

## ESP32 + SSD1305 Oled (Heltec)

**Prerequistes:**

 - RAMDISK mounted at `/Volumes/ramdisk[number]`
   ```diskutil erasedisk apfs tempramdisk$RANDOM $(hdiutil attach -nomount ram://$((1024*1024*8)))```
 - Arduino Application
 - MacOS X (macOS)
 - Heltec ESP32 (v1 and v2 are ok)


**Usage:**

Run command `make` to upload and connect `serial` device.
You can override these variables to your needs:

 - `PORT`: `/dev/cu.[your_port_name]`, replace it with your board's
 - `BAUD`: `115200`, match it with the code (for monitoring purposes)
 - `BUILD`: Temporary build location, you can use `mktemp -d`
 - `ARDUINO`: Full path to your arduino binary. Including `./Content/MacOS/Arduino`.

```
compile : compiles sources into `BUILD`.
upload  : compiles then uploads into `PORT`.
monitor : `cat`s device in given `PORT` with `BAUD`.
images  : generate header files from `.bmp` bitmap images.
ss.bmp  : pull screenshot from device.
```

---

Note: Pulling screenshot may 'freeze' screen for a second :)
