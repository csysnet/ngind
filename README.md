# chat

wisely is to and what to not

parser
- ensure upper only see request as struct req
- cur stay expect
header exceed, merge extra
just method
gcc -o serv serv.c core/*.c http/*.c && ./serv


hey!! your WORLD


either core.h or forward declaration, leave include on .c
bug cret satisfy, right!!

cap duoi muon tao cai gi, phai qua cap tren
lower owning upper
either init or (create, destroy/close)
pdata for things without size
event write ative
void for nothing
rule forwarddeclare typedef struct listen_t listen_t;


rule of writing other include components that we use, everytime we use that, check the top
allow other lib at header
create if it own itself

minimize header as much as posible
cpchange header api only


b = pool_alloc(c->pool, sizeof(buf_t));
    b->start = pool_alloc(c->pool, MAX_INBUF);
    b->pos = b->start;
    b->last = b->start;
    b->end = b->start + MAX_INBUF;
