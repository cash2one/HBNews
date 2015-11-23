#ifndef user_mail_strategy_h__
#define user_mail_strategy_h__
#include "dbs_gs_proto.pb.h"
#include "def.h"
#include "config.h"
class CUser;
class CValue;
class IUserMailStrategy{
public:

	virtual ~IUserMailStrategy() {}

	virtual bool Init(CUser* pUser, db_user_mail_array* pReceMailArr, gs_dbs_user_info_op_req& db_req) = 0;
	
	virtual bool CreateMail(MAIL_TYPE eMailType, gs_dbs_user_info_op_req& db_req,
		const char* strParams = NULL, std::map<std::string, CValue>* mapParams = NULL, std::vector<stGameElement*>* pvecstGameEle = NULL) = 0;
	
	virtual bool CreateMail(MAIL_TYPE eMailType, gs_dbs_user_info_op_req& db_req, 
		const char* strParams = NULL, std::map<std::string, CValue>* mapParams = NULL, std::vector<stGameElement>* vecstGameEle = NULL) = 0;

	virtual void AddMail(db_user_mail& user_mail, gs_dbs_user_info_op_req& req, GS_C_NTF& ntf) = 0;
	virtual bool HasMails(std::vector<UINT32>& vecMailId) = 0;

	virtual void DelOutOfDateMail(::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mail >* pRepMail, gs_c_mail_ntf_array* pMailNtf, std::vector<struct_game_element>& vecEle) = 0; 

	//ɾ��һ���ʼ�
	//isOutOfDate ��ʾ�Ƿ��ǹ����ʼ� true:pClientRes��gs_c_mail_ntf_array���� false:gs_c_mail_opt_res���� ��ʼ��ʱ�������������ô�
	virtual bool DelMail(UINT32 iMailId, ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mail >* pRepMail, ::google::protobuf::Message* pClientRes = NULL, bool isOutOfDate = false) = 0;
	
	//����ɾ���ʼ�
	virtual bool DelMails(std::vector<UINT32>& vecMailId, gs_dbs_user_info_op_req* pDBReq, gs_c_mail_opt_res* pClientRes) = 0;

	//�����ʼ�״̬
	virtual bool SetMailReaded(UINT32 iMailId, gs_dbs_user_info_op_req* db_req, gs_c_mail_opt_res* pMailRes) = 0;
	virtual MAIL_STATUS GetMailStatus(UINT32 iMailId) = 0;
	//�ж��Ƿ��и���
	virtual bool IsMailsHasAttachs(std::vector<UINT32>& vecMailId) = 0;
	
	//��ȡ�����и������ʼ�id
	virtual std::vector<UINT32> GetAllHasAttachMailIDS() = 0;

	virtual bool GetMailAttach(UINT32 iMailID, gs_c_mail_opt_res* pRes, gs_dbs_user_info_op_req* db_req, GS_C_NTF& client_arr) = 0;

	virtual bool GetExistMailWithAttach(gs_dbs_user_info_op_req::struct_new_mail* new_mail, db_user_mail*& pDBMail) = 0;
	virtual bool AddAttachTo(const std::vector<struct_game_element*>& pvecGameEle, db_user_mail* pDBMail) = 0;
	virtual bool AddAttachTo(const std::vector<struct_game_element*>& pvecGameEle, user_mail* pUserMail) = 0;
	virtual bool AddAttachTo(const struct_game_element& gameEle, db_user_mail* pDBMail) = 0;
	virtual bool AddAttachTo(const struct_game_element& gameEle, user_mail* pUserMail) = 0;

};

class CUserMailStrategy : public IUserMailStrategy{
public:
	CUserMailStrategy():m_DBMails(NULL), m_pUser(NULL){}

	virtual bool Init(CUser* pUser, db_user_mail_array* pReceMailArr, gs_dbs_user_info_op_req& db_req);

	bool CreateMail(MAIL_TYPE eMailType, gs_dbs_user_info_op_req& db_req, 
		const char* strParams = NULL, std::map<std::string, CValue>* mapParams = NULL, std::vector<stGameElement*>* pvecstGameEle = NULL);
	
	bool CreateMail(MAIL_TYPE eMailType, gs_dbs_user_info_op_req& db_req, 
		const char* strParams = NULL, std::map<std::string, CValue>* mapParams = NULL, std::vector<stGameElement>* vecstGameEle = NULL);
	
	virtual void DelOutOfDateMail(::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mail >* pRepMail, gs_c_mail_ntf_array* pMailNtf, std::vector<struct_game_element>& vecEle); 
	//ɾ��һ���ʼ�
	virtual bool DelMail(UINT32 iMailId, ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mail >* pRepMail, ::google::protobuf::Message* pClientRes = NULL, bool isOutOfDate = false);
	
	virtual bool HasMails(std::vector<UINT32>& vecMailId);
	
	virtual bool HasMail(UINT32 iMailID);
	//����ɾ���ʼ�
	virtual bool DelMails(std::vector<UINT32>& vecMailId, gs_dbs_user_info_op_req* pDBReq, gs_c_mail_opt_res* pClientRes);

	//�����ʼ�״̬
	virtual bool SetMailReaded(UINT32 iMailId, gs_dbs_user_info_op_req* db_req, gs_c_mail_opt_res* pMailRes);
	
	virtual MAIL_STATUS GetMailStatus(UINT32 iMailId);
	//��ȡ�ʼ�������Ϣ
	db_mail_attach* GetMailAttachInfo(UINT32 iMailId);
	
	//�ж��ʼ��Ƿ��и���
	virtual bool IsMailsHasAttachs(std::vector<UINT32>& vecMailId);
	
	virtual bool GetMailAttach(UINT32 iMailID, gs_c_mail_opt_res* pRes, gs_dbs_user_info_op_req* db_req, GS_C_NTF& client_arr);

	virtual void AddMail(db_user_mail& user_mail, gs_dbs_user_info_op_req& req, GS_C_NTF& ntf);

	virtual std::vector<UINT32> GetAllHasAttachMailIDS();

	virtual bool GetExistMailWithAttach(gs_dbs_user_info_op_req::struct_new_mail* new_mail, db_user_mail*& pDBMail);

	virtual bool AddAttachTo(const std::vector<struct_game_element*>& pvecGameEle, db_user_mail* pDBMail);
	virtual bool AddAttachTo(const std::vector<struct_game_element*>& pvecGameEle, user_mail* pUserMail);
	virtual bool AddAttachTo(const struct_game_element& gameEle, db_user_mail* pDBMail);
	virtual bool AddAttachTo(const struct_game_element& gameEle, user_mail* pUserMail);

	void DelOldestMail(gs_dbs_user_info_op_req& req, GS_C_NTF& ntf);
	
private:
	std::vector<struct_game_element*> _get_mail_attach(UINT32 iMailID);
	db_user_mail_array* m_DBMails;
	CUser* m_pUser;
	std::map<UINT32, db_user_mail*> m_mapMails;//first:mailid, second:mail
	typedef std::map<UINT32, db_user_mail*>::iterator ITER_MAIL;
};

#endif