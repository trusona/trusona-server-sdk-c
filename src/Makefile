# MIT License
#
# Copyright (c) 2016, 2017 Trusona Inc (www.trusona.com) All Rights Reserved
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

all: objects
	if [ `uname` = "Linux" ]; then gcc -L/usr/local/lib -shared json.o net.o trusona.o internal.o hmac.o base64.o -luuid -lcurl -ljansson -lcrypto -o libtrusona.so; fi
	if [ `uname` = "Darwin" ]; then gcc -dynamiclib -luuid -lcurl -ljansson -lcrypto *.o -o libtrusona.dylib; fi
	make -s clean

objects: clean-all
	gcc -Wall -fPIC -c internal.c  -o internal.o
	gcc -Wall -fPIC -c trusona.c   -o trusona.o
	gcc -Wall -fPIC -c base64.c    -o base64.o
	gcc -Wall -fPIC -c utils.c     -o utils.o
	gcc -Wall -fPIC -c json.c      -o json.o
	gcc -Wall -fPIC -c hmac.c      -o hmac.o
	gcc -Wall -fPIC -c net.c       -o net.o

clean-all: clean
	$(shell rm -f *.so libtrusona.* a.out)

clean:
	$(shell rm -f *.o)

install: all
	$(shell mkdir -p /usr/local/lib /usr/local/include/trusona)
	$(shell strip trusona.so 2>/dev/null)
	$(shell if [ `uname` = "Linux" ]; then cp libtrusona.so /usr/local/lib/; fi)
	$(shell if [ `uname` = "Darwin" ]; then cp libtrusona.dylib /usr/local/lib/; fi)
	$(shell rm -f /usr/local/include/trusona/*.h)
	$(shell cp *.h /usr/local/include/trusona)
	$(shell rm -rf *.o infer-out)

test:
	## infer is available here - http://fbinfer.com
	infer -- make all
