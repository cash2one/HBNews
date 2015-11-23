/*
Navicat MySQL Data Transfer

Source Server         : localhost-mysql5.6-3307
Source Server Version : 50617
Source Host           : localhost:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50617
File Encoding         : 65001

Date: 2015-06-25 15:19:44
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `sys_vip_buy`
-- ----------------------------
DROP TABLE IF EXISTS `sys_vip_buy`;
CREATE TABLE `sys_vip_buy` (
  `buy_type` int(10) unsigned NOT NULL COMMENT '购买的东西：1.体力 2.点金 3.技能点',
  `no_less_than_buy_num` int(10) unsigned NOT NULL COMMENT '>=此购买次数的时候',
  `diamond` int(10) unsigned NOT NULL COMMENT '>=no_less_than_buy_num购买次数的时候需要花费的钻石数'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_vip_buy
-- ----------------------------
INSERT INTO sys_vip_buy VALUES ('1', '1', '50');
INSERT INTO sys_vip_buy VALUES ('1', '3', '100');
INSERT INTO sys_vip_buy VALUES ('1', '6', '200');
INSERT INTO sys_vip_buy VALUES ('1', '9', '400');
INSERT INTO sys_vip_buy VALUES ('2', '1', '10');
INSERT INTO sys_vip_buy VALUES ('2', '2', '20');
INSERT INTO sys_vip_buy VALUES ('2', '5', '40');
INSERT INTO sys_vip_buy VALUES ('2', '9', '100');
INSERT INTO sys_vip_buy VALUES ('3', '1', '10');
INSERT INTO sys_vip_buy VALUES ('3', '2', '20');
INSERT INTO sys_vip_buy VALUES ('3', '5', '40');
INSERT INTO sys_vip_buy VALUES ('3', '9', '100');
