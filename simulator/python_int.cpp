


#define PY_SSIZE_T_CLEAN
#include <Python.h>

#ifdef __cplusplus
  extern "C" {
#endif

void pyint_init(const char * _program)
{
    wchar_t *program = Py_DecodeLocale(_program, NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }
    Py_SetProgramName(program);  /* optional but recommended */
    Py_Initialize();
    PyRun_SimpleString("from time import time,ctime\n"
                       "print('Today is', ctime(time()))\n");
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    PyMem_RawFree(program);

}

void pyint_add(int x, int y)
{

}

void pyint_get_val(const char * var_name)
{

}

#ifdef __cplusplus
  }
#endif