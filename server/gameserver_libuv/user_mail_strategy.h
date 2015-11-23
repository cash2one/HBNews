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

	//删除一封邮件
	//isOutOfDate 表示是否是过期邮件 true:pClientRes传gs_c_mail_ntf_array参数 false:gs_c_mail_opt_res参数 初始化时两个参数都不用传
	virtual bool DelMail(UINT32 iMailId, ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mail >* pRepMail, ::google::protobuf::Message* pClientRes = NULL, bool isOutOfDate = false) = 0;
	
	//批量删除邮件
	virtual bool DelMails(std::vector<UINT32>& vecMailId, gs_dbs_user_info_op_req* pDBReq, gs_c_mail_opt_res* pClientRes) = 0;

	//设置邮件状态
	virtual bool SetMailReaded(UINT32 iMailId, gs_dbs_user_info_op_req* db_req, gs_c_mail_opt_res* pMailRes) = 0;
	virtual MAIL_STATUS GetMailStatus(UINT32 iMailId) = 0;
	//判断是否有附件
	virtual bool IsMailsHasAttachs(std::vector<UINT32>& vecMailId) = 0;
	
	//获取所有有附件的邮件id
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
	//删除一封邮件
	virtual bool DelMail(UINT32 iMailId, ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mail >* pRepMail, ::google::protobuf::Message* pClientRes = NULL, bool isOutOfDate = false);
	
	virtual bool HasMails(std::vector<UINT32>& vecMailId);
	
	virtual bool HasMail(UINT32 iMailID);
	//批量删除邮件
	virtual bool DelMails(std::vector<UINT32>& vecMailId, gs_dbs_user_info_op_req* pDBReq, gs_c_mail_opt_res* pClientRes);

	//设置邮件状态
	virtual bool SetMailReaded(UINT32 iMailId, gs_dbs_user_info_op_req* db_req, gs_c_mail_opt_res* pMailRes);
	
	virtual MAIL_STATUS GetMailStatus(UINT32 iMailId);
	//获取邮件副本信息
	db_mail_attach* GetMailAttachInfo(UINT32 iMailId);
	
	//判断邮件是否有附件
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