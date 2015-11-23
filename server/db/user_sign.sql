/*
Navicat MySQL Data Transfer

Source Server         : wh
Source Server Version : 50624
Source Host           : 127.0.0.1:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50624
File Encoding         : 65001

Date: 2015-08-28 11:08:10
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `user_sign`
-- ----------------------------
DROP TABLE IF EXISTS `user_sign`;
CREATE TABLE `user_sign` (
  `user_id` int(10) unsigned NOT NULL,
  `sign_1_times` int(10) unsigned NOT NULL COMMENT '每日签到次数',
  `sign_n_reward_status` varchar(255) NOT NULL COMMENT '累计签到领取状态(0：未领取 1：领取)',
  `last_sign_time` int(10) unsigned NOT NULL COMMENT '最后签到的时间',
  `add_signed_times` int(10) unsigned NOT NULL COMMENT '当月补签次数',
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_sign
-- ----------------------------
INSERT INTO user_sign VALUES ('24', '0', '0000', '0', '0');
INSERT INTO user_sign VALUES ('25', '0', '0000', '0', '3');
