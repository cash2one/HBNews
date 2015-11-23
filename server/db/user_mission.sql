/*
Navicat MySQL Data Transfer

Source Server         : wh
Source Server Version : 50624
Source Host           : 127.0.0.1:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50624
File Encoding         : 65001

Date: 2015-08-21 15:26:29
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `user_mission_chain`
-- ----------------------------
DROP TABLE IF EXISTS `user_mission_chain`;
CREATE TABLE `user_mission_chain` (
  `user_id` int(10) unsigned NOT NULL,
  `mission_chain_id` int(10) unsigned NOT NULL COMMENT '任务链id',
  `last_get_reward_mission_id` int(10) unsigned NOT NULL COMMENT '任务链上最后一个领取奖励的任务id',
  `last_get_reward_timestamp` int(10) unsigned NOT NULL COMMENT '最后领取时间戳',
  PRIMARY KEY (`user_id`,`mission_chain_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_mission_chain
-- ----------------------------

-- ----------------------------
-- Table structure for `user_mission_event`
-- ----------------------------
DROP TABLE IF EXISTS `user_mission_event`;
CREATE TABLE `user_mission_event` (
  `user_id` int(10) unsigned NOT NULL,
  `mission_event_type` int(10) unsigned NOT NULL,
  `mission_event_sub_type` int(10) unsigned NOT NULL,
  `last_update_timestamp` int(10) unsigned DEFAULT NULL COMMENT '最后一次更新时间戳',
  `last_day_value` int(10) unsigned DEFAULT NULL COMMENT '代表当前进度，1.月卡表示购买时间,2.套餐任务表示最后一次领取时间 3.非上面两种表示当前进度',
  `total_value` int(10) unsigned DEFAULT NULL COMMENT '历史总次数',
  PRIMARY KEY (`user_id`,`mission_event_type`,`mission_event_sub_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_mission_event
-- ----------------------------
