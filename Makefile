# Apache License. Version 2.0, January 2004
#
# Copyright (c) 2020 Trusona Inc (https://trusona.com) All Rights Reserved
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

all:
	make -C src
	make -C src/example
	if [ -d "src/pam" ]; then make -C src/pam; fi
	@make test

install:
	make install -C src
	make install -C src/example
	if [ -d "src/pam" ]; then make install -C src/pam; fi

uninstall:
	make uninstall -C src
	make uninstall -C src/example
	if [ -d "src/pam" ]; then make uninstall -C src/pam; fi

clean:
	@make clean -C src
	@make clean -C src/example
	@make clean -C src/tests
	@if [ -d "src/pam" ]; then make clean -C src/pam; fi

test:
	@if [ `ls src/libtrusona.* 2>/dev/null | wc -l` -eq 0 ]; then make -C src; fi
	@make test -C src/tests