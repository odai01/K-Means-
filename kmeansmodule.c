#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "cap.h"

static PyObject* kmeanswrapperfunc(PyObject *self, PyObject *args)
{
    PyObject* dpoints;
    PyObject* icents;
    PyObject* res;
    double* Cres;
    int N,d,k,iter;
    double epsilon;
    if(!PyArg_ParseTuple(args, "OOiiiid",&icents, &dpoints,&N,&d,&k,&iter,&epsilon)) {
        return NULL;
    }
    double *dp=(double *)malloc(N*d*sizeof(double));
    double *ic=(double *)malloc(k*d*sizeof(double));
    int i=0;
    int n1=PyObject_Length(dpoints),n2=PyObject_Length(icents);
    PyObject *item;
    for(i=0;i<n1;i++){
        item = PyList_GetItem(dpoints, i);
        dp[i]=PyFloat_AsDouble(item);
    }
    for(i=0;i<n2;i++){
        item = PyList_GetItem(icents, i);
        ic[i]=PyFloat_AsDouble(item);
    }
    Cres=kalg(ic,dp,N,d,k,iter,epsilon);
    res=PyList_New(k*d);
    for(i=0;i<k*d;i++){
        PyList_SetItem(res, i, Py_BuildValue("d", Cres[i]));
    }

    return res;
}

static PyMethodDef KMeansMethods[] = {
    {"fit",                   
      (PyCFunction) kmeanswrapperfunc, 
      METH_VARARGS,      
      PyDoc_STR("the function gets initial centroids, the data points, N:number of data points,d:dimension of each point,k:number of clusters, iter: number of maximum iterations")}, 
    {NULL, NULL, 0, NULL}   
};

static struct PyModuleDef KMeansModule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp", 
    NULL,
    -1,  
    KMeansMethods 
};
PyMODINIT_FUNC PyInit_mykmeanssp(void) {
    return PyModule_Create(&KMeansModule);
}



