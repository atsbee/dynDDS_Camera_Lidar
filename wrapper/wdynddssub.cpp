#include <stdlib.h>
#include <stdint.h>
#include "wdynddssub.h"
#include "../dynddssub/ScanDynamicSub.h"

struct subscriber {
    void *obj;
};


subscriber_t *subscriber_create() {
    subscriber_t *sub;
    ScanDynamicSub *obj;

    sub      = (subscriber_t*)malloc(sizeof(*sub));
    obj    = new ScanDynamicSub();
    sub->obj = obj;

    return sub;
}

void subscriber_destroy (subscriber_t *sub) {
    if (sub == NULL) {
      return;
    }
    delete static_cast<ScanDynamicSub *>(sub->obj);
    free(sub);
}

int subscriber_init(subscriber_t *sub) {
    ScanDynamicSub *obj;
    std::string xml_path = "../xmls/lidar.xml";
    std::string dyntype_name = "LidarData";
    std::string topic_name = "LidarTopic";
    if (sub == NULL) {
      return 0;
    }
    obj = static_cast<ScanDynamicSub *>(sub->obj);
        // initialize the subscriber
    obj->init(xml_path, dyntype_name, topic_name);

    return 1;
    
}

int subscriber_run(subscriber_t *sub, std::uint16_t* cscan){
	
    ScanDynamicSub *obj;
    std::vector<std::any> anyArray(3);

    if (sub == NULL) {
      return 0;
    }
    obj = static_cast<ScanDynamicSub *>(sub->obj);

    obj->run(anyArray.data(), anyArray.size());

    if (anyArray[1].type() == typeid(uint16_t*)){
      uint16_t* buffer_cscan = std::any_cast<std::uint16_t*>(anyArray[1]);
      for (int i = 0; i < 362; i++){
          cscan[i] = buffer_cscan[i];
      }
    } 
    else{
        //printf("wrong type1\n");
    }
    
    return 1; 
}
