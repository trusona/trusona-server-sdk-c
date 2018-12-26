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
#include "../internal.h"

const char *file = "57dd7db9cfa2.empty.txt";

void setup(void)
{
  system(concat_str("touch ", file));
}

void teardown(void)
{
  system(concat_str("rm -f ", file));
}

START_TEST(trim_removes_leading_and_trailing_whitespaces)
{
  ck_assert_str_eq(trim("     test"), "test");
  ck_assert_str_eq(trim("   test  "), "test");
  ck_assert_str_eq(trim("test     "), "test");
}
END_TEST;

START_TEST(trim_does_not_fail_on_NULL_input)
{
  ck_assert_pstr_eq(trim(NULL), NULL);
  ck_assert_pstr_ne(trim(NULL), "");
}
END_TEST;

START_TEST(file_perms_will_return_correct_600_value)
{
  system(concat_str("chmod 0600 ", file));
  ck_assert_int_eq(file_perms(file), 600);
}
END_TEST;

START_TEST(file_perms_will_return_correct_666_value)
{
  system(concat_str("chmod 0666 ", file));
  ck_assert_int_eq(file_perms(file), 666);
}
END_TEST;

START_TEST(file_perms_will_return_correct_0_value)
{
  system(concat_str("chmod 0 ", file));
  ck_assert_int_eq(file_perms(file), 0);
}
END_TEST;

START_TEST(file_perms_will_return_negative_1_for_nonexistant_file)
{
  ck_assert_int_eq(file_perms("a665160a-7c18-4584-8608-21e149b10a04"), -1);
}
END_TEST;

Suite *utils_suite(void)
{
  Suite *suite;
  TCase *trim_tests;
  TCase *perms_tests;

  suite = suite_create("utils");

  trim_tests  = tcase_create("trim utils");
  perms_tests = tcase_create("perm utils");

  tcase_add_test(trim_tests, trim_removes_leading_and_trailing_whitespaces);
  tcase_add_test(trim_tests, trim_does_not_fail_on_NULL_input);

  tcase_add_unchecked_fixture(perms_tests, setup, teardown);
  tcase_add_test(perms_tests, file_perms_will_return_correct_600_value);
  tcase_add_test(perms_tests, file_perms_will_return_correct_666_value);
  tcase_add_test(perms_tests, file_perms_will_return_correct_0_value);
  tcase_add_test(perms_tests, file_perms_will_return_negative_1_for_nonexistant_file);

  suite_add_tcase(suite, trim_tests);
  suite_add_tcase(suite, perms_tests);

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