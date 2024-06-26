CXX = g++
CXXFLAGS =  -march=native -g -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
 			-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported \
  			-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security \
   			-Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual \
    		-Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
	 		-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods \
	  		-Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef \
	   		-Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix \
	    	-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector \
		 	-fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging \
		  	-fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla \
			-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
TARGET = hash
SourcePrefix = src/
BuildPrefix = build/
BuildFolder = build
Include = -Iinclude -Ilib/Color_console_output/include -Ilib/Simple_list/include -Ilib/Data_buffer/include -Ilib/Onegin_task/include

Sources = hash_table.cpp hash_table_errors.cpp hash_functions.cpp hash_test.cpp
Asm_file = custom_strcmp.s
Asm_obj  = custom_strcmp.o
Main = main.cpp

Libs = -Llib/Color_console_output/build/ -lColor_output -Llib/Simple_list/build/ -lSimple_list  -Llib/Onegin_task/build/ -lOnegin_task

Source = $(addprefix $(SourcePrefix), $(Sources))
MainObject = $(patsubst %.cpp, $(BuildPrefix)%.o, $(Main))

objects = $(patsubst $(SourcePrefix)%.cpp, $(BuildPrefix)%.o, $(Source))

.PHONY : all clean folder release debug

all : release

debug : prepare folder $(TARGET)
	@echo [DEBUG_MODE]

release : CXXFLAGS = -march=native -g -O3 -DNDEBUG
release : prepare folder $(TARGET)
	@echo [RELEASE_MODE]

prepare : 
	cd lib/Color_console_output && make 
	cd lib/Data_buffer          && make
	cd lib/Simple_list          && make
	cd lib/Onegin_task          && make

$(BuildPrefix)$(Asm_obj) :
	@echo [ASM_COMPILE] $(SourcePrefix)$(Asm_file) -o $(BuildPrefix)$(Asm_obj)
	@nasm -f elf64 -g $(SourcePrefix)$(Asm_file) -o $(BuildPrefix)$(Asm_obj)

$(BuildPrefix)%.o : $(SourcePrefix)%.cpp
	@echo [CXX] -c $< -o $@
	@$(CXX) $(CXXFLAGS) $(Include) -c $< -o $@

$(TARGET) : $(BuildPrefix)$(Asm_obj) $(objects) $(MainObject)
	@echo [CC] $^ -o $@
	@$(CXX) $(CXXFLAGS) $(Include) $^ $(Libs) -o $@

clean :
	cd lib/Simple_list          && make clean
	cd lib/Color_console_output && make clean
	cd lib/Data_buffer          && make clean
	cd lib/Onegin_task          && make clean
	rm $(BuildFolder)/*.o
	rm $(TARGET)

folder :
	mkdir -p $(BuildFolder)