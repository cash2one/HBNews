/*
Navicat MySQL Data Transfer

Source Server         : wh
Source Server Version : 50624
Source Host           : 127.0.0.1:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50624
File Encoding         : 65001

Date: 2015-09-22 13:56:28
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `user_statistics`
-- ----------------------------
DROP TABLE IF EXISTS `user_statistics`;
CREATE TABLE `user_statistics` (
  `user_id` int(10) unsigned NOT NULL,
  `event_type` int(10) unsigned NOT NULL COMMENT '事件类型(20:玩家升级,召唤英雄,突破,升星,升级,装备升级,装备进化,装备）',
  `event_sub_type_reserve1` int(10) unsigned DEFAULT NULL COMMENT '事件子类型1(保留字段)',
  `event_sub_type_reserve2` int(10) unsigned DEFAULT NULL COMMENT '事件子类型2(保留字段)',
  `event_occur_time` datetime NOT NULL COMMENT '时间发生时间',
  `cur_id` int(10) unsigned NOT NULL COMMENT '关卡统计：关卡id或者新手任务Id  成长统计：玩家id或者英雄id  消费点统计：',
  `reserve_field1` int(10) DEFAULT NULL COMMENT '保留字段',
  `reserve_field2` int(10) DEFAULT NULL COMMENT '保留字段',
  `level` int(10) unsigned NOT NULL COMMENT '事件发生时的玩家等级，当事件是升级时表示玩家升级后的等级',
  `fight_power` int(10) unsigned DEFAULT NULL COMMENT '战斗力',
  `vip` int(10) unsigned NOT NULL COMMENT 'vip等级',
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='玩家数据统计';

-- ----------------------------
-- Records of user_statistics
-- ----------------------------
INSERT INTO user_statistics VALUES ('2011', '20', null, null, '2015-09-22 13:48:22', '0', null, null, '8', null, '16');
