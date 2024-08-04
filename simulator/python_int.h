
#pragma once


#ifdef __cplusplus
  extern "C" {
#endif

void pyint_init(const char * _program);

void pyint_add(int x, int y);

void pyint_get_val(const char * var_name);

#ifdef __cplusplus
  }
#endif