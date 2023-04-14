#......................................................
#.................lll..................................
#.................lll..................................
#.................lll..................................
#.xxxx..xxxx......lll...aaaaaa...nnnnnnnn....gggggggg..
#..xxxxxxxx.......lll..aaaaaaaa..nnnnnnnnn..ggggggggg..
#..xxxxxxxx.......lll..aaa.aaaa..nnnn.nnnn.ngggg.gggg..
#...xxxxxx........lll....aaaaaa..nnnn..nnn.nggg..gggg..
#....xxxx.........lll..aaaaaaaa..nnn...nnn.nggg...ggg..
#...xxxxxx........lll.laaaaaaaa..nnn...nnn.nggg...ggg..
#...xxxxxx........lll.laaa..aaa..nnn...nnn.nggg..gggg..
#..xxxxxxxx.......lll.laaa.aaaa..nnn...nnn.ngggg.gggg..
#.xxxx..xxxx......lll.laaaaaaaaaannn...nnn..ggggggggg..
#.xxxx..xxxx......lll..aaaaaaaaaannn...nnn..ggggggggg..
#.................................................ggg..
#...........................................gggg.gggg..
#...........................................ggggggggg..
#............................................ggggggg...
#......................................................

cmake_policy(SET CMP0048 NEW)

# Set cmake project information.
project(x VERSION 1.0.0)

# Set minimum cmake requirements.
cmake_minimum_required(VERSION 3.18)

# Change the default compiler to GCC.
set(CMAKE_C_COMPILER "gcc")

# Do not allow cmake to specify a C standard. Manually specify a C standard.
set(CMAKE_C_STANDARD_REQUIRED OFF)

# Set C flags for various cmake build environments.
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -std=c99")  # add -pedantic later
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3")

# Set a custom binary output directory path.
set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set the include directory.
include_directories("${PROJECT_SOURCE_DIR}/include")

# The the library binary directory.
link_directories("${PROJECT_SOURCE_DIR}/libexec")

#.......................................................................
#.bbb...................iii..lll........ddd..iii........................
#.bbb...................iii..lll........ddd..iii........................
#.bbb........................lll........ddd.............................
#.bbbbbbbb...uuu...uuu..iii..lll...dddddddd..iii..nnnnnnnn....gggggggg..
#.bbbbbbbbb..uuu...uuu..iii..lll..ddddddddd..iii..nnnnnnnnn..ggggggggg..
#.bbbb.bbbbb.uuu...uuu..iii..lll.ldddd.dddd..iii..nnnn.nnnn.ngggg.gggg..
#.bbbb..bbbb.uuu...uuu..iii..lll.lddd..dddd..iii..nnnn..nnn.nggg..gggg..
#.bbb...bbbb.uuu...uuu..iii..lll.lddd...ddd..iii..nnn...nnn.nggg...ggg..
#.bbb...bbbb.uuu...uuu..iii..lll.lddd...ddd..iii..nnn...nnn.nggg...ggg..
#.bbb...bbbb.uuu..uuuu..iii..lll.lddd..dddd..iii..nnn...nnn.nggg..gggg..
#.bbbb.bbbb..uuuu.uuuu..iii..lll.ldddd.dddd..iii..nnn...nnn.ngggg.gggg..
#.bbbbbbbbb..uuuuuuuuu..iii..lll..ddddddddd..iii..nnn...nnn..ggggggggg..
#.bbbbbbbb...uuuuuuuuu..iii..lll..ddddddddd..iii..nnn...nnn..ggggggggg..
#..................................................................ggg..
#............................................................gggg.gggg..
#............................................................ggggggggg..
#.............................................................ggggggg...
#.......................................................................

# Build all project subdirectorties.
add_subdirectory("${PROJECT_SOURCE_DIR}/src")
add_subdirectory("${PROJECT_SOURCE_DIR}/test")
add_subdirectory("${PROJECT_SOURCE_DIR}/src/out/repl")

#.............................................................
#...tt........................tt...iii........................
#..ttt.......................ttt...iii........................
#..ttt.......................ttt..............................
#.tttttt..eeeeee...ssssssss.sttttt.iii..nnnnnnnn....gggggggg..
#.tttttt.eeeeeeee..ssssssss.sttttt.iii..nnnnnnnnn..ggggggggg..
#..ttt...eeee.eeee.sss..sss..ttt...iii..nnnn.nnnn.ngggg.gggg..
#..ttt..eeee..eeee.sssss.....ttt...iii..nnnn..nnn.nggg..gggg..
#..ttt..eeeeeeeeee.ssssssss..ttt...iii..nnn...nnn.nggg...ggg..
#..ttt..eeeeeeeeee..ssssssss.ttt...iii..nnn...nnn.nggg...ggg..
#..ttt..eeee...........sssss.ttt...iii..nnn...nnn.nggg..gggg..
#..tttt..eeee.eeeeesss..ssss.tttt..iii..nnn...nnn.ngggg.gggg..
#..ttttt.eeeeeeee.esssssssss.ttttt.iii..nnn...nnn..ggggggggg..
#..ttttt..eeeeeee..ssssssss..ttttt.iii..nnn...nnn..ggggggggg..
#........................................................ggg..
#..................................................gggg.gggg..
#..................................................ggggggggg..
#...................................................ggggggg...
#.............................................................

# Import the CMake testing framework.
include(CTest)

# Configure software tests.
add_test(NAME ae_test COMMAND ae_test)
add_test(NAME queue_test COMMAND queue_test)
add_test(NAME stack_test COMMAND stack_test)
