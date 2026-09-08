#include <time.h>
#include <limits.h>

#include "ngd_timer.h"

static ngd_timer_t *head;


void
ngd_timer_module_init(void)
{
    head = NULL;
}


uint64_t
ngd_timer_module_get_now(void)
{
    //ms
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint64_t) ts.tv_sec * 1000
         + (uint64_t) ts.tv_nsec / 1000000;
}


int
ngd_timer_regis(ngd_timer_t *tmr,
                void (*handler)(ngd_timer_t *),
                void *data,
                uint64_t timeout_ms)
{
    ngd_timer_t *cur;

    if (tmr == NULL || handler == NULL) {
        return NGD_ERR;
    }

    tmr->expire = ngd_timer_module_get_now() + timeout_ms;
    tmr->handler = handler;
    tmr->data = data;

    tmr->prev = NULL;
    tmr->next = NULL;

    //
    if (head == NULL) {
        head = tmr;
        return NGD_OK;
    }
    //insert bef head
    if (tmr->expire < head->expire) {
        tmr->next = head;
        head->prev = tmr;
        head = tmr;

        return NGD_OK;
    }

    cur = head;

    while (cur->next != NULL &&
           cur->next->expire <= tmr->expire)
    {
        cur = cur->next;
    }

    tmr->next = cur->next;
    tmr->prev = cur;

    if (cur->next != NULL) {
        cur->next->prev = tmr;
    }

    cur->next = tmr;

    return NGD_OK;
}


int
ngd_timer_unregis(ngd_timer_t *tmr)
{
    if (tmr == NULL) {
        return NGD_ERR;
    }
    if (tmr == head) {
        head = tmr->next;

        if (head != NULL) {
            head->prev = NULL;
        }

        tmr->prev = NULL;
        tmr->next = NULL;

        return NGD_OK;
    }
    if (tmr->prev == NULL && tmr->next == NULL) {
        return NGD_OK;
    }
    //
    tmr->prev->next = tmr->next;

    if (tmr->next != NULL) {
        tmr->next->prev = tmr->prev;
    }
    //
    tmr->prev = NULL;
    tmr->next = NULL;
    //
    return NGD_OK;
}


int
ngd_timer_reset(ngd_timer_t *tmr, uint64_t timeout_ms)
{
    void (*handler)(ngd_timer_t *);
    void *data;
    //
    if (tmr == NULL) {
        return NGD_ERR;
    }
    //
    handler = tmr->handler;
    data = tmr->data;
    //
    ngd_timer_unregis(tmr);
    //
    return ngd_timer_regis(tmr,
                           handler,
                           data,
                           timeout_ms);
}


int
ngd_timer_module_get_next_timeout(void)
{
    uint64_t now;
    uint64_t timeout;

    if (head == NULL) {
        return -1;
    }

    now = ngd_timer_module_get_now();

    if (head->expire <= now) {
        return 0;
    }

    timeout = head->expire - now;

    if (timeout > INT_MAX) {
        return INT_MAX;
    }

    return (int) timeout;
}


void
ngd_timer_module_expire(uint64_t now)
{
    ngd_timer_t *tmr;

    while (head != NULL && head->expire <= now) {

        tmr = head;
        head = tmr->next;

        if (head != NULL) {
            head->prev = NULL;
        }

        tmr->prev = NULL;
        tmr->next = NULL;
        tmr->handler(tmr);
    }
}
