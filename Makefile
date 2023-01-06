NAME_BIN=PeHelper.lib
NAME_SUBS=

DIR_INC=include
DIR_SRC=src
DIR_BUILD=build
DIR_SUBS=subs

FLAGS_ASM=
FLAGS_C=
FLAGS_CPP=/std:c++20 /Gd /GR- /O1 /sdl- /GS-

LINK=lib /NODEFAULTLIB /SUBSYSTEM:WINDOWS

!IF !DEFINED(VSCMD_ARG_TGT_ARCH) || !DEFINED(AS) || !DEFINED(CC) || !DEFINED(CPP)
!ERROR "Not all macros are defined! (Did you use VS Development Powershell/Command Prompt?)"
!ENDIF

all: $(DIR_INC) $(DIR_SRC) $(DIR_BUILD) $(DIR_BUILD)\\$(VSCMD_ARG_TGT_ARCH) $(DIR_SUBS) $(DIR_BUILD)\\$(VSCMD_ARG_TGT_ARCH)\\$(NAME_BIN)

$(DIR_BUILD)\\$(VSCMD_ARG_TGT_ARCH)\\$(NAME_BIN): $(DIR_SRC)\\*.asm $(DIR_SRC)\\*.c $(DIR_SRC)\\*.cpp
	@$(MAKE) $(patsubst $(DIR_SRC)\\%, $(DIR_BUILD)\\$(VSCMD_ARG_TGT_ARCH)\\%, $(subst $(DIR_SRC)\\*.obj,, $(patsubst %.cpp, %.obj, $(patsubst %.c, %.obj, $(patsubst %.asm, %.obj, $(**))))))
	@$(LINK) $(patsubst %, -libpath:$(DIR_SUBS)\\%\\$(DIR_BUILD)\\$(VSCMD_ARG_TGT_ARCH), $(NAME_SUBS)) -out:$(DIR_BUILD)\\$(VSCMD_ARG_TGT_ARCH)\\$(NAME_BIN) $(patsubst $(DIR_SRC)\\%, $(DIR_BUILD)\\$(VSCMD_ARG_TGT_ARCH)\\%, $(subst $(DIR_SRC)\\*.obj,, $(patsubst %.cpp, %.obj, $(patsubst %.c, %.obj, $(patsubst %.asm, %.obj, $(**))))))

$(DIR_SRC)\\*.asm:
{$(DIR_SRC)}.asm{$(DIR_BUILD)\\$(VSCMD_ARG_TGT_ARCH)}.obj:
	@$(AS) $(FLAGS_ASM) -Fo $(@) -c $(<)

$(DIR_SRC)\\*.c:
{$(DIR_SRC)}.c{$(DIR_BUILD)\\$(VSCMD_ARG_TGT_ARCH)}.obj:
	@$(CC) $(FLAGS_C) -Fo:$(@) -I $(DIR_INC) $(patsubst %, -I $(DIR_SUBS)\%\$(DIR_INC), $(NAME_SUBS)) -c $(<)

$(DIR_SRC)\\*.cpp:
{$(DIR_SRC)}.cpp{$(DIR_BUILD)\\$(VSCMD_ARG_TGT_ARCH)}.obj:
	@$(CPP) $(FLAGS_CPP) -Fo:$(@) -I $(DIR_INC) $(patsubst %, -I $(DIR_SUBS)\%\$(DIR_INC), $(NAME_SUBS)) -c $(<)


$(DIR_INC) $(DIR_SRC) $(DIR_BUILD) $(DIR_BUILD)\\$(VSCMD_ARG_TGT_ARCH) $(DIR_SUBS):
	@mkdir $(@)

clean:
	@rmdir /s /q $(DIR_BUILD)