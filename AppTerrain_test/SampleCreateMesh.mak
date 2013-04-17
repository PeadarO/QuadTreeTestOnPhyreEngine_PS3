########################################################
# PS3 PPU Makefile generated for GNU Make by ProDG VSI #
########################################################

.SILENT:

unexport MAKEFLAGS
unexport MFLAGS

OUTDIR = .
INTDIR = debugoptobj-gcm_sn

LIBS=\

WLIBS=\

AS=ppu-lv2-gcc
AS_FLAGS=\
	-DSN_TARGET_PS3\
	-D_REENTRANT\
	-D_SPU\
	-DNDEBUG\
	-D_DEBUGOPT\
	-D__CELLOS_LV2__\
	-IC:/PhyreEngine/Include\
	-IC:/PhyreEngine/External\
	-IC:/PhyreEngine/utilities\
	-iquote.\
	-c\

CC=ppu-lv2-gcc
CC_FLAGS=\
	-DSN_TARGET_PS3\
	-D_REENTRANT\
	-D_SPU\
	-DNDEBUG\
	-D_DEBUGOPT\
	-D__CELLOS_LV2__\
	-IC:/PhyreEngine/Include\
	-IC:/PhyreEngine/External\
	-IC:/PhyreEngine/utilities\
	-iquote.\
	-c\
	-fpch-deps\
	-MMD -MV\
	@"C:\PhyreEngine/Build/GCC/CompileDebugOpt"\

CXX=ppu-lv2-gcc
CXX_FLAGS=\
	-DSN_TARGET_PS3\
	-D_REENTRANT\
	-D_SPU\
	-DNDEBUG\
	-D_DEBUGOPT\
	-D__CELLOS_LV2__\
	-IC:/PhyreEngine/Include\
	-IC:/PhyreEngine/External\
	-IC:/PhyreEngine/utilities\
	-iquote.\
	-c\
	-fpch-deps\
	-MMD -MV\
	@"C:\PhyreEngine/Build/GCC/CompileDebugOpt"\

LD=ppu-lv2-gcc
LD_FLAGS=\
	-LC:/PhyreEngine/Lib\
	@"C:\PhyreEngine/Build/GCC/LinkSampleDebugOptGCM"\

OBJS=\
	"debugoptobj-gcm_sn/Common.o"\
	"debugoptobj-gcm_sn/DebugingTexture.o"\
	"debugoptobj-gcm_sn/DefenseApplication.o"\
	"debugoptobj-gcm_sn/Objects.o"\
	"debugoptobj-gcm_sn/OutputText.o"\
	"debugoptobj-gcm_sn/TerrainMesh.o"\
	"debugoptobj-gcm_sn/TestCreateMesh.o"\

CST_OBJS=\

CST_OUTS=\

CUSTOM: $(CST_OUTS) $(CST_OBJS)

COMPILE: $(OBJS)

"debugoptobj-gcm_sn/Common.o":
	@echo ProDG Compiling "c:/PhyreEngine/Samples/Test/TestCreateMesh/Common.cpp"
	$(CXX) $(CXX_FLAGS) "c:/PhyreEngine/Samples/Test/TestCreateMesh/Common.cpp" -o $@

"debugoptobj-gcm_sn/DebugingTexture.o":
	@echo ProDG Compiling "c:/PhyreEngine/Samples/Test/TestCreateMesh/DebugingTexture.cpp"
	$(CXX) $(CXX_FLAGS) "c:/PhyreEngine/Samples/Test/TestCreateMesh/DebugingTexture.cpp" -o $@

"debugoptobj-gcm_sn/DefenseApplication.o":
	@echo ProDG Compiling "c:/PhyreEngine/Samples/Test/TestCreateMesh/DefenseApplication.cpp"
	$(CXX) $(CXX_FLAGS) "c:/PhyreEngine/Samples/Test/TestCreateMesh/DefenseApplication.cpp" -o $@

"debugoptobj-gcm_sn/Objects.o":
	@echo ProDG Compiling "c:/PhyreEngine/Samples/Test/TestCreateMesh/Objects.cpp"
	$(CXX) $(CXX_FLAGS) "c:/PhyreEngine/Samples/Test/TestCreateMesh/Objects.cpp" -o $@

"debugoptobj-gcm_sn/OutputText.o":
	@echo ProDG Compiling "c:/PhyreEngine/Samples/Test/TestCreateMesh/OutputText.cpp"
	$(CXX) $(CXX_FLAGS) "c:/PhyreEngine/Samples/Test/TestCreateMesh/OutputText.cpp" -o $@

"debugoptobj-gcm_sn/TerrainMesh.o":
	@echo ProDG Compiling "c:/PhyreEngine/Samples/Test/TestCreateMesh/TerrainMesh.cpp"
	$(CXX) $(CXX_FLAGS) "c:/PhyreEngine/Samples/Test/TestCreateMesh/TerrainMesh.cpp" -o $@

"debugoptobj-gcm_sn/TestCreateMesh.o":
	@echo ProDG Compiling "c:/PhyreEngine/Samples/Test/TestCreateMesh/TestCreateMesh.cpp"
	$(CXX) $(CXX_FLAGS) "c:/PhyreEngine/Samples/Test/TestCreateMesh/TestCreateMesh.cpp" -o $@

LINK: "./TestCreateMeshGCM.sdoelf"

"./TestCreateMeshGCM.sdoelf" : $(CST_OBJS) $(OBJS)
	@echo ProDG Linking $@
	$(LD) $(LD_FLAGS) -Wl,--start-group $(CST_OBJS) $(OBJS) $(LIBS) -Wl,--end-group -o $@

