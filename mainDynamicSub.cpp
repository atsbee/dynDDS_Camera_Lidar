// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file mainDynamicSub.cpp
 *
 */
#include "ScanDynamicSub.h"

#include <fastrtps/log/Log.h>
#include <mosquittopp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

using eprosima::fastdds::dds::Log;


/* Callback called when the client receives a CONNACK message from the broker. */
void on_connect(struct mosquitto *mosq, void *obj, int reason_code)
{
	printf("on_connect: %s\n", mosquitto_connack_string(reason_code));
	if(reason_code != 0){
		mosquitto_disconnect(mosq);
	}
}

void on_publish(struct mosquitto *mosq, void *obj, int mid)
{
	//printf("Message with mid %d has been published.\n", mid);
	printf("onpubmqtt\n\n");
}


/* This function pretends to read some data from a sensor and publish it.*/
void publish_sensor_data(struct mosquitto *mosq, uint16_t *array, uint16_t size, uint16_t id)
{
    int rc;
	int rrc;

    //rc = mosquitto_publish(mosq, NULL, "example/temperature", sizeof(array), array, 2, false);
	rc = mosquitto_publish(mosq, NULL, "example/array", size*sizeof(uint16_t), array, 2, false);
	//rrc = mosquitto_publish(mosq, NULL, "example/id", sizeof(id), &id, 2, false);
    if(rc != MOSQ_ERR_SUCCESS){
        fprintf(stderr, "Error publishing: %s\n", mosquitto_strerror(rc));
    }
    sleep(1);
}


int main()
{
	struct mosquitto *mosq;
	int rc;
	uint16_t from_dds_array[362] = {0};
	uint16_t from_dds_size;
	uint16_t from_dds_id;

	/* Required before calling other mosquitto functions */
	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, true, NULL);
	if(mosq == NULL){
		fprintf(stderr, "Error: Out of memory.\n");
		return 1;
	}

	/* Configure callbacks. This should be done before connecting ideally. */
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_publish_callback_set(mosq, on_publish);

	rc = mosquitto_connect(mosq, "192.168.178.58", 1883, 60);
	if(rc != MOSQ_ERR_SUCCESS){
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
		return 1;
	}

	/* Run the network loop in a background thread, this call returns quickly. */
	rc = mosquitto_loop_start(mosq);
	if(rc != MOSQ_ERR_SUCCESS){
		mosquitto_destroy(mosq);
		fprintf(stderr, "Error: %s\n", mosquitto_strerror(rc));
		return 1;
	}


    //DDS
    std::cout << "Starting sub" << std::endl;
    ScanDynamicSub mysub;

	std::any var1, var2, var3;
	std::vector<std::any> anyArray(3);


    if (mysub.init())
    {
		printf("init success\n");
		mysub.run(anyArray.data(), anyArray.size(), "Scanner1Topic");
		printf("run success\n");
    }

    // //MQTT
	 while(1){
		sleep(1);
        
        if(mysub.m_listener.n_samples >= 1){
            publish_sensor_data(mosq, from_dds_array, from_dds_size, from_dds_id);
            //publish_sensor_data(mosq, from_dds_array, 200, from_dds_id);
            mysub.m_listener.n_samples = 0;
            //printf("n_samples: %d\n", mysub.m_listener.n_samples);
        }
			

		std::cout << "any1: " <<  std::any_cast<u_int16_t>(anyArray[0]) << std::endl;
				
		uint16_t* retrievedArray = std::any_cast<uint16_t*>(anyArray[1]);
		// Print the elements of the retrieved array
		std::cout << "any2: " ;
		for (size_t i = 0; i < 5; ++i) {
			std::cout << retrievedArray[i] << " ";
		}		
		std::cout << std::endl;

		std::cout << "any3: " <<  std::any_cast<u_int16_t>(anyArray[2]) << std::endl;
	 }

	mosquitto_lib_cleanup();
    Log::Reset();
	return 0;
}
