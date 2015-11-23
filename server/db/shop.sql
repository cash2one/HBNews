/*
Navicat MySQL Data Transfer

Source Server         : localhost-mysql5.6-3307
Source Server Version : 50617
Source Host           : localhost:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50617
File Encoding         : 65001

Date: 2015-07-20 22:53:39
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `sys_shop`
-- ----------------------------
DROP TABLE IF EXISTS `sys_shop`;
CREATE TABLE `sys_shop` (
  `shop_type` tinyint(10) unsigned NOT NULL COMMENT '所属的商店(1=杂货店，2=奇物阁，3=异界宝库，4=竞技场，5=远征）',
  `shop_name` varchar(63) NOT NULL COMMENT '商店的名称',
  `activity_shop_part_id` int(10) unsigned NOT NULL COMMENT '活动时额外商店组成部分',
  `activity_time_type` int(10) unsigned NOT NULL COMMENT '活动时间类型',
  `activity_time` varchar(511) NOT NULL COMMENT '活动时间段',
  PRIMARY KEY (`shop_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_shop
-- ----------------------------

-- ----------------------------
-- Table structure for `sys_shop_packet`
-- ----------------------------
DROP TABLE IF EXISTS `sys_shop_packet`;
CREATE TABLE `sys_shop_packet` (
  `shop_packet_id` int(10) unsigned NOT NULL COMMENT '所属商店包id',
  `element_type_id` varchar(63) NOT NULL COMMENT '某个物体 格式：元素类型-元素id ',
  `element_num_arr` varchar(127) NOT NULL COMMENT '物品多种数量',
  `element_probability_arr` varchar(127) NOT NULL COMMENT '物品多种数量对应的概率（此处一定是N选1）',
  `element_consume_type_id_num_arr` varchar(63) NOT NULL COMMENT '物品多个数量所对应的的需要的消耗 格式： 元素类型-元素id-元素数量',
  `element_flag_arr` varchar(63) NOT NULL COMMENT '标签（0=不显示，1=热卖，2=打折，3=限时抢购） 格式 0|1|2',
  PRIMARY KEY (`shop_packet_id`,`element_type_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_shop_packet
-- ----------------------------
INSERT INTO sys_shop_packet VALUES ('1', '8-1', '1|2|3', '10000|20000|70000', '11-1-100|11-1-200|11-1-300', '0|1|2');

-- ----------------------------
-- Table structure for `sys_shop_part`
-- ----------------------------
DROP TABLE IF EXISTS `sys_shop_part`;
CREATE TABLE `sys_shop_part` (
  `shop_part_id` int(10) unsigned NOT NULL COMMENT '各个商店的组成部分  id',
  `shop_type` tinyint(10) unsigned NOT NULL COMMENT '所属的商店',
  `shop_packet_id_arr` varchar(255) NOT NULL COMMENT '包含的 商店包id',
  `burst_num` tinyint(3) unsigned NOT NULL COMMENT '产出种类（如果大于前一个字段的种类，则先全部产出一遍，然后再判断）',
  PRIMARY KEY (`shop_part_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_shop_part
-- ----------------------------
