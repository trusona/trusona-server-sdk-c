// Apache License. Version 2.0, January 2004
//
// Copyright (c) 2016, 2017, 2018 Trusona Inc (https://trusona.com) All Rights Reserved
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <check.h>
#include "../utils.h"

const char *r_file = "test.empty.txt";
const char *r_link = "test.link.txt";

void setup()
{
  system("echo 1 > test.empty.txt");
  system("ln -sf test.empty.txt test.link.txt");
}

void teardown()
{
  system("rm -f test.*.txt");
}

START_TEST(will_return_predefined_ID_for_nonexistant_files)
{
  ck_assert_int_eq(owned_by("no-such-file.txt"), UINT_MAX - 4);
}
END_TEST;

START_TEST(will_return_predefined_ID_for_symbolic_links)
{
  ck_assert_int_eq(owned_by(r_link), UINT_MAX - 2);
}
END_TEST;

START_TEST(will_return_ID_matching_current_user_for_files_owned_by_current_user)
{
  ck_assert_int_eq(owned_by(r_file), geteuid());
}
END_TEST;

Suite *utils_suite(void)
{
  Suite *suite;
  TCase *tests;

  suite = suite_create("File Ownership Tests");
  tests = tcase_create("tests");

  tcase_add_checked_fixture(tests, setup, teardown);
  tcase_add_test(tests, will_return_predefined_ID_for_nonexistant_files);
  tcase_add_test(tests, will_return_ID_matching_current_user_for_files_owned_by_current_user);
  tcase_add_test(tests, will_return_predefined_ID_for_symbolic_links);

  suite_add_tcase(suite, tests);

  return(suite);
}

int main(void)
{
  int      failures;
  Suite *  suite;
  SRunner *runner;

  suite    = utils_suite();
  runner   = srunner_create(suite);
  failures = -1;

  srunner_run_all(runner, CK_NORMAL);
  failures = srunner_ntests_failed(runner);
  srunner_free(runner);
  return((failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}