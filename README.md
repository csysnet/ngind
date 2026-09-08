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
http://localhost:CUSTOM_PORT/index.html
```
- better look?
```bash
http://localhost:CUSTOM_PORT/fancy/index.html
```
- specific file?
```bash
http://localhost:CUSTOM_PORT/filename
```
- note: filename must contain their extension, `filename="index.html"` not `filename="index"`

custom:
- port: goto `./src/server.c`, change `#define PORT 8080` -> `#define PORT CUSTOM_PORT`
- file serving: go ./src/www/, add which file you want to serve
