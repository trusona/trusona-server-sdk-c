# Apache License. Version 2.0, January 2004
#
# Copyright (c) 2016-2020 Trusona Inc (https://trusona.com) All Rights Reserved
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
LIBRARY_PATH=/usr/local/opt/openssl/lib

all: objects
	if [ `uname` = "Linux" ] || [ `uname` = "FreeBSD" ]; then gcc -shared  *.o -luuid -lcurl -ljansson -lcrypto -o libtrusona.so; fi
	if [ `uname` = "Darwin" ]; then gcc -dynamiclib -luuid -lcurl -ljansson -lcrypto *.o -o libtrusona.dylib; fi

objects: clean
	gcc $(INCLUDES) -Wall -fPIC -c trusonafication.c
	gcc $(INCLUDES) -Wall -fPIC -c file_utils.c
	gcc $(INCLUDES) -Wall -fPIC -c str_utils.c
	gcc $(INCLUDES) -Wall -fPIC -c internal.c
	gcc $(INCLUDES) -Wall -fPIC -c trusona.c
	gcc $(INCLUDES) -Wall -fPIC -c base64.c
	gcc $(INCLUDES) -Wall -fPIC -c json.c
	gcc $(INCLUDES) -Wall -fPIC -c hmac.c
	gcc $(INCLUDES) -Wall -fPIC -c net.c

clean:
	rm -f *.so libtrusona.* *.out *.o

install: all
	mkdir -p /usr/local/lib /usr/local/include/trusona
	rm -f /usr/local/lib/libtrusona.*
	if [ `uname` = "Linux" ] || [ `uname` = "FreeBSD" ]; then cp libtrusona.so /usr/local/lib/; fi
	if [ `uname` = "Darwin" ]; then cp libtrusona.dylib /usr/local/lib/; fi
	rm -f /usr/local/include/trusona/*.h
	cp *.h /usr/local/include/trusona

uninstall:
	rm -rf /usr/local/include/trusona/ /usr/local/lib/libtrusona.*