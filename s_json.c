#include <jjp_wrap.h>
#include <jsmn.h>
#include <jsonpath.h>
#include <s_json.h>
#include <stdlib.h>
#include <string.h>

struct s_json {
  const char *json_string;
  jsmn_parser parser;
  jsmntok_t *tokens;
  int tokens_max;
  int tokens_c;
};

static void set_ok(s_json_err_t *rc) {
  if (rc) {
    *rc = S_JSON_OK;
  }
}

#ifdef check
#undef check
#endif
#define check(A, B, C)                                                         \
  if (!(A)) {                                                                  \
    if (rc) {                                                                  \
      *rc = B;                                                                 \
    }                                                                          \
    goto C;                                                                    \
  }

#define check_input_params()                                                   \
  {                                                                            \
    check(json, S_JSON_ERR_WRONG_JSON, final_cleanup);                         \
    check(json->tokens_c > 0, S_JSON_ERR_INTERNAL, final_cleanup);             \
    check(json, S_JSON_ERR_WRONG_ROOT, final_cleanup);                         \
  }

static void j_cleanup(s_json_t *json) {
  if (json) {
    if (json->tokens) {
      free(json->tokens);
    }
    free(json);
  }
}

s_json_t *s_json_init(const char *json_string, size_t json_string_len,
                      s_json_err_t *rc) {

  s_json_t *json;
  jsmntok_t *tmp;

  check(json_string, S_JSON_ERR_PARSE, final_cleanup);
  json = malloc(sizeof(struct s_json));
  check(json, S_JSON_ERR_NO_MEM, final_cleanup);
  jsmn_init(&json->parser);
  json->json_string = json_string;
  json->tokens_max = 100;
  json->tokens = NULL;
  json->tokens_c = JSMN_ERROR_NOMEM;
  while (json->tokens_c == JSMN_ERROR_NOMEM) {
    json->tokens_max *= 2;
    tmp = realloc(json->tokens, json->tokens_max * sizeof(jsmntok_t));
    check(tmp, S_JSON_ERR_NO_MEM, json_cleanup);
    json->tokens = tmp;
    json->tokens_c =
        jsmn_parse(&json->parser, json->json_string, json_string_len,
                   json->tokens, json->tokens_max);
  }
  check(json->tokens_c > 0, S_JSON_ERR_PARSE, json_cleanup);
  set_ok(rc);
  return json;

json_cleanup:
  j_cleanup(json);
final_cleanup:
  return NULL;
}

int s_json_int(s_json_t *json, const char *json_path, int root_object_index,
               s_json_err_t *rc) {

  int rc2;
  int res;

  check_input_params();
  res = jjp_int(json->json_string, json->tokens, json->tokens_c, json_path,
                root_object_index, &rc2);
  check(rc2 == 0, S_JSON_NOT_FOUND, final_cleanup);
  set_ok(rc);
  return res;

final_cleanup:
  return 0;
}

long s_json_long(s_json_t *json, const char *json_path, int root_object_index,
                 s_json_err_t *rc) {

  int rc2;
  long res;

  check_input_params();
  res = jjp_long(json->json_string, json->tokens, json->tokens_c, json_path,
                 root_object_index, &rc2);
  check(rc2 == 0, S_JSON_NOT_FOUND, final_cleanup);
  set_ok(rc);
  return res;

final_cleanup:
  return 0L;
}

double s_json_double(s_json_t *json, const char *json_path,
                     int root_object_index, s_json_err_t *rc) {

  int rc2;
  double res;

  check_input_params();
  res = jjp_double(json->json_string, json->tokens, json->tokens_c, json_path,
                   root_object_index, &rc2);
  check(rc2 == 0, S_JSON_NOT_FOUND, final_cleanup);
  set_ok(rc);
  return res;

final_cleanup:
  return 0.0;
}

float s_json_float(s_json_t *json, const char *json_path, int root_object_index,
                   s_json_err_t *rc) {

  int rc2;
  float res;

  check_input_params();
  res = jjp_float(json->json_string, json->tokens, json->tokens_c, json_path,
                  root_object_index, &rc2);
  check(rc2 == 0, S_JSON_NOT_FOUND, final_cleanup);
  set_ok(rc);
  return res;

final_cleanup:
  return 0.0f;
}

int s_json_boolean(s_json_t *json, const char *json_path, int root_object_index,
                   s_json_err_t *rc) {

  int rc2;
  int res;

  check_input_params();
  res = jjp_boolean(json->json_string, json->tokens, json->tokens_c, json_path,
                    root_object_index, &rc2);
  check(rc2 == 0, S_JSON_NOT_FOUND, final_cleanup);
  set_ok(rc);
  return res;

final_cleanup:
  return 0.0f;
}

char *s_json_string(s_json_t *json, const char *json_path,
                    int root_object_index, s_json_err_t *rc) {

  const char *str;
  char *res;
  size_t len;

  check_input_params();

  s_json_string_raw(&str, &len, json, json_path, root_object_index, rc);
  check(rc == S_JSON_OK, *rc, final_cleanup);
  res = malloc((len + 1) * sizeof(char));
  check(res, S_JSON_ERR_NO_MEM, final_cleanup);
  (void)strncpy(res, str, len);
  res[len] = '\0';
  set_ok(rc);
  return res;

final_cleanup:
  return NULL;
}

void s_json_string_raw(const char **string_raw, size_t *string_raw_length,
                       s_json_t *json, const char *json_path,
                       int root_object_index, s_json_err_t *rc) {

  int i;
  int len;

  check_input_params();
  i = jjp_jsonpath_first(json->json_string, json->tokens, json->tokens_c,
                         json_path, root_object_index);
  check(i >= 0 && json->tokens[i].type == JSMN_STRING, S_JSON_NOT_FOUND,
        final_cleanup);
  len = json->tokens[i].end - json->tokens[i].start;
  check(len >= 0, S_JSON_ERR_INTERNAL, final_cleanup);
  set_ok(rc);
  *string_raw_length = len;
  *string_raw = json->json_string + json->tokens[i].start;
  return;

final_cleanup:
  *string_raw = NULL;
  *string_raw_length = 0;
  return;
}

int s_json_object(s_json_t *json, const char *json_path, int root_object_index,
                  s_json_err_t *rc) {

  int index;

  check_input_params();
  index = jjp_jsonpath_first(json->json_string, json->tokens, json->tokens_c,
                             json_path, root_object_index);
  check(index >= 0, S_JSON_NOT_FOUND, final_cleanup);
  set_ok(rc);
  return index;

final_cleanup:
  return 0;
}

void s_json_cleanup(s_json_t *json) { j_cleanup(json); }
