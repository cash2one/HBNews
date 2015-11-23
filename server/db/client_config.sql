/*
Navicat MySQL Data Transfer

Source Server         : localhost-mysql5.6-3307
Source Server Version : 50617
Source Host           : localhost:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50617
File Encoding         : 65001

Date: 2015-10-09 16:21:00
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `client_config`
-- ----------------------------
DROP TABLE IF EXISTS `client_config`;
CREATE TABLE `client_config` (
  `name` varchar(255) NOT NULL,
  `md5` varchar(128) DEFAULT '',
  `last_update_time` datetime DEFAULT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of client_config
-- ----------------------------
INSERT INTO client_config VALUES ('aoyi', 'helloworld!', null);
INSERT INTO client_config VALUES ('bag', 'helloworld!', null);
INSERT INTO client_config VALUES ('battle', 'helloworld!', null);
INSERT INTO client_config VALUES ('bossskill', 'helloworld!', null);
INSERT INTO client_config VALUES ('boss_prop', 'helloworld!', null);
INSERT INTO client_config VALUES ('buff', 'helloworld!', null);
INSERT INTO client_config VALUES ('chapter_map', 'helloworld!', '2015-10-08 11:20:34');
INSERT INTO client_config VALUES ('coinprice', 'helloworld!', null);
INSERT INTO client_config VALUES ('equip', 'helloworld!', null);
INSERT INTO client_config VALUES ('evolutioncoin', 'helloworld!', null);
INSERT INTO client_config VALUES ('herostar', 'helloworld!', null);
INSERT INTO client_config VALUES ('hero_prop', 'helloworld!', null);
INSERT INTO client_config VALUES ('hero_skill', 'helloworld!', null);
INSERT INTO client_config VALUES ('monster_prop', 'helloworld!', null);
INSERT INTO client_config VALUES ('passive', 'helloworld!', null);
INSERT INTO client_config VALUES ('prizeshow', 'helloworld!', null);
INSERT INTO client_config VALUES ('qualityrule', 'helloworld!', null);
INSERT INTO client_config VALUES ('realtimecheck', 'helloworld!', null);
INSERT INTO client_config VALUES ('resetprice', 'helloworld!', null);
INSERT INTO client_config VALUES ('rule', 'helloworld!', null);
INSERT INTO client_config VALUES ('shopresetprice', 'helloworld!', null);
INSERT INTO client_config VALUES ('spprice', 'helloworld!', null);
INSERT INTO client_config VALUES ('staprice', 'helloworld!', null);
INSERT INTO client_config VALUES ('sub_task', 'helloworld!', null);
INSERT INTO client_config VALUES ('task', 'helloworld!', null);
INSERT INTO client_config VALUES ('triggerattack', 'helloworld!', null);
INSERT INTO client_config VALUES ('triggerstartcondition', 'helloworld!', null);
INSERT INTO client_config VALUES ('vip', 'helloworld!', null);
INSERT INTO client_config VALUES ('vipfunction', 'helloworld!', null);
