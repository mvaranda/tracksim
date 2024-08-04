


#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "python_int.h"

// logging for C
#define LOG printf
#define LOG_E printf

#ifdef __cplusplus
  extern "C" {
#endif

static PyObject *pName = NULL, *pModule = NULL;

void simInt_destroy()
{
    Py_DECREF(pModule);
    Py_FinalizeEx();
}

void simInt_init(const char * _program)
{
    const char * filename = "simulator";

    Py_Initialize();
    pName = PyUnicode_DecodeFSDefault(filename);

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule == NULL) {
        PyErr_Print();
        LOG_E("Failed to load \"%s\"\n", filename);
        return;
    }
}


void simInt_add(int x, int y)
{

}

void simInt_get_val(const char * var_name)
{

}

void simInt_clearAll()
{

}

bool simInt_addItem(item_t * item)
{
    PyObject *pFunc, *pValue, *dic, *pArgs;

    const char * func_name = "add_item";

    // create a dictionary for the item
    dic = PyDict_New();
    if (!dic) {
        LOG_E("could not create a dic\n");
        return false;
    }
    PyObject *key = PyUnicode_FromString("name");
    if (!key) {
        LOG_E("could not create a key\n");
        Py_DECREF(dic);
        return false;
    }
    PyObject *value = PyUnicode_FromString(item->name);
    if (!value) {
        LOG_E("could not create a value\n");
        Py_DECREF(key);
        Py_DECREF(dic);
        return false;
    }
    PyDict_SetItem(dic, key, value);
    Py_DECREF(key);
    Py_DECREF(value);

    pArgs = PyTuple_New(1);
    if (!pArgs) {
        LOG_E("could not create args tuple\n");
        Py_DECREF(dic);
        return false;
    }
    PyTuple_SetItem(pArgs, 0, dic);

    // call python function passing the dic as argument
    pFunc = PyObject_GetAttrString(pModule, func_name);
    if (pFunc && PyCallable_Check(pFunc)) {
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
            LOG("Result of add_item call: %ld\n", PyLong_AsLong(pValue));
            Py_DECREF(pValue);
        }
    }
    Py_DECREF(pFunc);
    Py_DECREF(pArgs);
    Py_DECREF(dic);


    return true;
}


bool simInt_addSegment(segment_t * seg)
{
    return true;
}

#ifdef __cplusplus
  }
#endif