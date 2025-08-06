#include "ejercicio2.h"
#include <string.h>

const int MAX_ARRAY = 200;

arg_result *remote_call_1_svc(arg_input * arginput, struct svc_req *req){
	static arg_result result;
	int arrayA[MAX_ARRAY];
	int arrayB[MAX_ARRAY];

	for(int i = 0; i< MAX_ARRAY; i++){
		arrayA[i] = i;
		arrayB[i] = MAX_ARRAY - i;
		result.response[i] = arrayA[i]+arrayB[i];
	}
	return &result;
}
