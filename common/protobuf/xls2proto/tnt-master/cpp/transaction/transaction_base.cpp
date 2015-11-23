/**
 * @file:   transaction_base.cpp
 * @author: jameyli <lgy AT live DOT com>
 * @brief:
 */

#include "transaction_base.h"
#include "transaction_mgr.h"
#include "logging.h"

TransactionBase::TransactionBase(unsigned int cmd)
:   id_(0),
    cmd_(cmd),
    uin_(0),
    state_(STATE_IDLE),
    phase_(0),
    curr_cmd_(0),
    timeout_timer_id_(0),
    app_frame_(NULL, 0)
{
}

TransactionBase::~TransactionBase()
{
}

// // ���������Լ���ʵ�������ػ���ָ�룬���Ƴ��󹤳�ģʽ
// virtual TransactionBase* CreateTransaction(unsigned int cmd) = 0;

// ���¹���
// ��Ϊ������һֱ���ڵģ������ٴ�ʹ����Ҫ��ʼ��
// ����������������Ҳ�п�����Ҫ��ʼ��, Ϊ�˱����������ظú�����
// ���ǵ��û���ķ��������Խ����������ĺ����ֿ�
void TransactionBase::ReConstructBase()
{
    FUNC_TRACE(uin_);

    static unsigned int trans_id_generator_ =
        TransactionMgrSigleton::get_const_instance().trans_id_begin_;


    ++trans_id_generator_;
    if (0 == trans_id_generator_)
    {
        ++trans_id_generator_;
    }

    id_ = trans_id_generator_;

    state_ = STATE_AWAKE;

    TNT_LOG_DEBUG(0, 0, "%u|%u", id_, trans_id_generator_);

    return;
}

// �ṩ������Ľӿ�
void TransactionBase::ReConstruct()
{
}

// ��װһ��
void TransactionBase::ReConstructAll()
{
    FUNC_TRACE(uin_);

    ReConstructBase();
    ReConstruct();
}

void TransactionBase::ReDestructBase()
{
    FUNC_TRACE(uin_);

    id_ = 0;
    uin_ = 0;
    phase_ = 0;
    curr_cmd_ = 0;
    timeout_timer_id_ = 0;
}

void TransactionBase::ReDestruct()
{
}

void TransactionBase::ReDestructAll()
{
    FUNC_TRACE(uin_);

    ReDestruct();
    ReDestructBase();
}

//XXX: �⼸���ӿڷ���ֵ С�� 0 �����˳�����
// �յ���һ����Ϣ
// ��һ����Ϣ�Ƚ����⣬���ڴ�������ʱ��һЩ��ʼ��
int TransactionBase::ProcessFirstFrame(const AppFrame& app_frame)
{
    FUNC_TRACE(uin_);

    app_frame_ = app_frame;
    memcpy(&bus_header_, app_frame.bus_header, sizeof(bus_header_));
    memcpy(&app_header_, app_frame.app_header, sizeof(app_header_));
//    bus_header_ = app_frame.bus_header;
//    app_header_ = app_frame.app_header;

    uin_ = app_header_.uiUin;

    return OnEvent();
}

// �յ�������Ϣ
int TransactionBase::ProcessOtherFrame(const AppFrame& app_frame)
{
    FUNC_TRACE(uin_);

    // ����Ƿ���������CMD
    // Ϊ�˺�֮ǰ�ļ��ݣ��������û������curr_cmd_��������һ��
    if (GetCurrCmd() == 0)
    {
        SetCurrCmd(app_frame.app_header->ushCmdID);
    }
    else if (GetCurrCmd() != app_frame.app_header->ushCmdID)
    {
        TNT_LOG_WARN(0, uin_, "not waiting cmd|0X%08X|0X%08X",
                     GetCurrCmd(), app_frame.app_header->ushCmdID);

        return -1;
    }

    app_frame_ = app_frame;
    memcpy(&bus_header_, app_frame.bus_header, sizeof(bus_header_));
    memcpy(&app_header_, app_frame.app_header, sizeof(app_header_));

    return OnEvent();
}

