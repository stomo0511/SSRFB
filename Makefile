#
UNAME = $(shell uname)
ifeq ($(UNAME),Linux)
  BLAS_ROOT = /opt/intel/compilers_and_libraries/linux/mkl
  BLAS_INC_DIR = $(BLAS_ROOT)/include
  BLAS_LIB_DIR = $(BLAS_ROOT)/lib/intel64
  SBLAS_LIBS = -Wl,--start-group $(BLAS_LIB_DIR)/libmkl_intel_lp64.a $(BLAS_LIB_DIR)/libmkl_sequential.a $(BLAS_LIB_DIR)/libmkl_core.a -Wl,--end-group -lpthread -ldl -lm
  TMATRIX_ROOT = /home/stomo/WorkSpace/TileMatrix
  COREBLAS_ROOT = /home/stomo/WorkSpace/CoreBlas
  CXX =	g++
endif
ifeq ($(UNAME),Darwin)
  BLAS_ROOT = /opt/intel/compilers_and_libraries/mac/mkl
  BLAS_INC_DIR = $(BLAS_ROOT)/include
  BLAS_LIB_DIR = $(BLAS_ROOT)/lib
  SBLAS_LIBS = $(BLAS_LIB_DIR)/libmkl_intel_lp64.a $(BLAS_LIB_DIR)/libmkl_sequential.a $(BLAS_LIB_DIR)/libmkl_core.a -lpthread -ldl -lm
  TMATRIX_ROOT = /Users/stomo/WorkSpace/TileAlgorithm/TileMatrix
  COREBLAS_ROOT = /Users/stomo/WorkSpace/TileAlgorithm/CoreBlas
  CXX =	/usr/local/bin/g++ 
endif

#
PLASMA_ROOT = /opt/plasma-17.1
PLASMA_INC_DIR = $(PLASMA_ROOT)/include
PLASMA_LIB_DIR = $(PLASMA_ROOT)/lib
PLASMA_LIBS = -lcoreblas -lplasma
#
TMATRIX_INC_DIR = $(TMATRIX_ROOT)
TMATRIX_LIB_DIR = $(TMATRIX_ROOT)
TMATRIX_LIBS = -lTileMatrix
#
COREBLAS_INC_DIR = $(COREBLAS_ROOT)
COREBLAS_LIB_DIR = $(COREBLAS_ROOT)
COREBLAS_LIBS = -lCoreBlasTile
#
CXXFLAGS =	-fopenmp -m64 -O2 -I$(BLAS_INC_DIR) -I$(PLASMA_INC_DIR) -I$(TMATRIX_INC_DIR) -I$(COREBLAS_INC_DIR)
#

GM_OBJS = SuperM.o Matrix.o Tile.o GEMM.o
SS0_OBJS = SSRFB.o
GE0_OBJS = GEQRT.o
TS0_OBJS = TSQRT.o
LA0_OBJS = LARFB.o

all: SS0 GE0 TS0 LA0

GE0: $(GE0_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(GE0_OBJS) \
	-L$(TMATRIX_LIB_DIR) $(TMATRIX_LIBS) \
	-L$(COREBLAS_LIB_DIR) $(COREBLAS_LIBS) \
	-L$(PLASMA_LIB_DIR) $(PLASMA_LIBS) -L$(BLAS_LIB_DIR) $(SBLAS_LIBS)

TS0: $(TS0_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(TS0_OBJS) \
	-L$(TMATRIX_LIB_DIR) $(TMATRIX_LIBS) \
	-L$(COREBLAS_LIB_DIR) $(COREBLAS_LIBS) \
	-L$(PLASMA_LIB_DIR) $(PLASMA_LIBS) -L$(BLAS_LIB_DIR) $(SBLAS_LIBS)

LA0: $(LA0_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(LA0_OBJS) \
	-L$(TMATRIX_LIB_DIR) $(TMATRIX_LIBS) \
	-L$(COREBLAS_LIB_DIR) $(COREBLAS_LIBS) \
	-L$(PLASMA_LIB_DIR) $(PLASMA_LIBS) -L$(BLAS_LIB_DIR) $(SBLAS_LIBS)


SS0 : $(SS0_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(SS0_OBJS) \
	-L$(TMATRIX_LIB_DIR) $(TMATRIX_LIBS) \
	-L$(COREBLAS_LIB_DIR) $(COREBLAS_LIBS) \
	-L$(PLASMA_LIB_DIR) $(PLASMA_LIBS) -L$(BLAS_LIB_DIR) $(SBLAS_LIBS)

GM : $(GM_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(GM_OBJS) -L$(PLASMA_LIB_DIR) $(PLASMA_LIBS) $(LIBBLAS) -L$(BLAS_LIB_DIR) $(SBLAS_LIBS)

.cpp.o :
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f Makefile~ *.cpp~ *.h~ *.o GM SS0 SS1 GE1 TS1 LA1 TT1 TM1
