#ifndef __WARLIDARSUBSCRIBER_H__
#define __WARLIDARSUBSCRIBER_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
struct subscriber;
typedef struct subscriber subscriber_t;

subscriber_t *subscriber_create();
void subscriber_destroy(subscriber_t *sub);

int subscriber_init(subscriber_t *sub);
int subscriber_run(subscriber_t *sub, uint16_t *cscan);

#ifdef __cplusplus
}
#endif

#endif /* __WARLIDARSUBSCRIBER_H__ */
