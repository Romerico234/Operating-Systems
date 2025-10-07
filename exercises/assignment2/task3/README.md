# How to Run

## Installation (for VSCode + MSYS2/MinGW)
- Follow this link to set up MinGW for VSCode:  
  https://code.visualstudio.com/docs/cpp/config-mingw

- Make sure you install the **MinGW-w64 UCRT64 toolchain** via MSYS2:
  ```bash
  pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
  ```

- IMPORTANT: Always use the MSYS2 UCRT64 (or MINGW64) shell, not the plain MSYS shell.
    - If you’re in the wrong shell, you’ll see bash: gcc: command not found
    - Open MSYS2 UCRT64 from the Start Menu
    - Check with: `echo $MSYSTEM`

```bash
cd task3/

gcc -Wall -O2 server.c -o server.exe -lws2_32

gcc -Wall -O2 client.c -o client.exe -lws2_32

./server.exe # In one terminal
./client.exe # In another terminal

```