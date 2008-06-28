# Microsoft Developer Studio Generated NMAKE File, Based on Vrml2OGL.dsp
!IF "$(CFG)" == ""
CFG=Vrml2OGL - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Vrml2OGL - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Vrml2OGL - Win32 Release" && "$(CFG)" !=\
 "Vrml2OGL - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Vrml2OGL.mak" CFG="Vrml2OGL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Vrml2OGL - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "Vrml2OGL - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Vrml2OGL - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Vrml2OGL.exe"

!ELSE 

ALL : "$(OUTDIR)\Vrml2OGL.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\FindNodes.obj"
	-@erase "$(INTDIR)\GeometryUtils.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\Vrml2OGL.obj"
	-@erase "$(INTDIR)\Vrml2OGLUtils.obj"
	-@erase "$(OUTDIR)\Vrml2OGL.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D\
 "_MBCS" /Fp"$(INTDIR)\Vrml2OGL.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Vrml2OGL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:no /pdb:"$(OUTDIR)\Vrml2OGL.pdb" /machine:I386\
 /out:"$(OUTDIR)\Vrml2OGL.exe" 
LINK32_OBJS= \
	"$(INTDIR)\FindNodes.obj" \
	"$(INTDIR)\GeometryUtils.obj" \
	"$(INTDIR)\Vrml2OGL.obj" \
	"$(INTDIR)\Vrml2OGLUtils.obj"

"$(OUTDIR)\Vrml2OGL.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Vrml2OGL - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Vrml2OGL.exe"

!ELSE 

ALL : "$(OUTDIR)\Vrml2OGL.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\FindNodes.obj"
	-@erase "$(INTDIR)\GeometryUtils.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\Vrml2OGL.obj"
	-@erase "$(INTDIR)\Vrml2OGLUtils.obj"
	-@erase "$(OUTDIR)\Vrml2OGL.exe"
	-@erase "$(OUTDIR)\Vrml2OGL.ilk"
	-@erase "$(OUTDIR)\Vrml2OGL.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /D "_MBCS" /Fp"$(INTDIR)\Vrml2OGL.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\"\
 /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Vrml2OGL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\Vrml2OGL.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)\Vrml2OGL.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\FindNodes.obj" \
	"$(INTDIR)\GeometryUtils.obj" \
	"$(INTDIR)\Vrml2OGL.obj" \
	"$(INTDIR)\Vrml2OGLUtils.obj"

"$(OUTDIR)\Vrml2OGL.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Vrml2OGL - Win32 Release" || "$(CFG)" ==\
 "Vrml2OGL - Win32 Debug"
SOURCE=.\FindNodes.cpp

!IF  "$(CFG)" == "Vrml2OGL - Win32 Release"

DEP_CPP_FINDN=\
	".\FindNodes.h"\
	".\GeometryUtils.h"\
	".\SharedDefs.h"\
	".\Vrml2OGL.h"\
	".\Vrml2OGLUtils.h"\
	

"$(INTDIR)\FindNodes.obj" : $(SOURCE) $(DEP_CPP_FINDN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Vrml2OGL - Win32 Debug"

DEP_CPP_FINDN=\
	".\FindNodes.h"\
	".\GeometryUtils.h"\
	".\SharedDefs.h"\
	".\Vrml2OGL.h"\
	".\Vrml2OGLUtils.h"\
	

"$(INTDIR)\FindNodes.obj" : $(SOURCE) $(DEP_CPP_FINDN) "$(INTDIR)"


!ENDIF 

SOURCE=.\GeometryUtils.cpp

!IF  "$(CFG)" == "Vrml2OGL - Win32 Release"

DEP_CPP_GEOME=\
	".\GeometryUtils.h"\
	".\SharedDefs.h"\
	".\Vrml2OGL.h"\
	

"$(INTDIR)\GeometryUtils.obj" : $(SOURCE) $(DEP_CPP_GEOME) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Vrml2OGL - Win32 Debug"

DEP_CPP_GEOME=\
	".\GeometryUtils.h"\
	".\SharedDefs.h"\
	".\Vrml2OGL.h"\
	

"$(INTDIR)\GeometryUtils.obj" : $(SOURCE) $(DEP_CPP_GEOME) "$(INTDIR)"


!ENDIF 

SOURCE=.\Vrml2OGL.cpp

!IF  "$(CFG)" == "Vrml2OGL - Win32 Release"

DEP_CPP_VRML2=\
	".\FindNodes.h"\
	".\GeometryUtils.h"\
	".\SharedDefs.h"\
	".\Vrml2OGL.h"\
	".\Vrml2OGLUtils.h"\
	

"$(INTDIR)\Vrml2OGL.obj" : $(SOURCE) $(DEP_CPP_VRML2) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Vrml2OGL - Win32 Debug"

DEP_CPP_VRML2=\
	".\FindNodes.h"\
	".\GeometryUtils.h"\
	".\SharedDefs.h"\
	".\Vrml2OGL.h"\
	".\Vrml2OGLUtils.h"\
	

"$(INTDIR)\Vrml2OGL.obj" : $(SOURCE) $(DEP_CPP_VRML2) "$(INTDIR)"


!ENDIF 

SOURCE=.\Vrml2OGLUtils.cpp

!IF  "$(CFG)" == "Vrml2OGL - Win32 Release"

DEP_CPP_VRML2O=\
	".\FindNodes.h"\
	".\GeometryUtils.h"\
	".\SharedDefs.h"\
	".\Vrml2OGL.h"\
	".\Vrml2OGLUtils.h"\
	

"$(INTDIR)\Vrml2OGLUtils.obj" : $(SOURCE) $(DEP_CPP_VRML2O) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Vrml2OGL - Win32 Debug"

DEP_CPP_VRML2O=\
	".\FindNodes.h"\
	".\GeometryUtils.h"\
	".\SharedDefs.h"\
	".\Vrml2OGL.h"\
	".\Vrml2OGLUtils.h"\
	

"$(INTDIR)\Vrml2OGLUtils.obj" : $(SOURCE) $(DEP_CPP_VRML2O) "$(INTDIR)"


!ENDIF 


!ENDIF 

