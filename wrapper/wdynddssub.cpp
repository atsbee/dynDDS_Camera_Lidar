#include <stdlib.h>
#include <stdint.h>
#include "wArLidarSubscriber.h"
#include "ArLidarSubscriber.h"

struct subscriber {
    void *obj;
};


subscriber_t *subscriber_create() {
    subscriber_t *sub;
    ArLidarSubscriber *obj;

    sub      = (subscriber_t*)malloc(sizeof(*sub));
    obj    = new ArLidarSubscriber();
    sub->obj = obj;

    return sub;
}

void subscriber_destroy (subscriber_t *sub) {
    if (sub == NULL) {
      return;
    }
    delete static_cast<ArLidarSubscriber *>(sub->obj);
    free(sub);
}

int subscriber_init(subscriber_t *sub) {
    ArLidarSubscriber *obj;

    if (sub == NULL) {
      return 0;
    }
    obj = static_cast<ArLidarSubscriber *>(sub->obj);
    return obj->init();
    
}

int subscriber_run(subscriber_t *sub, std::uint16_t* cscan){
	
    ArLidarSubscriber *obj;

    if (sub == NULL) {
      return 0;
    }
    obj = static_cast<ArLidarSubscriber *>(sub->obj);
    
    return obj->run(cscan); 
}
