
run := mpirun
mcc  := mpicc
cc := gcc


all: 
	$(cc) DataGenerator.c -o DataGenerator
	$(mcc) RedHipercubo.c -o RedHipercubo

generateData8:
	./DataGenerator 8

hiperCubo_D3:
	$(run) -n 8 ./RedHipercubo