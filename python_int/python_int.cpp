/*
*************************************************************
*                                                           *
*              Part of the Tracks Simulator                 *
*                                                           *
* Copyright: Marcelo Varanda                                *
* License: GPL3                                             *
*                                                           *
************************************************************
*/

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "python_int.h"

// logging for C
#define LOG printf
#define LOG_E printf


extern bool cpp_sim_create_item(item_t * it);
extern bool cpp_sim_create_segment(segment_t * seg);
extern bool cpp_sim_create_text(text_t * txt);
extern bool cpp_sim_create_train(train_t * train);
extern bool cpp_sim_cmd_to_ui(const char * cmd);


#ifdef __cplusplus
  extern "C" {
#endif

static PyObject *pModule = NULL;

//****************************************************
//*       calls from python to C (extension)         *
//****************************************************

static int numargs=0;

static PyObject* sim_cmd_to_ui(PyObject *self, PyObject *args)
{
    char *str_ptr;
    if (!PyArg_ParseTuple(args, "s",
        &str_ptr)) {
            printf("PyArg_ParseTuple fail\n");
            PyErr_Print();
            return NULL;
    }
    cpp_sim_cmd_to_ui(str_ptr);

    Py_RETURN_NONE;
}

static PyObject* sim_create_item(PyObject *self, PyObject *args)
{
    char *str_ptr;
    item_t it;
    memset(&it, 0, sizeof(it));
    if (!PyArg_ParseTuple(args, "lslllll", //"lsddlll", 
        &it.sim_id,
        &str_ptr, // later it.type,
        &it.pos_x,
        &it.pos_y,
        &it.color_r,
        &it.color_g,
        &it.color_b)) {
            printf("PyArg_ParseTuple fail\n");
            PyErr_Print();
            return NULL;
    }
    strncpy(it.type, str_ptr, sizeof(it.type) - 1);

    //printf("sim_create_item for sim_id: %d\n", it.sim_id);
    printf("sim_create_item for sim_id: %d\ntype: %s\n", it.sim_id, it.type);
    cpp_sim_create_item(&it);

    Py_RETURN_NONE;
}

static PyObject* sim_create_segment(PyObject *self, PyObject *args)
{
    static segment_t seg; // it sems that python has a limited stack ?
printf("<<<<<<<<<<< sim_create_segment >>>>>>>>>>>>>>\n");
    char * str_ptr; // = "Hello";

    memset(&seg, 0, sizeof(seg));
    if (!PyArg_ParseTuple(args, "lslllllllll", 
        &seg.sim_id,
        &str_ptr,
        &seg.pos_x,
        &seg.pos_y,
        &seg.color_r,
        &seg.color_g,
        &seg.color_b,
        &seg.startTrackPoint_id,
        &seg.endTrackPoint_id,
        &seg.startLightState,
        &seg.endLightState)) {
            printf("PyArg_ParseTuple fail\n");
            PyErr_Print();
            return NULL;
    }

    strncpy(seg.type, str_ptr, sizeof(seg.type) - 1);
    printf("sim_create_segment for sim_id: %d\ntype: %s\n", seg.sim_id, seg.type);
    cpp_sim_create_segment(&seg);

    Py_RETURN_NONE;
}

static PyObject* sim_create_text(PyObject *self, PyObject *args)
{
    static text_t txt; // it sems that python has a limited stack ?
printf("<<<<<<<<<<< sim_create_text >>>>>>>>>>>>>>\n");
    char * text_ptr, * font_ptr;

    memset(&txt, 0, sizeof(txt));
    if (!PyArg_ParseTuple(args, "ssllllll", 
        &text_ptr,
        &font_ptr,
        &txt.size,
        &txt.pos_x,
        &txt.pos_y,
        &txt.color_r,
        &txt.color_g,
        &txt.color_b)) {
            printf("PyArg_ParseTuple fail\n");
            PyErr_Print();
            return NULL;
    }

    strncpy(txt.text, text_ptr, sizeof(txt.text) - 1);
    printf("sim_create_text for text: %s\n", txt.text);
    cpp_sim_create_text(&txt);

    Py_RETURN_NONE;
}



static PyObject* sim_create_train(PyObject *self, PyObject *args)
{
    static train_t train; // it sems that python has a limited stack ?
printf("<<<<<<<<<<< sim_create_train >>>>>>>>>>>>>>\n");
    char * text_ptr, * font_ptr;
    PyObject * listObj;
    int type;
    char * tok;         /* delimiter tokens for strtok */
    int cols;           /* number of cols to parse, from the left */

    memset(&train, 0, sizeof(train));
    if (!PyArg_ParseTuple(args, "llllllllO",
        &train.train_number,
        &train.sim_id,
        &train.pos_x,
        &train.pos_y,
        &train.speed,      // ticks/second
        &train.enabled,
        &train.reverse,
        &train.start_time,  // ticks
        &listObj)) {
            printf("PyArg_ParseTuple fail\n");
            PyErr_Print();
            return NULL;
    }
    int size = PyList_Size(listObj);
    for (int i=0; i < size; i++) {
        PyObject * intObj = PyList_GetItem(listObj, i);
        train.route_seg_ids[i] = PyLong_AsLong( intObj );
        LOG("train.route_seg_ids[%d] = %d\n", i, train.route_seg_ids[i] );
    }


    cpp_sim_create_train(&train);

    Py_RETURN_NONE;
}


static PyObject* sim_numargs(PyObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
    return PyLong_FromLong(numargs);
}

static PyMethodDef SimMethods[] = {
    {"numargs", sim_numargs, METH_VARARGS,
        "Return the number of arguments received by the process."},
    {"create_item", sim_create_item, METH_VARARGS,
        "Create a item in the UI domain."},
    {"create_segment", sim_create_segment, METH_VARARGS,
        "Create a segment in the UI domain."},
    {"create_text", sim_create_text, METH_VARARGS,
        "Create a text in the UI domain."},
    {"create_train", sim_create_train, METH_VARARGS,
        "Create a train in the UI domain."},
    {"cmd_to_ui", sim_cmd_to_ui, METH_VARARGS,
        "Create a train in the UI domain."},
        
    {NULL, NULL, 0, NULL}
};

static PyModuleDef EmbModule = {
    PyModuleDef_HEAD_INIT, "sim", NULL, -1, SimMethods,
    NULL, NULL, NULL, NULL
};

static PyObject* PyInit_sim(void)
{
    return PyModule_Create(&EmbModule);
}

//****************************************************
//*          calls C to Python (Embedded)            *
//****************************************************
static wchar_t *program_name;
void simInt_destroy()
{
    if (!pModule) {
        printf("simInt_destroy: nothing to destroy\n");
        return;
    }
    //Py_FinalizeEx();

    Py_DECREF(pModule);
    pModule = NULL;
    Py_FinalizeEx();

    //PyMem_RawFree(program_name);
}

bool simInt_init(const char * _program)
{
    const char * filename = "python_int";
    PyObject *pName = NULL;

    if (pModule) {
        printf("simInt_init: called but interpreter already running\n");
        return true; //false;
    }

    program_name = Py_DecodeLocale(_program, NULL);
    if (program_name == NULL) {
        fprintf(stderr, "simInt_init: Fatal error, cannot decode prog name\n");
        exit(1);
    }
    Py_SetProgramName(program_name);  /* optional but recommended */

    PyImport_AppendInittab("sim", &PyInit_sim);

    Py_Initialize();
    pName = PyUnicode_DecodeFSDefault(filename);

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule == NULL) {
        PyErr_Print();
        LOG_E("Failed to load \"%s\"\n", filename);
        return false;
    }
    return true;
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

// static const char * toChars(PyObject * _value)
// {
//   const char *value;
//   if (!PyArg_ParseTuple(_value, "s", &value))
//       return NULL; /* fails! */
//   return value;
// }
static bool add_string_to_dic(PyObject * dic, const char * _key, const char * _val)
{
    PyObject *key = PyUnicode_FromString(_key);
    if (!key) {
        LOG_E("could not create a key %s\n", _key);
        return false;
    }

    PyObject *value = PyUnicode_FromString(_val);
    if (!value) {
        LOG_E("could not create a value %s\n", _val);
        Py_DECREF(key);
        return false;
    }
    PyDict_SetItem(dic, key, value);
    Py_DECREF(key);
    Py_DECREF(value);
    return true;
}
//PyLong_FromLong
static bool add_long_to_dic(PyObject * dic, const char * _key, int _val)
{
    PyObject *key = PyUnicode_FromString(_key);
    if (!key) {
        LOG_E("could not create a key %s\n", _key);
        return false;
    }

    PyObject *value = PyLong_FromLong(_val);
    if (!value) {
        LOG_E("could not create a value %d\n", _val);
        Py_DECREF(key);
        return false;
    }
    PyDict_SetItem(dic, key, value);
    Py_DECREF(key);
    Py_DECREF(value);
    return true;
}

//PyLong_FromLong
#if 0 // not in use right now
static bool add_float_to_dic(PyObject * dic, const char * _key, float _val)
{
    PyObject *key = PyUnicode_FromString(_key);
    if (!key) {
        LOG_E("could not create a key %s\n", _key);
        return false;
    }

    PyObject *value = PyLong_FromDouble(_val);
    if (!value) {
        LOG_E("could not create a value %f\n", _val);
        Py_DECREF(key);
        return false;
    }
    PyDict_SetItem(dic, key, value);
    Py_DECREF(key);
    Py_DECREF(value);
    return true;
}
#endif

bool simInt_addItem(item_t * item)
{
    PyObject *pFunc, *pValue, *dic, *pArgs;

    const char * func_name = "add_item";

    // create a dictionary for the item
    dic = PyDict_New();
    if (!dic) {
        LOG_E("could not create a dic for item\n");
        return false;
    }

    // populate all fields
    if (!add_string_to_dic(dic, "name", item->name)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_string_to_dic(dic, "type", item->type)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "sim_id", item->sim_id)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "pos_x", item->pos_x)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "pos_y", item->pos_y)) {
        Py_DECREF(dic);
        return false;
    }

    if (!add_long_to_dic(dic, "color_r", item->color_r)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "color_g", item->color_g)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "color_b", item->color_b)) {
        Py_DECREF(dic);
        return false;
    }

    char seg_key_name[64];
    for (int i = 0; i < MAX_NUM_SEGS_PER_TRACKPOINT; i++) {
        snprintf(seg_key_name, sizeof(seg_key_name), "segment_id_%d", i);
        if (!add_long_to_dic(dic, seg_key_name, item->segment_id[i])) {
            Py_DECREF(dic);
            return false;
        }
    }

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
            // LOG("Result of add_item call: %s\n", PyUnicode_AsUTF8(pValue)); //
            //LOG("Python Items size: %ld\n", PyLong_AsLong(pValue));
            Py_DECREF(pValue);
        }
    }
    Py_DECREF(pFunc);
    Py_DECREF(pArgs);
    // Note: no need to Py_DECREF(dic) as PyTuple_SetItem seals reference

    // Hack to test object access for potential console
    // PyRun_SimpleString("print(\"GLOBAL_VAR from PyRun_SimpleString:\")\n"
    //                    "print(GLOBAL_VAR)\n");
    // PyRun_SimpleString("print(\"PyRun_SimpleString: make a = 100\")\na = \"abcd\"\n"
    //                    "print(\"PyRun_SimpleString: a = \" + a)\n");

    return true;
}

