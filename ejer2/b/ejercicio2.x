const INPUT_SIZE = 4096;
const OUTPUT_SIZE = 2048;

struct arg_input {
	opaque data[INPUT_SIZE];
};

struct arg_result {
	opaque response[OUTPUT_SIZE];
};

program EJERCICIO2PROG {
	version EJERCICIO2VERS {
		arg_result remote_call(arg_input) = 1;
	} = 1;
} = 0x20000005;
