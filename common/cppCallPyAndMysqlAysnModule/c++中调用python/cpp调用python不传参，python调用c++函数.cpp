// py将要调用的函数，发请求给gameserver
static PyObject* sendGameServerAgainObtainDataFromDBRequest(PyObject *pSelf, PyObject *pParams)
{
	// TODO 发请求给gameserver
	CNetGameServerMgr::GetGlobalInstance()->requestAgainObtainDataFromDB();

	//Sleep(10000);

	long result = CMainApp::GetGlobalInstance()->miReloadDataReslut;
	return PyInt_FromLong(result);
}

static PyObject* getGameServerRerturn(PyObject *pSelf, PyObject *pParams)
{
	long result = CMainApp::GetGlobalInstance()->miReloadDataReslut;
	return PyInt_FromLong(result);
}


//http server 接到小米充值请求后调用
static PyObject* xiaomiNotifyPlatformRecharge(PyObject *pSelf, PyObject *pParams)
{
	char *arg;

	// 获取python调用此函数时传进的参数  
	if (!PyArg_ParseTuple(pParams, "s", &arg))  
	{  
		return NULL;  
	}

	std::map<std::string, std::string> paramMap;

	std::string param;
	for (int i = 0; i < strlen(arg); ++i)
	{
		param += arg[i];
	}

	// 通知gameserver
	CNetGameServerMgr::GetGlobalInstance()->requestXiaomiPlatformRecharge(paramMap);

	Py_INCREF(Py_None);  

	return Py_None;  
}

//http server 接到阿里充值请求后调用
static PyObject* aliNotifyPlatformRecharge(PyObject *pSelf, PyObject *pParams)
{
	char *arg;

	// 获取python调用此函数时传进的参数  
	if (!PyArg_ParseTuple(pParams, "s", &arg))  
	{  
		return NULL;  
	}

	std::map<std::string, std::string> paramMap;

	std::string param;
	for (int i = 0; i < strlen(arg); ++i)
	{
		param += arg[i];
	}

	// 通知gameserver
	CNetGameServerMgr::GetGlobalInstance()->requestAliPlatformRecharge(paramMap);

	Py_INCREF(Py_None);  

	return Py_None;  
}


// 将需要导出的函数的名字、函数地址等信息保存到PyMethodDef类型的数组中
// 导出到python的函数名、函数指针、函数形式（常用两种：METH_NOARGS(没参数)，METH_VARARGS(有参数)）、函数的doc
// 最后一行的NULL时结束标记
static PyMethodDef Methods[]=
{
	{"send_game_server_gain_obtain_data_from_DB_request", sendGameServerAgainObtainDataFromDBRequest, METH_NOARGS, NULL},
	{"get_game_server_rerturn", getGameServerRerturn, METH_NOARGS, NULL},
	{"xiaomi_notify_platform_recharge", xiaomiNotifyPlatformRecharge, METH_VARARGS, NULL},
	{"ali_notify_platform_recharge", aliNotifyPlatformRecharge, METH_VARARGS, NULL},

	{NULL, NULL, 0, NULL} 
};



// 调用simplehttpserver.py脚本
DWORD WINAPI ThreadFunc(PVOID pVoid)
{
	Py_Initialize();

	if (!Py_IsInitialized())
	{
		std::cout << "Py_Initialize() failed" << std::endl;
		CLogMgr::GetGlobalInstance()->setLog("Py_IsInitialized() failed");
		system("pause");
		return 0;
	}

	if (!PyImport_AddModule("Sample"))
	{
		std::cout << "Host API module could not be created." << std::endl;
	}

	PyObject *module = Py_InitModule("Sample", Methods);
	if (!module)
	{
		std::cout << "Host Api functions could not be initialized." << std::endl;
		return 0;
	}


	int v1 = PyRun_SimpleString("import sys");
	//int v2 = PyRun_SimpleString("sys.path.append('./')");

	PyObject *pName = NULL;
	PyObject *pModule = NULL;
	PyObject *pDict = NULL;
	PyObject *pFunc1 = NULL;
	PyObject *pFunc2 = NULL;
	PyObject *pArgs1 = NULL;
	PyObject *pArgs2 = NULL;
	PyObject *pReturn1 = NULL;
	PyObject *pReturn2 = NULL;

	//载入脚本
	pName = PyString_FromString("simplehttpserver");
	pModule = PyImport_Import(pName);

	if (!pModule)
	{
		std::cout << "can not find simplehttpserver.py" << std::endl;
		CLogMgr::GetGlobalInstance()->setLog("can not find simplehttpserver.py");
		system("pause");
		return 0;
	}

	pDict = PyModule_GetDict(pModule);
	if (!pDict)
	{
		std::cout << "PyModule_GetDict failed" << std::endl;
		CLogMgr::GetGlobalInstance()->setLog("PyModule_GetDict failed");
		system("pause");
		return 0;
	}	


	if (pReturn1 != NULL)
	{
		Py_DECREF(pReturn1);
	}

	if (pName != NULL)
	{
		Py_DECREF(pName);
	}

	if (pArgs1 != NULL)
	{
		Py_DECREF(pArgs1);
	}

	if (pArgs2 != NULL)
	{
		Py_DECREF(pArgs2);
	}

	if (pModule != NULL)
	{
		Py_DECREF(pModule);
	}

	if (pFunc1 != NULL)
	{
		Py_DECREF(pFunc1);
	}

	if (pFunc2 != NULL)
	{
		Py_DECREF(pFunc2);
	}

	if (pReturn2 != NULL)
	{
		Py_DECREF(pReturn2);
	}

	//关闭python
	Py_Finalize();

	return 0;
}