//---------------- Train -------------------
bool simInt_addTrain(train_t * train)
{
    int ret;
    PyObject *pFunc, *pValue, *dic, *pArgs;

    const char * func_name = "add_train";

    // create a dictionary for the train
    dic = PyDict_New();
    if (!dic) {
        LOG_E("could not create a dic for train\n");
        return false;
    }

    // populate all fields
    if (!add_long_to_dic(dic, "train_number", train->train_number)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "sim_id", train->sim_id)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "pos_x", train->pos_x)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "pos_y", train->pos_y)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "speed", train->speed)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "start_time", train->start_time)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "enabled", train->enabled)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "reverse", train->reverse)) {
        Py_DECREF(dic);
        return false;
    }

    PyObject * list = PyList_New(0);
    int i = 0;
    for (i = 0; i < NUM_MAX_SEGMENTS_PER_ROUTE; i++) {
        if (train->route_seg_ids[i] == 0) 
            break;

        PyObject *value = PyLong_FromLong(train->route_seg_ids[i]);
        if (!value) {
            LOG_E("could not create a value %d\n", train->route_seg_ids[i]);
            Py_DECREF(list);
            Py_DECREF(dic);
            return false;
        }

        ret = PyList_Insert(list, i, value);
        if (ret) {
            LOG_E("could not add id=%d to the route list", i);
            Py_DECREF(list);
            Py_DECREF(dic);
            return false;
        }
        Py_DECREF(value);
    }

    if (i > 0) {
        PyObject *key = PyUnicode_FromString("route");
        if (!key) {
            LOG_E("could not create a key for route");
            Py_DECREF(list);
            Py_DECREF(dic);
            return false;
        }
        PyDict_SetItem(dic, key, list);
        Py_DECREF(key);

    }
    Py_DECREF(list);


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
            // LOG("Result of add_train call: %s\n", PyUnicode_AsUTF8(pValue)); //
            //LOG("Python Items size: %ld\n", PyLong_AsLong(pValue));
            Py_DECREF(pValue);
        }
    }
    Py_DECREF(pFunc);
    Py_DECREF(pArgs);

    return true;
}

