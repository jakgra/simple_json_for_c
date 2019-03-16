# A Simple JSON parser written in C that supports jsonpath queries
Internally it uses jsmn (https://github.com/zserge/jsmn) for parsing the JSON string and jsmn-jsonpath (https://github.com/jakgra/jsmn-jsonpath) for the jsonpath queries support.  

## Usage
It is very simple. If this is the `main.c` file:
```c
#include <s_json.h>
#include <string.h>
#include <stdio.h>

const char *json_str = "{ \"message\": \"Hello world.\", \"a_number\": 7 }";

int main() {
  s_json_t *json;

  json = s_json_init(json_str, strlen(json_str), NULL);
  printf(
         "%s I'm agent 00%d.\n",
         s_json_string(json, "$.message", 0, NULL),
         s_json_int(json, "$.a_number", 0, NULL)
        );

  return 0;
}
```
Build an run it:  
```bash
git clone https://github.com/jakgra/simple_json_for_c.git
git clone https://github.com/zserge/jsmn.git
git clone https://github.com/jakgra/jsmn-jsonpath.git
cc -Isimple_json_for_c -Ijsmn -Ijsmn-jsonpath -DJSMN_PARENT_LINKS -o main main.c jsmn/jsmn.c jsmn-jsonpath/jsonpath.c jsmn-jsonpath/jjp_wrap.c simple_json_for_c/s_json.c
./main
```
For error handling and all the available functions read the heavily documented but still short `s_json.h` file.  
If you prefer man documentation execute: `cd docs && doxygen` and then `man docs/man/man3/s_json.h.3`  
If you prefer pdf documentation execute: `cd docs && doxygen && cd latex && make` and then open `docs/latex/refman.pdf`  
If you prefer html documentation execute: `cd docs && doxygen` and then open `docs/html/index.html`  
If you like to read your documentation online it's here: https://jakgra.github.io/simple_json_for_c_online_docs/s__json_8h.html  

## Contributing
All interest, usage, questions, bug reports, issues, feature requests, pull requests, forks, ... are kindly welcomed.  
For all of the above please create a new github issue and I should reply to you shortly afterwards.  

