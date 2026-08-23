#include "ngd_transport.h"
//
static ngd_timer_t *head;
//
void
ngd_timer_module_init(void)
{
    head = NULL;
}
//
int
ngd_timer_regis(ngd_conn_t *c, uint64_t timeout_ms)
{
    ngd_timer_t *timer;
    ngd_timer_t *cur;
    //
    timer = &c->timer;
    timer->expire = ngd_timer_get_now() + timeout_ms;
    timer->conn = c;
    timer->active = true;
    //
    if (head) {
        timer->next = NULL;
        head = timer;
    } else {
        if (timer->expire < head->expire) {
            timer->next = head;
            head = timer;
        } else {
            cur = head;
            next = head->next;
            while (cur && cur->next)
            {
                if (timer->expire < cur->expire)

                head = head->next;
            }

            if NEW smaller than NEXT, insert to prev next,
        }
    }
    //
}
//
int ngd_timer_reset(ngd_conn_t *c, uint64_t timeout_ms);
void ngd_timer_expire(uint64_t now);
int ngd_timer_next_timeout(void);
uint64_t ngd_timer_get_now(void);
