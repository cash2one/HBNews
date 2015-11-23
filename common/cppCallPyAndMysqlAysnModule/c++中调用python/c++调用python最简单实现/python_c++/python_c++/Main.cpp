#include <Python.h>
#include <iostream>


int main()
{
	//初始化系统中得python解释器
	Py_Initialize();


	//python的解释器是否已经初始化，返回true（非0）表明已经初始化，返回false（0）表明没有初始化
	if (!Py_IsInitialized())
	{
		std::cout << "Py_Initialize() failed" << std::endl;
		system("pause");
		return -1;
	}

	//PyRun_SimpleString是一个宏，执行的时这个函数：PyRun_SimpleStringFlags，会执行python的代码，参数就是python的代码
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");

	//在c++中python的对象都是PyObject *类型
	PyObject *pName = NULL;
	PyObject *pModule = NULL;
	PyObject *pDict = NULL;
	PyObject *pFunc = NULL;
	PyObject *pArgs = NULL;
	PyObject *pReturn = NULL;

	//载入脚本
	//加载指定目录下得test_py.py文件，失败返回null
	pName = PyString_FromString("test_py");
	pModule = PyImport_Import(pName);

	if (!pModule)
	{
		std::cout << "can not find test_py.py" << std::endl;
		system("pause");
		return -1;
	}

	//得到模块名称空间下的字典对象
	pDict = PyModule_GetDict(pModule);
	if (!pDict)
	{
		system("pause");
		return -1;
	}

	//找出函数名为token_post的函数
	//PyCallable_Check判断传入的参数是否是可调用的
	pFunc = PyDict_GetItemString(pDict, "add");

	std::cout << "pFunc: " << pFunc << std::endl;
	if (!pFunc || !PyCallable_Check(pFunc))
	{
		std::cout << "can not find function [add]" << std::endl;
		system("pause");
		return -1;
	}

	//创建元祖对象，参数是长度
	//pArgs = PyTuple_New(2);
	pArgs = PyTuple_New(1);

	//PyTuple_SetItem 将数据插入到元祖中去，第一个是元祖对象，第二个是元祖序号，第三个参数是python类型的数据，成功返回0
	//Py_BuildValue创建可以插入到元祖中的数据类型，第一个参数是格式，"i"就是int类型，第二个就是要传入的数据
	//int setItemVal = PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", 34));
	
	//setItemVal = PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", 16));
	int setItemVal = PyTuple_SetItem(pArgs, 0, Py_BuildValue("ii", 34, 16));
	if (setItemVal != 0)
	{
		std::cout << "PyTuple_SetItem failed" << std::endl;
		return -1;
	}

	//调用python中的函数，第一个参数是要调用的函数，第二个参数是要传入的参数(要是不需要参数，传入NULL)， pReturn保存python中的返回值
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//PyInt_AS_LONG将python 的返回值转成c的类型
	//要是python的返回值是一个元祖，可以使用PyArg_ParseTuple函数来解析
	if (NULL == pReturn)
	{
		std::cout << "PyObject_CallObject failed" << std::endl;
		return -1;
	}
	std::cout << "python return: " << PyInt_AS_LONG(pReturn) << std::endl;

	//Py_DECREF 减少PyObject指针的引用计数
	//一定要注意，不可调用Py_DECREF(pDict)，否则会引起异常。
	//原因是：查看python C/api中PyModule_GetDict函数的返回值可以知道，返回的是一个指向别的对象的引用（Return value: Borrowed reference），不可以重复释放资源
	//只有返回值是指向新引用的对象（Return value: New reference）才可以调用Py_DECREF函数
	if (pName != NULL)
	{
		Py_DECREF(pName);
	}

	if (pFunc != NULL)
	{
		Py_DECREF(pFunc);
	}

	if (pArgs != NULL)
	{
		Py_DECREF(pArgs);
	}

	if (pModule != NULL)
	{
		Py_DECREF(pModule);
	}

	if (pReturn != NULL)
	{
		Py_DECREF(pReturn);
	}
	
	//关闭python，释放资源
	Py_Finalize();


	system("pause");

	return 0;
}

