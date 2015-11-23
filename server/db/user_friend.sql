/*
Navicat MySQL Data Transfer

Source Server         : wh
Source Server Version : 50624
Source Host           : 127.0.0.1:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50624
File Encoding         : 65001

Date: 2015-10-23 14:13:57
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `user_friend`
-- ----------------------------
DROP TABLE IF EXISTS `user_friend`;
CREATE TABLE `user_friend` (
  `user_id` int(10) NOT NULL,
  `friend_id` int(10) NOT NULL,
  `last_friend_give_element_timestamp` int(10) NOT NULL COMMENT '最近一次好友赠送体力时间',
  `last_me_give_element_timestamp` int(10) NOT NULL COMMENT '最近赠送好友体力时间',
  PRIMARY KEY (`user_id`,`friend_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_friend
-- ----------------------------

-- ----------------------------
-- Table structure for `user_friend_apply`
-- ----------------------------
DROP TABLE IF EXISTS `user_friend_apply`;
CREATE TABLE `user_friend_apply` (
  `user_id` int(10) NOT NULL,
  `apply_user_id` int(10) NOT NULL,
  PRIMARY KEY (`user_id`,`apply_user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_friend_apply
-- ----------------------------
