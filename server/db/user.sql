/*
Navicat MySQL Data Transfer

Source Server         : localhost-mysql5.6-3307
Source Server Version : 50617
Source Host           : localhost:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50617
File Encoding         : 65001

Date: 2015-07-08 09:05:41
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `user`
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `user_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(63) NOT NULL,
  `level` int(10) unsigned NOT NULL,
  `exp` int(10) unsigned NOT NULL,
  `vip` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `diamond` int(10) unsigned NOT NULL DEFAULT '0',
  `gold` int(10) unsigned NOT NULL,
  `health_cur` int(10) unsigned NOT NULL COMMENT '体力',
  `health_last_update_timestamp` int(10) unsigned NOT NULL COMMENT '体力数据上次计算刷新的时间',
  `skill_point` int(10) unsigned NOT NULL,
  `skill_point_update_timestamp` int(10) unsigned NOT NULL COMMENT '技能点上次计算刷新的时间',
  `invoke_gold_free_left_num` int(10) unsigned NOT NULL COMMENT '上一次金币免费召唤当天剩余使用次数',
  `invoke_gold_free_update_timestamp` int(10) unsigned NOT NULL COMMENT '上一次金币免费召唤功能使用的时间， 客户端已经有了剩余次数 只要剩余次数还有 并且这个时间距离现在超过了cd时间 客户端和服务器就判定可以使用这个功能',
  `invoke_gold_n_total_num` int(10) unsigned NOT NULL COMMENT '总共金币十连抽召唤的次数',
  `invoke_diamond_free_left_num` int(10) unsigned NOT NULL COMMENT '上一次钻石免费召唤当天剩余使用次数',
  `invoke_diamond_free_update_timestamp` int(10) unsigned NOT NULL COMMENT '上一次钻石免费召唤 计算cd刷新的时间 ： 初始化的时候和满到不满的这个时间',
  `invoke_diamond_n_total_num` int(10) unsigned NOT NULL COMMENT '总共钻石十连抽召唤的次数',
  `buy_used_times_gold` tinyint(3) unsigned NOT NULL COMMENT '金币购买已使用次数',
  `buy_used_times_health` tinyint(3) unsigned NOT NULL COMMENT '体力购买已使用次数',
  `buy_used_times_skill_point` tinyint(3) unsigned NOT NULL COMMENT '技能点购买已使用次数',
  `buy_used_times_reset_timestamp` int(10) unsigned NOT NULL COMMENT '上次购买(金币，体力，技能点)次数重置时间',
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=26 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user
-- ----------------------------
