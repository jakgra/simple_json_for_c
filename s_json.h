#ifndef s_json_h__
#define s_json_h__

/**
 * A list of errors.
 */
typedef enum {
  S_JSON_OK = 0,
  S_JSON_NOT_FOUND,   /* No results found */
  S_JSON_ERR_NO_MEM,  /* malloc() failed */
  S_JSON_ERR_PARSE,   /* Invalid JSON */
  S_JSON_ERR_IS_NULL, /* The passed s_json_t object is NULL */
  S_JSON_ERR_INTERNAL /* This shouldn't occur.
                     If it does please file a bug report.  */
} s_json_err_t;

/**
 * Opaque JSON document object.
 */
typedef struct s_json s_json_t;

/**
 * Parse the json_string and return an opaque s_json_t object you later use to
 * access json values. You should call s_json_cleanup() when you finish using
 * the object.
 *
 * \param[in]  json_string The json string to parse. It is not copied to new
 * memory, so it has to exist at least as long as the s_json_t object exists.
 *
 * \param[in] json_string_len The length of the json string.
 *
 * \param[out] rc Used for error handling. Can be NULL. See A list of errors.
 *
 * \return NULL on error a valid s_json_t pointer otherwise. For better error
 * handling user the rc parameter.
 */
s_json_t *s_json_init(const char *json_string, size_t json_string_len,
                      s_json_err_t *rc);

/**
 * \param[in]  json A valid/initialized s_json_t object.
 *
 * \param[in] rc Used for error handling. Can be NULL. See A list of errors.
 *
 * \param[out] rc Used for error handling. Can be NULL. See A list of errors.
 *
 * \return 0 on error or the first integer found in json otherwise. For better
 * error handling user the rc parameter.
 */
int s_json_int(s_json_t *json, const char *json_path, s_json_err_t *rc);

/**
 * See s_json_int()
 */
long s_json_long(s_json_t *json, const char *json_path, s_json_err_t *rc);

/**
 * See s_json_int()
 */
double s_json_double(s_json_t *json, const char *json_path, s_json_err_t *rc);

/**
 * See s_json_int()
 */
float s_json_float(s_json_t *json, const char *json_path, s_json_err_t *rc);

/**
 * Parse true/false to (int)1/(int)0
 * See s_json_int()
 */
int s_json_boolean(s_json_t *json, const char *json_path, s_json_err_t *rc);

/**
 * See s_json_int()
 *
 * \return a '\0' terminated C string that it allocated via malloc() or NULL on
 * error. It's your responsibility to free it!
 */
char *s_json_string(s_json_t *json, const char *json_path, s_json_err_t *rc);

/**
 * See s_json_int()
 *
 * \param[out] string_raw_length The length of the returned NON '\0' terminated
 * string.
 *
 * \return a NON '\0' terminated string!!! You have to use the string_raw_length
 * parameter to find the end of the string. The string is not malloc()-ed as it
 * still uses the original json_string you passed into the s_json_init()
 * function for the backing memory storage. So it is only valid as long as
 * json_string is valid!!!
 */
const char *s_json_string_raw(s_json_t *json, const char *json_path,
                              int *string_raw_length, s_json_err_t *rc);

/**
 * Cleanup the memory resources associated with the given opaque s_json_t
 * object.
 */
void s_json_cleanup(s_json_t *json);

#endif
