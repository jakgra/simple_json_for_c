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
  size_t res_len;
  int obj;

  json = s_json_init(json_str, strlen(json_str), &rc);
  check(json && rc == S_JSON_OK, final_cleanup);

  s_json_string_raw(&res, &res_len, json, "$.pills[0].color", 0, &rc);
  check(rc == S_JSON_OK && res, final_cleanup);
  printf("If you take the %.*s pill,", (int)res_len, res);

  s_json_string_raw(&res, &res_len, json, "$.pills[0].action", 0, &rc);
  check(rc == S_JSON_OK && res, final_cleanup);
  printf(" you may as well just %.*s your life away.\n", (int)res_len, res);

  obj = s_json_object(json, "$.pills[1]", 0, &rc);
  check(rc == S_JSON_OK, final_cleanup);

  s_json_string_raw(&res, &res_len, json, "@.color", obj, &rc);
  check(rc == S_JSON_OK && res, final_cleanup);
  printf("If you take the %.*s pill,", (int)res_len, res);

  s_json_string_raw(&res, &res_len, json, "@.action", obj, &rc);
  check(rc == S_JSON_OK && res, final_cleanup);
  printf(" you may as well just %.*s.\n", (int)res_len, res);

  s_json_cleanup(json);

  return 0;
final_cleanup:
  s_json_cleanup(json);
  return -1;
}
