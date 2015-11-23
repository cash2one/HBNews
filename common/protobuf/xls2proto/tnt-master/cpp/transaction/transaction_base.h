/**
 * @file:   transaction_base.h
 * @author: jameyli <lgy AT live DOT com>
 * @brief:  ����(��Ự, �����Ựһ����ָһ�����ӣ���������ʵ��һ������)�Ļ���
 */

#ifndef TRANSACTION_BASE_H
#define TRANSACTION_BASE_H

#include "app_frame.h"

/**
 * @brief:  ������
 * ������������������, û�й�����ӿ�
 */
class TransactionBase
{
    friend class TransactionMgr;

protected:
    /**
     * @brief:  �����״̬
     */
    enum TransactionState
    {
        STATE_IDLE = 0,     /// ����
        STATE_AWAKE = 1,    /// ����
        STATE_ACTIVE = 2,   /// �
        STATE_TIMEOUT = 3,  /// ��ʱ
    };

    enum TransactionReturn
    {
        RETURN_WAIT = 1,        /// �ȴ�, �����첽�ȴ���һ����������Ϣ�ķ��ػ�ʱ
        RETURN_CONTINUE  = 2,   /// �������ٴε���
        RETURN_EXIT  = 3,       /// �˳�����ʱ��Ҫ��������
    };

    enum TransactionWaitInterval
    {
        WAIT_ONE_SECONDS = 1000,
        WAIT_TWO_SECONDS = 2000,
        WAIT_THREE_SECONDS = 3000,
        WAIT_FOUR_SECONDS = 4000,
        WAIT_FIVE_SECONDS = 5000,

        WAIT_TEN_SECONDS = 10000,

        WAIT_FIFTEEN_SECONDS = 15000,
    };

protected:
    TransactionBase(unsigned int cmd);
    virtual ~TransactionBase() = 0;

protected:
    // // ���������Լ���ʵ�������ػ���ָ�룬���Ƴ��󹤳�ģʽ
    // virtual TransactionBase* CreateTransaction(unsigned int cmd) = 0;

    // ���¹���
    // ��Ϊ������һֱ���ڵģ������ٴ�ʹ����Ҫ��ʼ��
    // ����������������Ҳ�п�����Ҫ��ʼ��, Ϊ�˱����������ظú�����
    // ���ǵ��û���ķ��������Խ����������ĺ����ֿ�
    void ReConstructBase();

    // �ṩ������Ľӿ�
    virtual void ReConstruct();

    // ��װһ��
    void ReConstructAll();

    void ReDestructBase();

    virtual void ReDestruct();

    void ReDestructAll();

    //XXX: �⼸���ӿڷ���ֵ С�� 0 �����˳�����
    // �յ���һ����Ϣ
    // ��һ����Ϣ�Ƚ����⣬���ڴ�������ʱ��һЩ��ʼ��
    int ProcessFirstFrame(const AppFrame& app_frame);

    // �յ�������Ϣ
    int ProcessOtherFrame(const AppFrame& app_frame);

    int SetTimeoutTimer(TransactionWaitInterval interval_usec);
    int CancelTimeoutTimer();
    int ProcessTimeout(size_t timer_id);

    /**
     * @brief:  ������һ���׶�
     *
     * @param  phase �׶�ID
     * @param  interval_usec ��ʱʱ��
     * @param  waiting_cmd ����Ľ׶�Ҫ�ȴ���CMD
     */
    void EnterPhase(unsigned int phase,
                    TransactionWaitInterval interval_usec,
                    unsigned int waiting_cmd = 0);


protected:
    /**
     * @brief:  ״̬������
     *
     * @return: < 0 �˳�����
     */
    int OnEvent();

    // ��Ҫ����ʵ�ֵļ����ӿ�
    // ��һ�α�����
    virtual TransactionReturn OnAwake() = 0;

    // ����״̬�ٴε���
    virtual TransactionReturn OnActive(){return RETURN_EXIT;}

    // ��ʱ������
    virtual TransactionReturn OnTimeout(){return RETURN_EXIT;}

public:
    inline unsigned int id() const
    {
        return id_;
    }

    inline unsigned int uin() const
    {
        return uin_;
    }

    inline void set_uin(unsigned uin)
    {
        uin_ = uin;
    }
    /**
     * @brief:  ��ȡ����ע���CMD
     */
    inline unsigned int cmd() const
    {
        return cmd_;
    }

    inline const TransactionState& state() const
    {
        return state_;
    }

    inline unsigned int phase() const
    {
        return phase_;
    }

    inline void SetPhase(unsigned int phase)
    {
        phase_ = phase;
        curr_cmd_ = 0;
    }


    /**
     * @brief:  ��ǰ����
     */
    inline unsigned int GetCurrCmd() const
    {
        return curr_cmd_;
    }

    inline void SetCurrCmd(unsigned int curr_cmd)
    {
        curr_cmd_ = curr_cmd;
    }



    inline const AppFrame& GetAppFrame() const
    {
        return app_frame_;
    }

    inline const BusHeader& GetBusHeader() const
    {
        return bus_header_;
    }

    inline const AppHeader& GetAppHeader() const
    {
        return app_header_;
    }

    virtual void Dump() const
    {
        // TNT_LOG_DEBUG(0, uin_, "TransactionBase|%u|%u|%u", id_, cmd_, state_);
    }

private:
    // ����ID
    unsigned int id_;
    // ����������
    unsigned int cmd_;
    // ������uin
    unsigned int uin_;
    // ״̬
    TransactionState state_;
    // �׶�, ���ڿ�����һ����Ϊ
    unsigned int phase_;
    // ��ǰ��CMD
    // ����ǵȴ�״̬�����ǵȴ���CMD
    // ����Ѿ����룬���ǵ�ǰ����ִ�е�CMD
    unsigned int curr_cmd_;

    // ��ʱ��ʱ��
    size_t timeout_timer_id_;

    AppFrame app_frame_;

    // snslib::BusHeader bus_header_;
    // snslib::AppHeader app_header_;
};

#endif //TNT_TRANSACTION_BASE_H

