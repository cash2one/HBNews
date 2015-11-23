/*
Navicat MySQL Data Transfer

Source Server         : wh
Source Server Version : 50624
Source Host           : 127.0.0.1:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50624
File Encoding         : 65001

Date: 2015-10-28 11:13:38
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `sys_setting`
-- ----------------------------
DROP TABLE IF EXISTS `sys_setting`;
CREATE TABLE `sys_setting` (
  `key` varchar(255) NOT NULL COMMENT '键，配置项名字,同一功能模块的配置值用共同的前缀。  （注意， 只允许包含 英文 下划线或者数字 但是不要以数字开头）',
  `value_type` tinyint(3) unsigned NOT NULL COMMENT '值的类型：1.整数 2.字符串 3.浮点数',
  `value` varchar(511) NOT NULL COMMENT 'key所对应的值',
  `desc` varchar(511) DEFAULT '' COMMENT '参数功能描述'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_setting
-- ----------------------------
INSERT INTO sys_setting VALUES ('Buy_Coin_Default', '1', '20000', '钻石兑换金币初始值');
INSERT INTO sys_setting VALUES ('Buy_Skill_Point', '1', '10', '技能点购买一次获得的技能点点数');
INSERT INTO sys_setting VALUES ('Buy_Health_Point', '1', '120', '一次购买体力获得点数');
INSERT INTO sys_setting VALUES ('Buy_Coin_Grow', '1', '1000', '钻石兑换金币成长值');
INSERT INTO sys_setting VALUES ('Invoke_Gold_1', '1', '10000', '金币单抽价格');
INSERT INTO sys_setting VALUES ('Invoke_Gold_N', '1', '90000', '金币十连价格');
INSERT INTO sys_setting VALUES ('Invoke_Diamond_1', '1', '280', '钻石单抽价格');
INSERT INTO sys_setting VALUES ('Invoke_Diamond_N', '1', '2520', '钻石十连价格');
INSERT INTO sys_setting VALUES ('Invoke_Gold_Free_CD', '1', '300', '金币免费单抽cd时间 秒为单位');
INSERT INTO sys_setting VALUES ('Invoke_Gold_Free_Per_Day', '1', '5', '每日金币免费单抽次数');
INSERT INTO sys_setting VALUES ('Invoke_Diamond_Free_CD', '1', '79200', '钻石免费单抽cd时间 秒为单位');
INSERT INTO sys_setting VALUES ('Invoke_Diamond_Free_Max_num', '1', '1', '钻石免费单抽最大剩余次数');
INSERT INTO sys_setting VALUES ('Invoke_Vip_Diamond_1', '1', '420', 'VIP单抽价格');
INSERT INTO sys_setting VALUES ('Invoke_Vip_Diamond_N', '1', '3780', 'VIP十连价格');
INSERT INTO sys_setting VALUES ('Initial_Player_Belongings', '2', '8-901-1|5-4-1|12-0-50000', '初始玩家财产');
INSERT INTO sys_setting VALUES ('Initial_Player_Vip_Level', '1', '5', '初始VIP等级');
INSERT INTO sys_setting VALUES ('Initial_Player_Level', '1', '1', '初始玩家等级');
INSERT INTO sys_setting VALUES ('Vip_Level_Function_Invoke_Vip', '1', '13', 'VIP宝箱开启VIP等级');
INSERT INTO sys_setting VALUES ('Vip_Level_Function_Speed_Kill_Main_Task', '1', '1', '主线任务可扫荡VIP等级');
INSERT INTO sys_setting VALUES ('Vip_Level_Function_Speed_Kill_Free_Task', '1', '1', '自由任务可扫荡VIP等级');
INSERT INTO sys_setting VALUES ('Vip_Level_Function_Buy_Gold', '1', '1', '点金功能开启VIP等级');
INSERT INTO sys_setting VALUES ('Vip_Level_Function_Buy_Health', '1', '1', '允许玩家购买体力VIP等级');
INSERT INTO sys_setting VALUES ('Vip_Level_Function_Upgrade_All_Equip', '1', '3', '允许玩家装备全部升级VIP等级');
INSERT INTO sys_setting VALUES ('Vip_Level_Function_Buy_Skill_Point', '1', '1', '允许购买技能点VIP等级');
INSERT INTO sys_setting VALUES ('Vip_Level_Function_Speed_Kill_N', '1', '4', '十连扫荡(>1的扫荡)功能开启VIP等级');
INSERT INTO sys_setting VALUES ('Vip_Level_Function_Check_In_Cover', '1', '5', '补签功能开启VIP等级');
INSERT INTO sys_setting VALUES ('Vip_Level_Function_Speed_Kill_Activity_Task', '1', '7', '活动副本可扫荡VIP等级');
INSERT INTO sys_setting VALUES ('Vip_Level_Function_Qiwuge', '1', '9', '永久开启奇物阁VIP等级');
INSERT INTO sys_setting VALUES ('Vip_Level_Function_Yijiebaoku', '1', '11', '永久开启异界宝库VIP等级');
INSERT INTO sys_setting VALUES ('Shop_Qiwuge_Switch', '1', '1', '奇物阁系统功能开关 0关闭商店 1开启商店');
INSERT INTO sys_setting VALUES ('Shop_Qiwuge_Open_Activity_time', '2', '', '奇物阁运营活动的开启时间,配置格式参考时间条件');
INSERT INTO sys_setting VALUES ('Shop_Qiwuge_Open_Probability', '1', '1000', '开启奇物阁概率（分母为100000）');
INSERT INTO sys_setting VALUES ('Shop_Qiwuge_Open_Time_Limit', '1', '3600', '开启奇物阁时间（秒）');
INSERT INTO sys_setting VALUES ('Shop_Qiwuge_Reset_Time', '2', '75600', '奇物阁刷新时间（以凌晨00:00为基准）');
INSERT INTO sys_setting VALUES ('Shop_Yijiebaoku_Switch', '1', '1', '异界宝库系统功能开关 0关闭商店 1开启商店');
INSERT INTO sys_setting VALUES ('Shop_Yijiebaoku_Open_Activity_time', '2', '', '异界宝库运营活动的开启时间,配置格式参考时间条件');
INSERT INTO sys_setting VALUES ('Shop_Yijiebaoku_Open_Probability', '1', '1000', '开启异界宝库概率（分母为100000）');
INSERT INTO sys_setting VALUES ('Shop_Yijiebaoku_Open_Time_Limit', '1', '3600', '开启异界宝库时间（秒）');
INSERT INTO sys_setting VALUES ('Shop_Yijiebaoku_Reset_Time', '2', '75600', '异界宝库刷新时间（以凌晨00:00为基准）');
INSERT INTO sys_setting VALUES ('Shop_Common_Switch', '1', '1', '杂货店系统功能开关 0关闭商店 1开启商店');
INSERT INTO sys_setting VALUES ('Shop_Common_Reset_Time', '2', '32400|43200|64800|75600', '杂物店刷新时间（以凌晨00:00为基准）');
INSERT INTO sys_setting VALUES ('Shop_Arena_Switch', '1', '1', '竞技场商店系统功能开关 0关闭商店 1开启商店');
INSERT INTO sys_setting VALUES ('Shop_Arena_Reset_Time', '2', '75600', '竞技场商店刷新时间（以凌晨00:00为基准）');
INSERT INTO sys_setting VALUES ('Shop_Expedition_Switch', '1', '1', '远征商店系统功能开关 0关闭商店 1开启商店');
INSERT INTO sys_setting VALUES ('Shop_Expedition_Reset_Time', '2', '75600', '远征商店刷新时间（以凌晨00:00为基准）');
INSERT INTO sys_setting VALUES ('Shop_Packet_Element_base_Weight', '1', '10', '物品基础权重');
INSERT INTO sys_setting VALUES ('Shop_Packet_Element_extra_Weight_factor1', '1', '10', '额外权重系数1');
INSERT INTO sys_setting VALUES ('Shop_Packet_Element_extra_Weight_factor2', '1', '10', '额外权重系数2');
INSERT INTO sys_setting VALUES ('Global_Reset_Time', '1', '14400', '参数刷新时间（以凌晨00:00为基准），大部分的参数均以此时间刷新，包括本日已点金次数，本日精英副本已重置次数，每日任务计数参数等等');
INSERT INTO sys_setting VALUES ('Month_Card_Each_Buy_Increase_Seconds', '1', '2592000', '购买一次月卡，增加剩余秒数(30天)');
INSERT INTO sys_setting VALUES ('Month_Card_Each_Renew_Increase_Days', '1', '33', '续费一次月卡，增加剩余天数');
INSERT INTO sys_setting VALUES ('Month_Card_Buy_Less_Than_Seconds', '1', '259200', '剩余天数小于等于该值时，才可以购买新的月卡(3天)');
INSERT INTO sys_setting VALUES ('Health_Limit', '1', '500', '玩家可继续获得体力的上限，大于等于该值，玩家不能获得体力，但玩家在小于此值时进行的最后一次获取体力的操作，可以允许玩家的体力值超过这个上限');
INSERT INTO sys_setting VALUES ('Health_Initial', '1', '60', '体力回复上限初始值');
INSERT INTO sys_setting VALUES ('Health_Grow', '1', '1', '体力回复上限每级成长值');
INSERT INTO sys_setting VALUES ('Health_Present_Factor_When_User_Level_Up', '1', '2', '体力增加系数（当玩家升级时，会获得体力的奖励，该奖励为玩家升级之前的等级*该系数）');
INSERT INTO sys_setting VALUES ('Health_Recover_Time', '1', '300', '体力自动回复1点需要时间（秒）');
INSERT INTO sys_setting VALUES ('Skill_Point_Recover_Time', '1', '300', '技能点自动回复1点需要时间（秒）');
INSERT INTO sys_setting VALUES ('Speed_Kill_Diamond_Per', '1', '20', '单次扫荡需要的钻石数量');
INSERT INTO sys_setting VALUES ('Speed_Kill_Limit', '1', '10', '扫荡券单次可使用的最大数量');
INSERT INTO sys_setting VALUES ('Max_Num_Vip_Level', '1', '16', 'vip上限');
INSERT INTO sys_setting VALUES ('Max_Num_User_Level', '1', '80', '玩家等级上限（英雄等级上限受玩家等级上限的限制，因此不填）');
INSERT INTO sys_setting VALUES ('Max_Num_Hero_Star', '1', '5', '英雄星级上限');
INSERT INTO sys_setting VALUES ('Max_Num_Hero_Quality', '1', '11', '英雄品质上限');
INSERT INTO sys_setting VALUES ('User_Level_Function_Arena', '1', '15', '竞技场开放等级');
INSERT INTO sys_setting VALUES ('User_Level_Function_Shop_Common', '1', '10', '杂货店功能开放等级');
INSERT INTO sys_setting VALUES ('User_Level_Function_Shop_Qiwuge', '1', '10', '奇物阁功能开放等级');
INSERT INTO sys_setting VALUES ('User_Level_Function_Shop_Yijiebaoku', '1', '10', '异界宝库功能开放等级');
INSERT INTO sys_setting VALUES ('User_Level_Function_Shop_Arena', '1', '1', '竞技场商店开放等级, 这个变量不使用！ 判断依据User_Level_Function_Arena');
INSERT INTO sys_setting VALUES ('User_Level_Function_Shop_Expedition', '1', '1', '远征商店开放等级');
INSERT INTO sys_setting VALUES ('User_Level_Function_Invoke', '1', '1', '祭坛功能开放等级');
INSERT INTO sys_setting VALUES ('User_Level_Function_Upgrade_Hero_Skill', '1', '1', '升级技能功能开放等级');
INSERT INTO sys_setting VALUES ('User_Level_Function_Expedition', '1', '1', '远征功能开放等级');
INSERT INTO sys_setting VALUES ('User_Level_Function_Trial_Tower', '1', '1', '试炼塔功能开放等级');
INSERT INTO sys_setting VALUES ('User_Level_Function_Friend', '1', '17', '好友功能开放等级');
INSERT INTO sys_setting VALUES ('User_Level_Function_Hello_World', '1', '1', '世界频道说话功能开放等级');
INSERT INTO sys_setting VALUES ('User_Level_Function_Everyday_Mission', '1', '4', '每日任务开放等级');
INSERT INTO sys_setting VALUES ('User_Level_Function_Guild', '1', '1', '公会功能开放等级');
INSERT INTO sys_setting VALUES ('User_Level_Function_Achivement_Mission', '1', '4', '成就任务开放等级（包含头像框任务开放）');
INSERT INTO sys_setting VALUES ('User_Level_Function_Mail', '1', '5', '邮箱功能开放等级');
INSERT INTO sys_setting VALUES ('User_Level_Function_CheckIn', '1', '8', '签到功能开放等级');
INSERT INTO sys_setting VALUES ('Switch_Allow_Same_User_Enter', '1', '1', '是否允许同账户的人登陆并挤掉前一个客户端， 0允许 1不允许');
INSERT INTO sys_setting VALUES ('Month_Card1_Recharge_Num', '1', '25', '月卡任务1，需要累计充值金额');
INSERT INTO sys_setting VALUES ('Month_Card2_Recharge_Num', '1', '88', '月卡任务2，需要累计充值金额');
INSERT INTO sys_setting VALUES ('Month_Card1_Reward_Diamond', '1', '120', '月卡1每日钻石');
INSERT INTO sys_setting VALUES ('Month_Card2_Reward_Diamond', '1', '120', '月卡2每日钻石');
INSERT INTO sys_setting VALUES ('Arena_History_Rank_Prize_factor1', '1', '606000', '竞技场历史排名奖励参数a');
INSERT INTO sys_setting VALUES ('Arena_History_Rank_Prize_factor2', '1', '100', '竞技场历史排名奖励参数b');
INSERT INTO sys_setting VALUES ('Arena_Auto_Prize_Time', '2', '75600', '竞技场发奖时间 可填多个 距离凌晨的秒数');
INSERT INTO sys_setting VALUES ('Arena_Challenge_CD', '1', '300', '竞技场挑战cd时间');
INSERT INTO sys_setting VALUES ('Arena_Fight_Time_Out', '1', '300', '竞技场挑战超时时间 过了时间判负(该参数与客户端参数不同，因为奥义释放期间，倒计时暂停，所以服务器没办法得知客户端实际的倒计时时间）');
INSERT INTO sys_setting VALUES ('Arena_Challenge_CD_Reset_Consume_Diamond', '1', '20', '竞技场挑战cd时间重置 消耗钻石');
INSERT INTO sys_setting VALUES ('Arena_Challenge_Num', '1', '5', '竞技场每日初始次数及重置获得次数');
INSERT INTO sys_setting VALUES ('Arena_Fight_Win_Prize', '2', '16-0-20|12-0-10000', '竞技场单次挑战胜利奖励');
INSERT INTO sys_setting VALUES ('Arena_Fight_Lose_Prize', '2', '16-0-20', '竞技场单次挑战失败奖励');
INSERT INTO sys_setting VALUES ('Mail_UnReaded_Mail_Valid_Time', '1', '2592000', '未读邮件删除时限');
INSERT INTO sys_setting VALUES ('Mail_Max_Num', '1', '99', '邮件最大封数');
INSERT INTO sys_setting VALUES ('Mail_Guide_Attach', '2', '13-0-200', '新手引导附件');
INSERT INTO sys_setting VALUES ('Mail_Phone_Attach', '2', '13-0-200', '绑定手机邮件附件');
INSERT INTO sys_setting VALUES ('Mail_Readed_Mail_Valid_Time', '1', '604800', '已读邮件删除时限');
INSERT INTO sys_setting VALUES ('CheckIn_Retrieve_Num_Per_Month', '1', '3', '每月补签次数');
INSERT INTO sys_setting VALUES ('Chapter_Map_Born_Point', '2', '1|36', '地图出生点章节ID');
INSERT INTO sys_setting VALUES ('Guid_Send_Mail_Level', '1', '5', '引导触发发送邮件等级');
INSERT INTO sys_setting VALUES ('Friend_Del_Max_Num_Per_Day', '1', '5', '每日删除好友上限');
INSERT INTO sys_setting VALUES ('Friend_Max_Num', '1', '50', '玩家总好友上限');
INSERT INTO sys_setting VALUES ('Friend_Give', '2', '14-0-1', '好友系统赠送内容物');
INSERT INTO sys_setting VALUES ('Friend_Give_Back', '2', '12-0-10000', '好友系统回馈内容物');
INSERT INTO sys_setting VALUES ('Friend_Recommend_Num', '1', '5', '推荐好友数量');
INSERT INTO sys_setting VALUES ('Friend_Invite_Max_Num', '1', '10', '邀请好友上限');
INSERT INTO sys_setting VALUES ('Friend_Confirm_Max_Num', '1', '50', '申请好友列表（待确认）上限');
