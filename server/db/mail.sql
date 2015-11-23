/*
Navicat MySQL Data Transfer

Source Server         : wh
Source Server Version : 50624
Source Host           : 127.0.0.1:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50624
File Encoding         : 65001

Date: 2015-07-15 14:16:05
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `user_mail_attach`
-- ----------------------------
DROP TABLE IF EXISTS `user_mail_attach`;
CREATE TABLE `user_mail_attach` (
  `id` int(10) NOT NULL,
  `type` int(10) NOT NULL COMMENT '元素类型(金币,道具，经验等)',
  `element_id` int(10) NOT NULL,
  `num` int(10) NOT NULL,
  `mail_id` int(10) NOT NULL,
  `user_id` int(10) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_mail_attach
-- ----------------------------

-- ----------------------------
-- Table structure for `user_received_sys_mail`
-- ----------------------------
DROP TABLE IF EXISTS `user_received_sys_mail`;
CREATE TABLE `user_received_sys_mail` (
  `user_id` int(10) NOT NULL,
  `user_sys_mail_id` int(10) NOT NULL COMMENT '未接收的全服邮件id',
  PRIMARY KEY (`user_id`,`user_sys_mail_id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_received_sys_mail
-- ----------------------------

-- ----------------------------
-- Table structure for `user_sys_mail`
-- ----------------------------
DROP TABLE IF EXISTS `user_sys_mail`;
CREATE TABLE `user_sys_mail` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `content` varchar(1000) NOT NULL,
  `send_time` datetime NOT NULL,
  `type` tinyint(1) NOT NULL COMMENT '领取类和非领取类',
  `valid_time` int(10) unsigned NOT NULL COMMENT '邮件有效时间,单位为秒',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=111 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_sys_mail
-- ----------------------------

-- ----------------------------
-- Table structure for `user_sys_mail_attach`
-- ----------------------------
DROP TABLE IF EXISTS `user_sys_mail_attach`;
CREATE TABLE `user_sys_mail_attach` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `type` int(10) NOT NULL COMMENT '元素类型(金币,道具，经验等)',
  `element_id` int(10) NOT NULL,
  `num` int(10) NOT NULL,
  `mail_id` int(10) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=153 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_sys_mail_attach
-- ----------------------------

-- ----------------------------
-- Table structure for `user_mail`
-- ----------------------------
DROP TABLE IF EXISTS `user_mail`;
CREATE TABLE `user_mail` (
  `mail_id` int(10) NOT NULL AUTO_INCREMENT,
  `user_id` int(10) NOT NULL,
  `send_time` datetime NOT NULL COMMENT '邮件发送时间',
  `content` varchar(1000) DEFAULT NULL,
  `status` tinyint(1) NOT NULL COMMENT '表示该邮件是否已读取1：已读 2:未读',
  `type` tinyint(2) NOT NULL COMMENT '1.系统公告邮件2.奖励通知 3.道具堆叠到达上限，通过邮件发送',
  PRIMARY KEY (`mail_id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=632 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_mail
-- ----------------------------
