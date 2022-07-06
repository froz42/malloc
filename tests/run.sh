#!/bin/sh
# LD LIB PRELOAD PATH
export LD_LIBRARY_PATH=../lib:$LD_LIBRARY_PATH
# set LD_PRELOAD to load the library
export LD_PRELOAD=../libft_malloc.so
# run the test
$@