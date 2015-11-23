/*
Navicat MySQL Data Transfer

Source Server         : wh
Source Server Version : 50624
Source Host           : 127.0.0.1:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50624
File Encoding         : 65001

Date: 2015-09-18 10:39:13
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `sys_marquee`
-- ----------------------------
DROP TABLE IF EXISTS `sys_marquee`;
CREATE TABLE `sys_marquee` (
  `id` int(10) unsigned NOT NULL COMMENT '1:祭坛抽取\r\n2:玩家突破\r\n3:英雄召唤\r\n4:英雄升星\r\n5:竞技场挑战',
  `value` varchar(255) NOT NULL COMMENT '判断值',
  `show_times` int(10) unsigned NOT NULL COMMENT '显示次数',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_marquee
-- ----------------------------
INSERT INTO sys_marquee VALUES ('1', '2', '1');
INSERT INTO sys_marquee VALUES ('2', '7,8,9,10,11', '1');
INSERT INTO sys_marquee VALUES ('3', '2', '1');
INSERT INTO sys_marquee VALUES ('4', '4', '1');
INSERT INTO sys_marquee VALUES ('5', '1,2,3', '3');
