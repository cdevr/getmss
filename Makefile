all:
	clang getmss.c -Wall -o getmss

debug:
	clang -g getmss.c -Wall -o getmss
