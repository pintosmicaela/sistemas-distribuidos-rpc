struct nombre_tarea{
    string archivo<256>;
    int numero;
};

program EJECUTARTAREAPROG {
    version EJECUTARTAREAVERS {
        int EJECUTARTAREA(nombre_tarea) = 1;
    } = 1;
} = 0x20000002;
