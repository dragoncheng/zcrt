AUTOMAKE_OPTIONS=foreign  
INCLUDES= -I../zcrt

AM_CFLAGS=-DLINUX_OS

bin_PROGRAMS=testzcrt
testzcrt_SOURCES=tests/test_api_decode.c tests/test_api_impl.c tests/test_ptzcrt.c
testzcrt_LDADD=../libzcrt.a  -lpthread -ldl