//--------------- Segment ------------------

bool simInt_addSegment(segment_t * seg)
{
    PyObject *pFunc, *pValue, *dic, *pArgs;

    const char * func_name = "add_segment";

    // create a dictionary for the seg
    dic = PyDict_New();
    if (!dic) {
        LOG_E("could not create a dic for segment\n");
        return false;
    }

    // populate all fields
    if (!add_string_to_dic(dic, "name", seg->name)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_string_to_dic(dic, "type", seg->type)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "sim_id", seg->sim_id)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "pos_x", seg->pos_x)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "pos_y", seg->pos_y)) {
        Py_DECREF(dic);
        return false;
    }

    if (!add_long_to_dic(dic, "color_r", seg->color_r)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "color_g", seg->color_g)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "color_b", seg->color_b)) {
        Py_DECREF(dic);
        return false;
    }


    if (!add_long_to_dic(dic, "startTrackPoint_id", seg->startTrackPoint_id)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "endTrackPoint_id", seg->endTrackPoint_id)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "startLightState", seg->startLightState)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "endLightState", seg->endLightState)) {
        Py_DECREF(dic);
        return false;
    }

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
            // LOG("Result of add_item call: %s\n", PyUnicode_AsUTF8(pValue)); //
            //LOG("Python Items size: %ld\n", PyLong_AsLong(pValue));
            Py_DECREF(pValue);
        }
    }
    Py_DECREF(pFunc);
    Py_DECREF(pArgs);    
    // Note: no need to Py_DECREF(dic) as PyTuple_SetItem seals reference

    return true;
}

