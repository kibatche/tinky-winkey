CCFLAGS=/Wall /WX
CCFLAG_DEBUG=/DEBUG
SRCS = srcs/main.cpp
OBJS = main.obj
OBJS_DEBUG = main.obj
INCL = -I ./includes
RM = rm -rf
NAME = tinky
NAME_DEBUG = tinky_debug

all: $(NAME)
debug: $(ODIR_DEBUG) $(NAME_DEBUG)


#main file
$(NAME): $(OBJS)
	link.exe $(CCFLAGS)  $(INCL)  $(OBJS) -o $(NAME)
.cpp.obj:
	$(CC) $(CCFLAGS) $(INCL) -c $<

#debug file
$(NAME_DEBUG): $(LIB_EXE) $(OBJS_DEBUG)
	$(CC) $(CCFLAGS_DEBUG) $(OBJS_DEBUG) -o $(NAME_DEBUG) $(INCL)
$(ODIR_DEBUG)%.obj: $(SOURCE_DIR)%.cpp
	$(CC) $(CCFLAGS_DEBUG) -c $< -o $@ $(INCL)

clean:
	$(RM) $(OBJS) $(OBJS_DEBUG_DEBUG)

fclean:
	$(RM) $(NAME) $(NAME_DEBUG)

re: fclean
	@nmake -j `nproc` all
redebug: fclean debug
.PHONY:	all clean re fclean