#include <jak_dbg.h>
#include <s_json.h>
#include <stdio.h>
#include <string.h>

const char *json_str = "{\"pills\": [ \
{ \"color\": \"blue\", \"action\": \"sleep\" }, \
{ \"color\": \"red\", \"action\": \"die\" } \
	] }";

int main() {

  s_json_t *json;
  s_json_err_t rc;
  const char *res;
  int res_len;

  json = s_json_init(json_str, strlen(json_str), &rc);
  check(json && rc == S_JSON_OK, final_cleanup);

  rc = s_json_string_raw(&res, &res_len, json, "$.pills[0].color");
  check(rc == S_JSON_OK && res, final_cleanup);
  printf("If you take the %.*s pill,", res_len, res);

  rc = s_json_string_raw(&res, &res_len, json, "$.pills[0].action");
  check(rc == S_JSON_OK && res, final_cleanup);
  printf(" you may as well just %.*s your life away.\n", res_len, res);

  return 0;
final_cleanup:
  return -1;
}
