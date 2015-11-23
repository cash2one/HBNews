/*
Navicat MySQL Data Transfer

Source Server         : localhost-mysql5.6-3307
Source Server Version : 50617
Source Host           : localhost:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50617
File Encoding         : 65001

Date: 2015-07-28 15:06:48
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `user_shop`
-- ----------------------------
DROP TABLE IF EXISTS `user_shop`;
CREATE TABLE `user_shop` (
  `user_id` int(10) unsigned NOT NULL,
  `shop_type` tinyint(3) unsigned NOT NULL,
  `last_auto_refresh_time` int(10) unsigned NOT NULL COMMENT '最后一次自动刷新商店的时间 这个时间存的配置的那些时间点的绝对时间！',
  `last_manual_refresh_time` int(10) unsigned NOT NULL COMMENT '最后手动刷新商店的时间',
  `last_day_manual_refresh_num` int(10) unsigned NOT NULL COMMENT '最后一次刷新商店的那天 当天已经手动刷新了多少次',
  `total_manual_refresh_num` int(10) unsigned NOT NULL COMMENT '历史累积手动刷新了多少次',
  `random_open_start_timestamp` int(10) unsigned NOT NULL COMMENT '随机开启商店时 商店开放的起始时间',
  `random_open_end_timestamp` int(10) unsigned NOT NULL COMMENT '随机开启商店时 商店开放的结束时间',
  PRIMARY KEY (`user_id`,`shop_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_shop
-- ----------------------------

-- ----------------------------
-- Table structure for `user_shop_buy`
-- ----------------------------
DROP TABLE IF EXISTS `user_shop_buy`;
CREATE TABLE `user_shop_buy` (
  `user_id` int(10) unsigned NOT NULL,
  `element_type` int(10) unsigned NOT NULL,
  `element_id` int(10) unsigned NOT NULL,
  `last_day_buy_num` int(10) unsigned NOT NULL,
  `last_buy_time` int(10) unsigned NOT NULL,
  PRIMARY KEY (`user_id`,`element_type`,`element_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_shop_buy
-- ----------------------------

-- ----------------------------
-- Table structure for `user_shop_item`
-- ----------------------------
DROP TABLE IF EXISTS `user_shop_item`;
CREATE TABLE `user_shop_item` (
  `user_id` int(10) unsigned NOT NULL,
  `shop_type` tinyint(3) unsigned NOT NULL,
  `pos` tinyint(3) unsigned NOT NULL COMMENT '位置 否则无法标识和更新一个道具信息！',
  `element_type` tinyint(10) unsigned NOT NULL,
  `element_id` int(10) unsigned NOT NULL,
  `element_num` int(10) unsigned NOT NULL,
  `consume_type` tinyint(10) unsigned NOT NULL,
  `consume_id` int(10) unsigned NOT NULL,
  `consume_num` int(10) unsigned NOT NULL,
  `flag` tinyint(10) unsigned NOT NULL,
  `is_sold` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`user_id`,`shop_type`,`pos`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_shop_item
-- ----------------------------
