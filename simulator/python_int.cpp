


#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "python_int.h"

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
    // PyObject *pFunc, *pFunc2;
    // PyObject *pArgs, *pValue;

    const char * filename = "simulator";
    // const char * func_name = "multiply";

    Py_Initialize();
    pName = PyUnicode_DecodeFSDefault(filename);
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);


    if (pModule != NULL) {
        /* All good */
#if 0
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

        pFunc2 = PyObject_GetAttrString(pModule, "print_global");
        /* pFunc2 is a new reference */

        if (pFunc2 && PyCallable_Check(pFunc2)) {
          pValue = PyObject_CallObject(pFunc2, 0);
          if (pValue != NULL) {
              printf("Result of call: %ld\n", PyLong_AsLong(pValue));
              Py_DECREF(pValue);
          }
          else {
              Py_DECREF(pFunc2);
              Py_DECREF(pModule);
              PyErr_Print();
              fprintf(stderr,"pFunc2 Call failed\n");
              return;
          }
        }


        // test if console would work
#if 1
        PyRun_SimpleString("for i in range(10):\n\n"
                       "  print(\"Simul console inputs \" + str(i))\n");
        PyRun_SimpleString("print(\"PyRun_SimpleString: GLOBAL_VAR = \" + GLOBAL_VAR)\n");

#else
        PyRun_SimpleString("for i in range(10):\n");
        PyRun_SimpleString("  print(\"Simul console inputs \" + str(i))\n");
        PyRun_SimpleString("\n");
#endif


        Py_XDECREF(pFunc);
//        Py_DECREF(pModule);
#endif
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", filename);
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

bool simInt_addItem__(item_t * item)
{
    PyObject *pFunc, *pValue, *pArgs, *pFunc2;

    const char * func_name = "multiply"; //"add_item";


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
                return false;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", func_name);
        }

        pFunc2 = PyObject_GetAttrString(pModule, "print_global");
        /* pFunc2 is a new reference */

        if (pFunc2 && PyCallable_Check(pFunc2)) {
          pValue = PyObject_CallObject(pFunc2, 0);
          if (pValue != NULL) {
              printf("Result of call: %ld\n", PyLong_AsLong(pValue));
              Py_DECREF(pValue);
          }
          else {
              Py_DECREF(pFunc2);
              Py_DECREF(pModule);
              PyErr_Print();
              fprintf(stderr,"pFunc2 Call failed\n");
              return false;
          }
        }


        // test if console would work
#if 1
        PyRun_SimpleString("for i in range(10):\n\n"
                       "  print(\"Simul console inputs \" + str(i))\n");
        PyRun_SimpleString("print(\"PyRun_SimpleString: GLOBAL_VAR = \" + GLOBAL_VAR)\n");

#else
        PyRun_SimpleString("for i in range(10):\n");
        PyRun_SimpleString("  print(\"Simul console inputs \" + str(i))\n");
        PyRun_SimpleString("\n");
#endif


        Py_XDECREF(pFunc);

    return true;
}

bool simInt_addItem(item_t * item)
{
    PyObject *pFunc, *pValue, *dic, *pArgs;

    const char * func_name = "add_item";

    // create a dictionary for the item
    dic = PyDict_New();
    if (!dic) {
        printf("could not create a dic\n");
        return false;
    }
    PyObject *key = PyUnicode_FromString("name");
    PyObject *value = PyUnicode_FromString(item->name);
    PyDict_SetItem(dic, key, value);
    Py_DECREF(key);
    Py_DECREF(value);

    pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, dic);

    // call python function passing the dic
    pFunc = PyObject_GetAttrString(pModule, func_name);
    if (pFunc && PyCallable_Check(pFunc)) {
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
            printf("Result of add_item call: %ld\n", PyLong_AsLong(pValue));
            Py_DECREF(pValue);
        }
    }
    Py_DECREF(pArgs);

    return true;
}


bool simInt_addSegment(segment_t * seg)
{
    return true;
}

#ifdef __cplusplus
  }
#endif