/*
Navicat MySQL Data Transfer

Source Server         : localhost-mysql5.6-3307
Source Server Version : 50617
Source Host           : localhost:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50617
File Encoding         : 65001

Date: 2015-06-25 15:19:37
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `sys_vip`
-- ----------------------------
DROP TABLE IF EXISTS `sys_vip`;
CREATE TABLE `sys_vip` (
  `vip_level` int(10) unsigned NOT NULL COMMENT 'vip等级',
  `rmb` int(10) unsigned NOT NULL COMMENT '达到此等级需要达到的累计充值金额',
  `prize` varchar(511) NOT NULL COMMENT '达到此vip等级时 每天可领取的礼物 格式为： type-id-num 比如 8-1-1|8-2-1|8-3-1',
  `buy_num_health` int(10) unsigned NOT NULL COMMENT '可购买体力次数',
  `buy_num_instance` int(10) unsigned NOT NULL COMMENT '控制购买精英副本次数',
  `buy_num_arena` int(10) unsigned NOT NULL COMMENT '控制购买竞技场挑战次数',
  `buy_num_gold` int(10) unsigned NOT NULL COMMENT '控制可点金次数'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_vip
-- ----------------------------
INSERT INTO sys_vip VALUES ('1', '0', '8-186-30', '1', '0', '0', '3');
INSERT INTO sys_vip VALUES ('2', '1', '8-186-40', '2', '1', '0', '8');
INSERT INTO sys_vip VALUES ('3', '10', '8-186-50', '3', '2', '1', '20');
INSERT INTO sys_vip VALUES ('4', '30', '8-186-60', '4', '3', '2', '30');
INSERT INTO sys_vip VALUES ('5', '50', '8-186-70', '5', '4', '3', '40');
INSERT INTO sys_vip VALUES ('6', '100', '8-186-80', '6', '5', '4', '50');
INSERT INTO sys_vip VALUES ('7', '200', '8-186-90', '7', '6', '5', '60');
INSERT INTO sys_vip VALUES ('8', '300', '8-186-100', '8', '7', '6', '70');
INSERT INTO sys_vip VALUES ('9', '500', '8-186-110', '9', '8', '7', '80');
INSERT INTO sys_vip VALUES ('10', '700', '8-186-120', '10', '9', '8', '90');
INSERT INTO sys_vip VALUES ('11', '1000', '8-186-130', '11', '10', '9', '100');
INSERT INTO sys_vip VALUES ('12', '1500', '8-186-140', '12', '11', '10', '110');
INSERT INTO sys_vip VALUES ('13', '2000', '8-186-150', '13', '12', '11', '120');
INSERT INTO sys_vip VALUES ('14', '5000', '8-186-160', '14', '13', '12', '130');
INSERT INTO sys_vip VALUES ('15', '10000', '8-186-170', '15', '14', '13', '140');
INSERT INTO sys_vip VALUES ('16', '15000', '8-186-180', '16', '15', '14', '150');
