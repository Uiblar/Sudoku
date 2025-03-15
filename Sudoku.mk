##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=Sudoku
ConfigurationName      :=Release
WorkspaceConfiguration :=Release
WorkspacePath          :=C:/temp/repos/c/ws2
ProjectPath            :=C:/temp/repos/c/ws2/Sudoku
IntermediateDirectory  :=../build-$(WorkspaceConfiguration)/Sudoku
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=jonas
Date                   :=15/03/2025
CodeLitePath           :="C:/Program Files/CodeLite"
MakeDirCommand         :=mkdir -p
LinkerName             :=C:/cygwin64/bin/gcc.exe
SharedObjectLinkerName :=C:/cygwin64/bin/gcc.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputDirectory        :=C:/temp/repos/c/ws2/build-$(WorkspaceConfiguration)/bin
OutputFile             :=..\build-$(WorkspaceConfiguration)\bin\$(ProjectName).exe
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := 
RcCompilerName         :=windres
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := C:/cygwin64/bin/ar.exe rcu
CXX      := C:/cygwin64/bin/x86_64-pc-cygwin-g++.exe
CC       := C:/cygwin64/bin/gcc.exe
CXXFLAGS :=  -O2 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/cygwin64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/sudoku_hint.c$(ObjectSuffix) $(IntermediateDirectory)/main.c$(ObjectSuffix) $(IntermediateDirectory)/sudoku_solver.c$(ObjectSuffix) $(IntermediateDirectory)/sudoku_cli.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) "$(IntermediateDirectory)"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "$(IntermediateDirectory)"
	@$(MakeDirCommand) "$(OutputDirectory)"

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "$(IntermediateDirectory)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/sudoku_hint.c$(ObjectSuffix): sudoku_hint.c $(IntermediateDirectory)/sudoku_hint.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/temp/repos/c/ws2/Sudoku/sudoku_hint.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sudoku_hint.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sudoku_hint.c$(DependSuffix): sudoku_hint.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sudoku_hint.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sudoku_hint.c$(DependSuffix) -MM sudoku_hint.c

$(IntermediateDirectory)/sudoku_hint.c$(PreprocessSuffix): sudoku_hint.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sudoku_hint.c$(PreprocessSuffix) sudoku_hint.c

$(IntermediateDirectory)/main.c$(ObjectSuffix): main.c $(IntermediateDirectory)/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/temp/repos/c/ws2/Sudoku/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/main.c$(DependSuffix) -MM main.c

$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix) main.c

$(IntermediateDirectory)/sudoku_solver.c$(ObjectSuffix): sudoku_solver.c $(IntermediateDirectory)/sudoku_solver.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/temp/repos/c/ws2/Sudoku/sudoku_solver.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sudoku_solver.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sudoku_solver.c$(DependSuffix): sudoku_solver.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sudoku_solver.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sudoku_solver.c$(DependSuffix) -MM sudoku_solver.c

$(IntermediateDirectory)/sudoku_solver.c$(PreprocessSuffix): sudoku_solver.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sudoku_solver.c$(PreprocessSuffix) sudoku_solver.c

$(IntermediateDirectory)/sudoku_cli.c$(ObjectSuffix): sudoku_cli.c $(IntermediateDirectory)/sudoku_cli.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/temp/repos/c/ws2/Sudoku/sudoku_cli.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/sudoku_cli.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/sudoku_cli.c$(DependSuffix): sudoku_cli.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/sudoku_cli.c$(ObjectSuffix) -MF$(IntermediateDirectory)/sudoku_cli.c$(DependSuffix) -MM sudoku_cli.c

$(IntermediateDirectory)/sudoku_cli.c$(PreprocessSuffix): sudoku_cli.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/sudoku_cli.c$(PreprocessSuffix) sudoku_cli.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


