/**
 * @file:   transaction_mgr.h
 * @author: jameyli <lgy AT live DOT com>
 * @brief:  ���������
 *
 * ����Transaction, ������������:һ��������Ҫ�ಽ���첽����ʱ��
 * ��Ҫ����������
 *
 * û��ʹ�ûỰSession������, ��һ��session�����ʾ�����ӣ�����һ��
 * session���ܻ��ж������.
 *
 * ���ڲ�ͬ�����ֵĴ����߼���һ������Ҫ���������Ҳ��һ��������
 * һ�㲻ͬ�������������ǲ�ͬ������������, ������������ʵ������
 * �����ǵȼ۵�
 *
 * ��������Ҫע��Ż���������д���
 *
 * һ��������ռһ������ʵ��.
 *
 * һ��������ע�����ڷ�������ʼ��ʱΪ�����һ������Ͱ��Ͱ�Ĵ�С��ͳһ�ģ���һ
 * ���������壬��ʱ��֧�ֶ�̬��������ˣ��������һ��Ǳ�ڵ����⣬Ƶ�ʽϸߵ�����
 * (����)���ܻ���Ϊû�п��е�������޷�����.�����������м�������:
 *      1 ���ĺ�ֵͳһ���Ͱ�Ĵ�С, ���ܻ������Դ�˷�
 *      2 ���������ж�̬�ı�Ͱ�Ĵ�С, ���ܻᵼ����Դ���䲻�ɿ�
 *      3 ����������Ƶ�ʷ��࣬���ò�ͬ������
 *      4 ���
 *2012-03-01 �Ż�һ��Ͱ�Ĵ�С
 *������ͬ�����첽��
 *ͬ��������ֻ��Ҫһ���͹���
 *
 * XXX:�Ƿ���Ҫ�־û����ܹ�������Ȼ���ã�����ʵ��ʱȴ�������
 * ϸ����Ҫ���ǣ�Ȩ���ȷ����һ���汾�Ȳ����ǳ־û�
 *
 * TODO: ʲôʱ�����ʹ���������?
 * 1 ͨ������ӿ��ڲ�����״̬
 * �����������Ҫ�ں����˳�ʱ����
 * 2 ͨ������ӿڵķ���ֵ
 * ����ֻ��Ҫ����ͳһ�ķ���ֵ
 * �ƺ�2 ����һЩ
 *
 * TODO:
 * 1 ��ʱ��, ����϶��趨ʱ�߼�
 *   ����Ķ�ʱ�������������������ά��
 * 2 ͳһ����Ϣ���ͽӿ�
 *
 */

#ifndef TRANSACTION_MGR_H
#define TRANSACTION_MGR_H

#include <vector>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include "boost/serialization/singleton.hpp"
#include "comm/timer_pool/timer_pool.h"
#include "tnt_transaction_base.h"

enum TransctionMode
{
    TRANSCTION_MODE_SYN = 0,
    TRANSCTION_MODE_ASY = 1,

    TRANSCTION_MODE_COUNT
};

/**
 * @brief: ����Ͱ
 * ��ǰ���е�ͳһ��������
 * ������ע����Ϊ�����һ��Ψһ��Ͱ
 *
 */
class TransctionBucket
{
public:
    TransctionBucket(unsigned int cmd)
        : cmd_(cmd)
    {
        trans_list_.clear();
    }

    ~TransctionBucket()
    {
    }

public:
    unsigned int cmd() const
    {
        return cmd_;
    }

    int push(TransactionBase* ptrans)
    {
        FUNC_TRACE(0);

        trans_list_.push_back(ptrans);
        dump();

        return 0;
    }

    TransactionBase* pop()
    {
        FUNC_TRACE(0);

        dump();

        if (trans_list_.size() <= 0)
        {
            LOG_WARN(0, 0, "there is no free transaction|%u", cmd_);
            return NULL;
        }

        TransactionBase* ptrans = trans_list_.back();
        trans_list_.pop_back();

        return ptrans;
    }

