# Pinpoint all targets. 
TARGETS = omp_matmul_orig omp_matmul_collapse omp_matmul_dynamic

.PHONY : all
all : $(TARGETS)

.PHONY : default
default : all

omp_matmul_orig: 	omp_matmul_orig.c
			   	 	icc -o omp_matmul_orig omp_matmul_orig.c -qopenmp

omp_matmul_collapse: 	omp_matmul_collapse.c
			   	 	icc -o omp_matmul_collapse omp_matmul_collapse.c -qopenmp

omp_matmul_dynamic: 	omp_matmul_dynamic.c
			   	 	icc -o omp_matmul_dynamic omp_matmul_dynamic.c -qopenmp

clean:
	rm -f omp_matmul_orig
	rm -f omp_matmul_collapse
	rm -f omp_matmul_dynamic