//------------------ text ---------------------

bool simInt_addText(text_t * txt)
{
    PyObject *pFunc, *pValue, *dic, *pArgs;

    const char * func_name = "add_text";

    // create a dictionary for the txt
    dic = PyDict_New();
    if (!dic) {
        LOG_E("could not create a dic for segment\n");
        return false;
    }

    // populate all fields
    if (!add_string_to_dic(dic, "text", txt->text)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_string_to_dic(dic, "font_name", txt->font_name)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "size", txt->size)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "pos_x", txt->pos_x)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "pos_y", txt->pos_y)) {
        Py_DECREF(dic);
        return false;
    }

    if (!add_long_to_dic(dic, "color_r", txt->color_r)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "color_g", txt->color_g)) {
        Py_DECREF(dic);
        return false;
    }
    if (!add_long_to_dic(dic, "color_b", txt->color_b)) {
        Py_DECREF(dic);
        return false;
    }

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
            // LOG("Result of add_item call: %s\n", PyUnicode_AsUTF8(pValue)); //
            LOG("Python text size: %ld\n", PyLong_AsLong(pValue));
            Py_DECREF(pValue);
        }
    }
    Py_DECREF(pFunc);
    Py_DECREF(pArgs);    
    // Note: no need to Py_DECREF(dic) as PyTuple_SetItem seals reference

    return true;
}