int TransactionBase::SetTimeoutTimer(TransactionWaitInterval interval_usec)
{
    FUNC_TRACE(uin_);

    SetCurrCmd(0);

    CancelTimeoutTimer();

    int ret = TransactionMgrSigleton::get_mutable_instance().SetTimer(id_, interval_usec, timeout_timer_id_);
    if (0 != ret)
    {
        TNT_LOG_ERROR(0, uin_, "set timer error|%u", id_);
        return -1;
    }

    return 0;
}

int TransactionBase::CancelTimeoutTimer()
{
    FUNC_TRACE(uin_);

    if (0 == timeout_timer_id_)
    {
        return 0;
    }

    int ret = TransactionMgrSigleton::get_mutable_instance().CancelTimer(timeout_timer_id_);
    if (0 != ret)
    {
        TNT_LOG_ERROR(0, uin_, "cancel timer error|%u|%lu", id_, timeout_timer_id_);
        return -1;
    }

    timeout_timer_id_ = 0;

    return 0;
}

int TransactionBase::ProcessTimeout(size_t timer_id)
{
    FUNC_TRACE(uin_);

    if (timeout_timer_id_ == timer_id)
    {
        timeout_timer_id_ = 0;

        state_ = STATE_TIMEOUT;
    }

    return OnEvent();
}

/**
 * @brief:  ������һ���׶�
 *
 * @param  phase �׶�ID
 * @param  interval_usec ��ʱʱ��
 * @param  waiting_cmd ����Ľ׶�Ҫ�ȴ���CMD
 */
void TransactionBase::EnterPhase(unsigned int phase,
                                 TransactionWaitInterval interval_usec,
                                 unsigned int waiting_cmd)
{
    FUNC_TRACE(uin_);

    SetPhase(phase);
    SetTimeoutTimer(interval_usec);

    SetCurrCmd(waiting_cmd);
}


/**
 * @brief:  ״̬������
 *
 * @return: < 0 �˳�����
 */
int TransactionBase::OnEvent()
{
    FUNC_TRACE(uin_);

    // ȡ����ʱ��
    CancelTimeoutTimer();

    int ret = 0;
    TransactionReturn trans_ret = RETURN_EXIT;

    // 2012-06-15
    // ����������Ӹ��쳣��׽����Ȼ�������ڴ�����ʹ���쳣
    // ����ȷʵû���뵽���쳣�����ʵİ취
    // ��Ϊ������δ֪�ģ������ʵ��Ҳ��δ֪��
    // �޷���֤ÿһ��ʵ�ֶ����Ͻ���
    //
    // ���������쳣ʱ��Ӧ���ó�������ػ��ǹ����˳�?
    //
    try
    {
        switch (state_)
        {
            case STATE_AWAKE:
                {
                    trans_ret = OnAwake();
                    break;
                }
            case STATE_ACTIVE:
                {
                    trans_ret = OnActive();
                    break;
                }
            case STATE_TIMEOUT:
                {
                    trans_ret = OnTimeout();
                    break;
                }
            default:
                {
                    TNT_LOG_ERROR(0, uin_, "error state %u", state_);
                }
        }
    }
    catch (std::exception& e)
    {
        TNT_LOG_ERROR(0, uin_, "exception|0X%08X|%u|%u|%u|%s",
                      cmd_, id_, state_, phase_, e.what());

        // ����������˳��ɡ�����
        trans_ret = RETURN_EXIT;
    }

    switch (trans_ret)
    {
        case RETURN_WAIT:
            {
                if (0 == timeout_timer_id_)
                {
                    //2013-08-19, jamey
                    //����ȴ�ȴ����������ʱ��, ���������˳���
                    state_ = STATE_IDLE;

                    TNT_LOG_DEBUG(0, uin(), "no timer trans exit|%u|%u", id(), cmd());
                    TransactionMgrSigleton::get_mutable_instance().FreeTransaction(this);
                }
                else
                {
                    state_ = STATE_ACTIVE;
                }
                break;
            }
        case RETURN_CONTINUE:
            {
                state_ = STATE_ACTIVE;

                return OnEvent();
            }
        case RETURN_EXIT:
            {
                state_ = STATE_IDLE;

                TNT_LOG_DEBUG(0, uin(), "trans exit|%u|%u", id(), cmd());
                TransactionMgrSigleton::get_mutable_instance().FreeTransaction(this);
//                ret = -1;

                break;
            }
    }

    return ret;
}


