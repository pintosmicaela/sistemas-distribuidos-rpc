struct entero_param {
    int valor;
};

struct multiplicacion_param {
	int operando1;
	int operando2;
	int operando3;
	int operando4;
};

struct suma_param {
        int operando1;
        int operando2;
        int operando3;
        int operando4;
};

program MINISERVICIOS {
    version SERVICIOSVERS {
        string OBTENER_NOMBRE_MAQUINA(void) = 1;
	string OBTENER_HORA_SISTEMA(void) = 2;
	string CONVERTIR_DECIMAL_A_BINARIO(entero_param) = 3;
	int MULTIPLICAR(multiplicacion_param) = 4;
	int SUMAR(suma_param) = 5;
    } = 1;
} = 0x20000002;

