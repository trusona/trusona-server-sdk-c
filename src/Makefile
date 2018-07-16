# MIT License
#
# Copyright (c) 2016, 2017, 2018 Trusona Inc (www.trusona.com) All Rights Reserved
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

INCLUDES=-I/usr/local/opt/openssl/include -I/usr/local/include -I/usr/include

all: objects
	if [ `uname` = "Linux" ] || [ `uname` = "FreeBSD" ]; then gcc -shared -luuid -lcurl -ljansson -lcrypto *.o -o libtrusona.so; fi
	if [ `uname` = "Darwin" ]; then gcc -dynamiclib -luuid -lcurl -ljansson -lcrypto *.o -o libtrusona.dylib; fi
	make -s clean

objects: clean-all
	gcc $(INCLUDES) -Wall -fPIC -c trusonafication.c  -o trusonafication.o
	gcc $(INCLUDES) -Wall -fPIC -c internal.c         -o internal.o
	gcc $(INCLUDES) -Wall -fPIC -c trusona.c          -o trusona.o
	gcc $(INCLUDES) -Wall -fPIC -c base64.c           -o base64.o
	gcc $(INCLUDES) -Wall -fPIC -c utils.c            -o utils.o
	gcc $(INCLUDES) -Wall -fPIC -c json.c             -o json.o
	gcc $(INCLUDES) -Wall -fPIC -c hmac.c             -o hmac.o
	gcc $(INCLUDES) -Wall -fPIC -c net.c              -o net.o

clean-all: clean
	rm -f *.so libtrusona.* a.out

clean:
	rm -f *.o

install: all
	mkdir -p /usr/local/lib /usr/local/include/trusona
	rm -f /usr/local/lib/libtrusona.*
	if [ `uname` = "Linux" ] || [ `uname` = "FreeBSD" ]; then cp libtrusona.so /usr/local/lib/; fi
	if [ `uname` = "Darwin" ]; then cp libtrusona.dylib /usr/local/lib/; fi
	rm -f /usr/local/include/trusona/*.h
	cp *.h /usr/local/include/trusona
	rm -rf *.o infer-out

uninstall:
	rm -rf /usr/local/include/trusona/ /usr/local/lib/libtrusona.*

test:
	## infer is available here - http://fbinfer.com
	infer -- make all