    size_t size() const
    {
        return trans_list_.size();
    }

    void dump() const
    {
        LOG_DEBUG(0, 0, "TransctionBucket|%u|%lu", cmd_, trans_list_.size());
    }

private:
    std::vector<TransactionBase*> trans_list_;

private:
    unsigned int cmd_;
};

typedef struct tagTransactionTimer
{
    unsigned int trans_id;
}TransactionTimer;

// һ���û�ͬʱֻ����һ�������ض����������
class TransactionLocker
{
public:
    TransactionLocker(unsigned int uin, unsigned cmd)
    : uin_(uin), cmd_(uin)
    {
    }
    ~TransactionLocker(){}

    unsigned int uin_;
    unsigned int cmd_;
};

class HashOfTransactionLocker
{
public:
    size_t operator() (const TransactionLocker& locker) const
    {
        return (size_t(locker.uin_) +  locker.cmd_);
    }
};

class EqualOfTransactionLocker
{
public:
    size_t operator() (const TransactionLocker& rhs, const TransactionLocker& lhs) const
    {
        return ((rhs.uin_ == lhs.uin_) && (rhs.cmd_ == lhs.cmd_));
    }
};



/**
 * @brief:  ���������
 * ���������Դ��������Ͱ, ����ڴ�й©�����������ԭ��
 */
class TransactionMgr
{
    friend class TransactionBase;

    static const unsigned int MAX_SYN_TRANSANCTION_NUM_PER_CMD = 1;
    static const unsigned int MAX_ASY_TRANSANCTION_NUM_PER_CMD = 1024;

protected:
    TransactionMgr();
    ~TransactionMgr();

public:
    int InitTransactionMgr();

    /**
     * @brief: ע������
     *
     * ��ʼ��ʱ���ã����漰�ڴ����
     * �տ�ʼ���ü̳еĶ�̬��ʵ��
     * int RegisterCommand(unsigned int cmd, TransactionBase& trans)
     * ��������ģ��Ҳ��ʵ�֣���������Ҳ����Ҫʵ�ֳ��󹤳�
     *
     * @tparam ConcreteTransactionType ��������ľ���������
     * @param  cmd ��Ҫע�������
     *
     * @return: 0 �ɹ��� ��0 ʧ��
     */
    template<typename ConcreteTransactionType>
    int RegisterCommand(unsigned int cmd, TransctionMode tm = TRANSCTION_MODE_ASY);


    /**
     * @brief:  ��������Ƿ��Ѿ�ע��
     *
     * @param  cmd ������
     *
     * @return: �����
     */
    bool CheckCmdIsRegistered(unsigned int cmd) const;

    // ��Ҫ�ӿ�
    /**
     * @brief: ����������Ϣ
     *
     * @param  app_frame ������Ϣ֡
     *
     * @return: ������
     *          0   �ɹ�
     *          ��0 ʧ��
     */
    int ProcessAppFrame(const AppFrame& app_frame);

    /**
     * @brief: ��鳬ʱ, ����ѭ����ʱ���
     */
    void HandleTimeout();

    /**
     * @brief: ͳ����Ϣ
     */
    void CheckStatistic();

    inline void SetUseLocker()
    {
        is_use_locker_ = true;
    }

private:
    // ��ʱ���ӿ�
    int SetTimer(unsigned int trans_id, time_t timeout_usec, size_t& timer_id);
    int CancelTimer(size_t timer_id);

    // ���һ���µ�����ʵ��
    TransactionBase* GetNewTransaction(unsigned int uin, unsigned int cmd);
    // ȡ�����е�����
    TransactionBase* GetTransaction(unsigned int trans_id);

    // �ͷ�����ʵ��
    int FreeTransaction(TransactionBase* ptrans);

