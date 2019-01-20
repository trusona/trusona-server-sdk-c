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

LOCAL_LIBS=./..
INCLUDES=-I/usr/local/opt/openssl/include -I/usr/local/include -I/usr/include
COMPILE_OPTIONS=-L$(LOCAL_LIBS) $(INCLUDES) -Wl,-rpath,/usr/local/lib -Wl,-rpath,$(LOCAL_LIBS)

#LIB_OPTIONS=-lcheck -ltrusona -lsubunit -lrt -lm -lpthread
LIB_OPTIONS=-lcheck -ltrusona

build: clean
	gcc $(COMPILE_OPTIONS) -Wall -fPIC check_file_perms.c $(LIB_OPTIONS) -o check_file_perms.bin
	gcc $(COMPILE_OPTIONS) -Wall -fPIC check_owned_by.c   $(LIB_OPTIONS) -o check_owned_by.bin
	gcc $(COMPILE_OPTIONS) -Wall -fPIC check_concat.c     $(LIB_OPTIONS) -o check_concat.bin
	gcc $(COMPILE_OPTIONS) -Wall -fPIC check_append.c     $(LIB_OPTIONS) -o check_append.bin
	gcc $(COMPILE_OPTIONS) -Wall -fPIC check_trim.c       $(LIB_OPTIONS) -o check_trim.bin
	gcc $(COMPILE_OPTIONS) -Wall -fPIC check_hmac.c       $(LIB_OPTIONS) -o check_hmac.bin
	@ln -sf ../libtrusona.* .

clean:
	@rm -f *.bin libtrusona.*

test: build
	@./check_file_perms.bin
	@./check_owned_by.bin
	@./check_concat.bin
	@./check_append.bin
	@./check_hmac.bin
	@./check_trim.bin
	@make clean

all: test