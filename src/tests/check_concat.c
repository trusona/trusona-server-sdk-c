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
#include "../str_utils.h"

START_TEST(will_concatenate_strings_successfully)
{
  ck_assert_pstr_eq(concat_str("1234", "5678"), "12345678");
  ck_assert_pstr_eq(concat_str("", "test"), "test");
  ck_assert_pstr_eq(concat_str("test", ""), "test");
  ck_assert_pstr_eq(concat_str("", ""), "\0");
  ck_assert_pstr_eq(concat_str("", ""), "");
}
END_TEST;

START_TEST(will_not_trim_spaces_from_concatenated_strings)
{
  ck_assert_pstr_eq(concat_str(" 1234 ", " 5678 "), " 1234  5678 ");
  ck_assert_pstr_eq(concat_str(" ", " "), "  ");
}
END_TEST;

START_TEST(will_return_NULL_if_any_inputs_are_NULL)
{
  ck_assert_pstr_eq(concat_str(NULL, " test "), NULL);
  ck_assert_pstr_eq(concat_str(NULL, NULL), NULL);
  ck_assert_pstr_eq(concat_str("test", NULL), NULL);
}
END_TEST;

Suite *utils_suite(void)
{
  Suite *suite;
  TCase *tests;

  suite = suite_create("String Concatenation Tests");
  tests = tcase_create("tests");

  tcase_add_test(tests, will_concatenate_strings_successfully);
  tcase_add_test(tests, will_not_trim_spaces_from_concatenated_strings);
  tcase_add_test(tests, will_return_NULL_if_any_inputs_are_NULL);

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
  return(failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}