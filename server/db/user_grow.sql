/*
Navicat MySQL Data Transfer

Source Server         : wh
Source Server Version : 50624
Source Host           : 127.0.0.1:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50624
File Encoding         : 65001

Date: 2015-09-14 17:09:17
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `user_grow`
-- ----------------------------
DROP TABLE IF EXISTS `user_grow`;
CREATE TABLE `user_grow` (
  `user_id` int(10) unsigned NOT NULL,
  `event_occur_time` datetime NOT NULL COMMENT '时间发生时间',
  `event_type` int(10) unsigned NOT NULL COMMENT '事件类型(20:玩家升级,召唤英雄,突破,升星,升级,装备升级,装备进化,装备）',
  `hero_id` int(10) unsigned NOT NULL COMMENT '当时间类型是玩家升级时，此字段填0',
  `level` int(10) unsigned NOT NULL COMMENT '事件发生时的玩家等级，当事件是升级时表示玩家升级后的等级',
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='玩家成长信息统计';

-- ----------------------------
-- Records of user_grow
-- ----------------------------
