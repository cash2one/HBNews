/*
Navicat MySQL Data Transfer

Source Server         : localhost-mysql5.6-3307
Source Server Version : 50617
Source Host           : localhost:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50617
File Encoding         : 65001

Date: 2015-07-29 09:31:11
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `sys_shop_refresh`
-- ----------------------------
DROP TABLE IF EXISTS `sys_shop_refresh`;
CREATE TABLE `sys_shop_refresh` (
  `refresh_num` int(10) unsigned NOT NULL COMMENT '<=此次刷新次数时',
  `diamond` int(10) unsigned NOT NULL COMMENT '需要的钻石数'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_shop_refresh
-- ----------------------------
INSERT INTO sys_shop_refresh VALUES ('1', '10');
INSERT INTO sys_shop_refresh VALUES ('2', '20');
INSERT INTO sys_shop_refresh VALUES ('5', '40');
INSERT INTO sys_shop_refresh VALUES ('9', '100');
