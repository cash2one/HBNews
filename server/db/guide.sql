/*
Navicat MySQL Data Transfer

Source Server         : wh
Source Server Version : 50624
Source Host           : 127.0.0.1:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50624
File Encoding         : 65001

Date: 2015-09-11 12:29:10
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `user_guide`
-- ----------------------------
DROP TABLE IF EXISTS `user_guide`;
CREATE TABLE `user_guide` (
  `user_id` int(10) unsigned NOT NULL,
  `guide_id` smallint(5) unsigned NOT NULL,
  `time` datetime NOT NULL COMMENT '完成时间',
  PRIMARY KEY (`user_id`,`guide_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_guide
-- ----------------------------
INSERT INTO user_guide VALUES ('2009', '1', '2015-09-11 10:41:03');
INSERT INTO user_guide VALUES ('2009', '12', '2015-09-11 11:26:30');
