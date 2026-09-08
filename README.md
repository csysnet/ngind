# ngind
features:
- static file server

tech:
- epoll
- C 

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