    /**
     * @brief:���һ��������ֹͬһ���û���ͬ����ͬͬʱ���ڶ����������
     */
    int LockUinTrans(unsigned int uin, unsigned int cmd);
    void UnLockUinTrans(unsigned int uin, unsigned int cmd);

public:
    // ����ID��ʼֵ
    unsigned int trans_id_begin_;

private:
    // ��ǰ����������б���map�������id����
    typedef std::tr1::unordered_map<unsigned int /*trans_id*/, TransactionBase*> TransactionMap;
    typedef TransactionMap::value_type TransactionMapValueType;
    typedef TransactionMap::iterator TransactionMapIter;
    typedef TransactionMap::const_iterator TransactionMapConstIter;
    typedef std::pair<TransactionMapIter, bool> TransactionMapInserRet;

    TransactionMap active_transaction_map_;

    // Ӧ�û��и����г�, ���������ֲ���
    typedef std::tr1::unordered_map<unsigned int /*cmd*/, TransctionBucket*> TransctionBucketMap;
    typedef TransctionBucketMap::value_type TransactionBucketMapValueType;
    typedef TransctionBucketMap::iterator TransactionBucketMapIter;
    typedef TransctionBucketMap::const_iterator TransactionBucketMapConstIter;
    typedef std::pair<TransactionBucketMapIter, bool> TransactionBucketMapInserRet;

    TransctionBucketMap idle_transaction_map_;

    // ��ʱ��
    snslib::CTimerPool<TransactionTimer> timer_pool_;

    // ������
    typedef std::tr1::unordered_set<TransactionLocker, HashOfTransactionLocker, EqualOfTransactionLocker> TransactionLockerPool;
    typedef TransactionLockerPool::value_type TransactionLockerPoolValueType;
    typedef TransactionLockerPool::iterator TransactionLockerPoolIter;
    typedef TransactionLockerPool::const_iterator TransactionLockerPoolConstIter;
    typedef std::pair<TransactionLockerPoolIter, bool> TransactionLockerPoolInserRet;


    bool is_use_locker_;
    TransactionLockerPool locker_pool_;
};

typedef boost::serialization::singleton<TransactionMgr> TransactionMgrSigleton;



/**
 * @brief: ע������
 *
 * ��ʼ��ʱ���ã����漰�ڴ����
 * �տ�ʼ���ü̳еĶ�̬��ʵ��
 * int RegisterCommand(unsigned int cmd, TransactionBase& trans)
 * ��������ģ��Ҳ��ʵ�֣���������Ҳ����Ҫʵ�ֳ��󹤳�
 *
 * @tparam ConcreteTransactionType ��������ľ���������
 * @param  cmd ��Ҫע�������
 *
 * @return: 0 �ɹ��� ��0 ʧ��
 */
template<typename ConcreteTransactionType> int
TransactionMgr::RegisterCommand(unsigned int cmd, TransctionMode tm)
{
    FUNC_TRACE(0);

    LOG_DEBUG(0, 0, "cmd = 0X%08X", cmd);

    TransactionBucketMapIter iter = idle_transaction_map_.find(cmd);
    if (iter != idle_transaction_map_.end())
    {
        LOG_ERROR(0, 0, "Cmd Register again|0X%08X", cmd);
        return -1;
    }

    // ����һ���µ�Ͱ
    unsigned int trans_num = (tm == TRANSCTION_MODE_SYN) ?
        MAX_SYN_TRANSANCTION_NUM_PER_CMD : MAX_ASY_TRANSANCTION_NUM_PER_CMD;

    TransctionBucket* trans_bucket = new TransctionBucket(cmd);
    for (unsigned int i=0; i<trans_num; ++i)
    {
        TransactionBase* ptrans = new ConcreteTransactionType(cmd);
        if (NULL == ptrans)
        {
            TNT_LOG_ERROR(0, 0, "CreateTransaction failed|0X%08X", cmd);
            return -2;
        }

        trans_bucket->push(ptrans);
    }

    idle_transaction_map_[cmd] = trans_bucket;

    return 0;
}

#endif //TNT_TRANSACTION_MGR_H
