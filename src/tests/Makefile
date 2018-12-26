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
LOCAL_LIBS=-L$(shell pwd)/..

build: clean
	gcc $(LOCAL_LIBS) $(INCLUDES) -Wall -fPIC check_utils.c -lcheck -ltrusona -o check_utils
	@ln -sf ../libtrusona.* .

clean:
	@rm -f *.o check_utils libtrusona.*

test: build
	@./check_utils
	@make clean