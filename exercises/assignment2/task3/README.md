# How to Run


**Prerequisites (for VSCode):** 
- Install https://www.mysys2.org/
- Install C/C++ Extension Pack
- Add MSYS2 terminal to VSCode
- Instal gcc compiler using `pacman -S mingw-w64-ucrt-x86_64-gcc`
- Check if installed using `gcc --version`


```bash
cd task3/

gcc -Wall -O2 server.c -o server

gcc -Wall -O2 client.c -o client

./server # In one terminal
./client # In another terminal

```