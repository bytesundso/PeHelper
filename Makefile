#binary name
BINNAME=SneakCalls.lib

#internal dirs
SRCDIR=src
INCDIR=include
DESTDIR=build

#flags
AFLAGS=
CFLAGS=
CPPFLAGS=/std:c++20 /Gd /GR- /O1 /sdl- /GS-

#external dirs
ADDINCDIRS=
LIBDIRS=

#link
LINK=lib /NODEFAULTLIB /SUBSYSTEM:WINDOWS

!IF !DEFINED(VSCMD_ARG_TGT_ARCH) || !DEFINED(AS) || !DEFINED(CC) || !DEFINED(CPP)
!ERROR "Not all macros are defined! (Did you use VS Development Powershell/Command Prompt?)"
!ENDIF

all: $(DESTDIR) $(DESTDIR)\$(BINNAME)

$(DESTDIR)\$(BINNAME): $(SRCDIR)\*.$(VSCMD_ARG_TGT_ARCH).asm $(SRCDIR)\*.c $(SRCDIR)\*.cpp
	@$(MAKE) $(patsubst $(DESTDIR)\*%, , $(patsubst $(SRCDIR)\\%, $(DESTDIR)\\%, $(patsubst %.cpp, %.obj, $(patsubst %.c, %.obj, $(patsubst %.asm, %.obj, $(?))))))
	$(LINK) $(patsubst %, -libpath:%, $(LIBDIR)) -out:$(DESTDIR)\$(BINNAME) $(patsubst $(DESTDIR)\*%, , $(patsubst $(SRCDIR)\\%, $(DESTDIR)\\%, $(patsubst %.cpp, %.obj, $(patsubst %.c, %.obj, $(patsubst %.asm, %.obj, $(?))))))

$(SRCDIR)\*.$(VSCMD_ARG_TGT_ARCH).asm:
{$(SRCDIR)}.asm{$(DESTDIR)}.obj:
	$(AS) $(AFLAGS) -Fo $(@) -c $(<)

$(SRCDIR)\*.c:
{$(SRCDIR)}.c{$(DESTDIR)}.obj:
	$(CC) $(CFLAGS) -Fo:$(@) -I $(INCDIR) $(patsubst %, -I %, $(ADDINCDIRS)) -c $(<)

$(SRCDIR)\*.cpp:
{$(SRCDIR)}.cpp{$(DESTDIR)}.obj:
	$(CPP) $(CPPFLAGS) -Fo:$(@) -I $(INCDIR) $(patsubst %, -I %, $(ADDINCDIRS)) -c $(<)

$(DESTDIR):
	@mkdir $(DESTDIR)

clean:
	@rmdir /s /q $(DESTDIR)