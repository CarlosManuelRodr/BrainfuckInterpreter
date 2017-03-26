WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres
CFLAGS_RELEASE = -O2
OBJDIR_RELEASE = obj
DEP_RELEASE = 
OUT_RELEASE = bin/BrainfuckInterpreter


OBJ_RELEASE := 
OBJ_RELEASE += \
$(OBJDIR_RELEASE)/BFInterpreter.o \
$(OBJDIR_RELEASE)/main.o \
$(OBJDIR_RELEASE)/BFOperations.o


all: release

clean: clean_release

		
before_release: 
	@echo ""
	@echo "###################################"
	@echo "#                                 #"
	@echo "#  Building BrainfuckInterpreter  #"
	@echo "#                                 #"
	@echo "###################################"
	@echo ""
	@test -d bin/ || mkdir -p bin/
	@test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 
	@echo ""
	@echo "#############################"
	@echo "#                           #"
	@echo "#            Done.          #"
	@echo "#                           #"
	@echo "#############################"
	@echo ""

release: before_release out_release after_release

out_release: $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(OBJ_RELEASE) -o $(OUT_RELEASE)


$(OBJDIR_RELEASE)/BFInterpreter.o: src/BFInterpreter.cpp
	$(CXX) $(CFLAGS_RELEASE) -c src/BFInterpreter.cpp -o $(OBJDIR_RELEASE)/BFInterpreter.o

$(OBJDIR_RELEASE)/main.o: src/main.cpp
	$(CXX) $(CFLAGS_RELEASE) -c src/main.cpp -o $(OBJDIR_RELEASE)/main.o

$(OBJDIR_RELEASE)/BFOperations.o: src/BFOperations.cpp
	$(CXX) $(CFLAGS_RELEASE) -c src/BFOperations.cpp -o $(OBJDIR_RELEASE)/BFOperations.o




clean_release: 
	@echo "######################"
	@echo "#                    #"
	@echo "#    Cleaning up     #"
	@echo "#                    #"
	@echo "######################"
	@echo ""
	@rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	@rm -fd bin obj


.PHONY: before_release after_release clean_release