cmake_minimum_required(VERSION 3.5)
project(minigooglebot VERSION 0.1 LANGUAGES C)

add_executable(minigooglebot source/avl.c source/avl.h source/bool.h source/list.c source/list.h source/main.c source/site.c source/site.h source/WordTree.c source/WordTree.h source/googlebot.h source/googlebot.c)
set_property(TARGET minigooglebot PROPERTY C_STARDARD 90)
set_property(TARGET minigooglebot PROPERTY C_EXTENSIONS OFF)
