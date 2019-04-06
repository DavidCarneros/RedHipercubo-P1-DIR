# RedHipercubo-P1-DIR
Red_Hipercubo::Practica1::DIR

COMO EJECUTAR LA PRACTICA: 

Dentro del directorio Practica: 
    Para compilar: 
        make all

    Para ejecutar:

        Primeros generamos los datos. Hay dos formas para ello
            1) make generateData8         # Genera 8 datos 
            2) ./DataGenerator 8          # Genera 8 datos

        Despues ejecutamos el programa. Hay dos formas: 
            1) make hiperCubo_D3
            2) mpirun -n 8 ./RedHipercubo
