gcc -Icore -Itransport -Ihttp -o serv serv.c core/*c transport/*.c http/*.c && ./serv
gcc -g -O0 -Icore -Itransport -Ihttp -o serv serv.c core/*c transport/*.c http/*.c && gdb ./ser
