/*
Navicat MySQL Data Transfer

Source Server         : localhost-mysql5.6-3307
Source Server Version : 50617
Source Host           : localhost:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50617
File Encoding         : 65001

Date: 2015-08-21 15:12:26
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `user_arena`
-- ----------------------------
DROP TABLE IF EXISTS `user_arena`;
CREATE TABLE `user_arena` (
  `rank` int(10) unsigned NOT NULL COMMENT '竞技场排名 从1开始',
  `user_id` int(10) unsigned NOT NULL COMMENT '玩家id',
  `score` int(10) unsigned NOT NULL COMMENT '竞技场积分',
  `hero_id1` int(10) unsigned NOT NULL COMMENT '防守阵容英雄1',
  `hero_id2` int(10) unsigned NOT NULL COMMENT '防守阵容英雄2',
  `hero_id3` int(10) unsigned NOT NULL COMMENT '防守阵容英雄3',
  `hero_id4` int(10) unsigned NOT NULL COMMENT '防守阵容英雄4',
  `hero_id5` int(10) unsigned NOT NULL COMMENT '防守阵容英雄5',
  `last_challenge_time` int(10) unsigned NOT NULL COMMENT '上一次主动挑战的时间戳',
  `last_day_challenge_num` int(10) unsigned NOT NULL COMMENT '最后一次当天已经主动挑战的次数',
  `history_highest_rank` int(10) unsigned NOT NULL COMMENT '最高历史排名',
  `last_time_buy_arena_challenge` int(10) unsigned NOT NULL COMMENT '上次购买挑战次数的时间',
  `last_day_buy_arena_challenge_num` int(10) unsigned NOT NULL COMMENT '上次当日购买挑战次数的次数，上限在sys_vip表',
  PRIMARY KEY (`user_id`),
  KEY `rank` (`rank`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_arena
-- ----------------------------

-- ----------------------------
-- Table structure for `user_arena_log`
-- ----------------------------
DROP TABLE IF EXISTS `user_arena_log`;
CREATE TABLE `user_arena_log` (
  `attacker_user_id` int(10) unsigned NOT NULL COMMENT '挑战者user_id',
  `attacker_rank` int(10) unsigned NOT NULL COMMENT '挑战者排名',
  `attacker_power` int(10) unsigned NOT NULL COMMENT '挑战者此次挑战的战斗力',
  `attacker_name` varchar(255) NOT NULL COMMENT '进攻方名字',
  `attacker_level` int(10) unsigned NOT NULL COMMENT '进攻者等级',
  `attacker_hero_id1` int(10) unsigned NOT NULL COMMENT '进攻方英雄id1',
  `attacker_hero_quality1` tinyint(3) unsigned NOT NULL,
  `attacker_hero_level1` smallint(3) unsigned NOT NULL,
  `attacker_hero_star1` tinyint(3) unsigned NOT NULL,
  `attacker_hero_id2` int(10) unsigned NOT NULL COMMENT '进攻方英雄id2',
  `attacker_hero_quality2` tinyint(3) unsigned NOT NULL,
  `attacker_hero_level2` smallint(3) unsigned NOT NULL,
  `attacker_hero_star2` tinyint(3) unsigned NOT NULL,
  `attacker_hero_id3` int(10) unsigned NOT NULL COMMENT '进攻方英雄id3',
  `attacker_hero_quality3` tinyint(3) unsigned NOT NULL,
  `attacker_hero_level3` smallint(3) unsigned NOT NULL,
  `attacker_hero_star3` tinyint(3) unsigned NOT NULL,
  `attacker_hero_id4` int(10) unsigned NOT NULL COMMENT '进攻方英雄id4',
  `attacker_hero_quality4` tinyint(3) unsigned NOT NULL,
  `attacker_hero_level4` smallint(3) unsigned NOT NULL,
  `attacker_hero_star4` tinyint(3) unsigned NOT NULL,
  `attacker_hero_id5` int(10) unsigned NOT NULL COMMENT '进攻方英雄id1',
  `attacker_hero_quality5` tinyint(3) unsigned NOT NULL,
  `attacker_hero_level5` smallint(3) unsigned NOT NULL,
  `attacker_hero_star5` tinyint(3) unsigned NOT NULL,
  `defender_user_id` int(10) unsigned NOT NULL COMMENT '防守者user_id',
  `defender_rank` int(10) unsigned NOT NULL COMMENT '防守者排名',
  `defender_power` int(10) unsigned NOT NULL COMMENT '防守者战斗力',
  `defender_name` varchar(255) NOT NULL COMMENT '进攻方名字',
  `defender_level` int(10) unsigned NOT NULL COMMENT '进攻者等级',
  `defender_hero_id1` int(10) unsigned NOT NULL COMMENT '进攻方英雄id1',
  `defender_hero_quality1` tinyint(3) unsigned NOT NULL,
  `defender_hero_level1` smallint(3) unsigned NOT NULL,
  `defender_hero_star1` tinyint(3) unsigned NOT NULL,
  `defender_hero_id2` int(10) unsigned NOT NULL COMMENT '进攻方英雄id2',
  `defender_hero_quality2` tinyint(3) unsigned NOT NULL,
  `defender_hero_level2` smallint(3) unsigned NOT NULL,
  `defender_hero_star2` tinyint(3) unsigned NOT NULL,
  `defender_hero_id3` int(10) unsigned NOT NULL COMMENT '进攻方英雄id3',
  `defender_hero_quality3` tinyint(3) unsigned NOT NULL,
  `defender_hero_level3` smallint(5) unsigned NOT NULL,
  `defender_hero_star3` tinyint(3) unsigned NOT NULL,
  `defender_hero_id4` int(10) unsigned NOT NULL COMMENT '进攻方英雄id4',
  `defender_hero_quality4` tinyint(3) unsigned NOT NULL,
  `defender_hero_level4` smallint(3) unsigned NOT NULL,
  `defender_hero_star4` tinyint(3) unsigned NOT NULL,
  `defender_hero_id5` int(10) unsigned NOT NULL COMMENT '进攻方英雄id1',
  `defender_hero_quality5` tinyint(3) unsigned NOT NULL,
  `defender_hero_level5` smallint(3) unsigned NOT NULL,
  `defender_hero_star5` tinyint(3) unsigned NOT NULL,
  `is_win` int(10) unsigned NOT NULL COMMENT '挑战者是否获胜',
  `time` int(10) unsigned NOT NULL COMMENT '挑战发起时间'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of user_arena_log
-- ----------------------------
