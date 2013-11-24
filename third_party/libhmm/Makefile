.SUFFIXES: .cpp

#CFLAGS = -Wall -O3 -mpcu=itanium2-9000 -parallel \
#	-I./newmat10
CFLAGS = -Wall -O3

#GLLIBS=-L${GLUT_LIB_LOCATION} -lglut -lGL -lGLU
#LPATH=${GLUTPATH} 
#INCS= 
CXX=g++
#CXX=icpc

#libs = -L./newmat10 -lm -lnewmat
libs = -lm -lnewmat

.cpp.o:
	$(CXX) $(CFLAGS) -c $*.cpp

.c.o:
	$(CXX) $(CFLAGS) -c $*.c

a.out: sample_main.o HMM.h HMM.o State.h State.o k_mean.h k_mean.o mem.h mem.o  
	$(CXX) $(CFLAGS) main.o HMM.o State.o k_mean.o mem.o -o a.out ${libs}

clean: 
	rm -f *.o