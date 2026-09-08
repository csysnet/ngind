# ngind
features:

- static file server

compile: 
- cd ./src
- gcc -Icore -Itransport -Ihttp -o server server.c core/*c transport/*.c http/*.c
run:
- ./server
