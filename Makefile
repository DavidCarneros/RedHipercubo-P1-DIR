
run := mpirun
mcc  := mpicc
cc := gcc



generateData8:
	$(cc) DataGenerator.c -o DataGenerator
	./DataGenerator 8

hiperCubo_D3:
	$(mcc) RedHipercubo.c -o RedHipercubo
	$(run) -n 8 ./RedHipercubo