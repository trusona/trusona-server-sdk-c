# Apache License. Version 2.0, January 2004
#
# Copyright (c) 2016, 2017, 2018 Trusona Inc (https:#trusona.com) All Rights Reserved
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http:#www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
