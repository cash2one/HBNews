#ifndef user_mgr_h__
#define user_mgr_h__

#include <vector>

class CClientForAS;
class CClientForGS;

class CUserMgr
{
public:

	static std::vector<CClientForAS*>& GetAllASUsers(){
		return vAllusers_as;
	}
	static std::vector<CClientForGS*>& GetAllGSUsers(){
		return vAllusers_gs;
	}

private:
	static std::vector<CClientForAS*> vAllusers_as;
	static std::vector<CClientForGS*> vAllusers_gs;
};

#endif // user_mgr_h__