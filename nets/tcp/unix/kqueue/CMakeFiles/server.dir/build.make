# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.12.4/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.12.4/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/qingdujun/works/workspaces/moss/nets/tcp/unix/kqueue

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/qingdujun/works/workspaces/moss/nets/tcp/unix/kqueue

# Include any dependencies generated for this target.
include CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server.dir/flags.make

CMakeFiles/server.dir/kqueue_server.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/kqueue_server.c.o: kqueue_server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/qingdujun/works/workspaces/moss/nets/tcp/unix/kqueue/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/server.dir/kqueue_server.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/kqueue_server.c.o   -c /Users/qingdujun/works/workspaces/moss/nets/tcp/unix/kqueue/kqueue_server.c

CMakeFiles/server.dir/kqueue_server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/kqueue_server.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/qingdujun/works/workspaces/moss/nets/tcp/unix/kqueue/kqueue_server.c > CMakeFiles/server.dir/kqueue_server.c.i

CMakeFiles/server.dir/kqueue_server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/kqueue_server.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/qingdujun/works/workspaces/moss/nets/tcp/unix/kqueue/kqueue_server.c -o CMakeFiles/server.dir/kqueue_server.c.s

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/kqueue_server.c.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

server: CMakeFiles/server.dir/kqueue_server.c.o
server: CMakeFiles/server.dir/build.make
server: CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/qingdujun/works/workspaces/moss/nets/tcp/unix/kqueue/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server.dir/build: server

.PHONY : CMakeFiles/server.dir/build

CMakeFiles/server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server.dir/clean

CMakeFiles/server.dir/depend:
	cd /Users/qingdujun/works/workspaces/moss/nets/tcp/unix/kqueue && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/qingdujun/works/workspaces/moss/nets/tcp/unix/kqueue /Users/qingdujun/works/workspaces/moss/nets/tcp/unix/kqueue /Users/qingdujun/works/workspaces/moss/nets/tcp/unix/kqueue /Users/qingdujun/works/workspaces/moss/nets/tcp/unix/kqueue /Users/qingdujun/works/workspaces/moss/nets/tcp/unix/kqueue/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server.dir/depend

