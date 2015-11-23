#ifndef client_config_mgr_h__
#define client_config_mgr_h__

#pragma once

#include "gs_c_proto.pb.h"
#include "def.h"

struct uv_stream_s;

class CClientConfigMgr{
public:
	static CClientConfigMgr* GetInstance(){
		static CClientConfigMgr instance;
		return &instance;
	}

	bool Init();

	gs_c_config_res * get_gs_c_config_res(){
		ASSERT(m_hasInited);
		return &m_config;
	}

	//bool get_gs_c_config_res(const c_gs_config_req &req, gs_c_config_res &res){
	//	ASSERT(m_hasInited);
	//	if (req.names_size() == 0){
	//		res.CopyFrom(m_config);
	//	}
	//	else{//��ѡ��ĸ�
	//		res.a
	//	}
	//}

	UINT32 get_gs_c_config_bin(BYTE* &ret){
		ASSERT(m_hasInited);
		if (m_config_bin){
			ret = m_config_bin;
			return m_config_bin_len;
		}
		ret = NULL;
		return 0;
	}

	//gs_c_json_config_version_res* get_gs_c_json_config_version_res(){
	//	ASSERT(m_hasInited);
	//	CHECK_FALSE_RETURN_FALSE(m_hasInited);
	//	return &m_json_config_version;
	//}

	UINT32 get_gs_c_json_config_version_bin(BYTE* &ret){
		ASSERT(m_hasInited);
		if (m_json_config_version_bin){
			ret = m_json_config_version_bin;
			return m_json_config_version_bin_len;
		}

		ret = NULL;
		return 0;
	}

	bool handle_json_config_req(uv_stream_s* stream, const c_gs_json_config_req &req);

private:
	
	CClientConfigMgr():m_hasInited(false),m_config_bin(NULL){}
	bool m_hasInited;

//>>>  ��pb�ṹ�ͱ���ʽ ���ͻ���������Ϣ�Ͱ汾����ʱ
	gs_c_config_res m_config;
	BYTE* m_config_bin;
	UINT32 m_config_bin_len;

	gs_c_json_config_version_res m_config_version;
	BYTE* m_config_version_bin;
	UINT32 m_config_version_bin_len;
//<<<

//>>> ��json�ļ� ���ͻ���������Ϣ�Ͱ汾����ʱ 

	//�����ͻ��������б� ����md5�� ���ҹ�����ͻ��˵�md5�����Ӧ
	bool _parseMd5();
	//���������ļ�����
	bool _parseJsonFile();

	class CClientConfig{
		std::string file_name;//utf8��ʽ��
		std::string md5;//utf8��ʽ��
		std::string content;//utf8��ʽ��

		friend class CClientConfigMgr;
	};

	std::map<std::string, CClientConfig*> m_mapConfigs;//utf8��ʽ��key:filename->cfg

	//������json��pb����
	//gs_c_json_config_res m_json_config;
	BYTE* m_json_config_bin;
	UINT32 m_json_config_bin_len;

	//�����İ汾��Ϣpb����
	//gs_c_json_config_version_res m_json_config_version;
	BYTE* m_json_config_version_bin;
	UINT32 m_json_config_version_bin_len;

	//<<<
};

#endif // client_config_mgr_h__

