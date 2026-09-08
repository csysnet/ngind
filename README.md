# ngind
features:
- static file server

tech:
- epoll
- C 

run: 
- cd ./src
- gcc -Icore -Itransport -Ihttp -o server server.c core/*c transport/*.c http/*.c
- ./server
