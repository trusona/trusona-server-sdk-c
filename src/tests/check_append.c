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

char *buffer = NULL;

void setup()
{
  buffer = calloc(1, sizeof(char) * MAX_STR);
}

void teardown()
{
  free(buffer);
  buffer = NULL;
}

START_TEST(will_append_strings_successfully)
{
  strcpy(buffer, "1234");
  append_str(&buffer, "5678");
  ck_assert_pstr_eq(buffer, "12345678");
}
END_TEST;

START_TEST(will_append_strings_with_whitespace_successfully)
{
  strcpy(buffer, " 1234 ");
  append_str(&buffer, " 5678 ");
  ck_assert_pstr_eq(buffer, " 1234  5678 ");
}
END_TEST;

START_TEST(will_not_perform_any_operations_if_source_is_NULL)
{
  strcpy(buffer, " 1234 ");
  append_str(&buffer, NULL);
  ck_assert_pstr_eq(buffer, " 1234 ");
}
END_TEST;

START_TEST(will_not_perform_any_operations_if_destination_is_NULL)
{
  append_str(NULL, "1234");
  ck_assert_pstr_eq(buffer, "\0"); // OR
  ck_assert_pstr_eq(buffer, "");
}
END_TEST;

Suite *utils_suite(void)
{
  Suite *suite;
  TCase *tests;

  suite = suite_create("String Append Tests");
  tests = tcase_create("tests");

  tcase_add_checked_fixture(tests, setup, teardown);

  tcase_add_test(tests, will_append_strings_successfully);
  tcase_add_test(tests, will_append_strings_with_whitespace_successfully);
  tcase_add_test(tests, will_not_perform_any_operations_if_destination_is_NULL);
  tcase_add_test(tests, will_not_perform_any_operations_if_source_is_NULL);

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