bool simInt_save(const char * file)
{
    PyObject *pFunc, *pValue, *filename, *pArgs;
    bool result = false;

    const char * func_name = "save";

    filename = PyUnicode_FromString(file);
    pArgs = PyTuple_New(1);
    if (!pArgs) {
        LOG_E("simInt_save: could not create args\n");
        Py_DECREF(filename);
        return false;
    }

    PyTuple_SetItem(pArgs, 0, filename);

    // call python function passing the dic as argument
    pFunc = PyObject_GetAttrString(pModule, func_name);
    if (pFunc && PyCallable_Check(pFunc)) {
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
            int r = PyLong_AsLong(pValue);
            LOG("simInt_save: Python Items size: %d\n", r);
            Py_DECREF(pValue);
            result = r;
        }
    }
    Py_DECREF(pFunc);
    Py_DECREF(pArgs);   
//    Py_DECREF(filename);  // PyTuple_SetItem steals ref for filename

    return result;
}

bool simInt_start()
{
    PyObject *pFunc, *pValue;
    bool result = false;

    const char * func_name = "start";

    pFunc = PyObject_GetAttrString(pModule, func_name);
    if (pFunc && PyCallable_Check(pFunc)) {
        pValue = PyObject_CallObject(pFunc, NULL);
        if (pValue != NULL) {
            int r = PyLong_AsLong(pValue);
            LOG("simInt_start: Python Items size: %d\n", r);
            Py_DECREF(pValue);
            result = r;
        }
    }
    Py_DECREF(pFunc); 

    return result;
}

bool simInt_timer_tick()
{
    PyObject *pFunc, *pValue;
    bool result = false;

    const char * func_name = "timer_tick";

    pFunc = PyObject_GetAttrString(pModule, func_name);
    if (pFunc && PyCallable_Check(pFunc)) {
        pValue = PyObject_CallObject(pFunc, NULL);
        if (pValue != NULL) {
            int r = PyLong_AsLong(pValue);
            LOG("simInt_start: Python Items size: %d\n", r);
            Py_DECREF(pValue);
            result = r;
        }
    }
    Py_DECREF(pFunc); 

    return result;
}

bool simInt_load(const char * file)
{
    PyObject *pFunc, *pValue, *filename, *pArgs;
    bool result = false;

    const char * func_name = "load";

    filename = PyUnicode_FromString(file);
    pArgs = PyTuple_New(1);
    if (!pArgs) {
        LOG_E("simInt_load: could not create args\n");
        Py_DECREF(filename);
        return false;
    }

    PyTuple_SetItem(pArgs, 0, filename);

    // call python function passing the dic as argument
    pFunc = PyObject_GetAttrString(pModule, func_name);
    if (pFunc && PyCallable_Check(pFunc)) {
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pValue != NULL) {
            int r = PyLong_AsLong(pValue);
            LOG("simInt_load: returned: %d\n", r);
            Py_DECREF(pValue);
            result = r;
        }
    }
    Py_DECREF(pFunc);
    Py_DECREF(pArgs);   
//    Py_DECREF(filename);  // PyTuple_SetItem steals ref for filename

    return result;
}

#ifdef __cplusplus
  }
#endif