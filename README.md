# ngind
features:
- static file server

tech:
- Epoll
- C 
- Linux

run: 
```bash
cd ./src
```
```bash
gcc -Icore -Itransport -Ihttp -o server server.c core/*c transport/*.c http/*.c
```
```bash
./server
```

use:
- open browser, type
```bash
http://localhost:8080/index.html
```
- better look?
```bash
http://localhost:8080/fancy/index.html
```
- specific file/port?
```bash
http://localhost:CUSTOM_PORT/FILENAME
```
- note: filename must contain their extension, `FILENAME="index.html"` not `FILENAME="index"`

custom:
- port: goto `./src/server.c`, change `#define PORT 8080` -> `#define PORT CUSTOM_PORT`
- file serving: go ./src/www/, add which file you want to serve
