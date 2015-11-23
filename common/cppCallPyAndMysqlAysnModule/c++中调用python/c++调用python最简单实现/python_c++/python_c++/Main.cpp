#include <Python.h>
#include <iostream>


int main()
{
	//��ʼ��ϵͳ�е�python������
	Py_Initialize();


	//python�Ľ������Ƿ��Ѿ���ʼ��������true����0�������Ѿ���ʼ��������false��0������û�г�ʼ��
	if (!Py_IsInitialized())
	{
		std::cout << "Py_Initialize() failed" << std::endl;
		system("pause");
		return -1;
	}

	//PyRun_SimpleString��һ���ִ꣬�е�ʱ���������PyRun_SimpleStringFlags����ִ��python�Ĵ��룬��������python�Ĵ���
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");

	//��c++��python�Ķ�����PyObject *����
	PyObject *pName = NULL;
	PyObject *pModule = NULL;
	PyObject *pDict = NULL;
	PyObject *pFunc = NULL;
	PyObject *pArgs = NULL;
	PyObject *pReturn = NULL;

	//����ű�
	//����ָ��Ŀ¼�µ�test_py.py�ļ���ʧ�ܷ���null
	pName = PyString_FromString("test_py");
	pModule = PyImport_Import(pName);

	if (!pModule)
	{
		std::cout << "can not find test_py.py" << std::endl;
		system("pause");
		return -1;
	}

	//�õ�ģ�����ƿռ��µ��ֵ����
	pDict = PyModule_GetDict(pModule);
	if (!pDict)
	{
		system("pause");
		return -1;
	}

	//�ҳ�������Ϊtoken_post�ĺ���
	//PyCallable_Check�жϴ���Ĳ����Ƿ��ǿɵ��õ�
	pFunc = PyDict_GetItemString(pDict, "add");

	std::cout << "pFunc: " << pFunc << std::endl;
	if (!pFunc || !PyCallable_Check(pFunc))
	{
		std::cout << "can not find function [add]" << std::endl;
		system("pause");
		return -1;
	}

	//����Ԫ����󣬲����ǳ���
	//pArgs = PyTuple_New(2);
	pArgs = PyTuple_New(1);

	//PyTuple_SetItem �����ݲ��뵽Ԫ����ȥ����һ����Ԫ����󣬵ڶ�����Ԫ����ţ�������������python���͵����ݣ��ɹ�����0
	//Py_BuildValue�������Բ��뵽Ԫ���е��������ͣ���һ�������Ǹ�ʽ��"i"����int���ͣ��ڶ�������Ҫ���������
	//int setItemVal = PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", 34));
	
	//setItemVal = PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", 16));
	int setItemVal = PyTuple_SetItem(pArgs, 0, Py_BuildValue("ii", 34, 16));
	if (setItemVal != 0)
	{
		std::cout << "PyTuple_SetItem failed" << std::endl;
		return -1;
	}

	//����python�еĺ�������һ��������Ҫ���õĺ������ڶ���������Ҫ����Ĳ���(Ҫ�ǲ���Ҫ����������NULL)�� pReturn����python�еķ���ֵ
	pReturn = PyObject_CallObject(pFunc, pArgs);

	//PyInt_AS_LONG��python �ķ���ֵת��c������
	//Ҫ��python�ķ���ֵ��һ��Ԫ�棬����ʹ��PyArg_ParseTuple����������
	if (NULL == pReturn)
	{
		std::cout << "PyObject_CallObject failed" << std::endl;
		return -1;
	}
	std::cout << "python return: " << PyInt_AS_LONG(pReturn) << std::endl;

	//Py_DECREF ����PyObjectָ������ü���
	//һ��Ҫע�⣬���ɵ���Py_DECREF(pDict)������������쳣��
	//ԭ���ǣ��鿴python C/api��PyModule_GetDict�����ķ���ֵ����֪�������ص���һ��ָ���Ķ�������ã�Return value: Borrowed reference�����������ظ��ͷ���Դ
	//ֻ�з���ֵ��ָ�������õĶ���Return value: New reference���ſ��Ե���Py_DECREF����
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
	
	//�ر�python���ͷ���Դ
	Py_Finalize();


	system("pause");

	return 0;
}

