


#define PY_SSIZE_T_CLEAN
#include <Python.h>

#ifdef __cplusplus
  extern "C" {
#endif

#if 1
void pyint_init(const char * _program)
{
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    const char * filename = "simulator";
    const char * func_name = "multiply";

    Py_Initialize();
    pName = PyUnicode_DecodeFSDefault(filename);
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, func_name);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {

            // populate tuple
            pArgs = PyTuple_New(2);
            pValue = PyLong_FromLong(50);       // first arg 50
            PyTuple_SetItem(pArgs, 0, pValue);
            pValue = PyLong_FromLong(4);        // second arg 4
            PyTuple_SetItem(pArgs, 1, pValue);


            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", func_name);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", filename);
        return;
    }
    if (Py_FinalizeEx() < 0) {
        return;
    }
}
#else
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
#endif

void pyint_add(int x, int y)
{

}

void pyint_get_val(const char * var_name)
{

}

#ifdef __cplusplus
  }
#endif