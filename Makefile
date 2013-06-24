CC = gcc
CXX = g++
LD = g++
CFLAGS = -Isrc -W -Wall -ggdb -std=c99

ifndef NAME
  NAME = tetris
endif

ifdef STATICLINK
  LIBS =
else
  LIBS =
endif

ifndef WINDOWS
ifdef MINGDIR
  WINDOWS = 1
endif
endif

ifdef WINDOWS
  ifdef ALLEGRO_INCLUDE
    CFLAGS += -I$(ALLEGRO_INCLUDE)
  endif

  RM = del /q
  CFLAGS += -D__GTHREAD_HIDE_WIN32API
  LFLAGS = -Wl,--subsystem,windows
  
  ifdef ALLEGRO_LIB
    LFLAGS += -L$(ALLEGRO_LIB)
  endif
  
  ifdef STATICLINK
    CFLAGS += -DALLEGRO_STATICLINK
    LFLAGS += -static-libgcc
    LIBS += -lallegro-5.0.8-monolith-static-mt-debug -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -lwinmm -ldsound -lopengl32 -lpsapi
    OBJDIR = obj/static
    BIN = $(NAME)_static.exe
  else
    LIBS += -lallegro-5.0.8-monolith-mt-debug
    OBJDIR = obj
    BIN = $(NAME).exe
  endif
else
  RM = rm -f
  ifdef STATICLINK
    LIBS += `pkg-config --libs --static allegro-5.0 allegro_acodec-5.0 allegro_audio-5.0 allegro_color-5.0 allegro_font-5.0 allegro_image-5.0 allegro_main-5.0 allegro_memfile-5.0 allegro_physfs-5.0 allegro_primitives-5.0 allegro_ttf-5.0`
    OBJDIR = obj/static
    BIN = $(NAME)_static
  else
    LIBS += `pkg-config --libs allegro-5.0 allegro_acodec-5.0 allegro_audio-5.0 allegro_color-5.0 allegro_font-5.0 allegro_image-5.0 allegro_main-5.0 allegro_memfile-5.0 allegro_physfs-5.0 allegro_primitives-5.0 allegro_ttf-5.0`
    OBJDIR = obj
    BIN = $(NAME)
  endif
endif

OBJ_CPP := $(addprefix $(OBJDIR)/, $(subst src/,,$(patsubst %.cpp,%.o,$(wildcard src/*.cpp) $(wildcard src/**/*.cpp))))
OBJ_C := $(addprefix $(OBJDIR)/, $(subst src/,,$(patsubst %.c,%.o,$(wildcard src/*.c) $(wildcard src/**/*.c))))

all: game

$(OBJDIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

game: $(OBJ_C) $(OBJ_CPP)
	$(LD) -o $(BIN) $(OBJ_C) $(OBJ_CPP) $(LIBS) $(LFLAGS)

clean:
ifdef WINDOWS
ifneq ($(OBJ_C),)
	-$(RM) $(subst /,\,$(OBJ_C))
endif
ifneq ($(OBJ_CPP),)
	-$(RM) $(subst /,\,$(OBJ_CPP))
endif
else
ifneq ($(OBJ_C),)
	-$(RM) $(OBJ_C)
endif
ifneq ($(OBJ_CPP),)
	-$(RM) $(OBJ_CPP)
endif
endif

veryclean: clean
	-$(RM) $(BIN)