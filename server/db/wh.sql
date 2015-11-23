/*
Navicat MySQL Data Transfer

Source Server         : wh
Source Server Version : 50624
Source Host           : 127.0.0.1:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50624
File Encoding         : 65001

Date: 2015-06-16 16:00:23
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for -sys_burst_plan
-- ----------------------------
DROP TABLE IF EXISTS `-sys_burst_plan`;
CREATE TABLE `-sys_burst_plan` (
  `id` int(10) unsigned NOT NULL,
  `plan_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `plan_id` (`plan_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of -sys_burst_plan
-- ----------------------------

-- ----------------------------
-- Table structure for -sys_chapter_mappoint_mine
-- ----------------------------
DROP TABLE IF EXISTS `-sys_chapter_mappoint_mine`;
CREATE TABLE `-sys_chapter_mappoint_mine` (
  `id` int(10) unsigned NOT NULL COMMENT '据点id 可以是城市 也可以小点',
  `type` enum('CITY','STRONGHOLD') NOT NULL COMMENT '类型 1.城市 2.小点',
  `name` varchar(255) DEFAULT NULL COMMENT '据点的名字 城市需要 小点不需要',
  `joined_mappoint` tinyblob COMMENT '最大255字节 也就是说 连接点最多 255/4 = 63个！',
  `position` point DEFAULT NULL COMMENT '坐标位置',
  `hidden` tinyint(1) unsigned NOT NULL COMMENT '是否隐藏 0否 1是。 当隐藏的时候 即使有边链接到这个点，客户端判断这个点隐藏了 也要取消掉这些连线！',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of -sys_chapter_mappoint_mine
-- ----------------------------

-- ----------------------------
-- Table structure for -sys_hero
-- ----------------------------
DROP TABLE IF EXISTS `-sys_hero`;
CREATE TABLE `-sys_hero` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `hero_id` int(10) unsigned NOT NULL COMMENT '英雄id',
  `name_cn` varchar(63) NOT NULL,
  `area` tinyint(3) unsigned NOT NULL COMMENT '所属势力',
  `nature_property` tinyint(3) unsigned NOT NULL,
  `occupation` tinyint(3) unsigned NOT NULL,
  `health` int(3) unsigned NOT NULL,
  `damage` int(10) unsigned NOT NULL,
  `damageGrow` int(10) unsigned NOT NULL,
  `pdd` int(10) unsigned NOT NULL,
  `pddGrow` int(10) unsigned NOT NULL,
  `mdd` int(10) NOT NULL,
  `mddGrow` int(10) unsigned NOT NULL,
  `hitLevel` int(10) unsigned NOT NULL,
  `hitGrow` int(10) unsigned NOT NULL,
  `criLevel` int(10) unsigned NOT NULL,
  `criGrow` int(10) unsigned NOT NULL,
  `tenacityLevel` int(10) unsigned NOT NULL,
  `tenacityGrow` int(10) unsigned NOT NULL,
  `pddPierce` int(10) unsigned NOT NULL,
  `pddPierceGrow` int(10) unsigned NOT NULL,
  `type_Normal` int(10) unsigned NOT NULL,
  `type_Skill` int(10) unsigned NOT NULL,
  `percent_Skill` tinyint(3) unsigned NOT NULL,
  `addNum_Skill` int(10) unsigned NOT NULL,
  `addNumGrow_Skill` int(10) unsigned NOT NULL,
  `type_AoYi` tinyint(3) unsigned NOT NULL,
  `percent_AoYi` tinyint(3) unsigned NOT NULL,
  `addNum_AoYi` int(10) unsigned NOT NULL,
  `addNumGrow_AoYi` int(10) unsigned NOT NULL,
  `passiveSkill_1` int(10) unsigned NOT NULL,
  `passiveSkill_2` int(11) NOT NULL,
  `star` tinyint(3) unsigned NOT NULL,
  `quality` tinyint(3) unsigned NOT NULL,
  `physic_defense` int(10) unsigned NOT NULL,
  `dodgeLevel` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of -sys_hero
-- ----------------------------

-- ----------------------------
-- Table structure for -sys_task_mine
-- ----------------------------
DROP TABLE IF EXISTS `-sys_task_mine`;
CREATE TABLE `-sys_task_mine` (
  `id` int(10) unsigned NOT NULL,
  `type` enum('ACTIVITY','BRANCH','MAIN') NOT NULL,
  `mappoint_id` int(10) unsigned DEFAULT NULL COMMENT '副本所属的场景',
  `show_priority` tinyint(3) unsigned DEFAULT NULL COMMENT '副本显示优先级0~127，值越大越在前. 应该场景:某城市中有多个副本， 活动副本列表 ',
  `has_enter_condition` tinyint(1) NOT NULL COMMENT '是否有进入限制 0没有 1有； 假如有的话，这个副本的进入条件写在./instance/enter_condition/id.xml中',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of -sys_task_mine
-- ----------------------------

-- ----------------------------
-- Table structure for -user_base_info
-- ----------------------------
DROP TABLE IF EXISTS `-user_base_info`;
CREATE TABLE `-user_base_info` (
  `user_id` int(10) unsigned NOT NULL,
  `cur_mappoint_id` int(10) unsigned NOT NULL COMMENT '玩家当前所处的地图据点id',
  `level` int(10) unsigned NOT NULL COMMENT '玩家等级',
  `gold` int(10) unsigned NOT NULL DEFAULT '0',
  `diamond` int(10) unsigned NOT NULL,
  `health_cur` int(10) unsigned NOT NULL COMMENT '体力',
  `health_max` int(10) unsigned NOT NULL,
  `health_last_time_update` time NOT NULL COMMENT '体力数据上次保存的时间',
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of -user_base_info
-- ----------------------------

-- ----------------------------
-- Table structure for -user_bin
-- ----------------------------
DROP TABLE IF EXISTS `-user_bin`;
CREATE TABLE `-user_bin` (
  `user_id` int(10) unsigned NOT NULL COMMENT '用户id',
  `bag_bin` blob,
  `hero_bin` blob,
  `team_bin` blob,
  `task_bin` blob,
  `friend_bin` blob,
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of -user_bin
-- ----------------------------

-- ----------------------------
-- Table structure for -user_hero
-- ----------------------------
DROP TABLE IF EXISTS `-user_hero`;
CREATE TABLE `-user_hero` (
  `hero_instance_id` int(10) unsigned NOT NULL,
  `user_id` int(10) unsigned NOT NULL COMMENT '用户id',
  `hero_id` int(10) unsigned NOT NULL,
  `level` smallint(5) unsigned NOT NULL,
  `star` tinyint(3) unsigned NOT NULL COMMENT '星级 可能升星过',
  `got_time` time NOT NULL COMMENT '获取到的时间',
  PRIMARY KEY (`hero_instance_id`),
  KEY `user_id` (`user_id`),
  KEY `hero_id` (`hero_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of -user_hero
-- ----------------------------

-- ----------------------------
-- Table structure for -user_hero_team
-- ----------------------------
DROP TABLE IF EXISTS `-user_hero_team`;
CREATE TABLE `-user_hero_team` (
  `user_id` int(10) unsigned NOT NULL,
  `team1` varchar(255) NOT NULL COMMENT '用|分割 队伍里面的hero_instance_id|hero_instance_id|hero_instance_id',
  `team2` varchar(255) NOT NULL COMMENT '队伍里面的hero_instance_id',
  `team3` varchar(255) NOT NULL COMMENT '队伍里面的hero_instance_id',
  `team4` varchar(255) NOT NULL COMMENT '队伍里面的hero_instance_id',
  `team5` varchar(255) NOT NULL COMMENT '队伍里面的hero_instance_id',
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of -user_hero_team
-- ----------------------------

-- ----------------------------
-- Table structure for -user_task
-- ----------------------------
DROP TABLE IF EXISTS `-user_task`;
CREATE TABLE `-user_task` (
  `user_id` int(10) unsigned NOT NULL,
  `task_id` int(10) unsigned NOT NULL,
  `status` tinyint(3) unsigned NOT NULL COMMENT '0 未通关  1.已通关（奖励已给）',
  PRIMARY KEY (`user_id`,`task_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of -user_task
-- ----------------------------

-- ----------------------------
-- Table structure for client_config_aoyi
-- ----------------------------
DROP TABLE IF EXISTS `client_config_aoyi`;
CREATE TABLE `client_config_aoyi` (
  `id` varchar(100) DEFAULT NULL COMMENT 'ID',
  `factor` float DEFAULT NULL COMMENT '伤害系数'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of client_config_aoyi
-- ----------------------------
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_0', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_1', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_2', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_3', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_4', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_5', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_6', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_7', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_8', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_9', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_10', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_11', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_12', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_13', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_14', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_15', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_16', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_17', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_18', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_19', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_20', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_21', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_22', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_23', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_24', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_25', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero001_A_Hit_26', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero002_A_Hit_0', '1');
INSERT INTO `client_config_aoyi` VALUES ('Hero002_A_Hit_1', '1');
INSERT INTO `client_config_aoyi` VALUES ('Hero002_A_Hit_2', '1.2');
INSERT INTO `client_config_aoyi` VALUES ('Hero002_A_Hit_3', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero002_A_Hit_4', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero002_A_Hit_5', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero002_A_Hit_6', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero002_A_Hit_7', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero002_A_Hit_8', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero002_A_Hit_9', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero002_A_Hit_10', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero002_A_Hit_11', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero002_A_Hit_12', '1.5');
INSERT INTO `client_config_aoyi` VALUES ('Hero003_A_Hit_0', '3');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_0', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_1', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_2', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_3', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_4', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_5', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_6', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_7', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_8', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_9', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_10', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_11', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_12', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_13', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_14', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_15', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_16', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_17', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_18', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero004_A_Hit_19', '1.5');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_0', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_1', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_2', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_3', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_4', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_5', '0.8');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_6', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_7', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_8', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_9', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_10', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_11', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_12', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_13', '0.6');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_14', '1.2');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_15', '1.2');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_16', '1.2');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_17', '1.2');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_18', '1.2');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_19', '1.2');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_20', '1.2');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_21', '1.2');
INSERT INTO `client_config_aoyi` VALUES ('Hero005_A_Hit_22', '1.2');

-- ----------------------------
-- Table structure for client_config_bag
-- ----------------------------
DROP TABLE IF EXISTS `client_config_bag`;
CREATE TABLE `client_config_bag` (
  `id` varchar(100) DEFAULT NULL COMMENT '背包类物品id',
  `useType` int(10) DEFAULT NULL COMMENT '用途',
  `quality` int(10) DEFAULT NULL COMMENT '品质',
  `sellPrice` int(10) DEFAULT NULL COMMENT '出售价格',
  `exp` int(10) DEFAULT NULL COMMENT '获取经验值',
  `needs` varchar(100) DEFAULT NULL COMMENT '合成配方',
  `needCounts` varchar(100) DEFAULT NULL COMMENT '合成配方对应物品数量',
  `needGolds` int(10) DEFAULT NULL COMMENT '合成需要金币数'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of client_config_bag
-- ----------------------------
INSERT INTO `client_config_bag` VALUES ('BagItem0001', '1', '2', '1500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0002', '1', '2', '1500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0003', '1', '2', '1500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0004', '1', '2', '1500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0005', '1', '2', '1500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0006', '1', '2', '1500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0007', '1', '2', '1500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0008', '1', '2', '1500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0009', '1', '2', '1500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0010', '1', '2', '1500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0011', '1', '2', '1500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0012', '1', '2', '1500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0013', '1', '3', '2000', '0', 'BagItem0064|BagItem0070', '1|1', '2000');
INSERT INTO `client_config_bag` VALUES ('BagItem0014', '1', '3', '2000', '0', 'BagItem0065|BagItem0071', '1|1', '2000');
INSERT INTO `client_config_bag` VALUES ('BagItem0015', '1', '3', '2000', '0', 'BagItem0066|BagItem0072', '1|1', '2000');
INSERT INTO `client_config_bag` VALUES ('BagItem0016', '1', '3', '2000', '0', 'BagItem0067|BagItem0073', '1|1', '2000');
INSERT INTO `client_config_bag` VALUES ('BagItem0017', '1', '3', '2000', '0', 'BagItem0068|BagItem0074', '1|1', '2000');
INSERT INTO `client_config_bag` VALUES ('BagItem0018', '1', '3', '2000', '0', 'BagItem0069|BagItem0075', '1|1', '2000');
INSERT INTO `client_config_bag` VALUES ('BagItem0019', '1', '3', '2000', '0', 'BagItem0076|BagItem0081|BagItem0086', '1|1|1', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0020', '1', '3', '2000', '0', 'BagItem0077|BagItem0082|BagItem0087', '1|1|1', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0021', '1', '3', '2000', '0', 'BagItem0078|BagItem0083|BagItem0088', '1|1|1', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0022', '1', '3', '2000', '0', 'BagItem0079|BagItem0084|BagItem0089', '1|1|1', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0023', '1', '3', '2000', '0', 'BagItem0080|BagItem0085|BagItem0090', '1|1|1', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0024', '1', '3', '2000', '0', 'BagItem0091|BagItem0097', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0025', '1', '3', '2000', '0', 'BagItem0092|BagItem0098', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0026', '1', '3', '2000', '0', 'BagItem0093|BagItem0099', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0027', '1', '3', '2000', '0', 'BagItem0094|BagItem0100', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0028', '1', '3', '2000', '0', 'BagItem0095|BagItem0101', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0029', '1', '3', '2000', '0', 'BagItem0096|BagItem0102', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0030', '1', '4', '2500', '0', 'BagItem0103|BagItem0109', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0031', '1', '4', '2500', '0', 'BagItem0104|BagItem0110', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0032', '1', '4', '2500', '0', 'BagItem0105|BagItem0111', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0033', '1', '4', '2500', '0', 'BagItem0106|BagItem0112', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0034', '1', '4', '2500', '0', 'BagItem0107|BagItem0113', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0035', '1', '4', '2500', '0', 'BagItem0108|BagItem0114', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0036', '1', '4', '2500', '0', 'BagItem0115|BagItem0120|BagItem0125', '2|2|2', '6000');
INSERT INTO `client_config_bag` VALUES ('BagItem0037', '1', '4', '2500', '0', 'BagItem0116|BagItem0121|BagItem0126', '2|2|2', '6000');
INSERT INTO `client_config_bag` VALUES ('BagItem0038', '1', '4', '2500', '0', 'BagItem0117|BagItem0122|BagItem0127', '2|2|2', '6000');
INSERT INTO `client_config_bag` VALUES ('BagItem0039', '1', '4', '2500', '0', 'BagItem0118|BagItem0123|BagItem0128', '2|2|2', '6000');
INSERT INTO `client_config_bag` VALUES ('BagItem0040', '1', '4', '2500', '0', 'BagItem0119|BagItem0124|BagItem0129', '2|2|2', '6000');
INSERT INTO `client_config_bag` VALUES ('BagItem0041', '1', '4', '2500', '0', 'BagItem0130|BagItem0136', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0042', '1', '4', '2500', '0', 'BagItem0131|BagItem0137', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0043', '1', '4', '2500', '0', 'BagItem0132|BagItem0138', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0044', '1', '4', '2500', '0', 'BagItem0133|BagItem0139', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0045', '1', '4', '2500', '0', 'BagItem0134|BagItem0140', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0046', '1', '4', '2500', '0', 'BagItem0135|BagItem0141', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0047', '1', '4', '2500', '0', 'BagItem0142|BagItem0148', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0048', '1', '4', '2500', '0', 'BagItem0143|BagItem0149', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0049', '1', '4', '2500', '0', 'BagItem0144|BagItem0150', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0050', '1', '4', '2500', '0', 'BagItem0145|BagItem0151', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0051', '1', '4', '2500', '0', 'BagItem0146|BagItem0152', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0052', '1', '4', '2500', '0', 'BagItem0147|BagItem0153', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0053', '1', '5', '3000', '0', 'BagItem0154|BagItem0160', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0054', '1', '5', '3000', '0', 'BagItem0155|BagItem0161', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0055', '1', '5', '3000', '0', 'BagItem0156|BagItem0162', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0056', '1', '5', '3000', '0', 'BagItem0157|BagItem0163', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0057', '1', '5', '3000', '0', 'BagItem0158|BagItem0164', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0058', '1', '5', '3000', '0', 'BagItem0159|BagItem0165', '2|2', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0059', '1', '5', '3000', '0', 'BagItem0166|BagItem0171|BagItem0176', '2|2|2', '6000');
INSERT INTO `client_config_bag` VALUES ('BagItem0060', '1', '5', '3000', '0', 'BagItem0167|BagItem0099|BagItem0177', '2|2|2', '6000');
INSERT INTO `client_config_bag` VALUES ('BagItem0061', '1', '5', '3000', '0', 'BagItem0168|BagItem0173|BagItem0178', '2|2|2', '6000');
INSERT INTO `client_config_bag` VALUES ('BagItem0062', '1', '5', '3000', '0', 'BagItem0169|BagItem0174|BagItem0179', '2|2|2', '6000');
INSERT INTO `client_config_bag` VALUES ('BagItem0063', '1', '5', '3000', '0', 'BagItem0170|BagItem0175|BagItem0180', '2|2|2', '6000');
INSERT INTO `client_config_bag` VALUES ('BagItem0064', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0065', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0066', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0067', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0068', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0069', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0070', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0071', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0072', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0073', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0074', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0075', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0076', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0077', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0078', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0079', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0080', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0081', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0082', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0083', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0084', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0085', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0086', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0087', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0088', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0089', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0090', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0091', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0092', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0093', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0094', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0095', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0096', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0097', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0098', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0099', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0100', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0101', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0102', '1', '3', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0103', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0104', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0105', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0106', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0107', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0108', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0109', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0110', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0111', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0112', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0113', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0114', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0115', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0116', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0117', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0118', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0119', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0120', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0121', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0122', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0123', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0124', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0125', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0126', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0127', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0128', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0129', '1', '4', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0130', '1', '4', '2500', '0', 'BagItem1001', '3', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0131', '1', '4', '2500', '0', 'BagItem1002', '3', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0132', '1', '4', '2500', '0', 'BagItem1003', '3', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0133', '1', '4', '2500', '0', 'BagItem1004', '3', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0134', '1', '4', '2500', '0', 'BagItem1005', '3', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0135', '1', '4', '2500', '0', 'BagItem1006', '3', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0136', '1', '4', '2500', '0', 'BagItem1007', '3', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0137', '1', '4', '2500', '0', 'BagItem1008', '3', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0138', '1', '4', '2500', '0', 'BagItem1009', '3', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0139', '1', '4', '2500', '0', 'BagItem1010', '3', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0140', '1', '4', '2500', '0', 'BagItem1011', '3', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0141', '1', '4', '2500', '0', 'BagItem1012', '3', '3000');
INSERT INTO `client_config_bag` VALUES ('BagItem0142', '1', '4', '2500', '0', 'BagItem1013', '4', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0143', '1', '4', '2500', '0', 'BagItem1014', '4', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0144', '1', '4', '2500', '0', 'BagItem1015', '4', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0145', '1', '4', '2500', '0', 'BagItem1016', '4', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0146', '1', '4', '2500', '0', 'BagItem1017', '4', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0147', '1', '4', '2500', '0', 'BagItem1018', '4', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0148', '1', '4', '2500', '0', 'BagItem1019', '4', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0149', '1', '4', '2500', '0', 'BagItem1020', '4', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0150', '1', '4', '2500', '0', 'BagItem1021', '4', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0151', '1', '4', '2500', '0', 'BagItem1022', '4', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0152', '1', '4', '2500', '0', 'BagItem1023', '4', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0153', '1', '4', '2500', '0', 'BagItem1024', '4', '4000');
INSERT INTO `client_config_bag` VALUES ('BagItem0154', '1', '5', '3000', '0', 'BagItem1025', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0155', '1', '5', '3000', '0', 'BagItem1026', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0156', '1', '5', '3000', '0', 'BagItem1027', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0157', '1', '5', '3000', '0', 'BagItem1028', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0158', '1', '5', '3000', '0', 'BagItem1029', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0159', '1', '5', '3000', '0', 'BagItem1030', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0160', '1', '5', '3000', '0', 'BagItem1031', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0161', '1', '5', '3000', '0', 'BagItem1032', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0162', '1', '5', '3000', '0', 'BagItem1033', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0163', '1', '5', '3000', '0', 'BagItem1034', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0164', '1', '5', '3000', '0', 'BagItem1035', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0165', '1', '5', '3000', '0', 'BagItem1036', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0166', '1', '5', '3000', '0', 'BagItem1037', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0167', '1', '5', '3000', '0', 'BagItem1038', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0168', '1', '5', '3000', '0', 'BagItem1039', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0169', '1', '5', '3000', '0', 'BagItem1040', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0170', '1', '5', '3000', '0', 'BagItem1041', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0171', '1', '5', '3000', '0', 'BagItem1042', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0172', '1', '5', '3000', '0', 'BagItem1043', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0173', '1', '5', '3000', '0', 'BagItem1044', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0174', '1', '5', '3000', '0', 'BagItem1045', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0175', '1', '5', '3000', '0', 'BagItem1046', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0176', '1', '5', '3000', '0', 'BagItem1047', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0177', '1', '5', '3000', '0', 'BagItem1048', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0178', '1', '5', '3000', '0', 'BagItem1049', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0179', '1', '5', '3000', '0', 'BagItem1050', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0180', '1', '5', '3000', '0', 'BagItem1051', '5', '5000');
INSERT INTO `client_config_bag` VALUES ('BagItem0501', '2', '1', '100', '1000', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0502', '2', '2', '200', '2000', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0503', '2', '3', '300', '3000', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0504', '2', '4', '400', '4000', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0505', '2', '5', '500', '5000', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0601', '3', '3', '100', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0701', '4', '1', '1000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0702', '4', '1', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0703', '4', '2', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0704', '4', '2', '4000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0705', '4', '3', '5000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0706', '4', '3', '6000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0707', '4', '4', '7000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0708', '4', '4', '8000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0709', '4', '5', '9000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem0710', '4', '5', '10000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1001', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1002', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1003', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1004', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1005', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1006', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1007', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1008', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1009', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1010', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1011', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1012', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1013', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1014', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1015', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1016', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1017', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1018', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1019', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1020', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1021', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1022', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1023', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1024', '5', '4', '2000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1025', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1026', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1027', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1028', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1029', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1030', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1031', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1032', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1033', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1034', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1035', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1036', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1037', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1038', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1039', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1040', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1041', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1042', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1043', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1044', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1045', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1046', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1047', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1048', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1049', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1050', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem1051', '5', '5', '2500', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2001', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2002', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2003', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2004', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2005', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2006', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2007', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2008', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2009', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2010', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2011', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2012', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2013', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2014', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2015', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2016', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2017', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2018', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2025', '6', '5', '3000', '0', 'NA', 'NA', '0');
INSERT INTO `client_config_bag` VALUES ('BagItem2026', '6', '5', '3000', '0', 'NA', 'NA', '0');

-- ----------------------------
-- Table structure for client_config_battle
-- ----------------------------
DROP TABLE IF EXISTS `client_config_battle`;
CREATE TABLE `client_config_battle` (
  `id` varchar(100) DEFAULT NULL COMMENT 'id',
  `infos` varchar(100) DEFAULT NULL COMMENT '怪物id',
  `positions` varchar(100) DEFAULT NULL COMMENT '位置'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of client_config_battle
-- ----------------------------
INSERT INTO `client_config_battle` VALUES ('Battle0001', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0002', 'Monster001,Monster001,Monster001,Monster001', '3,7,12,8');
INSERT INTO `client_config_battle` VALUES ('Battle0003', 'Monster001,Monster001,Monster001', '3,4,11');
INSERT INTO `client_config_battle` VALUES ('Battle0004', 'Monster001,Monster001,Monster001,Monster001,Monster001', '3,6,8,10,12');
INSERT INTO `client_config_battle` VALUES ('Battle0005', 'Monster001,Monster001,Monster001', '1,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0006', 'Monster001,Monster001,Boss101', '1,3,8');
INSERT INTO `client_config_battle` VALUES ('Battle0007', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0008', 'Monster001,Monster001,Monster001,Monster001', '2,4,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0009', 'Monster001,Monster001,Boss102', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0010', 'Monster001,Monster001,Monster001,Monster001', '3,5,7,9');
INSERT INTO `client_config_battle` VALUES ('Battle0011', 'Monster001,Monster001,Monster001,Boss104', '1,5,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0012', 'Monster001,Monster001,Monster001,Monster001', '2,6,4,10');
INSERT INTO `client_config_battle` VALUES ('Battle0013', 'Monster001,Monster001,Monster001,Monster001', '1,7,5,11');
INSERT INTO `client_config_battle` VALUES ('Battle0014', 'Monster001,Monster001,Monster001,Boss105', '1,6,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0015', 'Monster001_1,Monster001_1,Monster001_1', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0016', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '1,3,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0017', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1', '2,4,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0018', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '3,1,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0019', 'Monster001_2,Monster001_2,Monster001_2', '2,4,12');
INSERT INTO `client_config_battle` VALUES ('Battle0020', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,5,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0021', 'Boss105,Boss104', '8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0022', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0023', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,7,11,12');
INSERT INTO `client_config_battle` VALUES ('Battle0024', 'Boss102,Boss001', '10,5');
INSERT INTO `client_config_battle` VALUES ('Battle0025', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0026', 'Monster001,Monster001,Monster001,Monster001', '3,7,12,8');
INSERT INTO `client_config_battle` VALUES ('Battle0027', 'Monster001,Monster001,Monster001', '3,4,11');
INSERT INTO `client_config_battle` VALUES ('Battle0028', 'Monster001,Monster001,Monster001,Monster001,Monster001', '3,6,8,10,12');
INSERT INTO `client_config_battle` VALUES ('Battle0029', 'Monster001,Monster001,Monster001', '1,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0030', 'Monster001,Monster001,Boss101', '1,3,8');
INSERT INTO `client_config_battle` VALUES ('Battle0031', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0032', 'Monster001,Monster001,Monster001,Monster001', '2,4,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0033', 'Monster001,Monster001,Boss102', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0034', 'Monster001,Monster001,Monster001,Monster001', '3,5,7,9');
INSERT INTO `client_config_battle` VALUES ('Battle0035', 'Monster001,Monster001,Monster001,Boss104', '1,5,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0036', 'Monster001,Monster001,Monster001,Monster001', '2,6,4,10');
INSERT INTO `client_config_battle` VALUES ('Battle0037', 'Monster001,Monster001,Monster001,Monster001', '1,7,5,11');
INSERT INTO `client_config_battle` VALUES ('Battle0038', 'Monster001,Monster001,Monster001,Boss105', '1,6,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0039', 'Monster001_1,Monster001_1,Monster001_1', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0040', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '1,3,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0041', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1', '2,4,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0042', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '3,1,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0043', 'Monster001_2,Monster001_2,Monster001_2', '2,4,12');
INSERT INTO `client_config_battle` VALUES ('Battle0044', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,5,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0045', 'Boss105,Boss104', '8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0046', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0047', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,7,11,12');
INSERT INTO `client_config_battle` VALUES ('Battle0048', 'Boss102,Boss001', '10,5');
INSERT INTO `client_config_battle` VALUES ('Battle0049', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0050', 'Monster001,Monster001,Monster001,Monster001', '3,7,12,8');
INSERT INTO `client_config_battle` VALUES ('Battle0051', 'Monster001,Monster001,Monster001', '3,4,11');
INSERT INTO `client_config_battle` VALUES ('Battle0052', 'Monster001,Monster001,Monster001,Monster001,Monster001', '3,6,8,10,12');
INSERT INTO `client_config_battle` VALUES ('Battle0053', 'Monster001,Monster001,Monster001', '1,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0054', 'Monster001,Monster001,Boss101', '1,3,8');
INSERT INTO `client_config_battle` VALUES ('Battle0055', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0056', 'Monster001,Monster001,Monster001,Monster001', '2,4,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0057', 'Monster001,Monster001,Boss102', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0058', 'Monster001,Monster001,Monster001,Monster001', '3,5,7,9');
INSERT INTO `client_config_battle` VALUES ('Battle0059', 'Monster001,Monster001,Monster001,Boss104', '1,5,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0060', 'Monster001,Monster001,Monster001,Monster001', '2,6,4,10');
INSERT INTO `client_config_battle` VALUES ('Battle0061', 'Monster001,Monster001,Monster001,Monster001', '1,7,5,11');
INSERT INTO `client_config_battle` VALUES ('Battle0062', 'Monster001,Monster001,Monster001,Boss105', '1,6,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0063', 'Monster001_1,Monster001_1,Monster001_1', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0064', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '1,3,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0065', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1', '2,4,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0066', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '3,1,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0067', 'Monster001_2,Monster001_2,Monster001_2', '2,4,12');
INSERT INTO `client_config_battle` VALUES ('Battle0068', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,5,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0069', 'Boss105,Boss104', '8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0070', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0071', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,7,11,12');
INSERT INTO `client_config_battle` VALUES ('Battle0072', 'Boss102,Boss001', '10,5');
INSERT INTO `client_config_battle` VALUES ('Battle0073', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0074', 'Monster001,Monster001,Monster001,Monster001', '3,7,12,8');
INSERT INTO `client_config_battle` VALUES ('Battle0075', 'Monster001,Monster001,Monster001', '3,4,11');
INSERT INTO `client_config_battle` VALUES ('Battle0076', 'Monster001,Monster001,Monster001,Monster001,Monster001', '3,6,8,10,12');
INSERT INTO `client_config_battle` VALUES ('Battle0077', 'Monster001,Monster001,Monster001', '1,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0078', 'Monster001,Monster001,Boss101', '1,3,8');
INSERT INTO `client_config_battle` VALUES ('Battle0079', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0080', 'Monster001,Monster001,Monster001,Monster001', '2,4,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0081', 'Monster001,Monster001,Boss102', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0082', 'Monster001,Monster001,Monster001,Monster001', '3,5,7,9');
INSERT INTO `client_config_battle` VALUES ('Battle0083', 'Monster001,Monster001,Monster001,Boss104', '1,5,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0084', 'Monster001,Monster001,Monster001,Monster001', '2,6,4,10');
INSERT INTO `client_config_battle` VALUES ('Battle0085', 'Monster001,Monster001,Monster001,Monster001', '1,7,5,11');
INSERT INTO `client_config_battle` VALUES ('Battle0086', 'Monster001,Monster001,Monster001,Boss105', '1,6,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0087', 'Monster001_1,Monster001_1,Monster001_1', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0088', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '1,3,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0089', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1', '2,4,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0090', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '3,1,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0091', 'Monster001_2,Monster001_2,Monster001_2', '2,4,12');
INSERT INTO `client_config_battle` VALUES ('Battle0092', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,5,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0093', 'Boss105,Boss104', '8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0094', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0095', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,7,11,12');
INSERT INTO `client_config_battle` VALUES ('Battle0096', 'Boss102,Boss001', '10,5');
INSERT INTO `client_config_battle` VALUES ('Battle0097', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0098', 'Monster001,Monster001,Monster001,Monster001', '3,7,12,8');
INSERT INTO `client_config_battle` VALUES ('Battle0099', 'Monster001,Monster001,Monster001', '3,4,11');
INSERT INTO `client_config_battle` VALUES ('Battle0100', 'Monster001,Monster001,Monster001,Monster001,Monster001', '3,6,8,10,12');
INSERT INTO `client_config_battle` VALUES ('Battle0101', 'Monster001,Monster001,Monster001', '1,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0102', 'Monster001,Monster001,Boss101', '1,3,8');
INSERT INTO `client_config_battle` VALUES ('Battle0103', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0104', 'Monster001,Monster001,Monster001,Monster001', '2,4,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0105', 'Monster001,Monster001,Boss102', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0106', 'Monster001,Monster001,Monster001,Monster001', '3,5,7,9');
INSERT INTO `client_config_battle` VALUES ('Battle0107', 'Monster001,Monster001,Monster001,Boss104', '1,5,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0108', 'Monster001,Monster001,Monster001,Monster001', '2,6,4,10');
INSERT INTO `client_config_battle` VALUES ('Battle0109', 'Monster001,Monster001,Monster001,Monster001', '1,7,5,11');
INSERT INTO `client_config_battle` VALUES ('Battle0110', 'Monster001,Monster001,Monster001,Boss105', '1,6,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0111', 'Monster001_1,Monster001_1,Monster001_1', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0112', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '1,3,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0113', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1', '2,4,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0114', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '3,1,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0115', 'Monster001_2,Monster001_2,Monster001_2', '2,4,12');
INSERT INTO `client_config_battle` VALUES ('Battle0116', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,5,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0117', 'Boss105,Boss104', '8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0118', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0119', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,7,11,12');
INSERT INTO `client_config_battle` VALUES ('Battle0120', 'Boss102,Boss001', '10,5');
INSERT INTO `client_config_battle` VALUES ('Battle0121', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0122', 'Monster001,Monster001,Monster001,Monster001', '3,7,12,8');
INSERT INTO `client_config_battle` VALUES ('Battle0123', 'Monster001,Monster001,Monster001', '3,4,11');
INSERT INTO `client_config_battle` VALUES ('Battle0124', 'Monster001,Monster001,Monster001,Monster001,Monster001', '3,6,8,10,12');
INSERT INTO `client_config_battle` VALUES ('Battle0125', 'Monster001,Monster001,Monster001', '1,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0126', 'Monster001,Monster001,Boss101', '1,3,8');
INSERT INTO `client_config_battle` VALUES ('Battle0127', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0128', 'Monster001,Monster001,Monster001,Monster001', '2,4,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0129', 'Monster001,Monster001,Boss102', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0130', 'Monster001,Monster001,Monster001,Monster001', '3,5,7,9');
INSERT INTO `client_config_battle` VALUES ('Battle0131', 'Monster001,Monster001,Monster001,Boss104', '1,5,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0132', 'Monster001,Monster001,Monster001,Monster001', '2,6,4,10');
INSERT INTO `client_config_battle` VALUES ('Battle0133', 'Monster001,Monster001,Monster001,Monster001', '1,7,5,11');
INSERT INTO `client_config_battle` VALUES ('Battle0134', 'Monster001,Monster001,Monster001,Boss105', '1,6,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0135', 'Monster001_1,Monster001_1,Monster001_1', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0136', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '1,3,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0137', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1', '2,4,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0138', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '3,1,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0139', 'Monster001_2,Monster001_2,Monster001_2', '2,4,12');
INSERT INTO `client_config_battle` VALUES ('Battle0140', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,5,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0141', 'Boss105,Boss104', '8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0142', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0143', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,7,11,12');
INSERT INTO `client_config_battle` VALUES ('Battle0144', 'Boss102,Boss001', '10,5');
INSERT INTO `client_config_battle` VALUES ('Battle0145', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0146', 'Monster001,Monster001,Monster001,Monster001', '3,7,12,8');
INSERT INTO `client_config_battle` VALUES ('Battle0147', 'Monster001,Monster001,Monster001', '3,4,11');
INSERT INTO `client_config_battle` VALUES ('Battle0148', 'Monster001,Monster001,Monster001,Monster001,Monster001', '3,6,8,10,12');
INSERT INTO `client_config_battle` VALUES ('Battle0149', 'Monster001,Monster001,Monster001', '1,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0150', 'Monster001,Monster001,Boss101', '1,3,8');
INSERT INTO `client_config_battle` VALUES ('Battle0151', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0152', 'Monster001,Monster001,Monster001,Monster001', '2,4,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0153', 'Monster001,Monster001,Boss102', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0154', 'Monster001,Monster001,Monster001,Monster001', '3,5,7,9');
INSERT INTO `client_config_battle` VALUES ('Battle0155', 'Monster001,Monster001,Monster001,Boss104', '1,5,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0156', 'Monster001,Monster001,Monster001,Monster001', '2,6,4,10');
INSERT INTO `client_config_battle` VALUES ('Battle0157', 'Monster001,Monster001,Monster001,Monster001', '1,7,5,11');
INSERT INTO `client_config_battle` VALUES ('Battle0158', 'Monster001,Monster001,Monster001,Boss105', '1,6,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0159', 'Monster001_1,Monster001_1,Monster001_1', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0160', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '1,3,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0161', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1', '2,4,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0162', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '3,1,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0163', 'Monster001_2,Monster001_2,Monster001_2', '2,4,12');
INSERT INTO `client_config_battle` VALUES ('Battle0164', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,5,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0165', 'Boss105,Boss104', '8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0166', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0167', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,7,11,12');
INSERT INTO `client_config_battle` VALUES ('Battle0168', 'Boss102,Boss001', '10,5');
INSERT INTO `client_config_battle` VALUES ('Battle0169', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0170', 'Monster001,Monster001,Monster001,Monster001', '3,7,12,8');
INSERT INTO `client_config_battle` VALUES ('Battle0171', 'Monster001,Monster001,Monster001', '3,4,11');
INSERT INTO `client_config_battle` VALUES ('Battle0172', 'Monster001,Monster001,Monster001,Monster001,Monster001', '3,6,8,10,12');
INSERT INTO `client_config_battle` VALUES ('Battle0173', 'Monster001,Monster001,Monster001', '1,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0174', 'Monster001,Monster001,Boss101', '1,3,8');
INSERT INTO `client_config_battle` VALUES ('Battle0175', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0176', 'Monster001,Monster001,Monster001,Monster001', '2,4,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0177', 'Monster001,Monster001,Boss102', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0178', 'Monster001,Monster001,Monster001,Monster001', '3,5,7,9');
INSERT INTO `client_config_battle` VALUES ('Battle0179', 'Monster001,Monster001,Monster001,Boss104', '1,5,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0180', 'Monster001,Monster001,Monster001,Monster001', '2,6,4,10');
INSERT INTO `client_config_battle` VALUES ('Battle0181', 'Monster001,Monster001,Monster001,Monster001', '1,7,5,11');
INSERT INTO `client_config_battle` VALUES ('Battle0182', 'Monster001,Monster001,Monster001,Boss105', '1,6,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0183', 'Monster001_1,Monster001_1,Monster001_1', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0184', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '1,3,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0185', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1', '2,4,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0186', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '3,1,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0187', 'Monster001_2,Monster001_2,Monster001_2', '2,4,12');
INSERT INTO `client_config_battle` VALUES ('Battle0188', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,5,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0189', 'Boss105,Boss104', '8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0190', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0191', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,7,11,12');
INSERT INTO `client_config_battle` VALUES ('Battle0192', 'Boss102,Boss001', '10,5');
INSERT INTO `client_config_battle` VALUES ('Battle0193', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0194', 'Monster001,Monster001,Monster001,Monster001', '3,7,12,8');
INSERT INTO `client_config_battle` VALUES ('Battle0195', 'Monster001,Monster001,Monster001', '3,4,11');
INSERT INTO `client_config_battle` VALUES ('Battle0196', 'Monster001,Monster001,Monster001,Monster001,Monster001', '3,6,8,10,12');
INSERT INTO `client_config_battle` VALUES ('Battle0197', 'Monster001,Monster001,Monster001', '1,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0198', 'Monster001,Monster001,Boss101', '1,3,8');
INSERT INTO `client_config_battle` VALUES ('Battle0199', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0200', 'Monster001,Monster001,Monster001,Monster001', '2,4,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0201', 'Monster001,Monster001,Boss102', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0202', 'Monster001,Monster001,Monster001,Monster001', '3,5,7,9');
INSERT INTO `client_config_battle` VALUES ('Battle0203', 'Monster001,Monster001,Monster001,Boss104', '1,5,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0204', 'Monster001,Monster001,Monster001,Monster001', '2,6,4,10');
INSERT INTO `client_config_battle` VALUES ('Battle0205', 'Monster001,Monster001,Monster001,Monster001', '1,7,5,11');
INSERT INTO `client_config_battle` VALUES ('Battle0206', 'Monster001,Monster001,Monster001,Boss105', '1,6,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0207', 'Monster001_1,Monster001_1,Monster001_1', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0208', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '1,3,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0209', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1', '2,4,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0210', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '3,1,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0211', 'Monster001_2,Monster001_2,Monster001_2', '2,4,12');
INSERT INTO `client_config_battle` VALUES ('Battle0212', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,5,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0213', 'Boss105,Boss104', '8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0214', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0215', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,7,11,12');
INSERT INTO `client_config_battle` VALUES ('Battle0216', 'Boss102,Boss001', '10,5');
INSERT INTO `client_config_battle` VALUES ('Battle0217', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0218', 'Monster001,Monster001,Monster001,Monster001', '3,7,12,8');
INSERT INTO `client_config_battle` VALUES ('Battle0219', 'Monster001,Monster001,Monster001', '3,4,11');
INSERT INTO `client_config_battle` VALUES ('Battle0220', 'Monster001,Monster001,Monster001,Monster001,Monster001', '3,6,8,10,12');
INSERT INTO `client_config_battle` VALUES ('Battle0221', 'Monster001,Monster001,Monster001', '1,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0222', 'Monster001,Monster001,Boss101', '1,3,8');
INSERT INTO `client_config_battle` VALUES ('Battle0223', 'Monster001,Monster001,Monster001', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0224', 'Monster001,Monster001,Monster001,Monster001', '2,4,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0225', 'Monster001,Monster001,Boss102', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0226', 'Monster001,Monster001,Monster001,Monster001', '3,5,7,9');
INSERT INTO `client_config_battle` VALUES ('Battle0227', 'Monster001,Monster001,Monster001,Boss104', '1,5,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0228', 'Monster001,Monster001,Monster001,Monster001', '2,6,4,10');
INSERT INTO `client_config_battle` VALUES ('Battle0229', 'Monster001,Monster001,Monster001,Monster001', '1,7,5,11');
INSERT INTO `client_config_battle` VALUES ('Battle0230', 'Monster001,Monster001,Monster001,Boss105', '1,6,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0231', 'Monster001_1,Monster001_1,Monster001_1', '1,5,9');
INSERT INTO `client_config_battle` VALUES ('Battle0232', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '1,3,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0233', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1', '2,4,9,11');
INSERT INTO `client_config_battle` VALUES ('Battle0234', 'Monster001_1,Monster001_1,Monster001_1,Monster001_1,Monster001_1', '3,1,5,7,11');
INSERT INTO `client_config_battle` VALUES ('Battle0235', 'Monster001_2,Monster001_2,Monster001_2', '2,4,12');
INSERT INTO `client_config_battle` VALUES ('Battle0236', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,5,9,10');
INSERT INTO `client_config_battle` VALUES ('Battle0237', 'Boss105,Boss104', '8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0238', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,8,10');
INSERT INTO `client_config_battle` VALUES ('Battle0239', 'Monster001_2,Monster001_2,Monster001_2,Monster001_2,Monster001_2', '1,6,7,11,12');
INSERT INTO `client_config_battle` VALUES ('Battle0240', 'Boss102,Boss001', '10,5');
INSERT INTO `client_config_battle` VALUES ('Battle0241', 'Boss102,Boss001', '10,5');

-- ----------------------------
-- Table structure for client_config_bossprop
-- ----------------------------
DROP TABLE IF EXISTS `client_config_bossprop`;
CREATE TABLE `client_config_bossprop` (
  `id` varchar(100) DEFAULT NULL COMMENT 'ID',
  `pointId` varchar(100) DEFAULT NULL COMMENT '对应英雄',
  `area` int(10) DEFAULT NULL COMMENT '国家',
  `occupation` int(10) DEFAULT NULL COMMENT '职业',
  `level` int(10) DEFAULT NULL COMMENT '等级',
  `health` int(10) DEFAULT NULL COMMENT '生命',
  `damage` int(10) DEFAULT NULL COMMENT '攻击',
  `pdd` int(10) DEFAULT NULL COMMENT '护甲',
  `mdd` int(10) DEFAULT NULL COMMENT '魔抗',
  `hitLevel` int(10) DEFAULT NULL COMMENT '命中等级',
  `dodgeLevel` int(10) DEFAULT NULL COMMENT '闪避等级',
  `criLevel` int(10) DEFAULT NULL COMMENT '暴击等级',
  `tenacityLevel` int(10) DEFAULT NULL COMMENT '韧性等级',
  `pddPierce` int(10) DEFAULT NULL COMMENT '穿透',
  `passiveSkillId` varchar(100) DEFAULT NULL COMMENT '被动技能',
  `passiveSkillLevel` varchar(100) DEFAULT NULL COMMENT '被动技能等级',
  `range_X` float DEFAULT NULL COMMENT '普通攻击伤害范围 X',
  `range_Y` float DEFAULT NULL COMMENT '普通攻击伤害范围 Y',
  `range_Z` float DEFAULT NULL COMMENT '普通攻击伤害范围 Z',
  `attackSpeed` float DEFAULT NULL COMMENT '攻击速度（m/s）',
  `h_Dis` float DEFAULT NULL COMMENT '水平攻击距离（m）',
  `v_Dis` float DEFAULT NULL COMMENT '垂直攻击距离（m）',
  `moveSpeed` float DEFAULT NULL COMMENT '移动速度(m/s)',
  `star` int(10) DEFAULT NULL COMMENT '星级',
  `stage_B` int(10) DEFAULT NULL COMMENT '必杀阶段数',
  `hitNum_B` varchar(100) DEFAULT NULL COMMENT '必杀每阶段对应hit数',
  `skillTimeList` varchar(100) DEFAULT NULL COMMENT 'BOSS的必杀释放时间顺序',
  `warningTimeList` varchar(100) DEFAULT NULL COMMENT 'BOSS的必杀预警时间',
  `maxIntensity` int(10) DEFAULT NULL COMMENT '初始强度',
  `recoverIntensityRate` int(10) DEFAULT NULL COMMENT '每秒强度恢复（num/s）',
  `minIntensity` int(10) DEFAULT NULL COMMENT '硬直临界值（必杀可浮空和击退）',
  `type_Normal` int(10) DEFAULT NULL COMMENT '攻击类型'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of client_config_bossprop
-- ----------------------------
INSERT INTO `client_config_bossprop` VALUES ('Boss001', 'NA', '1', '1', '1', '15000', '200', '100', '100', '100', '100', '100', '100', '0', 'PassiveSkill001,PassiveSkill002,PassiveSkill003', '1,2,3', '0', '0', '0', '2', '2', '2', '3', '1', '2', '3,1', '4,7,5,6,8', '4,2,4,2,4', '100', '5', '0', '1');
INSERT INTO `client_config_bossprop` VALUES ('Boss001_1', 'NA', '1', '1', '2', '10000', '260', '100', '100', '100', '100', '100', '100', '0', 'NA', 'NA', '0', '0', '0', '3', '2', '2', '3', '1', '2', '3,1', '4,7,5,6,8', '4,2,4,2,4', '100', '7', '0', '1');
INSERT INTO `client_config_bossprop` VALUES ('Boss101', 'Hero001', '1', '1', '3', '20000', '150', '100', '100', '100', '100', '100', '100', '0', 'NA', 'NA', '0', '0', '0', '1.5', '1.5', '1', '3', '1', '1', '5', '5,8,4,6,7', '3,3,3,3,3', '100', '6', '0', '1');
INSERT INTO `client_config_bossprop` VALUES ('Boss102', 'Hero002', '1', '2', '4', '15000', '90', '100', '100', '100', '100', '100', '100', '0', 'NA', 'NA', '0', '0', '0', '1.3', '10', '7', '3', '1', '1', '6', '4,7,6,5,5', '3,3,3,3,3', '100', '5', '0', '1');
INSERT INTO `client_config_bossprop` VALUES ('Boss104', 'Hero004', '1', '3', '5', '12000', '110', '100', '100', '100', '100', '100', '100', '0', 'NA', 'NA', '0', '0', '0', '1.3', '9', '6', '3', '1', '3', '2,2,2', '5,6,4,7,5', '2.5,2.5,2.5,2.5,2.5', '100', '6', '0', '1');
INSERT INTO `client_config_bossprop` VALUES ('Boss105', 'Hero005', '1', '1', '6', '10000', '150', '100', '100', '100', '100', '100', '100', '0', 'NA', 'NA', '0', '0', '0', '1', '2', '2', '3', '1', '1', '5', '4,5,6,6,5', '2,1.5,1.5,2,2', '100', '5', '0', '1');

-- ----------------------------
-- Table structure for client_config_bossskill
-- ----------------------------
DROP TABLE IF EXISTS `client_config_bossskill`;
CREATE TABLE `client_config_bossskill` (
  `id` varchar(100) DEFAULT NULL COMMENT 'ID',
  `factor` float DEFAULT NULL COMMENT '伤害系数',
  `distance` float DEFAULT NULL COMMENT '击退距离',
  `height` float DEFAULT NULL COMMENT '浮空高度',
  `range_x` float DEFAULT NULL COMMENT '伤害范围-X',
  `range_y` float DEFAULT NULL COMMENT '伤害范围-Y',
  `range_z` float DEFAULT NULL COMMENT '伤害范围-Z',
  `waitTime` float DEFAULT NULL COMMENT 'hit时间间隔'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of client_config_bossskill
-- ----------------------------
INSERT INTO `client_config_bossskill` VALUES ('Boss001_B_Hit_A_0', '0.6', '0', '0', '1', '2', '3', '0');
INSERT INTO `client_config_bossskill` VALUES ('Boss001_B_Hit_A_1', '0.7', '0', '0', '1', '2', '3', '0');
INSERT INTO `client_config_bossskill` VALUES ('Boss001_B_Hit_A_2', '1', '0', '7', '100', '10', '3', '0');
INSERT INTO `client_config_bossskill` VALUES ('Boss001_B_Hit_B_0', '0.8', '2', '0', '3', '2', '3', '0');
INSERT INTO `client_config_bossskill` VALUES ('Boss101_B_Hit_A_0', '0.6', '1', '1', '1', '2', '2', '0');
INSERT INTO `client_config_bossskill` VALUES ('Boss101_B_Hit_A_1', '0.8', '0', '0', '1', '2', '2', '0.25');
INSERT INTO `client_config_bossskill` VALUES ('Boss101_B_Hit_A_2', '0.8', '0', '0', '1', '2', '2', '0.25');
INSERT INTO `client_config_bossskill` VALUES ('Boss101_B_Hit_A_3', '0.8', '0', '0', '1', '2', '2', '0.25');
INSERT INTO `client_config_bossskill` VALUES ('Boss101_B_Hit_A_4', '0.8', '0', '0', '1', '2', '2', '0.25');
INSERT INTO `client_config_bossskill` VALUES ('Boss102_B_Hit_A_0', '0.8', '0', '0', '1', '0.75', '1', '0');
INSERT INTO `client_config_bossskill` VALUES ('Boss102_B_Hit_A_1', '0.8', '0', '0', '1', '0.75', '1', '0.15');
INSERT INTO `client_config_bossskill` VALUES ('Boss102_B_Hit_A_2', '0.8', '0', '0', '0.5', '0.75', '1', '0.15');
INSERT INTO `client_config_bossskill` VALUES ('Boss102_B_Hit_A_3', '0.8', '0', '0', '0.5', '0.75', '1', '0.15');
INSERT INTO `client_config_bossskill` VALUES ('Boss102_B_Hit_A_4', '1.2', '0', '0.5', '0.5', '0.5', '1', '0.15');
INSERT INTO `client_config_bossskill` VALUES ('Boss102_B_Hit_A_5', '1.2', '0', '2', '0.1', '0.1', '0.1', '0.2');
INSERT INTO `client_config_bossskill` VALUES ('Boss104_B_Hit_A_0', '0.5', '0', '0', '1', '0.75', '0.5', '0');
INSERT INTO `client_config_bossskill` VALUES ('Boss104_B_Hit_A_1', '0.6', '0.5', '1', '1', '0.75', '0.5', '0.15');
INSERT INTO `client_config_bossskill` VALUES ('Boss104_B_Hit_B_0', '0.7', '0', '0', '1', '0.75', '0.5', '0');
INSERT INTO `client_config_bossskill` VALUES ('Boss104_B_Hit_B_1', '0.8', '0.5', '1', '1', '0.75', '0.5', '0.15');
INSERT INTO `client_config_bossskill` VALUES ('Boss104_B_Hit_C_0', '0.9', '0', '0', '1', '0.75', '0.5', '0');
INSERT INTO `client_config_bossskill` VALUES ('Boss104_B_Hit_C_1', '1', '0.5', '1', '1', '0.75', '0.5', '0.15');
INSERT INTO `client_config_bossskill` VALUES ('Boss105_B_Hit_A_0', '0.8', '0', '1.5', '0.5', '1', '1', '0');
INSERT INTO `client_config_bossskill` VALUES ('Boss105_B_Hit_A_1', '0.8', '0', '1.3', '0.5', '0.5', '1', '0.2');
INSERT INTO `client_config_bossskill` VALUES ('Boss105_B_Hit_A_2', '0.8', '0', '1.2', '0.5', '0.5', '1', '0.2');
INSERT INTO `client_config_bossskill` VALUES ('Boss105_B_Hit_A_3', '0.8', '0', '1.1', '0.5', '0.5', '1', '0.2');
INSERT INTO `client_config_bossskill` VALUES ('Boss105_B_Hit_A_4', '0.8', '0', '1', '0.5', '0.5', '1', '0.2');

-- ----------------------------
-- Table structure for client_config_chaptermap
-- ----------------------------
DROP TABLE IF EXISTS `client_config_chaptermap`;
CREATE TABLE `client_config_chaptermap` (
  `id` varchar(100) DEFAULT NULL COMMENT 'id',
  `capterType` int(10) DEFAULT NULL COMMENT '所属大章',
  `pointType` int(10) DEFAULT NULL COMMENT '类型',
  `openLevel` int(10) DEFAULT NULL COMMENT '开放等级'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of client_config_chaptermap
-- ----------------------------
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint001', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint002', '1', '1', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint003', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint004', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint005', '1', '1', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint006', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint007', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint008', '1', '1', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint009', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint010', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint011', '1', '1', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint012', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint013', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint014', '1', '1', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint015', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint016', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint017', '1', '1', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint018', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint019', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint020', '1', '1', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint021', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint022', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint023', '1', '1', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint024', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint025', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint026', '1', '1', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint027', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint028', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint029', '1', '1', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint030', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint031', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint032', '1', '1', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint033', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint034', '1', '2', '0');
INSERT INTO `client_config_chaptermap` VALUES ('MapPoint035', '1', '1', '0');

-- ----------------------------
-- Table structure for client_config_equip
-- ----------------------------
DROP TABLE IF EXISTS `client_config_equip`;
CREATE TABLE `client_config_equip` (
  `id` varchar(100) DEFAULT NULL COMMENT '英雄装备id',
  `pos` int(10) DEFAULT NULL COMMENT '装备部位',
  `quality` int(10) DEFAULT NULL COMMENT '品质',
  `health` int(10) DEFAULT NULL COMMENT '生命',
  `healthGrow` int(10) DEFAULT NULL COMMENT '生命成长',
  `damage` int(10) DEFAULT NULL COMMENT '攻击',
  `damageGrow` int(10) DEFAULT NULL COMMENT '攻击成长',
  `pdd` int(10) DEFAULT NULL COMMENT '护甲',
  `pddGrow` int(10) DEFAULT NULL COMMENT '护甲成长',
  `mdd` int(10) DEFAULT NULL COMMENT '魔抗',
  `mddGrow` int(10) DEFAULT NULL COMMENT '魔抗成长',
  `hitLevel` int(10) DEFAULT NULL COMMENT '命中等级',
  `hitGrow` int(10) DEFAULT NULL COMMENT '命中成长',
  `dodgeLevel` int(10) DEFAULT NULL COMMENT '闪避等级',
  `dodgeGrow` int(10) DEFAULT NULL COMMENT '闪避成长',
  `criLevel` int(10) DEFAULT NULL COMMENT '暴击等级',
  `criGrow` int(10) DEFAULT NULL COMMENT '暴击成长',
  `tenacityLevel` int(10) DEFAULT NULL COMMENT '韧性等级',
  `tenacityGrow` int(10) DEFAULT NULL COMMENT '韧性成长',
  `pddPierce` int(10) DEFAULT NULL COMMENT '穿透',
  `pddPierceGrow` int(10) DEFAULT NULL COMMENT '穿透成长',
  `needs` varchar(100) DEFAULT NULL COMMENT '进化需求材料',
  `needCounts` varchar(100) DEFAULT NULL COMMENT '材料数量',
  `nextId` varchar(100) DEFAULT NULL COMMENT '进化后英雄装备',
  `icon` varchar(100) DEFAULT NULL COMMENT '图标ID'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of client_config_equip
-- ----------------------------
INSERT INTO `client_config_equip` VALUES ('HeroEquip0001', '1', '1', '40', '10', '20', '4', '0', '0', '0', '0', '1', '0', '0', '0', '2', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0002', 'EqIcon001');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0002', '1', '2', '50', '11', '25', '5', '0', '0', '0', '0', '5', '0', '0', '0', '7', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0003', 'EqIcon002');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0003', '1', '3', '60', '12', '30', '6', '0', '0', '0', '0', '9', '0', '0', '0', '15', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0004', 'EqIcon002');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0004', '1', '4', '75', '14', '40', '8', '0', '0', '0', '0', '9', '1', '0', '0', '15', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0020', '3|3', 'HeroEquip0005', 'EqIcon003');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0005', '1', '5', '90', '16', '50', '10', '0', '0', '0', '0', '13', '1', '0', '0', '30', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0020', '2|4', 'HeroEquip0006', 'EqIcon003');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0006', '1', '6', '105', '18', '60', '12', '0', '0', '0', '0', '17', '1', '0', '0', '45', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0020', '2|2|4', 'HeroEquip0007', 'EqIcon003');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0007', '1', '7', '130', '21', '75', '15', '0', '0', '0', '0', '17', '2', '0', '0', '45', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0037', '3|3', 'HeroEquip0008', 'EqIcon004');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0008', '1', '8', '155', '24', '90', '18', '0', '0', '0', '0', '22', '2', '0', '0', '65', '4', '0', '0', '53', '2', 'BagItem0041|BagItem0037', '2|4', 'HeroEquip0009', 'EqIcon004');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0009', '1', '9', '180', '27', '105', '21', '0', '0', '0', '0', '27', '2', '0', '0', '85', '4', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0037', '2|2|6', 'HeroEquip0010', 'EqIcon004');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0010', '1', '10', '205', '30', '120', '24', '0', '0', '0', '0', '32', '2', '0', '0', '105', '4', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0060', '3|2|1', 'HeroEquip0011', 'EqIcon004');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0011', '1', '11', '235', '35', '142', '27', '0', '0', '0', '0', '42', '2', '0', '0', '105', '5', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0021', '1', '1', '40', '13', '5', '4', '0', '0', '0', '0', '3', '0', '0', '0', '8', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0022', 'EqIcon006');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0022', '1', '2', '50', '14', '8', '5', '0', '0', '0', '0', '8', '0', '0', '0', '12', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0023', 'EqIcon007');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0023', '1', '3', '60', '15', '17', '6', '0', '0', '0', '0', '12', '0', '0', '0', '16', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0024', 'EqIcon007');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0024', '1', '4', '70', '16', '27', '8', '0', '0', '0', '0', '12', '1', '0', '0', '21', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0021', '3|3', 'HeroEquip0025', 'EqIcon008');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0025', '1', '5', '90', '17', '37', '10', '0', '0', '0', '0', '16', '1', '0', '0', '26', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0021', '2|4', 'HeroEquip0026', 'EqIcon008');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0026', '1', '6', '110', '18', '47', '12', '0', '0', '0', '0', '20', '1', '0', '0', '31', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0021', '2|2|4', 'HeroEquip0027', 'EqIcon008');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0027', '1', '7', '130', '19', '59', '15', '0', '0', '0', '0', '20', '2', '0', '0', '42', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0038', '3|3', 'HeroEquip0028', 'EqIcon009');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0028', '1', '8', '150', '21', '71', '18', '0', '0', '0', '0', '25', '2', '0', '0', '53', '5', '0', '0', '53', '2', 'BagItem0041|BagItem0038', '2|4', 'HeroEquip0029', 'EqIcon009');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0029', '1', '9', '170', '23', '83', '20', '0', '0', '0', '0', '30', '2', '0', '0', '64', '6', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0038', '2|2|6', 'HeroEquip0030', 'EqIcon009');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0030', '1', '10', '190', '26', '106', '22', '0', '0', '0', '0', '35', '2', '0', '0', '76', '7', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0061', '3|2|1', 'HeroEquip0031', 'EqIcon009');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0031', '1', '11', '210', '30', '130', '25', '0', '0', '0', '0', '45', '2', '0', '0', '88', '8', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0041', '1', '1', '30', '8', '5', '4', '0', '0', '0', '0', '10', '0', '0', '0', '7', '2', '0', '0', '2', '0', 'BagItem0001', '1', 'HeroEquip0042', 'EqIcon011');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0042', '1', '2', '40', '10', '10', '5', '0', '0', '0', '0', '15', '0', '0', '0', '9', '3', '0', '0', '3', '0', 'BagItem0007', '2', 'HeroEquip0043', 'EqIcon012');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0043', '1', '3', '50', '12', '15', '6', '0', '0', '0', '0', '20', '0', '0', '0', '11', '3', '0', '0', '4', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0044', 'EqIcon012');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0044', '1', '4', '80', '15', '20', '8', '0', '0', '0', '0', '20', '1', '0', '0', '11', '4', '0', '0', '4', '1', 'BagItem0013|BagItem0023', '3|3', 'HeroEquip0045', 'EqIcon013');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0045', '1', '5', '110', '18', '35', '10', '0', '0', '0', '0', '26', '1', '0', '0', '14', '4', '0', '0', '5', '1', 'BagItem0024|BagItem0023', '2|4', 'HeroEquip0046', 'EqIcon013');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0046', '1', '6', '140', '23', '50', '12', '0', '0', '0', '0', '32', '1', '0', '0', '17', '4', '0', '0', '6', '1', 'BagItem0024|BagItem0030|BagItem0023', '2|2|4', 'HeroEquip0047', 'EqIcon013');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0047', '1', '7', '180', '28', '70', '15', '0', '0', '0', '0', '32', '2', '0', '0', '17', '5', '0', '0', '6', '2', 'BagItem0030|BagItem0040', '3|3', 'HeroEquip0048', 'EqIcon014');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0048', '1', '8', '220', '36', '90', '18', '0', '0', '0', '0', '39', '2', '0', '0', '20', '5', '0', '0', '7', '2', 'BagItem0041|BagItem0040', '2|4', 'HeroEquip0049', 'EqIcon014');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0049', '1', '9', '260', '44', '110', '20', '0', '0', '0', '0', '46', '2', '0', '0', '23', '5', '0', '0', '8', '2', 'BagItem0041|BagItem0047|BagItem0040', '2|2|6', 'HeroEquip0050', 'EqIcon014');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0050', '1', '10', '300', '52', '130', '22', '0', '0', '0', '0', '53', '2', '0', '0', '26', '5', '0', '0', '9', '2', 'BagItem0047|BagItem0053|BagItem0063', '3|2|1', 'HeroEquip0051', 'EqIcon014');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0051', '1', '11', '365', '65', '150', '25', '0', '0', '0', '0', '53', '3', '0', '0', '26', '6', '0', '0', '10', '2', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0061', '1', '1', '40', '13', '5', '4', '0', '0', '0', '0', '3', '0', '0', '0', '8', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0062', 'EqIcon016');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0062', '1', '2', '50', '14', '8', '5', '0', '0', '0', '0', '8', '0', '0', '0', '12', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0063', 'EqIcon017');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0063', '1', '3', '60', '15', '17', '6', '0', '0', '0', '0', '12', '0', '0', '0', '16', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0064', 'EqIcon017');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0064', '1', '4', '70', '16', '27', '8', '0', '0', '0', '0', '12', '1', '0', '0', '21', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0022', '3|3', 'HeroEquip0065', 'EqIcon018');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0065', '1', '5', '90', '17', '37', '10', '0', '0', '0', '0', '16', '1', '0', '0', '26', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0022', '2|4', 'HeroEquip0066', 'EqIcon018');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0066', '1', '6', '110', '18', '47', '12', '0', '0', '0', '0', '20', '1', '0', '0', '31', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0022', '2|2|4', 'HeroEquip0067', 'EqIcon018');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0067', '1', '7', '130', '19', '59', '15', '0', '0', '0', '0', '20', '2', '0', '0', '42', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0039', '3|3', 'HeroEquip0068', 'EqIcon019');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0068', '1', '8', '150', '21', '71', '18', '0', '0', '0', '0', '25', '2', '0', '0', '53', '5', '0', '0', '53', '2', 'BagItem0041|BagItem0039', '2|4', 'HeroEquip0069', 'EqIcon019');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0069', '1', '9', '170', '23', '83', '20', '0', '0', '0', '0', '30', '2', '0', '0', '64', '6', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0039', '2|2|6', 'HeroEquip0070', 'EqIcon019');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0070', '1', '10', '190', '26', '106', '22', '0', '0', '0', '0', '35', '2', '0', '0', '76', '7', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0062', '3|2|1', 'HeroEquip0071', 'EqIcon019');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0071', '1', '11', '210', '30', '130', '25', '0', '0', '0', '0', '45', '2', '0', '0', '88', '8', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0081', '1', '1', '40', '10', '20', '4', '0', '0', '0', '0', '1', '0', '0', '0', '2', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0082', 'EqIcon056');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0082', '1', '2', '50', '11', '25', '5', '0', '0', '0', '0', '5', '0', '0', '0', '7', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0083', 'EqIcon057');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0083', '1', '3', '60', '12', '30', '6', '0', '0', '0', '0', '9', '0', '0', '0', '15', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0084', 'EqIcon057');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0084', '1', '4', '75', '14', '40', '8', '0', '0', '0', '0', '9', '1', '0', '0', '15', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0019', '3|3', 'HeroEquip0085', 'EqIcon058');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0085', '1', '5', '90', '16', '50', '10', '0', '0', '0', '0', '13', '1', '0', '0', '30', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0019', '2|4', 'HeroEquip0086', 'EqIcon058');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0086', '1', '6', '105', '18', '60', '12', '0', '0', '0', '0', '17', '1', '0', '0', '45', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0019', '2|2|4', 'HeroEquip0087', 'EqIcon058');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0087', '1', '7', '130', '21', '75', '15', '0', '0', '0', '0', '17', '2', '0', '0', '45', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0036', '3|3', 'HeroEquip0088', 'EqIcon059');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0088', '1', '8', '155', '24', '90', '18', '0', '0', '0', '0', '22', '2', '0', '0', '65', '4', '0', '0', '53', '2', 'BagItem0041|BagItem0036', '2|4', 'HeroEquip0089', 'EqIcon059');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0089', '1', '9', '180', '27', '105', '21', '0', '0', '0', '0', '27', '2', '0', '0', '85', '4', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0036', '2|2|6', 'HeroEquip0090', 'EqIcon059');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0090', '1', '10', '205', '30', '120', '24', '0', '0', '0', '0', '32', '2', '0', '0', '105', '4', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0059', '3|2|1', 'HeroEquip0091', 'EqIcon059');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0091', '1', '11', '235', '35', '142', '27', '0', '0', '0', '0', '42', '2', '0', '0', '105', '5', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0101', '1', '1', '40', '10', '20', '4', '0', '0', '0', '0', '1', '0', '0', '0', '2', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0102', 'EqIcon021');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0102', '1', '2', '50', '11', '25', '5', '0', '0', '0', '0', '5', '0', '0', '0', '7', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0103', 'EqIcon022');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0103', '1', '3', '60', '12', '30', '6', '0', '0', '0', '0', '9', '0', '0', '0', '15', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0104', 'EqIcon022');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0104', '1', '4', '75', '14', '40', '8', '0', '0', '0', '0', '9', '1', '0', '0', '15', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0019', '3|3', 'HeroEquip0105', 'EqIcon023');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0105', '1', '5', '90', '16', '50', '10', '0', '0', '0', '0', '13', '1', '0', '0', '30', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0019', '2|4', 'HeroEquip0106', 'EqIcon023');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0106', '1', '6', '105', '18', '60', '12', '0', '0', '0', '0', '17', '1', '0', '0', '45', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0019', '2|2|4', 'HeroEquip0107', 'EqIcon023');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0107', '1', '7', '130', '21', '75', '15', '0', '0', '0', '0', '17', '2', '0', '0', '45', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0036', '3|3', 'HeroEquip0108', 'EqIcon024');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0108', '1', '8', '155', '24', '90', '18', '0', '0', '0', '0', '22', '2', '0', '0', '65', '4', '0', '0', '53', '2', 'BagItem0041|BagItem0036', '2|4', 'HeroEquip0109', 'EqIcon024');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0109', '1', '9', '180', '27', '105', '21', '0', '0', '0', '0', '27', '2', '0', '0', '85', '4', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0036', '2|2|6', 'HeroEquip0110', 'EqIcon024');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0110', '1', '10', '205', '30', '120', '24', '0', '0', '0', '0', '32', '2', '0', '0', '105', '4', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0059', '3|2|1', 'HeroEquip0111', 'EqIcon024');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0111', '1', '11', '235', '35', '142', '27', '0', '0', '0', '0', '42', '2', '0', '0', '105', '5', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0121', '1', '1', '40', '10', '20', '4', '0', '0', '0', '0', '1', '0', '0', '0', '2', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0122', 'EqIcon026');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0122', '1', '2', '50', '11', '25', '5', '0', '0', '0', '0', '5', '0', '0', '0', '7', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0123', 'EqIcon027');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0123', '1', '3', '60', '12', '30', '6', '0', '0', '0', '0', '9', '0', '0', '0', '15', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0124', 'EqIcon027');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0124', '1', '4', '75', '14', '40', '8', '0', '0', '0', '0', '9', '1', '0', '0', '15', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0020', '3|3', 'HeroEquip0125', 'EqIcon028');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0125', '1', '5', '90', '16', '50', '10', '0', '0', '0', '0', '13', '1', '0', '0', '30', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0020', '2|4', 'HeroEquip0126', 'EqIcon028');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0126', '1', '6', '105', '18', '60', '12', '0', '0', '0', '0', '17', '1', '0', '0', '45', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0020', '2|2|4', 'HeroEquip0127', 'EqIcon028');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0127', '1', '7', '130', '21', '75', '15', '0', '0', '0', '0', '17', '2', '0', '0', '45', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0037', '3|3', 'HeroEquip0128', 'EqIcon029');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0128', '1', '8', '155', '24', '90', '18', '0', '0', '0', '0', '22', '2', '0', '0', '65', '4', '0', '0', '53', '2', 'BagItem0041|BagItem0037', '2|4', 'HeroEquip0129', 'EqIcon029');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0129', '1', '9', '180', '27', '105', '21', '0', '0', '0', '0', '27', '2', '0', '0', '85', '4', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0037', '2|2|6', 'HeroEquip0130', 'EqIcon029');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0130', '1', '10', '205', '30', '120', '24', '0', '0', '0', '0', '32', '2', '0', '0', '105', '4', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0060', '3|2|1', 'HeroEquip0131', 'EqIcon029');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0131', '1', '11', '235', '35', '142', '27', '0', '0', '0', '0', '42', '2', '0', '0', '105', '5', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0141', '1', '1', '40', '13', '5', '4', '0', '0', '0', '0', '3', '0', '0', '0', '8', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0142', 'EqIcon031');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0142', '1', '2', '50', '14', '8', '5', '0', '0', '0', '0', '8', '0', '0', '0', '12', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0143', 'EqIcon032');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0143', '1', '3', '60', '15', '17', '6', '0', '0', '0', '0', '12', '0', '0', '0', '16', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0144', 'EqIcon032');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0144', '1', '4', '70', '16', '27', '8', '0', '0', '0', '0', '12', '1', '0', '0', '21', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0021', '3|3', 'HeroEquip0145', 'EqIcon033');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0145', '1', '5', '90', '17', '37', '10', '0', '0', '0', '0', '16', '1', '0', '0', '26', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0021', '2|4', 'HeroEquip0146', 'EqIcon033');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0146', '1', '6', '110', '18', '47', '12', '0', '0', '0', '0', '20', '1', '0', '0', '31', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0021', '2|2|4', 'HeroEquip0147', 'EqIcon033');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0147', '1', '7', '130', '19', '59', '15', '0', '0', '0', '0', '20', '2', '0', '0', '42', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0038', '3|3', 'HeroEquip0148', 'EqIcon034');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0148', '1', '8', '150', '21', '71', '18', '0', '0', '0', '0', '25', '2', '0', '0', '53', '5', '0', '0', '53', '2', 'BagItem0041|BagItem0038', '2|4', 'HeroEquip0149', 'EqIcon034');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0149', '1', '9', '170', '23', '83', '20', '0', '0', '0', '0', '30', '2', '0', '0', '64', '6', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0038', '2|2|6', 'HeroEquip0150', 'EqIcon034');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0150', '1', '10', '190', '26', '106', '22', '0', '0', '0', '0', '35', '2', '0', '0', '76', '7', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0061', '3|2|1', 'HeroEquip0151', 'EqIcon034');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0151', '1', '11', '210', '30', '130', '25', '0', '0', '0', '0', '45', '2', '0', '0', '88', '8', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0161', '1', '1', '40', '13', '5', '4', '0', '0', '0', '0', '3', '0', '0', '0', '8', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0162', 'EqIcon036');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0162', '1', '2', '50', '14', '8', '5', '0', '0', '0', '0', '8', '0', '0', '0', '12', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0163', 'EqIcon037');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0163', '1', '3', '60', '15', '17', '6', '0', '0', '0', '0', '12', '0', '0', '0', '16', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0164', 'EqIcon037');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0164', '1', '4', '70', '16', '27', '8', '0', '0', '0', '0', '12', '1', '0', '0', '21', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0022', '3|3', 'HeroEquip0165', 'EqIcon038');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0165', '1', '5', '90', '17', '37', '10', '0', '0', '0', '0', '16', '1', '0', '0', '26', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0022', '2|4', 'HeroEquip0166', 'EqIcon038');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0166', '1', '6', '110', '18', '47', '12', '0', '0', '0', '0', '20', '1', '0', '0', '31', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0022', '2|2|4', 'HeroEquip0167', 'EqIcon038');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0167', '1', '7', '130', '19', '59', '15', '0', '0', '0', '0', '20', '2', '0', '0', '42', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0039', '3|3', 'HeroEquip0168', 'EqIcon039');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0168', '1', '8', '150', '21', '71', '18', '0', '0', '0', '0', '25', '2', '0', '0', '53', '5', '0', '0', '53', '2', 'BagItem0041|BagItem0039', '2|4', 'HeroEquip0169', 'EqIcon039');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0169', '1', '9', '170', '23', '83', '20', '0', '0', '0', '0', '30', '2', '0', '0', '64', '6', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0039', '2|2|6', 'HeroEquip0170', 'EqIcon039');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0170', '1', '10', '190', '26', '106', '22', '0', '0', '0', '0', '35', '2', '0', '0', '76', '7', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0062', '3|2|1', 'HeroEquip0171', 'EqIcon039');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0171', '1', '11', '210', '30', '130', '25', '0', '0', '0', '0', '45', '2', '0', '0', '88', '8', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0181', '1', '1', '30', '8', '5', '4', '0', '0', '0', '0', '10', '0', '0', '0', '7', '2', '0', '0', '2', '0', 'BagItem0001', '1', 'HeroEquip0182', 'EqIcon041');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0182', '1', '2', '40', '10', '10', '5', '0', '0', '0', '0', '15', '0', '0', '0', '9', '3', '0', '0', '3', '0', 'BagItem0007', '2', 'HeroEquip0183', 'EqIcon042');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0183', '1', '3', '50', '12', '15', '6', '0', '0', '0', '0', '20', '0', '0', '0', '11', '3', '0', '0', '4', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0184', 'EqIcon042');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0184', '1', '4', '80', '15', '20', '8', '0', '0', '0', '0', '20', '1', '0', '0', '11', '4', '0', '0', '4', '1', 'BagItem0013|BagItem0023', '3|3', 'HeroEquip0185', 'EqIcon043');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0185', '1', '5', '110', '18', '35', '10', '0', '0', '0', '0', '26', '1', '0', '0', '14', '4', '0', '0', '5', '1', 'BagItem0024|BagItem0023', '2|4', 'HeroEquip0186', 'EqIcon043');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0186', '1', '6', '140', '23', '50', '12', '0', '0', '0', '0', '32', '1', '0', '0', '17', '4', '0', '0', '6', '1', 'BagItem0024|BagItem0030|BagItem0023', '2|2|4', 'HeroEquip0187', 'EqIcon043');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0187', '1', '7', '180', '28', '70', '15', '0', '0', '0', '0', '32', '2', '0', '0', '17', '5', '0', '0', '6', '2', 'BagItem0030|BagItem0040', '3|3', 'HeroEquip0188', 'EqIcon044');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0188', '1', '8', '220', '36', '90', '18', '0', '0', '0', '0', '39', '2', '0', '0', '20', '5', '0', '0', '7', '2', 'BagItem0041|BagItem0040', '2|4', 'HeroEquip0189', 'EqIcon044');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0189', '1', '9', '260', '44', '110', '20', '0', '0', '0', '0', '46', '2', '0', '0', '23', '5', '0', '0', '8', '2', 'BagItem0041|BagItem0047|BagItem0040', '2|2|6', 'HeroEquip0190', 'EqIcon044');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0190', '1', '10', '300', '52', '130', '22', '0', '0', '0', '0', '53', '2', '0', '0', '26', '5', '0', '0', '9', '2', 'BagItem0047|BagItem0053|BagItem0063', '3|2|1', 'HeroEquip0191', 'EqIcon044');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0191', '1', '11', '365', '65', '150', '25', '0', '0', '0', '0', '53', '3', '0', '0', '26', '6', '0', '0', '10', '2', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0201', '1', '1', '40', '13', '5', '4', '0', '0', '0', '0', '3', '0', '0', '0', '8', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0202', 'EqIcon061');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0202', '1', '2', '50', '14', '8', '5', '0', '0', '0', '0', '8', '0', '0', '0', '12', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0203', 'EqIcon062');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0203', '1', '3', '60', '15', '17', '6', '0', '0', '0', '0', '12', '0', '0', '0', '16', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0204', 'EqIcon062');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0204', '1', '4', '70', '16', '27', '8', '0', '0', '0', '0', '12', '1', '0', '0', '21', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0022', '3|3', 'HeroEquip0205', 'EqIcon063');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0205', '1', '5', '90', '17', '37', '10', '0', '0', '0', '0', '16', '1', '0', '0', '26', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0022', '2|4', 'HeroEquip0206', 'EqIcon063');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0206', '1', '6', '110', '18', '47', '12', '0', '0', '0', '0', '20', '1', '0', '0', '31', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0022', '2|2|4', 'HeroEquip0207', 'EqIcon063');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0207', '1', '7', '130', '19', '59', '15', '0', '0', '0', '0', '20', '2', '0', '0', '42', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0039', '3|3', 'HeroEquip0208', 'EqIcon064');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0208', '1', '8', '150', '21', '71', '18', '0', '0', '0', '0', '25', '2', '0', '0', '53', '5', '0', '0', '53', '2', 'BagItem0041|BagItem0039', '2|4', 'HeroEquip0209', 'EqIcon064');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0209', '1', '9', '170', '23', '83', '20', '0', '0', '0', '0', '30', '2', '0', '0', '64', '6', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0039', '2|2|6', 'HeroEquip0210', 'EqIcon064');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0210', '1', '10', '190', '26', '106', '22', '0', '0', '0', '0', '35', '2', '0', '0', '76', '7', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0062', '3|2|1', 'HeroEquip0211', 'EqIcon064');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0211', '1', '11', '210', '30', '130', '25', '0', '0', '0', '0', '45', '2', '0', '0', '88', '8', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0221', '1', '1', '40', '10', '20', '4', '0', '0', '0', '0', '1', '0', '0', '0', '2', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0222', 'EqIcon066');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0222', '1', '2', '50', '11', '25', '5', '0', '0', '0', '0', '5', '0', '0', '0', '7', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0223', 'EqIcon067');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0223', '1', '3', '60', '12', '30', '6', '0', '0', '0', '0', '9', '0', '0', '0', '15', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0224', 'EqIcon067');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0224', '1', '4', '75', '14', '40', '8', '0', '0', '0', '0', '9', '1', '0', '0', '15', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0019', '3|3', 'HeroEquip0225', 'EqIcon068');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0225', '1', '5', '90', '16', '50', '10', '0', '0', '0', '0', '13', '1', '0', '0', '30', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0019', '2|4', 'HeroEquip0226', 'EqIcon068');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0226', '1', '6', '105', '18', '60', '12', '0', '0', '0', '0', '17', '1', '0', '0', '45', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0019', '2|2|4', 'HeroEquip0227', 'EqIcon068');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0227', '1', '7', '130', '21', '75', '15', '0', '0', '0', '0', '17', '2', '0', '0', '45', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0036', '3|3', 'HeroEquip0228', 'EqIcon069');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0228', '1', '8', '155', '24', '90', '18', '0', '0', '0', '0', '22', '2', '0', '0', '65', '4', '0', '0', '53', '2', 'BagItem0041|BagItem0036', '2|4', 'HeroEquip0229', 'EqIcon069');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0229', '1', '9', '180', '27', '105', '21', '0', '0', '0', '0', '27', '2', '0', '0', '85', '4', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0036', '2|2|6', 'HeroEquip0230', 'EqIcon069');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0230', '1', '10', '205', '30', '120', '24', '0', '0', '0', '0', '32', '2', '0', '0', '105', '4', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0059', '3|2|1', 'HeroEquip0231', 'EqIcon069');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0231', '1', '11', '235', '35', '142', '27', '0', '0', '0', '0', '42', '2', '0', '0', '105', '5', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0241', '1', '1', '40', '10', '20', '4', '0', '0', '0', '0', '1', '0', '0', '0', '2', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0242', 'EqIcon086');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0242', '1', '2', '50', '11', '25', '5', '0', '0', '0', '0', '5', '0', '0', '0', '7', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0243', 'EqIcon087');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0243', '1', '3', '60', '12', '30', '6', '0', '0', '0', '0', '9', '0', '0', '0', '15', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0244', 'EqIcon087');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0244', '1', '4', '75', '14', '40', '8', '0', '0', '0', '0', '9', '1', '0', '0', '15', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0020', '3|3', 'HeroEquip0245', 'EqIcon088');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0245', '1', '5', '90', '16', '50', '10', '0', '0', '0', '0', '13', '1', '0', '0', '30', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0020', '2|4', 'HeroEquip0246', 'EqIcon088');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0246', '1', '6', '105', '18', '60', '12', '0', '0', '0', '0', '17', '1', '0', '0', '45', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0020', '2|2|4', 'HeroEquip0247', 'EqIcon088');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0247', '1', '7', '130', '21', '75', '15', '0', '0', '0', '0', '17', '2', '0', '0', '45', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0037', '3|3', 'HeroEquip0248', 'EqIcon089');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0248', '1', '8', '155', '24', '90', '18', '0', '0', '0', '0', '22', '2', '0', '0', '65', '4', '0', '0', '53', '2', 'BagItem0041|BagItem0037', '2|4', 'HeroEquip0249', 'EqIcon089');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0249', '1', '9', '180', '27', '105', '21', '0', '0', '0', '0', '27', '2', '0', '0', '85', '4', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0037', '2|2|6', 'HeroEquip0250', 'EqIcon089');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0250', '1', '10', '205', '30', '120', '24', '0', '0', '0', '0', '32', '2', '0', '0', '105', '4', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0060', '3|2|1', 'HeroEquip0251', 'EqIcon089');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0251', '1', '11', '235', '35', '142', '27', '0', '0', '0', '0', '42', '2', '0', '0', '105', '5', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0261', '1', '1', '40', '10', '20', '4', '0', '0', '0', '0', '1', '0', '0', '0', '2', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0262', 'EqIcon046');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0262', '1', '2', '50', '11', '25', '5', '0', '0', '0', '0', '5', '0', '0', '0', '7', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0263', 'EqIcon047');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0263', '1', '3', '60', '12', '30', '6', '0', '0', '0', '0', '9', '0', '0', '0', '15', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0264', 'EqIcon047');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0264', '1', '4', '75', '14', '40', '8', '0', '0', '0', '0', '9', '1', '0', '0', '15', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0019', '3|3', 'HeroEquip0265', 'EqIcon048');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0265', '1', '5', '90', '16', '50', '10', '0', '0', '0', '0', '13', '1', '0', '0', '30', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0019', '2|4', 'HeroEquip0266', 'EqIcon048');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0266', '1', '6', '105', '18', '60', '12', '0', '0', '0', '0', '17', '1', '0', '0', '45', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0019', '2|2|4', 'HeroEquip0267', 'EqIcon048');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0267', '1', '7', '130', '21', '75', '15', '0', '0', '0', '0', '17', '2', '0', '0', '45', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0036', '3|3', 'HeroEquip0268', 'EqIcon049');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0268', '1', '8', '155', '24', '90', '18', '0', '0', '0', '0', '22', '2', '0', '0', '65', '4', '0', '0', '53', '2', 'BagItem0041|BagItem0036', '2|4', 'HeroEquip0269', 'EqIcon049');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0269', '1', '9', '180', '27', '105', '21', '0', '0', '0', '0', '27', '2', '0', '0', '85', '4', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0036', '2|2|6', 'HeroEquip0270', 'EqIcon049');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0270', '1', '10', '205', '30', '120', '24', '0', '0', '0', '0', '32', '2', '0', '0', '105', '4', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0059', '3|2|1', 'HeroEquip0271', 'EqIcon049');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0271', '1', '11', '235', '35', '142', '27', '0', '0', '0', '0', '42', '2', '0', '0', '105', '5', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0281', '1', '1', '30', '8', '5', '4', '0', '0', '0', '0', '10', '0', '0', '0', '7', '2', '0', '0', '2', '0', 'BagItem0001', '1', 'HeroEquip0282', 'EqIcon071');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0282', '1', '2', '40', '10', '10', '5', '0', '0', '0', '0', '15', '0', '0', '0', '9', '3', '0', '0', '3', '0', 'BagItem0007', '2', 'HeroEquip0283', 'EqIcon072');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0283', '1', '3', '50', '12', '15', '6', '0', '0', '0', '0', '20', '0', '0', '0', '11', '3', '0', '0', '4', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0284', 'EqIcon072');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0284', '1', '4', '80', '15', '20', '8', '0', '0', '0', '0', '20', '1', '0', '0', '11', '4', '0', '0', '4', '1', 'BagItem0013|BagItem0023', '3|3', 'HeroEquip0285', 'EqIcon073');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0285', '1', '5', '110', '18', '35', '10', '0', '0', '0', '0', '26', '1', '0', '0', '14', '4', '0', '0', '5', '1', 'BagItem0024|BagItem0023', '2|4', 'HeroEquip0286', 'EqIcon073');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0286', '1', '6', '140', '23', '50', '12', '0', '0', '0', '0', '32', '1', '0', '0', '17', '4', '0', '0', '6', '1', 'BagItem0024|BagItem0030|BagItem0023', '2|2|4', 'HeroEquip0287', 'EqIcon073');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0287', '1', '7', '180', '28', '70', '15', '0', '0', '0', '0', '32', '2', '0', '0', '17', '5', '0', '0', '6', '2', 'BagItem0030|BagItem0040', '3|3', 'HeroEquip0288', 'EqIcon074');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0288', '1', '8', '220', '36', '90', '18', '0', '0', '0', '0', '39', '2', '0', '0', '20', '5', '0', '0', '7', '2', 'BagItem0041|BagItem0040', '2|4', 'HeroEquip0289', 'EqIcon074');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0289', '1', '9', '260', '44', '110', '20', '0', '0', '0', '0', '46', '2', '0', '0', '23', '5', '0', '0', '8', '2', 'BagItem0041|BagItem0047|BagItem0040', '2|2|6', 'HeroEquip0290', 'EqIcon074');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0290', '1', '10', '300', '52', '130', '22', '0', '0', '0', '0', '53', '2', '0', '0', '26', '5', '0', '0', '9', '2', 'BagItem0047|BagItem0053|BagItem0063', '3|2|1', 'HeroEquip0291', 'EqIcon074');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0291', '1', '11', '365', '65', '150', '25', '0', '0', '0', '0', '53', '3', '0', '0', '26', '6', '0', '0', '10', '2', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0301', '1', '1', '40', '10', '20', '4', '0', '0', '0', '0', '1', '0', '0', '0', '2', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0302', 'EqIcon051');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0302', '1', '2', '50', '11', '25', '5', '0', '0', '0', '0', '5', '0', '0', '0', '7', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0303', 'EqIcon052');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0303', '1', '3', '60', '12', '30', '6', '0', '0', '0', '0', '9', '0', '0', '0', '15', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0304', 'EqIcon052');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0304', '1', '4', '75', '14', '40', '8', '0', '0', '0', '0', '9', '1', '0', '0', '15', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0020', '3|3', 'HeroEquip0305', 'EqIcon053');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0305', '1', '5', '90', '16', '50', '10', '0', '0', '0', '0', '13', '1', '0', '0', '30', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0020', '2|4', 'HeroEquip0306', 'EqIcon053');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0306', '1', '6', '105', '18', '60', '12', '0', '0', '0', '0', '17', '1', '0', '0', '45', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0020', '2|2|4', 'HeroEquip0307', 'EqIcon053');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0307', '1', '7', '130', '21', '75', '15', '0', '0', '0', '0', '17', '2', '0', '0', '45', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0037', '3|3', 'HeroEquip0308', 'EqIcon054');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0308', '1', '8', '155', '24', '90', '18', '0', '0', '0', '0', '22', '2', '0', '0', '65', '4', '0', '0', '53', '2', 'BagItem0041|BagItem0037', '2|4', 'HeroEquip0309', 'EqIcon054');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0309', '1', '9', '180', '27', '105', '21', '0', '0', '0', '0', '27', '2', '0', '0', '85', '4', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0037', '2|2|6', 'HeroEquip0310', 'EqIcon054');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0310', '1', '10', '205', '30', '120', '24', '0', '0', '0', '0', '32', '2', '0', '0', '105', '4', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0060', '3|2|1', 'HeroEquip0311', 'EqIcon054');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0311', '1', '11', '235', '35', '142', '27', '0', '0', '0', '0', '42', '2', '0', '0', '105', '5', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0321', '1', '1', '40', '13', '5', '4', '0', '0', '0', '0', '3', '0', '0', '0', '8', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0322', 'EqIcon076');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0322', '1', '2', '50', '14', '8', '5', '0', '0', '0', '0', '8', '0', '0', '0', '12', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0323', 'EqIcon077');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0323', '1', '3', '60', '15', '17', '6', '0', '0', '0', '0', '12', '0', '0', '0', '16', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0324', 'EqIcon077');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0324', '1', '4', '70', '16', '27', '8', '0', '0', '0', '0', '12', '1', '0', '0', '21', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0021', '3|3', 'HeroEquip0325', 'EqIcon078');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0325', '1', '5', '90', '17', '37', '10', '0', '0', '0', '0', '16', '1', '0', '0', '26', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0021', '2|4', 'HeroEquip0326', 'EqIcon078');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0326', '1', '6', '110', '18', '47', '12', '0', '0', '0', '0', '20', '1', '0', '0', '31', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0021', '2|2|4', 'HeroEquip0327', 'EqIcon078');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0327', '1', '7', '130', '19', '59', '15', '0', '0', '0', '0', '20', '2', '0', '0', '42', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0038', '3|3', 'HeroEquip0328', 'EqIcon079');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0328', '1', '8', '150', '21', '71', '18', '0', '0', '0', '0', '25', '2', '0', '0', '53', '5', '0', '0', '53', '2', 'BagItem0041|BagItem0038', '2|4', 'HeroEquip0329', 'EqIcon079');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0329', '1', '9', '170', '23', '83', '20', '0', '0', '0', '0', '30', '2', '0', '0', '64', '6', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0038', '2|2|6', 'HeroEquip0330', 'EqIcon079');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0330', '1', '10', '190', '26', '106', '22', '0', '0', '0', '0', '35', '2', '0', '0', '76', '7', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0061', '3|2|1', 'HeroEquip0331', 'EqIcon079');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0331', '1', '11', '210', '30', '130', '25', '0', '0', '0', '0', '45', '2', '0', '0', '88', '8', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0341', '1', '1', '40', '10', '20', '4', '0', '0', '0', '0', '1', '0', '0', '0', '2', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0342', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0342', '1', '2', '50', '11', '25', '5', '0', '0', '0', '0', '5', '0', '0', '0', '7', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0343', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0343', '1', '3', '60', '12', '30', '6', '0', '0', '0', '0', '9', '0', '0', '0', '15', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0344', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0344', '1', '4', '75', '14', '40', '8', '0', '0', '0', '0', '9', '1', '0', '0', '15', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0019', '3|3', 'HeroEquip0345', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0345', '1', '5', '90', '16', '50', '10', '0', '0', '0', '0', '13', '1', '0', '0', '30', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0019', '2|4', 'HeroEquip0346', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0346', '1', '6', '105', '18', '60', '12', '0', '0', '0', '0', '17', '1', '0', '0', '45', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0019', '2|2|4', 'HeroEquip0347', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0347', '1', '7', '130', '21', '75', '15', '0', '0', '0', '0', '17', '2', '0', '0', '45', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0036', '3|3', 'HeroEquip0348', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0348', '1', '8', '155', '24', '90', '18', '0', '0', '0', '0', '22', '2', '0', '0', '65', '4', '0', '0', '53', '2', 'BagItem0041|BagItem0036', '2|4', 'HeroEquip0349', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0349', '1', '9', '180', '27', '105', '21', '0', '0', '0', '0', '27', '2', '0', '0', '85', '4', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0036', '2|2|6', 'HeroEquip0350', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0350', '1', '10', '205', '30', '120', '24', '0', '0', '0', '0', '32', '2', '0', '0', '105', '4', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0059', '3|2|1', 'HeroEquip0351', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0351', '1', '11', '235', '35', '142', '27', '0', '0', '0', '0', '42', '2', '0', '0', '105', '5', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0361', '1', '1', '40', '13', '5', '4', '0', '0', '0', '0', '3', '0', '0', '0', '8', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0362', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0362', '1', '2', '50', '14', '8', '5', '0', '0', '0', '0', '8', '0', '0', '0', '12', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0363', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0363', '1', '3', '60', '15', '17', '6', '0', '0', '0', '0', '12', '0', '0', '0', '16', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0364', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0364', '1', '4', '70', '16', '27', '8', '0', '0', '0', '0', '12', '1', '0', '0', '21', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0021', '3|3', 'HeroEquip0365', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0365', '1', '5', '90', '17', '37', '10', '0', '0', '0', '0', '16', '1', '0', '0', '26', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0021', '2|4', 'HeroEquip0366', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0366', '1', '6', '110', '18', '47', '12', '0', '0', '0', '0', '20', '1', '0', '0', '31', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0021', '2|2|4', 'HeroEquip0367', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0367', '1', '7', '130', '19', '59', '15', '0', '0', '0', '0', '20', '2', '0', '0', '42', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0038', '3|3', 'HeroEquip0368', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0368', '1', '8', '150', '21', '71', '18', '0', '0', '0', '0', '25', '2', '0', '0', '53', '5', '0', '0', '53', '2', 'BagItem0041|BagItem0038', '2|4', 'HeroEquip0369', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0369', '1', '9', '170', '23', '83', '20', '0', '0', '0', '0', '30', '2', '0', '0', '64', '6', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0038', '2|2|6', 'HeroEquip0370', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0370', '1', '10', '190', '26', '106', '22', '0', '0', '0', '0', '35', '2', '0', '0', '76', '7', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0061', '3|2|1', 'HeroEquip0371', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0371', '1', '11', '210', '30', '130', '25', '0', '0', '0', '0', '45', '2', '0', '0', '88', '8', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0381', '1', '1', '40', '13', '5', '4', '0', '0', '0', '0', '3', '0', '0', '0', '8', '1', '0', '0', '5', '0', 'BagItem0001', '1', 'HeroEquip0382', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0382', '1', '2', '50', '14', '8', '5', '0', '0', '0', '0', '8', '0', '0', '0', '12', '2', '0', '0', '10', '0', 'BagItem0007', '2', 'HeroEquip0383', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0383', '1', '3', '60', '15', '17', '6', '0', '0', '0', '0', '12', '0', '0', '0', '16', '2', '0', '0', '18', '0', 'BagItem0007|BagItem0013', '3|3', 'HeroEquip0384', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0384', '1', '4', '70', '16', '27', '8', '0', '0', '0', '0', '12', '1', '0', '0', '21', '3', '0', '0', '18', '1', 'BagItem0013|BagItem0022', '3|3', 'HeroEquip0385', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0385', '1', '5', '90', '17', '37', '10', '0', '0', '0', '0', '16', '1', '0', '0', '26', '3', '0', '0', '28', '1', 'BagItem0024|BagItem0022', '2|4', 'HeroEquip0386', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0386', '1', '6', '110', '18', '47', '12', '0', '0', '0', '0', '20', '1', '0', '0', '31', '3', '0', '0', '38', '1', 'BagItem0024|BagItem0030|BagItem0022', '2|2|4', 'HeroEquip0387', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0387', '1', '7', '130', '19', '59', '15', '0', '0', '0', '0', '20', '2', '0', '0', '42', '4', '0', '0', '38', '2', 'BagItem0030|BagItem0039', '3|3', 'HeroEquip0388', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0388', '1', '8', '150', '21', '71', '18', '0', '0', '0', '0', '25', '2', '0', '0', '53', '5', '0', '0', '53', '2', 'BagItem0041|BagItem0039', '2|4', 'HeroEquip0389', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0389', '1', '9', '170', '23', '83', '20', '0', '0', '0', '0', '30', '2', '0', '0', '64', '6', '0', '0', '68', '2', 'BagItem0041|BagItem0047|BagItem0039', '2|2|6', 'HeroEquip0390', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0390', '1', '10', '190', '26', '106', '22', '0', '0', '0', '0', '35', '2', '0', '0', '76', '7', '0', '0', '83', '2', 'BagItem0047|BagItem0053|BagItem0062', '3|2|1', 'HeroEquip0391', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip0391', '1', '11', '210', '30', '130', '25', '0', '0', '0', '0', '45', '2', '0', '0', '88', '8', '0', '0', '83', '3', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2001', '2', '1', '0', '0', '0', '0', '45', '2', '45', '2', '10', '2', '1', '0', '0', '0', '0', '0', '0', '0', 'BagItem0002', '1', 'HeroEquip2002', 'EqIcon301');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2002', '2', '2', '0', '0', '0', '0', '50', '3', '50', '3', '15', '3', '5', '0', '0', '0', '0', '0', '0', '0', 'BagItem0008', '2', 'HeroEquip2003', 'EqIcon302');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2003', '2', '3', '0', '0', '0', '0', '55', '4', '55', '4', '20', '3', '9', '0', '0', '0', '0', '0', '0', '0', 'BagItem0008|BagItem0014', '3|3', 'HeroEquip2004', 'EqIcon302');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2004', '2', '4', '0', '0', '0', '0', '60', '5', '60', '5', '20', '4', '9', '1', '0', '0', '0', '0', '0', '0', 'BagItem0014|BagItem0019', '3|2', 'HeroEquip2005', 'EqIcon303');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2005', '2', '5', '0', '0', '0', '0', '65', '6', '65', '6', '26', '4', '13', '1', '0', '0', '0', '0', '0', '0', 'BagItem0025|BagItem0019', '2|3', 'HeroEquip2006', 'EqIcon303');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2006', '2', '6', '0', '0', '0', '0', '70', '7', '70', '7', '38', '4', '17', '1', '0', '0', '0', '0', '0', '0', 'BagItem0025|BagItem0031|BagItem0019', '2|2|3', 'HeroEquip2007', 'EqIcon303');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2007', '2', '7', '0', '0', '0', '0', '80', '8', '80', '8', '46', '5', '17', '2', '0', '0', '0', '0', '0', '0', 'BagItem0031|BagItem0036', '3|2', 'HeroEquip2008', 'EqIcon304');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2008', '2', '8', '0', '0', '0', '0', '90', '9', '90', '9', '55', '5', '22', '2', '0', '0', '0', '0', '0', '0', 'BagItem0042|BagItem0036', '2|3', 'HeroEquip2009', 'EqIcon304');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2009', '2', '9', '0', '0', '0', '0', '100', '10', '100', '10', '64', '6', '27', '2', '0', '0', '0', '0', '0', '0', 'BagItem0042|BagItem0048|BagItem0036', '2|2|4', 'HeroEquip2010', 'EqIcon304');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2010', '2', '10', '0', '0', '0', '0', '110', '12', '110', '12', '73', '6', '32', '2', '0', '0', '0', '0', '0', '0', 'BagItem0048|BagItem0054|BagItem0059', '2|2|1', 'HeroEquip2011', 'EqIcon304');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2011', '2', '11', '0', '0', '0', '0', '125', '15', '125', '15', '82', '7', '42', '2', '0', '0', '0', '0', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2021', '2', '1', '0', '0', '0', '0', '45', '2', '45', '2', '10', '2', '1', '0', '0', '0', '0', '0', '0', '0', 'BagItem0002', '1', 'HeroEquip2022', 'EqIcon306');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2022', '2', '2', '0', '0', '0', '0', '50', '3', '50', '3', '15', '3', '5', '0', '0', '0', '0', '0', '0', '0', 'BagItem0008', '2', 'HeroEquip2023', 'EqIcon307');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2023', '2', '3', '0', '0', '0', '0', '55', '4', '55', '4', '20', '3', '9', '0', '0', '0', '0', '0', '0', '0', 'BagItem0008|BagItem0014', '3|3', 'HeroEquip2024', 'EqIcon307');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2024', '2', '4', '0', '0', '0', '0', '60', '5', '60', '5', '20', '4', '9', '1', '0', '0', '0', '0', '0', '0', 'BagItem0014|BagItem0020', '3|2', 'HeroEquip2025', 'EqIcon308');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2025', '2', '5', '0', '0', '0', '0', '65', '6', '65', '6', '26', '4', '13', '1', '0', '0', '0', '0', '0', '0', 'BagItem0025|BagItem0020', '2|3', 'HeroEquip2026', 'EqIcon308');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2026', '2', '6', '0', '0', '0', '0', '70', '7', '70', '7', '38', '4', '17', '1', '0', '0', '0', '0', '0', '0', 'BagItem0025|BagItem0031|BagItem0020', '2|2|3', 'HeroEquip2027', 'EqIcon308');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2027', '2', '7', '0', '0', '0', '0', '80', '8', '80', '8', '46', '5', '17', '2', '0', '0', '0', '0', '0', '0', 'BagItem0031|BagItem0037', '3|2', 'HeroEquip2028', 'EqIcon309');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2028', '2', '8', '0', '0', '0', '0', '90', '9', '90', '9', '55', '5', '22', '2', '0', '0', '0', '0', '0', '0', 'BagItem0042|BagItem0037', '2|3', 'HeroEquip2029', 'EqIcon309');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2029', '2', '9', '0', '0', '0', '0', '100', '10', '100', '10', '64', '6', '27', '2', '0', '0', '0', '0', '0', '0', 'BagItem0042|BagItem0048|BagItem0037', '2|2|4', 'HeroEquip2030', 'EqIcon309');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2030', '2', '10', '0', '0', '0', '0', '110', '12', '110', '12', '73', '6', '32', '2', '0', '0', '0', '0', '0', '0', 'BagItem0048|BagItem0054|BagItem0060', '2|2|1', 'HeroEquip2031', 'EqIcon309');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2031', '2', '11', '0', '0', '0', '0', '125', '15', '125', '15', '82', '7', '42', '2', '0', '0', '0', '0', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2041', '2', '1', '0', '0', '0', '0', '20', '3', '24', '3', '5', '1', '4', '0', '0', '0', '0', '0', '0', '0', 'BagItem0002', '1', 'HeroEquip2042', 'EqIcon311');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2042', '2', '2', '0', '0', '0', '0', '27', '5', '28', '3', '8', '2', '8', '0', '0', '0', '0', '0', '0', '0', 'BagItem0008', '2', 'HeroEquip2043', 'EqIcon312');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2043', '2', '3', '0', '0', '0', '0', '34', '6', '32', '3', '11', '2', '12', '0', '0', '0', '0', '0', '0', '0', 'BagItem0008|BagItem0014', '3|3', 'HeroEquip2044', 'EqIcon312');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2044', '2', '4', '0', '0', '0', '0', '46', '7', '36', '4', '16', '3', '12', '1', '0', '0', '0', '0', '0', '0', 'BagItem0014|BagItem0021', '3|2', 'HeroEquip2045', 'EqIcon313');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2045', '2', '5', '0', '0', '0', '0', '58', '8', '42', '5', '21', '3', '22', '1', '0', '0', '0', '0', '0', '0', 'BagItem0025|BagItem0021', '2|3', 'HeroEquip2046', 'EqIcon313');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2046', '2', '6', '0', '0', '0', '0', '70', '9', '48', '6', '26', '3', '32', '1', '0', '0', '0', '0', '0', '0', 'BagItem0025|BagItem0031|BagItem0021', '2|2|3', 'HeroEquip2047', 'EqIcon313');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2047', '2', '7', '0', '0', '0', '0', '85', '10', '56', '7', '37', '4', '32', '2', '0', '0', '0', '0', '0', '0', 'BagItem0031|BagItem0038', '3|2', 'HeroEquip2048', 'EqIcon314');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2048', '2', '8', '0', '0', '0', '0', '100', '11', '64', '8', '48', '5', '44', '2', '0', '0', '0', '0', '0', '0', 'BagItem0042|BagItem0038', '2|3', 'HeroEquip2049', 'EqIcon314');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2049', '2', '9', '0', '0', '0', '0', '115', '12', '72', '9', '59', '6', '56', '2', '0', '0', '0', '0', '0', '0', 'BagItem0042|BagItem0048|BagItem0038', '2|2|4', 'HeroEquip2050', 'EqIcon314');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2050', '2', '10', '0', '0', '0', '0', '130', '13', '80', '10', '71', '7', '68', '2', '0', '0', '0', '0', '0', '0', 'BagItem0048|BagItem0054|BagItem0061', '2|2|1', 'HeroEquip2051', 'EqIcon314');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2051', '2', '11', '0', '0', '0', '0', '157', '17', '91', '11', '83', '8', '82', '2', '0', '0', '0', '0', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2061', '2', '1', '0', '0', '0', '0', '22', '1', '22', '5', '5', '1', '4', '0', '0', '0', '0', '0', '0', '0', 'BagItem0002', '1', 'HeroEquip2062', 'EqIcon316');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2062', '2', '2', '0', '0', '0', '0', '28', '2', '30', '6', '8', '2', '8', '0', '0', '0', '0', '0', '0', '0', 'BagItem0008', '2', 'HeroEquip2063', 'EqIcon317');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2063', '2', '3', '0', '0', '0', '0', '34', '2', '38', '8', '11', '2', '12', '0', '0', '0', '0', '0', '0', '0', 'BagItem0008|BagItem0014', '3|3', 'HeroEquip2064', 'EqIcon317');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2064', '2', '4', '0', '0', '0', '0', '34', '3', '48', '9', '16', '3', '12', '1', '0', '0', '0', '0', '0', '0', 'BagItem0014|BagItem0022', '3|2', 'HeroEquip2065', 'EqIcon318');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2065', '2', '5', '0', '0', '0', '0', '42', '3', '58', '10', '21', '3', '22', '1', '0', '0', '0', '0', '0', '0', 'BagItem0025|BagItem0022', '2|3', 'HeroEquip2066', 'EqIcon318');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2066', '2', '6', '0', '0', '0', '0', '50', '3', '68', '11', '26', '3', '32', '1', '0', '0', '0', '0', '0', '0', 'BagItem0025|BagItem0031|BagItem0022', '2|2|3', 'HeroEquip2067', 'EqIcon318');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2067', '2', '7', '0', '0', '0', '0', '50', '4', '81', '13', '37', '4', '32', '2', '0', '0', '0', '0', '0', '0', 'BagItem0031|BagItem0039', '3|2', 'HeroEquip2068', 'EqIcon319');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2068', '2', '8', '0', '0', '0', '0', '60', '4', '94', '15', '48', '5', '44', '2', '0', '0', '0', '0', '0', '0', 'BagItem0042|BagItem0039', '2|3', 'HeroEquip2069', 'EqIcon319');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2069', '2', '9', '0', '0', '0', '0', '70', '4', '107', '17', '59', '6', '56', '2', '0', '0', '0', '0', '0', '0', 'BagItem0042|BagItem0048|BagItem0039', '2|2|4', 'HeroEquip2070', 'EqIcon319');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2070', '2', '10', '0', '0', '0', '0', '80', '4', '120', '19', '71', '7', '68', '2', '0', '0', '0', '0', '0', '0', 'BagItem0048|BagItem0054|BagItem0062', '2|2|1', 'HeroEquip2071', 'EqIcon319');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2071', '2', '11', '0', '0', '0', '0', '104', '4', '144', '24', '83', '8', '82', '2', '0', '0', '0', '0', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2081', '2', '1', '0', '0', '0', '0', '19', '4', '19', '3', '7', '0', '4', '0', '0', '0', '0', '0', '0', '0', 'BagItem0002', '1', 'HeroEquip2082', 'EqIcon321');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2082', '2', '2', '0', '0', '0', '0', '27', '5', '27', '5', '15', '0', '8', '0', '0', '0', '0', '0', '0', '0', 'BagItem0008', '2', 'HeroEquip2083', 'EqIcon322');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2083', '2', '3', '0', '0', '0', '0', '35', '6', '35', '6', '23', '0', '12', '0', '0', '0', '0', '0', '0', '0', 'BagItem0008|BagItem0014', '3|3', 'HeroEquip2084', 'EqIcon322');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2084', '2', '4', '0', '0', '0', '0', '50', '7', '50', '7', '33', '1', '20', '1', '0', '0', '0', '0', '0', '0', 'BagItem0014|BagItem0023', '3|2', 'HeroEquip2085', 'EqIcon323');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2085', '2', '5', '0', '0', '0', '0', '65', '8', '65', '7', '43', '1', '28', '1', '0', '0', '0', '0', '0', '0', 'BagItem0025|BagItem0023', '2|3', 'HeroEquip2086', 'EqIcon323');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2086', '2', '6', '0', '0', '0', '0', '80', '8', '80', '8', '53', '1', '36', '1', '0', '0', '0', '0', '0', '0', 'BagItem0025|BagItem0031|BagItem0023', '2|2|3', 'HeroEquip2087', 'EqIcon323');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2087', '2', '7', '0', '0', '0', '0', '100', '9', '100', '9', '68', '2', '49', '2', '0', '0', '0', '0', '0', '0', 'BagItem0031|BagItem0040', '3|2', 'HeroEquip2088', 'EqIcon324');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2088', '2', '8', '0', '0', '0', '0', '120', '10', '120', '11', '83', '2', '62', '2', '0', '0', '0', '0', '0', '0', 'BagItem0042|BagItem0040', '2|3', 'HeroEquip2089', 'EqIcon324');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2089', '2', '9', '0', '0', '0', '0', '140', '12', '140', '12', '98', '2', '75', '2', '0', '0', '0', '0', '0', '0', 'BagItem0042|BagItem0048|BagItem0040', '2|2|4', 'HeroEquip2090', 'EqIcon324');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2090', '2', '10', '0', '0', '0', '0', '160', '14', '160', '13', '113', '2', '88', '2', '0', '0', '0', '0', '0', '0', 'BagItem0048|BagItem0054|BagItem0063', '2|2|1', 'HeroEquip2091', 'EqIcon324');
INSERT INTO `client_config_equip` VALUES ('HeroEquip2091', '2', '11', '0', '0', '0', '0', '196', '16', '196', '16', '113', '3', '102', '2', '0', '0', '0', '0', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3001', '3', '1', '60', '5', '0', '0', '24', '2', '0', '0', '0', '0', '0', '0', '0', '0', '20', '0', '0', '0', 'BagItem0003', '1', 'HeroEquip3002', 'EqIcon401');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3002', '3', '2', '70', '9', '0', '0', '29', '3', '0', '0', '0', '0', '0', '0', '0', '0', '25', '1', '0', '0', 'BagItem0009', '2', 'HeroEquip3003', 'EqIcon402');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3003', '3', '3', '80', '13', '0', '0', '34', '3', '0', '0', '0', '0', '0', '0', '0', '0', '30', '1', '0', '0', 'BagItem0009|BagItem0015', '2|2', 'HeroEquip3004', 'EqIcon402');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3004', '3', '4', '90', '18', '0', '0', '40', '4', '0', '0', '0', '0', '0', '0', '0', '0', '36', '2', '0', '0', 'BagItem0015|BagItem0019', '2|2', 'HeroEquip3005', 'EqIcon403');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3005', '3', '5', '100', '23', '0', '0', '46', '4', '0', '0', '0', '0', '0', '0', '0', '0', '42', '2', '0', '0', 'BagItem0026|BagItem0019', '2|2', 'HeroEquip3006', 'EqIcon403');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3006', '3', '6', '120', '28', '0', '0', '52', '4', '0', '0', '0', '0', '0', '0', '0', '0', '48', '2', '0', '0', 'BagItem0026|BagItem0032|BagItem0019', '2|2|2', 'HeroEquip3007', 'EqIcon403');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3007', '3', '7', '140', '36', '0', '0', '59', '5', '0', '0', '0', '0', '0', '0', '0', '0', '55', '3', '0', '0', 'BagItem0032|BagItem0036', '2|2', 'HeroEquip3008', 'EqIcon404');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3008', '3', '8', '160', '44', '0', '0', '66', '6', '0', '0', '0', '0', '0', '0', '0', '0', '62', '3', '0', '0', 'BagItem0043|BagItem0036', '2|2', 'HeroEquip3009', 'EqIcon404');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3009', '3', '9', '180', '52', '0', '0', '73', '7', '0', '0', '0', '0', '0', '0', '0', '0', '69', '3', '0', '0', 'BagItem0043|BagItem0049|BagItem0036', '2|2|3', 'HeroEquip3010', 'EqIcon404');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3010', '3', '10', '200', '60', '0', '0', '80', '8', '0', '0', '0', '0', '0', '0', '0', '0', '76', '3', '0', '0', 'BagItem0049|BagItem0055|BagItem0059', '2|1|1', 'HeroEquip3011', 'EqIcon404');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3011', '3', '11', '233', '73', '0', '0', '89', '9', '0', '0', '0', '0', '0', '0', '0', '0', '84', '4', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3021', '3', '1', '60', '5', '0', '0', '24', '2', '0', '0', '0', '0', '0', '0', '0', '0', '20', '0', '0', '0', 'BagItem0003', '1', 'HeroEquip3022', 'EqIcon406');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3022', '3', '2', '70', '9', '0', '0', '29', '3', '0', '0', '0', '0', '0', '0', '0', '0', '25', '1', '0', '0', 'BagItem0009', '2', 'HeroEquip3023', 'EqIcon407');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3023', '3', '3', '80', '13', '0', '0', '34', '3', '0', '0', '0', '0', '0', '0', '0', '0', '30', '1', '0', '0', 'BagItem0009|BagItem0015', '2|2', 'HeroEquip3024', 'EqIcon407');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3024', '3', '4', '90', '18', '0', '0', '40', '4', '0', '0', '0', '0', '0', '0', '0', '0', '36', '2', '0', '0', 'BagItem0015|BagItem0020', '2|2', 'HeroEquip3025', 'EqIcon408');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3025', '3', '5', '100', '23', '0', '0', '46', '4', '0', '0', '0', '0', '0', '0', '0', '0', '42', '2', '0', '0', 'BagItem0026|BagItem0020', '2|2', 'HeroEquip3026', 'EqIcon408');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3026', '3', '6', '120', '28', '0', '0', '52', '4', '0', '0', '0', '0', '0', '0', '0', '0', '48', '2', '0', '0', 'BagItem0026|BagItem0032|BagItem0020', '2|2|2', 'HeroEquip3027', 'EqIcon408');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3027', '3', '7', '140', '36', '0', '0', '59', '5', '0', '0', '0', '0', '0', '0', '0', '0', '55', '3', '0', '0', 'BagItem0032|BagItem0037', '2|2', 'HeroEquip3028', 'EqIcon409');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3028', '3', '8', '160', '44', '0', '0', '66', '6', '0', '0', '0', '0', '0', '0', '0', '0', '62', '3', '0', '0', 'BagItem0043|BagItem0037', '2|2', 'HeroEquip3029', 'EqIcon409');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3029', '3', '9', '180', '52', '0', '0', '73', '7', '0', '0', '0', '0', '0', '0', '0', '0', '69', '3', '0', '0', 'BagItem0043|BagItem0049|BagItem0037', '2|2|3', 'HeroEquip3030', 'EqIcon409');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3030', '3', '10', '200', '60', '0', '0', '80', '8', '0', '0', '0', '0', '0', '0', '0', '0', '76', '3', '0', '0', 'BagItem0049|BagItem0055|BagItem0060', '2|1|1', 'HeroEquip3031', 'EqIcon409');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3031', '3', '11', '233', '73', '0', '0', '89', '9', '0', '0', '0', '0', '0', '0', '0', '0', '84', '4', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3041', '3', '1', '48', '2', '0', '0', '25', '2', '0', '0', '0', '0', '0', '0', '0', '0', '20', '1', '0', '0', 'BagItem0004', '1', 'HeroEquip3042', 'EqIcon411');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3042', '3', '2', '52', '3', '0', '0', '30', '3', '0', '0', '0', '0', '0', '0', '0', '0', '24', '2', '0', '0', 'BagItem0010', '2', 'HeroEquip3043', 'EqIcon412');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3043', '3', '3', '60', '4', '0', '0', '35', '4', '0', '0', '0', '0', '0', '0', '0', '0', '28', '2', '0', '0', 'BagItem0010|BagItem0016', '2|2', 'HeroEquip3044', 'EqIcon412');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3044', '3', '4', '70', '5', '0', '0', '41', '5', '0', '0', '0', '0', '0', '0', '0', '0', '33', '3', '0', '0', 'BagItem0016|BagItem0021', '2|2', 'HeroEquip3045', 'EqIcon413');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3045', '3', '5', '80', '6', '0', '0', '47', '6', '0', '0', '0', '0', '0', '0', '0', '0', '38', '3', '0', '0', 'BagItem0027|BagItem0021', '2|2', 'HeroEquip3046', 'EqIcon413');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3046', '3', '6', '90', '7', '0', '0', '53', '7', '0', '0', '0', '0', '0', '0', '0', '0', '43', '3', '0', '0', 'BagItem0027|BagItem0033|BagItem0021', '2|2|2', 'HeroEquip3047', 'EqIcon413');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3047', '3', '7', '105', '8', '0', '0', '61', '10', '0', '0', '0', '0', '0', '0', '0', '0', '51', '4', '0', '0', 'BagItem0033|BagItem0038', '2|2', 'HeroEquip3048', 'EqIcon414');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3048', '3', '8', '120', '9', '0', '0', '69', '12', '0', '0', '0', '0', '0', '0', '0', '0', '59', '4', '0', '0', 'BagItem0044|BagItem0038', '2|2', 'HeroEquip3049', 'EqIcon414');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3049', '3', '9', '135', '10', '0', '0', '77', '14', '0', '0', '0', '0', '0', '0', '0', '0', '67', '4', '0', '0', 'BagItem0044|BagItem0050|BagItem0038', '2|2|3', 'HeroEquip3050', 'EqIcon414');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3050', '3', '10', '150', '11', '0', '0', '85', '17', '0', '0', '0', '0', '0', '0', '0', '0', '75', '4', '0', '0', 'BagItem0050|BagItem0056|BagItem0061', '2|1|1', 'HeroEquip3051', 'EqIcon414');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3051', '3', '11', '173', '13', '0', '0', '100', '20', '0', '0', '0', '0', '0', '0', '0', '0', '85', '5', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3061', '3', '1', '270', '11', '0', '0', '9', '1', '0', '0', '0', '0', '0', '0', '0', '0', '5', '0', '0', '0', 'BagItem0005', '1', 'HeroEquip3062', 'EqIcon416');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3062', '3', '2', '290', '17', '0', '0', '12', '2', '0', '0', '0', '0', '0', '0', '0', '0', '10', '0', '0', '0', 'BagItem0011', '2', 'HeroEquip3063', 'EqIcon417');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3063', '3', '3', '310', '22', '0', '0', '15', '2', '0', '0', '0', '0', '0', '0', '0', '0', '18', '0', '0', '0', 'BagItem0011|BagItem0017', '2|2', 'HeroEquip3064', 'EqIcon417');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3064', '3', '4', '340', '24', '0', '0', '18', '3', '0', '0', '0', '0', '0', '0', '0', '0', '18', '1', '0', '0', 'BagItem0017|BagItem0022', '2|2', 'HeroEquip3065', 'EqIcon418');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3065', '3', '5', '370', '29', '0', '0', '21', '3', '0', '0', '0', '0', '0', '0', '0', '0', '28', '1', '0', '0', 'BagItem0028|BagItem0022', '2|2', 'HeroEquip3066', 'EqIcon418');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3066', '3', '6', '400', '35', '0', '0', '24', '3', '0', '0', '0', '0', '0', '0', '0', '0', '38', '1', '0', '0', 'BagItem0028|BagItem0034|BagItem0022', '2|2|2', 'HeroEquip3067', 'EqIcon418');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3067', '3', '7', '450', '46', '0', '0', '28', '4', '0', '0', '0', '0', '0', '0', '0', '0', '38', '2', '0', '0', 'BagItem0034|BagItem0039', '2|2', 'HeroEquip3068', 'EqIcon419');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3068', '3', '8', '500', '51', '0', '0', '32', '5', '0', '0', '0', '0', '0', '0', '0', '0', '53', '2', '0', '0', 'BagItem0045|BagItem0039', '2|2', 'HeroEquip3069', 'EqIcon419');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3069', '3', '9', '550', '58', '0', '0', '36', '6', '0', '0', '0', '0', '0', '0', '0', '0', '68', '2', '0', '0', 'BagItem0045|BagItem0051|BagItem0039', '2|2|3', 'HeroEquip3070', 'EqIcon419');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3070', '3', '10', '600', '68', '0', '0', '40', '7', '0', '0', '0', '0', '0', '0', '0', '0', '83', '2', '0', '0', 'BagItem0051|BagItem0057|BagItem0062', '2|1|1', 'HeroEquip3071', 'EqIcon419');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3071', '3', '11', '680', '80', '0', '0', '48', '8', '0', '0', '0', '0', '0', '0', '0', '0', '83', '3', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3081', '3', '1', '120', '5', '0', '0', '9', '0', '0', '0', '0', '0', '0', '0', '0', '0', '48', '2', '0', '0', 'BagItem0005', '1', 'HeroEquip3082', 'EqIcon421');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3082', '3', '2', '145', '6', '0', '0', '12', '1', '0', '0', '0', '0', '0', '0', '0', '0', '52', '3', '0', '0', 'BagItem0011', '2', 'HeroEquip3083', 'EqIcon422');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3083', '3', '3', '170', '7', '0', '0', '32', '1', '0', '0', '0', '0', '0', '0', '0', '0', '60', '4', '0', '0', 'BagItem0011|BagItem0017', '2|2', 'HeroEquip3084', 'EqIcon422');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3084', '3', '4', '200', '8', '0', '0', '32', '2', '0', '0', '0', '0', '0', '0', '0', '0', '70', '5', '0', '0', 'BagItem0017|BagItem0023', '2|2', 'HeroEquip3085', 'EqIcon423');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3085', '3', '5', '230', '9', '0', '0', '42', '2', '0', '0', '0', '0', '0', '0', '0', '0', '80', '6', '0', '0', 'BagItem0028|BagItem0023', '2|2', 'HeroEquip3086', 'EqIcon423');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3086', '3', '6', '260', '10', '0', '0', '52', '2', '0', '0', '0', '0', '0', '0', '0', '0', '90', '7', '0', '0', 'BagItem0028|BagItem0034|BagItem0023', '2|2|2', 'HeroEquip3087', 'EqIcon423');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3087', '3', '7', '295', '16', '0', '0', '49', '3', '0', '0', '0', '0', '0', '0', '0', '0', '105', '8', '0', '0', 'BagItem0034|BagItem0040', '2|2', 'HeroEquip3088', 'EqIcon424');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3088', '3', '8', '330', '25', '0', '0', '62', '3', '0', '0', '0', '0', '0', '0', '0', '0', '120', '9', '0', '0', 'BagItem0045|BagItem0040', '2|2', 'HeroEquip3089', 'EqIcon424');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3089', '3', '9', '365', '30', '0', '0', '75', '3', '0', '0', '0', '0', '0', '0', '0', '0', '135', '10', '0', '0', 'BagItem0045|BagItem0051|BagItem0040', '2|2|3', 'HeroEquip3090', 'EqIcon424');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3090', '3', '10', '400', '40', '0', '0', '84', '3', '0', '0', '0', '0', '0', '0', '0', '0', '150', '11', '0', '0', 'BagItem0051|BagItem0057|BagItem0063', '2|1|1', 'HeroEquip3091', 'EqIcon424');
INSERT INTO `client_config_equip` VALUES ('HeroEquip3091', '3', '11', '445', '45', '0', '0', '84', '4', '0', '0', '0', '0', '0', '0', '0', '0', '173', '13', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4001', '4', '1', '60', '5', '0', '0', '0', '0', '24', '2', '0', '0', '0', '0', '0', '0', '20', '0', '0', '0', 'BagItem0003', '1', 'HeroEquip4002', 'EqIcon501');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4002', '4', '2', '70', '9', '0', '0', '0', '0', '29', '3', '0', '0', '0', '0', '0', '0', '25', '1', '0', '0', 'BagItem0009', '2', 'HeroEquip4003', 'EqIcon502');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4003', '4', '3', '80', '13', '0', '0', '0', '0', '34', '3', '0', '0', '0', '0', '0', '0', '30', '1', '0', '0', 'BagItem0009|BagItem0015', '2|2', 'HeroEquip4004', 'EqIcon502');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4004', '4', '4', '90', '21', '0', '0', '0', '0', '40', '4', '0', '0', '0', '0', '0', '0', '36', '2', '0', '0', 'BagItem0015|BagItem0019', '2|2', 'HeroEquip4005', 'EqIcon503');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4005', '4', '5', '100', '23', '0', '0', '0', '0', '46', '4', '0', '0', '0', '0', '0', '0', '42', '2', '0', '0', 'BagItem0026|BagItem0019', '2|2', 'HeroEquip4006', 'EqIcon503');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4006', '4', '6', '120', '28', '0', '0', '0', '0', '52', '4', '0', '0', '0', '0', '0', '0', '48', '2', '0', '0', 'BagItem0026|BagItem0032|BagItem0019', '2|2|2', 'HeroEquip4007', 'EqIcon503');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4007', '4', '7', '140', '36', '0', '0', '0', '0', '59', '5', '0', '0', '0', '0', '0', '0', '55', '3', '0', '0', 'BagItem0032|BagItem0036', '2|2', 'HeroEquip4008', 'EqIcon504');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4008', '4', '8', '160', '44', '0', '0', '0', '0', '66', '6', '0', '0', '0', '0', '0', '0', '62', '3', '0', '0', 'BagItem0043|BagItem0036', '2|2', 'HeroEquip4009', 'EqIcon504');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4009', '4', '9', '180', '52', '0', '0', '0', '0', '73', '7', '0', '0', '0', '0', '0', '0', '69', '3', '0', '0', 'BagItem0043|BagItem0049|BagItem0036', '2|2|3', 'HeroEquip4010', 'EqIcon504');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4010', '4', '10', '200', '60', '0', '0', '0', '0', '80', '8', '0', '0', '0', '0', '0', '0', '76', '3', '0', '0', 'BagItem0049|BagItem0055|BagItem0059', '2|1|1', 'HeroEquip4011', 'EqIcon504');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4011', '4', '11', '233', '73', '0', '0', '0', '0', '89', '9', '0', '0', '0', '0', '0', '0', '84', '4', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4021', '4', '1', '60', '5', '0', '0', '0', '0', '24', '2', '0', '0', '0', '0', '0', '0', '20', '0', '0', '0', 'BagItem0003', '1', 'HeroEquip4022', 'EqIcon506');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4022', '4', '2', '70', '9', '0', '0', '0', '0', '29', '3', '0', '0', '0', '0', '0', '0', '25', '1', '0', '0', 'BagItem0009', '2', 'HeroEquip4023', 'EqIcon507');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4023', '4', '3', '80', '13', '0', '0', '0', '0', '34', '3', '0', '0', '0', '0', '0', '0', '30', '1', '0', '0', 'BagItem0009|BagItem0015', '2|2', 'HeroEquip4024', 'EqIcon507');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4024', '4', '4', '90', '18', '0', '0', '0', '0', '40', '4', '0', '0', '0', '0', '0', '0', '36', '2', '0', '0', 'BagItem0015|BagItem0020', '2|2', 'HeroEquip4025', 'EqIcon508');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4025', '4', '5', '100', '23', '0', '0', '0', '0', '46', '4', '0', '0', '0', '0', '0', '0', '42', '2', '0', '0', 'BagItem0026|BagItem0020', '2|2', 'HeroEquip4026', 'EqIcon508');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4026', '4', '6', '120', '28', '0', '0', '0', '0', '52', '4', '0', '0', '0', '0', '0', '0', '48', '2', '0', '0', 'BagItem0026|BagItem0032|BagItem0020', '2|2|2', 'HeroEquip4027', 'EqIcon508');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4027', '4', '7', '140', '36', '0', '0', '0', '0', '59', '5', '0', '0', '0', '0', '0', '0', '55', '3', '0', '0', 'BagItem0032|BagItem0037', '2|2', 'HeroEquip4028', 'EqIcon509');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4028', '4', '8', '160', '44', '0', '0', '0', '0', '66', '6', '0', '0', '0', '0', '0', '0', '62', '3', '0', '0', 'BagItem0043|BagItem0037', '2|2', 'HeroEquip4029', 'EqIcon509');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4029', '4', '9', '180', '52', '0', '0', '0', '0', '73', '7', '0', '0', '0', '0', '0', '0', '69', '3', '0', '0', 'BagItem0043|BagItem0049|BagItem0037', '2|2|3', 'HeroEquip4030', 'EqIcon509');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4030', '4', '10', '200', '60', '0', '0', '0', '0', '80', '8', '0', '0', '0', '0', '0', '0', '76', '3', '0', '0', 'BagItem0049|BagItem0055|BagItem0060', '2|1|1', 'HeroEquip4031', 'EqIcon509');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4031', '4', '11', '233', '73', '0', '0', '0', '0', '89', '9', '0', '0', '0', '0', '0', '0', '84', '4', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4041', '4', '1', '70', '17', '0', '0', '0', '0', '4', '0', '0', '0', '0', '0', '0', '0', '5', '0', '0', '0', 'BagItem0004', '1', 'HeroEquip4042', 'EqIcon511');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4042', '4', '2', '90', '29', '0', '0', '0', '0', '8', '0', '0', '0', '0', '0', '0', '0', '10', '0', '0', '0', 'BagItem0010', '2', 'HeroEquip4043', 'EqIcon512');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4043', '4', '3', '110', '34', '0', '0', '0', '0', '12', '0', '0', '0', '0', '0', '0', '0', '18', '0', '0', '0', 'BagItem0010|BagItem0016', '2|2', 'HeroEquip4044', 'EqIcon512');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4044', '4', '4', '140', '38', '0', '0', '0', '0', '12', '1', '0', '0', '0', '0', '0', '0', '18', '1', '0', '0', 'BagItem0016|BagItem0021', '2|2', 'HeroEquip4045', 'EqIcon513');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4045', '4', '5', '170', '44', '0', '0', '0', '0', '17', '1', '0', '0', '0', '0', '0', '0', '28', '1', '0', '0', 'BagItem0027|BagItem0021', '2|2', 'HeroEquip4046', 'EqIcon513');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4046', '4', '6', '200', '48', '0', '0', '0', '0', '22', '1', '0', '0', '0', '0', '0', '0', '38', '1', '0', '0', 'BagItem0027|BagItem0033|BagItem0021', '2|2|2', 'HeroEquip4047', 'EqIcon513');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4047', '4', '7', '250', '58', '0', '0', '0', '0', '22', '2', '0', '0', '0', '0', '0', '0', '38', '2', '0', '0', 'BagItem0033|BagItem0038', '2|2', 'HeroEquip4048', 'EqIcon514');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4048', '4', '8', '300', '70', '0', '0', '0', '0', '29', '2', '0', '0', '0', '0', '0', '0', '53', '2', '0', '0', 'BagItem0044|BagItem0038', '2|2', 'HeroEquip4049', 'EqIcon514');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4049', '4', '9', '350', '82', '0', '0', '0', '0', '36', '2', '0', '0', '0', '0', '0', '0', '68', '2', '0', '0', 'BagItem0044|BagItem0050|BagItem0038', '2|2|3', 'HeroEquip4050', 'EqIcon514');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4050', '4', '10', '400', '94', '0', '0', '0', '0', '43', '2', '0', '0', '0', '0', '0', '0', '83', '2', '0', '0', 'BagItem0050|BagItem0056|BagItem0061', '2|1|1', 'HeroEquip4051', 'EqIcon514');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4051', '4', '11', '468', '108', '0', '0', '0', '0', '43', '3', '0', '0', '0', '0', '0', '0', '83', '3', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4061', '4', '1', '15', '5', '0', '0', '0', '0', '15', '1', '0', '0', '0', '0', '0', '0', '20', '1', '0', '0', 'BagItem0005', '1', 'HeroEquip4062', 'EqIcon516');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4062', '4', '2', '25', '7', '0', '0', '0', '0', '20', '2', '0', '0', '0', '0', '0', '0', '24', '2', '0', '0', 'BagItem0011', '2', 'HeroEquip4063', 'EqIcon517');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4063', '4', '3', '35', '8', '0', '0', '0', '0', '25', '3', '0', '0', '0', '0', '0', '0', '28', '2', '0', '0', 'BagItem0011|BagItem0017', '2|2', 'HeroEquip4064', 'EqIcon517');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4064', '4', '4', '50', '11', '0', '0', '0', '0', '31', '4', '0', '0', '0', '0', '0', '0', '33', '3', '0', '0', 'BagItem0017|BagItem0022', '2|2', 'HeroEquip4065', 'EqIcon518');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4065', '4', '5', '65', '14', '0', '0', '0', '0', '37', '5', '0', '0', '0', '0', '0', '0', '38', '3', '0', '0', 'BagItem0028|BagItem0022', '2|2', 'HeroEquip4066', 'EqIcon518');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4066', '4', '6', '80', '17', '0', '0', '0', '0', '43', '6', '0', '0', '0', '0', '0', '0', '43', '3', '0', '0', 'BagItem0028|BagItem0034|BagItem0022', '2|2|2', 'HeroEquip4067', 'EqIcon518');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4067', '4', '7', '115', '21', '0', '0', '0', '0', '51', '7', '0', '0', '0', '0', '0', '0', '51', '4', '0', '0', 'BagItem0034|BagItem0039', '2|2', 'HeroEquip4068', 'EqIcon519');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4068', '4', '8', '140', '25', '0', '0', '0', '0', '59', '8', '0', '0', '0', '0', '0', '0', '59', '4', '0', '0', 'BagItem0045|BagItem0039', '2|2', 'HeroEquip4069', 'EqIcon519');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4069', '4', '9', '165', '29', '0', '0', '0', '0', '67', '10', '0', '0', '0', '0', '0', '0', '67', '4', '0', '0', 'BagItem0045|BagItem0051|BagItem0039', '2|2|3', 'HeroEquip4070', 'EqIcon519');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4070', '4', '10', '190', '34', '0', '0', '0', '0', '75', '12', '0', '0', '0', '0', '0', '0', '75', '4', '0', '0', 'BagItem0051|BagItem0057|BagItem0062', '2|1|1', 'HeroEquip4071', 'EqIcon519');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4071', '4', '11', '219', '39', '0', '0', '0', '0', '75', '15', '0', '0', '0', '0', '0', '0', '85', '5', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4081', '4', '1', '20', '7', '0', '0', '0', '0', '9', '0', '0', '0', '0', '0', '0', '0', '5', '2', '0', '0', 'BagItem0005', '1', 'HeroEquip4082', 'EqIcon521');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4082', '4', '2', '30', '8', '0', '0', '0', '0', '12', '1', '0', '0', '0', '0', '0', '0', '15', '3', '0', '0', 'BagItem0011', '2', 'HeroEquip4083', 'EqIcon522');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4083', '4', '3', '40', '9', '0', '0', '0', '0', '32', '1', '0', '0', '0', '0', '0', '0', '25', '4', '0', '0', 'BagItem0011|BagItem0017', '2|2', 'HeroEquip4084', 'EqIcon522');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4084', '4', '4', '50', '11', '0', '0', '0', '0', '32', '2', '0', '0', '0', '0', '0', '0', '40', '5', '0', '0', 'BagItem0017|BagItem0023', '2|2', 'HeroEquip4085', 'EqIcon523');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4085', '4', '5', '60', '13', '0', '0', '0', '0', '42', '2', '0', '0', '0', '0', '0', '0', '55', '6', '0', '0', 'BagItem0028|BagItem0023', '2|2', 'HeroEquip4086', 'EqIcon523');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4086', '4', '6', '70', '15', '0', '0', '0', '0', '52', '2', '0', '0', '0', '0', '0', '0', '70', '7', '0', '0', 'BagItem0028|BagItem0034|BagItem0023', '2|2|2', 'HeroEquip4087', 'EqIcon523');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4087', '4', '7', '95', '19', '0', '0', '0', '0', '49', '3', '0', '0', '0', '0', '0', '0', '90', '8', '0', '0', 'BagItem0034|BagItem0040', '2|2', 'HeroEquip4088', 'EqIcon524');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4088', '4', '8', '120', '22', '0', '0', '0', '0', '62', '3', '0', '0', '0', '0', '0', '0', '110', '9', '0', '0', 'BagItem0045|BagItem0040', '2|2', 'HeroEquip4089', 'EqIcon524');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4089', '4', '9', '145', '30', '0', '0', '0', '0', '75', '3', '0', '0', '0', '0', '0', '0', '130', '10', '0', '0', 'BagItem0045|BagItem0051|BagItem0040', '2|2|3', 'HeroEquip4090', 'EqIcon524');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4090', '4', '10', '180', '38', '0', '0', '0', '0', '84', '3', '0', '0', '0', '0', '0', '0', '150', '11', '0', '0', 'BagItem0051|BagItem0057|BagItem0063', '2|1|1', 'HeroEquip4091', 'EqIcon524');
INSERT INTO `client_config_equip` VALUES ('HeroEquip4091', '4', '11', '208', '48', '0', '0', '0', '0', '84', '4', '0', '0', '0', '0', '0', '0', '173', '13', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5001', '5', '1', '0', '0', '2', '1', '0', '0', '0', '0', '1', '0', '0', '0', '7', '1', '0', '0', '10', '1', 'BagItem0006', '1', 'HeroEquip5002', 'EqIcon601');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5002', '5', '2', '0', '0', '6', '1', '0', '0', '0', '0', '2', '0', '0', '0', '9', '2', '0', '0', '15', '2', 'BagItem0012', '2', 'HeroEquip5003', 'EqIcon602');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5003', '5', '3', '0', '0', '10', '2', '0', '0', '0', '0', '2', '0', '0', '0', '11', '2', '0', '0', '20', '2', 'BagItem0012|BagItem0018', '3|3', 'HeroEquip5004', 'EqIcon602');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5004', '5', '4', '0', '0', '18', '3', '0', '0', '0', '0', '2', '1', '0', '0', '11', '3', '0', '0', '26', '3', 'BagItem0018|BagItem0019', '3|2', 'HeroEquip5005', 'EqIcon603');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5005', '5', '5', '0', '0', '26', '4', '0', '0', '0', '0', '2', '1', '0', '0', '14', '3', '0', '0', '32', '3', 'BagItem0029|BagItem0019', '2|3', 'HeroEquip5006', 'EqIcon603');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5006', '5', '6', '0', '0', '34', '5', '0', '0', '0', '0', '2', '1', '0', '0', '17', '3', '0', '0', '38', '3', 'BagItem0029|BagItem0035|BagItem0019', '2|2|3', 'HeroEquip5007', 'EqIcon603');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5007', '5', '7', '0', '0', '43', '6', '0', '0', '0', '0', '2', '2', '0', '0', '17', '4', '0', '0', '46', '4', 'BagItem0035|BagItem0036', '3|2', 'HeroEquip5008', 'EqIcon604');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5008', '5', '8', '0', '0', '52', '7', '0', '0', '0', '0', '2', '2', '0', '0', '20', '4', '0', '0', '54', '4', 'BagItem0046|BagItem0036', '2|3', 'HeroEquip5009', 'EqIcon604');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5009', '5', '9', '0', '0', '61', '8', '0', '0', '0', '0', '2', '2', '0', '0', '23', '5', '0', '0', '62', '4', 'BagItem0046|BagItem0052|BagItem0036', '2|2|4', 'HeroEquip5010', 'EqIcon604');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5010', '5', '10', '0', '0', '70', '10', '0', '0', '0', '0', '2', '2', '0', '0', '26', '5', '0', '0', '70', '4', 'BagItem0052|BagItem0058|BagItem0059', '2|2|1', 'HeroEquip5011', 'EqIcon604');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5011', '5', '11', '0', '0', '81', '11', '0', '0', '0', '0', '2', '2', '0', '0', '26', '6', '0', '0', '85', '5', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5021', '5', '1', '0', '0', '2', '1', '0', '0', '0', '0', '1', '0', '0', '0', '7', '1', '0', '0', '10', '1', 'BagItem0006', '1', 'HeroEquip5022', 'EqIcon606');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5022', '5', '2', '0', '0', '6', '1', '0', '0', '0', '0', '2', '0', '0', '0', '9', '2', '0', '0', '15', '2', 'BagItem0012', '2', 'HeroEquip5023', 'EqIcon607');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5023', '5', '3', '0', '0', '10', '2', '0', '0', '0', '0', '2', '0', '0', '0', '11', '2', '0', '0', '20', '2', 'BagItem0012|BagItem0018', '3|3', 'HeroEquip5024', 'EqIcon607');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5024', '5', '4', '0', '0', '18', '3', '0', '0', '0', '0', '2', '1', '0', '0', '11', '3', '0', '0', '26', '3', 'BagItem0018|BagItem0020', '3|2', 'HeroEquip5025', 'EqIcon608');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5025', '5', '5', '0', '0', '26', '4', '0', '0', '0', '0', '2', '1', '0', '0', '14', '3', '0', '0', '32', '3', 'BagItem0029|BagItem0020', '2|3', 'HeroEquip5026', 'EqIcon608');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5026', '5', '6', '0', '0', '34', '5', '0', '0', '0', '0', '2', '1', '0', '0', '17', '3', '0', '0', '38', '3', 'BagItem0029|BagItem0035|BagItem0020', '2|2|3', 'HeroEquip5027', 'EqIcon608');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5027', '5', '7', '0', '0', '43', '6', '0', '0', '0', '0', '2', '2', '0', '0', '17', '4', '0', '0', '46', '4', 'BagItem0035|BagItem0037', '3|2', 'HeroEquip5028', 'EqIcon609');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5028', '5', '8', '0', '0', '52', '7', '0', '0', '0', '0', '2', '2', '0', '0', '20', '4', '0', '0', '54', '4', 'BagItem0046|BagItem0037', '2|3', 'HeroEquip5029', 'EqIcon609');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5029', '5', '9', '0', '0', '61', '8', '0', '0', '0', '0', '2', '2', '0', '0', '23', '5', '0', '0', '62', '4', 'BagItem0046|BagItem0052|BagItem0037', '2|2|4', 'HeroEquip5030', 'EqIcon609');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5030', '5', '10', '0', '0', '70', '10', '0', '0', '0', '0', '2', '2', '0', '0', '26', '5', '0', '0', '70', '4', 'BagItem0052|BagItem0058|BagItem0060', '2|2|1', 'HeroEquip5031', 'EqIcon609');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5031', '5', '11', '0', '0', '81', '11', '0', '0', '0', '0', '2', '2', '0', '0', '26', '6', '0', '0', '85', '5', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5041', '5', '1', '0', '0', '10', '1', '0', '0', '0', '0', '3', '0', '0', '0', '10', '1', '0', '0', '10', '1', 'BagItem0006', '1', 'HeroEquip5042', 'EqIcon611');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5042', '5', '2', '0', '0', '15', '1', '0', '0', '0', '0', '7', '0', '0', '0', '10', '2', '0', '0', '15', '2', 'BagItem0012', '2', 'HeroEquip5043', 'EqIcon612');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5043', '5', '3', '0', '0', '20', '2', '0', '0', '0', '0', '11', '0', '0', '0', '15', '2', '0', '0', '20', '2', 'BagItem0012|BagItem0018', '3|3', 'HeroEquip5044', 'EqIcon612');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5044', '5', '4', '0', '0', '30', '3', '0', '0', '0', '0', '11', '1', '0', '0', '20', '3', '0', '0', '26', '3', 'BagItem0018|BagItem0021', '3|2', 'HeroEquip5045', 'EqIcon613');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5045', '5', '5', '0', '0', '40', '4', '0', '0', '0', '0', '15', '1', '0', '0', '25', '3', '0', '0', '32', '3', 'BagItem0029|BagItem0021', '2|3', 'HeroEquip5046', 'EqIcon613');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5046', '5', '6', '0', '0', '50', '5', '0', '0', '0', '0', '19', '1', '0', '0', '30', '3', '0', '0', '38', '3', 'BagItem0029|BagItem0035|BagItem0021', '2|2|3', 'HeroEquip5047', 'EqIcon613');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5047', '5', '7', '0', '0', '60', '6', '0', '0', '0', '0', '19', '2', '0', '0', '30', '4', '0', '0', '46', '4', 'BagItem0035|BagItem0038', '3|2', 'HeroEquip5048', 'EqIcon614');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5048', '5', '8', '0', '0', '70', '7', '0', '0', '0', '0', '23', '2', '0', '0', '40', '4', '0', '0', '54', '4', 'BagItem0046|BagItem0038', '2|3', 'HeroEquip5049', 'EqIcon614');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5049', '5', '9', '0', '0', '80', '8', '0', '0', '0', '0', '27', '2', '0', '0', '50', '4', '0', '0', '62', '4', 'BagItem0046|BagItem0052|BagItem0038', '2|2|4', 'HeroEquip5050', 'EqIcon614');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5050', '5', '10', '0', '0', '90', '9', '0', '0', '0', '0', '31', '2', '0', '0', '65', '4', '0', '0', '70', '4', 'BagItem0052|BagItem0058|BagItem0061', '2|2|1', 'HeroEquip5051', 'EqIcon614');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5051', '5', '11', '0', '0', '110', '10', '0', '0', '0', '0', '31', '3', '0', '0', '65', '5', '0', '0', '85', '5', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5061', '5', '1', '0', '0', '10', '1', '0', '0', '0', '0', '3', '0', '0', '0', '10', '1', '0', '0', '10', '1', 'BagItem0006', '1', 'HeroEquip5062', 'EqIcon616');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5062', '5', '2', '0', '0', '15', '1', '0', '0', '0', '0', '7', '0', '0', '0', '10', '2', '0', '0', '15', '2', 'BagItem0012', '2', 'HeroEquip5063', 'EqIcon617');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5063', '5', '3', '0', '0', '20', '2', '0', '0', '0', '0', '11', '0', '0', '0', '15', '2', '0', '0', '20', '2', 'BagItem0012|BagItem0018', '3|3', 'HeroEquip5064', 'EqIcon617');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5064', '5', '4', '0', '0', '30', '3', '0', '0', '0', '0', '11', '1', '0', '0', '20', '3', '0', '0', '26', '3', 'BagItem0018|BagItem0022', '3|2', 'HeroEquip5065', 'EqIcon618');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5065', '5', '5', '0', '0', '40', '4', '0', '0', '0', '0', '15', '1', '0', '0', '25', '3', '0', '0', '32', '3', 'BagItem0029|BagItem0022', '2|3', 'HeroEquip5066', 'EqIcon618');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5066', '5', '6', '0', '0', '50', '5', '0', '0', '0', '0', '19', '1', '0', '0', '30', '3', '0', '0', '38', '3', 'BagItem0029|BagItem0035|BagItem0022', '2|2|3', 'HeroEquip5067', 'EqIcon618');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5067', '5', '7', '0', '0', '60', '6', '0', '0', '0', '0', '19', '2', '0', '0', '30', '4', '0', '0', '46', '4', 'BagItem0035|BagItem0039', '3|2', 'HeroEquip5068', 'EqIcon619');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5068', '5', '8', '0', '0', '70', '7', '0', '0', '0', '0', '23', '2', '0', '0', '40', '4', '0', '0', '54', '4', 'BagItem0046|BagItem0039', '2|3', 'HeroEquip5069', 'EqIcon619');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5069', '5', '9', '0', '0', '80', '8', '0', '0', '0', '0', '27', '2', '0', '0', '50', '4', '0', '0', '62', '4', 'BagItem0046|BagItem0052|BagItem0039', '2|2|4', 'HeroEquip5070', 'EqIcon619');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5070', '5', '10', '0', '0', '90', '9', '0', '0', '0', '0', '31', '2', '0', '0', '65', '4', '0', '0', '70', '4', 'BagItem0052|BagItem0058|BagItem0062', '2|2|1', 'HeroEquip5071', 'EqIcon619');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5071', '5', '11', '0', '0', '110', '10', '0', '0', '0', '0', '31', '3', '0', '0', '65', '5', '0', '0', '85', '5', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5081', '5', '1', '0', '0', '8', '3', '0', '0', '0', '0', '1', '0', '0', '0', '7', '1', '0', '0', '1', '0', 'BagItem0006', '1', 'HeroEquip5082', 'EqIcon621');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5082', '5', '2', '0', '0', '16', '4', '0', '0', '0', '0', '5', '0', '0', '0', '9', '2', '0', '0', '5', '0', 'BagItem0012', '2', 'HeroEquip5083', 'EqIcon622');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5083', '5', '3', '0', '0', '24', '5', '0', '0', '0', '0', '9', '0', '0', '0', '11', '2', '0', '0', '9', '0', 'BagItem0012|BagItem0018', '3|3', 'HeroEquip5084', 'EqIcon622');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5084', '5', '4', '0', '0', '36', '6', '0', '0', '0', '0', '9', '1', '0', '0', '11', '3', '0', '0', '9', '1', 'BagItem0018|BagItem0023', '3|2', 'HeroEquip5085', 'EqIcon623');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5085', '5', '5', '0', '0', '48', '7', '0', '0', '0', '0', '13', '1', '0', '0', '14', '3', '0', '0', '13', '1', 'BagItem0029|BagItem0023', '2|3', 'HeroEquip5086', 'EqIcon623');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5086', '5', '6', '0', '0', '60', '8', '0', '0', '0', '0', '17', '1', '0', '0', '17', '3', '0', '0', '17', '1', 'BagItem0029|BagItem0035|BagItem0023', '2|2|3', 'HeroEquip5087', 'EqIcon623');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5087', '5', '7', '0', '0', '80', '9', '0', '0', '0', '0', '17', '2', '0', '0', '17', '4', '0', '0', '17', '2', 'BagItem0035|BagItem0040', '3|2', 'HeroEquip5088', 'EqIcon624');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5088', '5', '8', '0', '0', '100', '10', '0', '0', '0', '0', '22', '2', '0', '0', '20', '4', '0', '0', '22', '2', 'BagItem0046|BagItem0040', '2|3', 'HeroEquip5089', 'EqIcon624');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5089', '5', '9', '0', '0', '120', '11', '0', '0', '0', '0', '27', '2', '0', '0', '23', '5', '0', '0', '27', '2', 'BagItem0046|BagItem0052|BagItem0040', '2|2|4', 'HeroEquip5090', 'EqIcon624');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5090', '5', '10', '0', '0', '140', '12', '0', '0', '0', '0', '32', '2', '0', '0', '26', '5', '0', '0', '32', '2', 'BagItem0052|BagItem0058|BagItem0063', '2|2|1', 'HeroEquip5091', 'EqIcon624');
INSERT INTO `client_config_equip` VALUES ('HeroEquip5091', '5', '11', '0', '0', '169', '15', '0', '0', '0', '0', '42', '2', '0', '0', '26', '6', '0', '0', '42', '2', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6001', '6', '1', '0', '0', '0', '0', '4', '1', '4', '1', '0', '0', '3', '1', '0', '0', '0', '0', '0', '0', 'BagItem0003', '1', 'HeroEquip6002', 'EqIcon701');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6002', '6', '2', '0', '0', '0', '0', '12', '1', '12', '1', '0', '0', '6', '2', '0', '0', '0', '0', '0', '0', 'BagItem0009', '2', 'HeroEquip6003', 'EqIcon702');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6003', '6', '3', '0', '0', '0', '0', '12', '2', '12', '2', '0', '0', '9', '3', '0', '0', '0', '0', '0', '0', 'BagItem0009|BagItem0015', '2|2', 'HeroEquip6004', 'EqIcon702');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6004', '6', '4', '0', '0', '0', '0', '18', '3', '18', '3', '0', '0', '14', '4', '0', '0', '0', '0', '0', '0', 'BagItem0015|BagItem0019', '2|2', 'HeroEquip6005', 'EqIcon703');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6005', '6', '5', '0', '0', '0', '0', '18', '4', '18', '4', '0', '0', '19', '4', '0', '0', '0', '0', '0', '0', 'BagItem0026|BagItem0019', '2|2', 'HeroEquip6006', 'EqIcon703');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6006', '6', '6', '0', '0', '0', '0', '28', '4', '28', '4', '0', '0', '24', '4', '0', '0', '0', '0', '0', '0', 'BagItem0026|BagItem0032|BagItem0019', '2|2|2', 'HeroEquip6007', 'EqIcon703');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6007', '6', '7', '0', '0', '0', '0', '28', '5', '28', '5', '0', '0', '36', '5', '0', '0', '0', '0', '0', '0', 'BagItem0032|BagItem0036', '2|2', 'HeroEquip6008', 'EqIcon704');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6008', '6', '8', '0', '0', '0', '0', '38', '5', '38', '5', '0', '0', '48', '6', '0', '0', '0', '0', '0', '0', 'BagItem0043|BagItem0036', '2|2', 'HeroEquip6009', 'EqIcon704');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6009', '6', '9', '0', '0', '0', '0', '38', '6', '38', '6', '0', '0', '60', '7', '0', '0', '0', '0', '0', '0', 'BagItem0043|BagItem0049|BagItem0036', '2|2|3', 'HeroEquip6010', 'EqIcon704');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6010', '6', '10', '0', '0', '0', '0', '38', '7', '38', '7', '0', '0', '72', '8', '0', '0', '0', '0', '0', '0', 'BagItem0049|BagItem0055|BagItem0059', '2|1|1', 'HeroEquip6011', 'EqIcon704');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6011', '6', '11', '0', '0', '0', '0', '38', '8', '38', '8', '0', '0', '84', '9', '0', '0', '0', '0', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6021', '6', '1', '0', '0', '0', '0', '4', '1', '4', '1', '0', '0', '3', '1', '0', '0', '0', '0', '0', '0', 'BagItem0003', '1', 'HeroEquip6022', 'EqIcon706');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6022', '6', '2', '0', '0', '0', '0', '12', '1', '12', '1', '0', '0', '6', '2', '0', '0', '0', '0', '0', '0', 'BagItem0009', '2', 'HeroEquip6023', 'EqIcon707');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6023', '6', '3', '0', '0', '0', '0', '12', '2', '12', '2', '0', '0', '9', '3', '0', '0', '0', '0', '0', '0', 'BagItem0009|BagItem0015', '2|2', 'HeroEquip6024', 'EqIcon707');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6024', '6', '4', '0', '0', '0', '0', '18', '3', '18', '3', '0', '0', '14', '4', '0', '0', '0', '0', '0', '0', 'BagItem0015|BagItem0020', '2|2', 'HeroEquip6025', 'EqIcon708');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6025', '6', '5', '0', '0', '0', '0', '18', '4', '18', '4', '0', '0', '19', '4', '0', '0', '0', '0', '0', '0', 'BagItem0026|BagItem0020', '2|2', 'HeroEquip6026', 'EqIcon708');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6026', '6', '6', '0', '0', '0', '0', '28', '4', '28', '4', '0', '0', '24', '4', '0', '0', '0', '0', '0', '0', 'BagItem0026|BagItem0032|BagItem0020', '2|2|2', 'HeroEquip6027', 'EqIcon708');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6027', '6', '7', '0', '0', '0', '0', '28', '5', '28', '5', '0', '0', '36', '5', '0', '0', '0', '0', '0', '0', 'BagItem0032|BagItem0037', '2|2', 'HeroEquip6028', 'EqIcon709');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6028', '6', '8', '0', '0', '0', '0', '38', '5', '38', '5', '0', '0', '48', '6', '0', '0', '0', '0', '0', '0', 'BagItem0043|BagItem0037', '2|2', 'HeroEquip6029', 'EqIcon709');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6029', '6', '9', '0', '0', '0', '0', '38', '6', '38', '6', '0', '0', '60', '7', '0', '0', '0', '0', '0', '0', 'BagItem0043|BagItem0049|BagItem0037', '2|2|3', 'HeroEquip6030', 'EqIcon709');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6030', '6', '10', '0', '0', '0', '0', '38', '7', '38', '7', '0', '0', '72', '8', '0', '0', '0', '0', '0', '0', 'BagItem0049|BagItem0055|BagItem0060', '2|1|1', 'HeroEquip6031', 'EqIcon709');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6031', '6', '11', '0', '0', '0', '0', '38', '8', '38', '8', '0', '0', '84', '9', '0', '0', '0', '0', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6041', '6', '1', '0', '0', '0', '0', '4', '1', '1', '0', '0', '0', '5', '2', '0', '0', '0', '0', '0', '0', 'BagItem0004', '1', 'HeroEquip6042', 'EqIcon711');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6042', '6', '2', '0', '0', '0', '0', '8', '2', '5', '0', '0', '0', '10', '2', '0', '0', '0', '0', '0', '0', 'BagItem0010', '2', 'HeroEquip6043', 'EqIcon712');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6043', '6', '3', '0', '0', '0', '0', '12', '3', '9', '0', '0', '0', '15', '4', '0', '0', '0', '0', '0', '0', 'BagItem0010|BagItem0016', '2|2', 'HeroEquip6044', 'EqIcon712');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6044', '6', '4', '0', '0', '0', '0', '16', '4', '9', '1', '0', '0', '20', '5', '0', '0', '0', '0', '0', '0', 'BagItem0016|BagItem0021', '2|2', 'HeroEquip6045', 'EqIcon713');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6045', '6', '5', '0', '0', '0', '0', '20', '5', '13', '1', '0', '0', '28', '5', '0', '0', '0', '0', '0', '0', 'BagItem0027|BagItem0021', '2|2', 'HeroEquip6046', 'EqIcon713');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6046', '6', '6', '0', '0', '0', '0', '25', '5', '17', '1', '0', '0', '36', '6', '0', '0', '0', '0', '0', '0', 'BagItem0027|BagItem0033|BagItem0021', '2|2|2', 'HeroEquip6047', 'EqIcon713');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6047', '6', '7', '0', '0', '0', '0', '30', '6', '17', '2', '0', '0', '45', '7', '0', '0', '0', '0', '0', '0', 'BagItem0033|BagItem0038', '2|2', 'HeroEquip6048', 'EqIcon714');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6048', '6', '8', '0', '0', '0', '0', '35', '7', '22', '2', '0', '0', '54', '7', '0', '0', '0', '0', '0', '0', 'BagItem0044|BagItem0038', '2|2', 'HeroEquip6049', 'EqIcon714');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6049', '6', '9', '0', '0', '0', '0', '40', '8', '27', '2', '0', '0', '63', '8', '0', '0', '0', '0', '0', '0', 'BagItem0044|BagItem0050|BagItem0038', '2|2|3', 'HeroEquip6050', 'EqIcon714');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6050', '6', '10', '0', '0', '0', '0', '45', '9', '32', '2', '0', '0', '75', '10', '0', '0', '0', '0', '0', '0', 'BagItem0050|BagItem0056|BagItem0061', '2|1|1', 'HeroEquip6051', 'EqIcon714');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6051', '6', '11', '0', '0', '0', '0', '51', '11', '42', '2', '0', '0', '91', '11', '0', '0', '0', '0', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6061', '6', '1', '0', '0', '0', '0', '3', '0', '16', '1', '0', '0', '5', '2', '0', '0', '0', '0', '0', '0', 'BagItem0005', '1', 'HeroEquip6062', 'EqIcon716');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6062', '6', '2', '0', '0', '0', '0', '7', '0', '21', '2', '0', '0', '10', '2', '0', '0', '0', '0', '0', '0', 'BagItem0011', '2', 'HeroEquip6063', 'EqIcon717');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6063', '6', '3', '0', '0', '0', '0', '11', '0', '26', '3', '0', '0', '15', '4', '0', '0', '0', '0', '0', '0', 'BagItem0011|BagItem0017', '2|2', 'HeroEquip6064', 'EqIcon717');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6064', '6', '4', '0', '0', '0', '0', '11', '1', '34', '4', '0', '0', '20', '5', '0', '0', '0', '0', '0', '0', 'BagItem0017|BagItem0022', '2|2', 'HeroEquip6065', 'EqIcon718');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6065', '6', '5', '0', '0', '0', '0', '15', '1', '42', '4', '0', '0', '28', '5', '0', '0', '0', '0', '0', '0', 'BagItem0028|BagItem0022', '2|2', 'HeroEquip6066', 'EqIcon718');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6066', '6', '6', '0', '0', '0', '0', '19', '1', '50', '4', '0', '0', '36', '6', '0', '0', '0', '0', '0', '0', 'BagItem0028|BagItem0034|BagItem0022', '2|2|2', 'HeroEquip6067', 'EqIcon718');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6067', '6', '7', '0', '0', '0', '0', '19', '2', '65', '5', '0', '0', '45', '7', '0', '0', '0', '0', '0', '0', 'BagItem0034|BagItem0039', '2|2', 'HeroEquip6068', 'EqIcon719');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6068', '6', '8', '0', '0', '0', '0', '23', '2', '80', '6', '0', '0', '54', '7', '0', '0', '0', '0', '0', '0', 'BagItem0045|BagItem0039', '2|2', 'HeroEquip6069', 'EqIcon719');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6069', '6', '9', '0', '0', '0', '0', '27', '2', '95', '7', '0', '0', '63', '8', '0', '0', '0', '0', '0', '0', 'BagItem0045|BagItem0051|BagItem0039', '2|2|3', 'HeroEquip6070', 'EqIcon719');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6070', '6', '10', '0', '0', '0', '0', '31', '2', '110', '8', '0', '0', '75', '10', '0', '0', '0', '0', '0', '0', 'BagItem0051|BagItem0057|BagItem0062', '2|1|1', 'HeroEquip6071', 'EqIcon719');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6071', '6', '11', '0', '0', '0', '0', '43', '3', '129', '9', '0', '0', '91', '11', '0', '0', '0', '0', '0', '0', 'NA', 'NA', 'NA', 'NA');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6081', '6', '1', '0', '0', '0', '0', '24', '2', '24', '2', '0', '0', '4', '1', '0', '0', '0', '0', '0', '0', 'BagItem0005', '1', 'HeroEquip6082', 'EqIcon721');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6082', '6', '2', '0', '0', '0', '0', '29', '3', '29', '3', '0', '0', '12', '1', '0', '0', '0', '0', '0', '0', 'BagItem0011', '2', 'HeroEquip6083', 'EqIcon722');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6083', '6', '3', '0', '0', '0', '0', '34', '3', '34', '3', '0', '0', '12', '2', '0', '0', '0', '0', '0', '0', 'BagItem0011|BagItem0017', '2|2', 'HeroEquip6084', 'EqIcon722');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6084', '6', '4', '0', '0', '0', '0', '40', '4', '40', '4', '0', '0', '18', '3', '0', '0', '0', '0', '0', '0', 'BagItem0017|BagItem0023', '2|2', 'HeroEquip6085', 'EqIcon723');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6085', '6', '5', '0', '0', '0', '0', '46', '4', '46', '4', '0', '0', '18', '4', '0', '0', '0', '0', '0', '0', 'BagItem0028|BagItem0023', '2|2', 'HeroEquip6086', 'EqIcon723');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6086', '6', '6', '0', '0', '0', '0', '52', '4', '52', '4', '0', '0', '28', '4', '0', '0', '0', '0', '0', '0', 'BagItem0028|BagItem0034|BagItem0023', '2|2|2', 'HeroEquip6087', 'EqIcon723');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6087', '6', '7', '0', '0', '0', '0', '59', '5', '59', '5', '0', '0', '28', '5', '0', '0', '0', '0', '0', '0', 'BagItem0034|BagItem0040', '2|2', 'HeroEquip6088', 'EqIcon724');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6088', '6', '8', '0', '0', '0', '0', '66', '6', '66', '6', '0', '0', '38', '5', '0', '0', '0', '0', '0', '0', 'BagItem0045|BagItem0040', '2|2', 'HeroEquip6089', 'EqIcon724');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6089', '6', '9', '0', '0', '0', '0', '73', '7', '73', '7', '0', '0', '38', '6', '0', '0', '0', '0', '0', '0', 'BagItem0045|BagItem0051|BagItem0040', '2|2|3', 'HeroEquip6090', 'EqIcon724');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6090', '6', '10', '0', '0', '0', '0', '80', '8', '80', '8', '0', '0', '38', '7', '0', '0', '0', '0', '0', '0', 'BagItem0051|BagItem0057|BagItem0063', '2|1|1', 'HeroEquip6091', 'EqIcon724');
INSERT INTO `client_config_equip` VALUES ('HeroEquip6091', '6', '11', '0', '0', '0', '0', '89', '9', '89', '9', '0', '0', '47', '7', '0', '0', '0', '0', '0', '0', 'NA', 'NA', 'NA', 'NA');

-- ----------------------------
-- Table structure for client_config_heroprop
-- ----------------------------
DROP TABLE IF EXISTS `client_config_heroprop`;
CREATE TABLE `client_config_heroprop` (
  `id` varchar(100) DEFAULT NULL COMMENT 'ID',
  `area` int(10) DEFAULT NULL COMMENT '国家',
  `property` int(10) DEFAULT NULL COMMENT '人物属性',
  `occupation` int(10) DEFAULT NULL COMMENT '职业',
  `health` int(10) DEFAULT NULL COMMENT '生命',
  `healthGrow` varchar(100) DEFAULT NULL COMMENT '生命成长',
  `damage` int(10) DEFAULT NULL COMMENT '攻击',
  `damageGrow` varchar(100) DEFAULT NULL COMMENT '攻击成长',
  `pdd` int(10) DEFAULT NULL COMMENT '护甲',
  `pddGrow` varchar(100) DEFAULT NULL COMMENT '护甲成长',
  `mdd` int(10) DEFAULT NULL COMMENT '魔抗',
  `mddGrow` varchar(100) DEFAULT NULL COMMENT '魔抗成长',
  `hitLevel` int(10) DEFAULT NULL COMMENT '命中等级',
  `hitGrow` varchar(100) DEFAULT NULL COMMENT '命中成长',
  `dodgeLevel` int(10) DEFAULT NULL COMMENT '闪避等级',
  `dodgeGrow` varchar(100) DEFAULT NULL COMMENT '闪避成长',
  `criLevel` int(10) DEFAULT NULL COMMENT '暴击等级',
  `criGrow` varchar(100) DEFAULT NULL COMMENT '暴击成长',
  `tenacityLevel` int(10) DEFAULT NULL COMMENT '韧性等级',
  `tenacityGrow` varchar(100) DEFAULT NULL COMMENT '韧性成长',
  `pddPierce` int(10) DEFAULT NULL COMMENT '穿透',
  `pddPierceGrow` varchar(100) DEFAULT NULL COMMENT '穿透成长',
  `type_Normal` int(10) DEFAULT NULL COMMENT '普通攻击类型',
  `type_Skill` int(10) DEFAULT NULL COMMENT '必杀攻击类型',
  `percent_Skill` int(10) DEFAULT NULL COMMENT '必杀增益百分比',
  `addNum_Skill` int(10) DEFAULT NULL COMMENT '必杀固定加成',
  `addNumGrow_Skill` int(10) DEFAULT NULL COMMENT '必杀固定加成成长',
  `type_AoYi` int(10) DEFAULT NULL COMMENT '奥义攻击类型',
  `percent_AoYi` int(10) DEFAULT NULL COMMENT '奥义增益百分比',
  `addNum_AoYi` int(10) DEFAULT NULL COMMENT '奥义固定加成',
  `addNumGrow_AoYi` int(10) DEFAULT NULL COMMENT '奥义固定加成成长',
  `passiveSkill_1` varchar(100) DEFAULT NULL COMMENT '被动技能1',
  `passiveSkill_2` varchar(100) DEFAULT NULL COMMENT '被动技能2',
  `passiveSkill_3` varchar(100) DEFAULT NULL COMMENT '被动技能3',
  `range_X` float DEFAULT NULL COMMENT '普通攻击伤害范围 X',
  `range_Y` float DEFAULT NULL COMMENT '普通攻击伤害范围 Y',
  `range_Z` float DEFAULT NULL COMMENT '普通攻击伤害范围 Z',
  `attackSpeed` float DEFAULT NULL COMMENT '攻击速度（m/s）',
  `h_Dis` float DEFAULT NULL COMMENT '水平攻击距离（m）',
  `v_Dis` float DEFAULT NULL COMMENT '垂直攻击距离（m）',
  `moveSpeed` float DEFAULT NULL COMMENT '移动速度(m/s)',
  `star` int(10) DEFAULT NULL COMMENT '初始星级',
  `stage_B` int(10) DEFAULT NULL COMMENT '必杀阶段数',
  `hitNum_B` varchar(100) DEFAULT NULL COMMENT '必杀每阶段对应hit数',
  `hitNum_A` int(10) DEFAULT NULL COMMENT '奥义hit数',
  `posNum` int(10) DEFAULT NULL COMMENT '位置优先级',
  `recoverTime_Skill` float DEFAULT NULL COMMENT '每恢复一必杀点所需要的时间（秒/必杀点）',
  `aoYiNum` int(10) DEFAULT NULL COMMENT '奥义需要的小球数量',
  `equip_Weapon` varchar(100) DEFAULT NULL COMMENT '武器',
  `equip_Necklace` varchar(100) DEFAULT NULL COMMENT '项链',
  `equip_Clothes` varchar(100) DEFAULT NULL COMMENT '衣服',
  `equip_Belt` varchar(100) DEFAULT NULL COMMENT '腰带',
  `equip_Ring` varchar(100) DEFAULT NULL COMMENT '戒指',
  `equip_Boot` varchar(100) DEFAULT NULL COMMENT '靴子',
  `needId` varchar(100) DEFAULT NULL COMMENT '对应碎片id',
  `skillTriggerProbability` int(10) DEFAULT NULL COMMENT '技能触发概率',
  `skillTriggerDuration` float DEFAULT NULL COMMENT '技能触发时间间隔'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of client_config_heroprop
-- ----------------------------
INSERT INTO `client_config_heroprop` VALUES ('Hero001', '1', '5', '1', '479', '190,230,275,330,399', '65', '6,8,10,12,15', '10', '4,6,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '52', '5,6,8,10,12', '13', '1,1,1,1,1', '2', '2', '60', '10', '20', '2', '60', '10', '20', 'PassiveSkill001', 'PassiveSkill002', 'PassiveSkill003', '0', '0', '0', '0.9', '1.5', '1', '3', '1', '1', '5', '26', '1000', '7', '20', 'HeroEquip0001', 'HeroEquip2021', 'HeroEquip3021', 'HeroEquip4021', 'HeroEquip5021', 'HeroEquip6021', 'BagItem2001', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero002', '1', '1', '2', '354', '83,100,120,145,174', '50', '23,28,34,41,50', '57', '7,9,11,14,17', '42', '1,1,2,2,2', '47', '2,3,4,5,7', '47', '2,3,4,5,7', '47', '2,3,4,5,7', '52', '3,5,7,9,12', '13', '1,1,1,1,1', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'PassiveSkill004', 'PassiveSkill005', 'PassiveSkill006', '0', '0', '0', '1.3', '10', '7', '2.8', '1', '1', '6', '13', '4000', '7', '20', 'HeroEquip0021', 'HeroEquip2041', 'HeroEquip3041', 'HeroEquip4041', 'HeroEquip5041', 'HeroEquip6041', 'BagItem2002', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero003', '1', '4', '4', '406', '104,158,208,232,313', '60', '15,18,20,25,30', '56', '10,11,16,20,25', '56', '10,11,16,20,25', '30', '1,1,2,2,3', '30', '1,2,2,3,3', '10', '1,1,1,1,1', '43', '1,1,2,2,3', '10', '1,2,2,3,3', '2', '3', '60', '10', '20', '3', '60', '10', '20', 'PassiveSkill007', 'PassiveSkill008', 'PassiveSkill009', '0', '0', '0', '1.5', '5', '8', '2.8', '1', '1', '1', '1', '2000', '7', '20', 'HeroEquip0041', 'HeroEquip2081', 'HeroEquip3081', 'HeroEquip4081', 'HeroEquip5081', 'HeroEquip6081', 'BagItem2003', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero004', '1', '1', '3', '354', '83,100,120,145,174', '50', '23,28,34,41,50', '42', '1,1,2,2,2', '57', '7,9,11,14,17', '47', '2,3,4,5,7', '47', '2,3,4,5,7', '47', '2,3,4,5,7', '52', '3,5,7,9,12', '13', '1,1,1,1,1', '2', '2', '60', '10', '20', '2', '60', '10', '20', 'PassiveSkill010', 'PassiveSkill011', 'PassiveSkill012', '0', '0', '0', '1.3', '7', '6', '3', '1', '3', '2,2,2', '20', '3000', '7', '20', 'HeroEquip0061', 'HeroEquip2061', 'HeroEquip3061', 'HeroEquip4061', 'HeroEquip5061', 'HeroEquip6061', 'BagItem2004', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero005', '1', '5', '1', '479', '190,230,275,330,399', '65', '6,8,10,12,15', '10', '4,6,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '52', '5,6,8,10,12', '13', '1,1,1,1,1', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'PassiveSkill013', 'PassiveSkill014', 'PassiveSkill015', '0', '0', '0', '1', '2', '2', '3.5', '1', '1', '5', '23', '1001', '7', '20', 'HeroEquip0081', 'HeroEquip2001', 'HeroEquip3001', 'HeroEquip4001', 'HeroEquip5001', 'HeroEquip6001', 'BagItem2005', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero006', '1', '2', '1', '479', '190,230,275,330,399', '65', '6,8,10,12,15', '10', '4,6,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '52', '5,6,8,10,12', '13', '1,1,1,1,1', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'NA', 'NA', 'NA', '0', '0', '0', '1.2', '2.2', '2', '4', '1', '1', '8', '0', '0', '7', '20', 'HeroEquip0101', 'HeroEquip2001', 'HeroEquip3001', 'HeroEquip4001', 'HeroEquip5001', 'HeroEquip6001', 'BagItem2006', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero007', '1', '3', '1', '479', '190,230,275,330,399', '65', '6,8,10,12,15', '10', '4,6,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '52', '5,6,8,10,12', '13', '1,1,1,1,1', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'NA', 'NA', 'NA', '0', '0', '0', '1', '2', '2', '3', '1', '1', '4', '0', '0', '7', '20', 'HeroEquip0121', 'HeroEquip2021', 'HeroEquip3021', 'HeroEquip4021', 'HeroEquip5021', 'HeroEquip6021', 'BagItem2007', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero008', '1', '4', '2', '354', '83,100,120,145,174', '50', '23,28,34,41,50', '57', '7,9,11,14,17', '42', '1,1,2,2,2', '47', '2,3,4,5,7', '47', '2,3,4,5,7', '47', '2,3,4,5,7', '52', '3,5,7,9,12', '13', '1,1,1,1,1', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'NA', 'NA', 'NA', '0', '0', '0', '1', '9', '7', '2.9', '1', '3', '2,2,2', '36', '0', '7', '20', 'HeroEquip0141', 'HeroEquip2041', 'HeroEquip3041', 'HeroEquip4041', 'HeroEquip5041', 'HeroEquip6041', 'BagItem2008', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero009', '1', '2', '3', '354', '83,100,120,145,174', '50', '23,28,34,41,50', '42', '1,1,2,2,2', '57', '7,9,11,14,17', '47', '2,3,4,5,7', '47', '2,3,4,5,7', '47', '2,3,4,5,7', '52', '3,5,7,9,12', '13', '1,1,1,1,1', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'NA', 'NA', 'NA', '0', '0', '0', '0.9', '8', '8', '2.9', '1', '3', '2,2,2', '16', '0', '7', '20', 'HeroEquip0161', 'HeroEquip2061', 'HeroEquip3061', 'HeroEquip4061', 'HeroEquip5061', 'HeroEquip6061', 'BagItem2009', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero010', '1', '3', '4', '406', '104,158,208,232,313', '60', '15,18,20,25,30', '56', '10,11,16,20,25', '56', '10,11,16,20,25', '30', '1,1,2,2,3', '30', '1,2,2,3,3', '10', '1,1,1,1,1', '43', '1,1,2,2,3', '10', '1,2,2,3,3', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'NA', 'NA', 'NA', '0', '0', '0', '1', '5', '8', '2.8', '1', '1', '1', '1', '0', '7', '20', 'HeroEquip0181', 'HeroEquip2081', 'HeroEquip3081', 'HeroEquip4081', 'HeroEquip5081', 'HeroEquip6081', 'BagItem2010', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero011', '1', '5', '3', '354', '83,100,120,145,174', '50', '23,28,34,41,50', '42', '1,1,2,2,2', '57', '7,9,11,14,17', '47', '2,3,4,5,7', '47', '2,3,4,5,7', '47', '2,3,4,5,7', '52', '3,5,7,9,12', '13', '1,1,1,1,1', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'NA', 'NA', 'NA', '0', '0', '0', '1.2', '7', '6', '3', '1', '1', '10', '27', '0', '7', '20', 'HeroEquip0201', 'HeroEquip2061', 'HeroEquip3061', 'HeroEquip4061', 'HeroEquip5061', 'HeroEquip6061', 'BagItem2011', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero012', '2', '2', '1', '479', '190,230,275,330,399', '65', '6,8,10,12,15', '10', '4,6,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '52', '5,6,8,10,12', '13', '1,1,1,1,1', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'NA', 'NA', 'NA', '0', '0', '0', '1.3', '1.3', '2', '4', '1', '1', '4', '28', '0', '7', '20', 'HeroEquip0221', 'HeroEquip2001', 'HeroEquip3001', 'HeroEquip4001', 'HeroEquip5001', 'HeroEquip6001', 'BagItem2012', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero013', '2', '2', '1', '479', '190,230,275,330,399', '65', '6,8,10,12,15', '10', '4,6,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '52', '5,6,8,10,12', '13', '1,1,1,1,1', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'NA', 'NA', 'NA', '0', '0', '0', '0.9', '1.4', '1.5', '3', '1', '1', '10', '32', '0', '7', '20', 'HeroEquip0241', 'HeroEquip2021', 'HeroEquip3021', 'HeroEquip4021', 'HeroEquip5021', 'HeroEquip6021', 'BagItem2013', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero014', '2', '5', '2', '479', '190,230,275,330,399', '65', '6,8,10,12,15', '10', '4,6,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '52', '5,6,8,10,12', '13', '1,1,1,1,1', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'NA', 'NA', 'NA', '0', '0', '0', '1.3', '1.1', '1.5', '5', '1', '1', '6', '31', '0', '7', '20', 'HeroEquip0261', 'HeroEquip2001', 'HeroEquip3001', 'HeroEquip4001', 'HeroEquip5001', 'HeroEquip6001', 'BagItem2014', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero015', '2', '3', '4', '406', '104,158,208,232,313', '60', '15,18,20,25,30', '56', '10,11,16,20,25', '56', '10,11,16,20,25', '30', '1,1,2,2,3', '30', '1,2,2,3,3', '10', '1,1,1,1,1', '43', '1,1,2,2,3', '10', '1,2,2,3,3', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'NA', 'NA', 'NA', '0', '0', '0', '1', '5', '8', '2.5', '1', '1', '1', '1', '0', '7', '20', 'HeroEquip0281', 'HeroEquip2081', 'HeroEquip3081', 'HeroEquip4081', 'HeroEquip5081', 'HeroEquip6081', 'BagItem2015', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero016', '2', '4', '1', '479', '190,230,275,330,399', '65', '6,8,10,12,15', '10', '4,6,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '52', '5,6,8,10,12', '13', '1,1,1,1,1', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'NA', 'NA', 'NA', '0', '0', '0', '1.2', '1.2', '1', '3.5', '1', '1', '5', '15', '0', '7', '20', 'HeroEquip0301', 'HeroEquip2021', 'HeroEquip3021', 'HeroEquip4021', 'HeroEquip5021', 'HeroEquip6021', 'BagItem2016', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero017', '2', '3', '2', '354', '83,100,120,145,174', '50', '23,28,34,41,50', '57', '7,9,11,14,17', '42', '1,1,2,2,2', '47', '2,3,4,5,7', '47', '2,3,4,5,7', '47', '2,3,4,5,7', '52', '3,5,7,9,12', '13', '1,1,1,1,1', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'NA', 'NA', 'NA', '0', '0', '0', '1.7', '8.5', '7', '3.5', '1', '1', '4', '33', '0', '7', '20', 'HeroEquip0321', 'HeroEquip2041', 'HeroEquip3041', 'HeroEquip4041', 'HeroEquip5041', 'HeroEquip6041', 'BagItem2017', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero018', '3', '4', '1', '479', '190,230,275,330,399', '65', '6,8,10,12,15', '10', '4,6,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '10', '4,6,6,8,10', '10', '4,5,6,8,10', '52', '5,6,8,10,12', '13', '1,1,1,1,1', '1', '1', '60', '10', '20', '1', '60', '10', '20', 'NA', 'NA', 'NA', '0', '0', '0', '0.7', '2', '2', '2', '1', '1', '4', '21', '0', '7', '20', 'HeroEquip0341', 'HeroEquip2001', 'HeroEquip3001', 'HeroEquip4001', 'HeroEquip5001', 'HeroEquip6001', 'BagItem2018', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero025', '3', '0', '2', '0', 'NA', '0', 'NA', '0', 'NA', '0', 'NA', '0', 'NA', '0', 'NA', '0', 'NA', '0', 'NA', '0', 'NA', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'NA', 'NA', 'NA', '0', '0', '0', '0', '0', '0', '3', '1', '0', 'NA', '0', '0', '7', '20', 'HeroEquip0321', 'HeroEquip2041', 'HeroEquip3041', 'HeroEquip4041', 'HeroEquip5041', 'HeroEquip6041', 'BagItem2025', '0', '0');
INSERT INTO `client_config_heroprop` VALUES ('Hero026', '2', '0', '3', '0', 'NA', '0', 'NA', '0', 'NA', '0', 'NA', '0', 'NA', '0', 'NA', '0', 'NA', '0', 'NA', '0', 'NA', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'NA', 'NA', 'NA', '0', '0', '0', '0', '0', '0', '3', '1', '0', 'NA', '0', '0', '7', '20', 'HeroEquip0341', 'HeroEquip2001', 'HeroEquip3001', 'HeroEquip4001', 'HeroEquip5001', 'HeroEquip6001', 'BagItem2026', '0', '0');

-- ----------------------------
-- Table structure for client_config_heroskill
-- ----------------------------
DROP TABLE IF EXISTS `client_config_heroskill`;
CREATE TABLE `client_config_heroskill` (
  `id` varchar(100) DEFAULT NULL COMMENT 'ID',
  `factor` float DEFAULT NULL COMMENT '伤害系数',
  `distance` float DEFAULT NULL COMMENT '击退距离',
  `height` float DEFAULT NULL COMMENT '浮空高度',
  `range_x` float DEFAULT NULL COMMENT '伤害范围-X',
  `range_y` float DEFAULT NULL COMMENT '伤害范围-Y',
  `range_z` float DEFAULT NULL COMMENT '伤害范围-Z',
  `waitTime` float DEFAULT NULL COMMENT 'hit时间间隔',
  `intensity` int(10) DEFAULT NULL COMMENT '伤害强度'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of client_config_heroskill
-- ----------------------------
INSERT INTO `client_config_heroskill` VALUES ('Hero001_B_Hit_A_0', '0.6', '0', '0', '0.5', '0.5', '1', '0', '5');
INSERT INTO `client_config_heroskill` VALUES ('Hero001_B_Hit_A_1', '0.8', '0', '0', '1', '0.5', '2', '0.25', '5');
INSERT INTO `client_config_heroskill` VALUES ('Hero001_B_Hit_A_2', '0.8', '0', '0', '1', '0.5', '2', '0.25', '5');
INSERT INTO `client_config_heroskill` VALUES ('Hero001_B_Hit_A_3', '0.8', '0', '0', '1', '0.5', '2', '0.25', '5');
INSERT INTO `client_config_heroskill` VALUES ('Hero001_B_Hit_A_4', '0.8', '1', '0.5', '1', '0.5', '2', '0.25', '5');
INSERT INTO `client_config_heroskill` VALUES ('Hero002_B_Hit_A_0', '0.8', '0', '0', '1', '0.75', '1', '0', '4');
INSERT INTO `client_config_heroskill` VALUES ('Hero002_B_Hit_A_1', '0.8', '0', '0', '1', '0.75', '1', '0.15', '4');
INSERT INTO `client_config_heroskill` VALUES ('Hero002_B_Hit_A_2', '0.8', '0', '0', '0.5', '0.75', '1', '0.15', '4');
INSERT INTO `client_config_heroskill` VALUES ('Hero002_B_Hit_A_3', '0.8', '0', '0', '0.5', '0.75', '1', '0.15', '4');
INSERT INTO `client_config_heroskill` VALUES ('Hero002_B_Hit_A_4', '1.2', '0', '0.5', '0.5', '0.5', '1', '0.15', '4');
INSERT INTO `client_config_heroskill` VALUES ('Hero002_B_Hit_A_5', '1.2', '0', '2', '0.1', '0.1', '0.1', '0.2', '4');
INSERT INTO `client_config_heroskill` VALUES ('Hero003_B_Hit_A_0', '0', '0', '0', '5', '5', '5', '0', '0');
INSERT INTO `client_config_heroskill` VALUES ('Hero004_B_Hit_A_0', '0.7', '0', '0', '1', '0.75', '0.5', '0', '4');
INSERT INTO `client_config_heroskill` VALUES ('Hero004_B_Hit_A_1', '0.8', '0.5', '1', '1', '0.75', '0.5', '0.15', '4');
INSERT INTO `client_config_heroskill` VALUES ('Hero004_B_Hit_B_0', '0.9', '0', '0', '1', '0.75', '0.5', '0', '4');
INSERT INTO `client_config_heroskill` VALUES ('Hero004_B_Hit_B_1', '1', '0.5', '1', '1', '0.75', '0.5', '0.15', '4');
INSERT INTO `client_config_heroskill` VALUES ('Hero004_B_Hit_C_0', '1.1', '0', '0', '1', '0.75', '0.5', '0', '4');
INSERT INTO `client_config_heroskill` VALUES ('Hero004_B_Hit_C_1', '1.2', '0.5', '1', '1', '0.75', '0.5', '0.15', '4');
INSERT INTO `client_config_heroskill` VALUES ('Hero005_B_Hit_A_0', '0.8', '0', '1.5', '0.5', '1', '1', '0', '5');
INSERT INTO `client_config_heroskill` VALUES ('Hero005_B_Hit_A_1', '0.8', '0', '1.3', '0.5', '0.5', '1', '0.2', '5');
INSERT INTO `client_config_heroskill` VALUES ('Hero005_B_Hit_A_2', '0.8', '0', '1.2', '0.5', '0.5', '1', '0.2', '5');
INSERT INTO `client_config_heroskill` VALUES ('Hero005_B_Hit_A_3', '0.8', '0', '1.1', '0.5', '0.5', '1', '0.2', '5');
INSERT INTO `client_config_heroskill` VALUES ('Hero005_B_Hit_A_4', '0.8', '0', '1', '0.5', '0.5', '1', '0.2', '5');

-- ----------------------------
-- Table structure for client_config_monsterprop
-- ----------------------------
DROP TABLE IF EXISTS `client_config_monsterprop`;
CREATE TABLE `client_config_monsterprop` (
  `id` varchar(100) DEFAULT NULL COMMENT 'ID',
  `area` int(10) DEFAULT NULL COMMENT '国家',
  `occupation` int(10) DEFAULT NULL COMMENT '职业',
  `level` int(10) DEFAULT NULL COMMENT '等级',
  `health` int(10) DEFAULT NULL COMMENT '生命',
  `damage` int(10) DEFAULT NULL COMMENT '攻击',
  `pdd` int(10) DEFAULT NULL COMMENT '护甲',
  `mdd` int(10) DEFAULT NULL COMMENT '魔抗',
  `hitLevel` int(10) DEFAULT NULL COMMENT '命中等级',
  `dodgeLevel` int(10) DEFAULT NULL COMMENT '闪避等级',
  `criLevel` int(10) DEFAULT NULL COMMENT '暴击等级',
  `tenacityLevel` int(10) DEFAULT NULL COMMENT '韧性等级',
  `pddPierce` int(10) DEFAULT NULL COMMENT '穿透',
  `range_X` float DEFAULT NULL COMMENT '普通攻击伤害范围 X',
  `range_Y` float DEFAULT NULL COMMENT '普通攻击伤害范围 Y',
  `range_Z` float DEFAULT NULL COMMENT '普通攻击伤害范围 Z',
  `attackSpeed` float DEFAULT NULL COMMENT '攻击速度（m/s）',
  `h_Dis` float DEFAULT NULL COMMENT '水平攻击距离（m）',
  `v_Dis` float DEFAULT NULL COMMENT '垂直攻击距离（m）',
  `moveSpeed` float DEFAULT NULL COMMENT '移动速度(m/s)',
  `star` int(10) DEFAULT NULL COMMENT '星级',
  `maxIntensity` int(10) DEFAULT NULL COMMENT '初始强度',
  `recoverIntensityRate` int(10) DEFAULT NULL COMMENT '每秒强度恢复（num/s）',
  `minIntensity` int(10) DEFAULT NULL COMMENT '硬直临界值（必杀可浮空和击退）',
  `type_Normal` int(10) DEFAULT NULL COMMENT '攻击类型'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of client_config_monsterprop
-- ----------------------------
INSERT INTO `client_config_monsterprop` VALUES ('Monster001', '1', '1', '1', '2500', '60', '100', '100', '100', '100', '100', '100', '0', '0', '0', '0', '1', '1', '1', '2', '1', '10', '1', '0', '1');
INSERT INTO `client_config_monsterprop` VALUES ('Monster001_1', '1', '1', '2', '3500', '70', '100', '100', '100', '100', '100', '100', '0', '0', '0', '0', '1', '1', '1', '2', '1', '20', '1', '0', '1');
INSERT INTO `client_config_monsterprop` VALUES ('Monster001_2', '1', '1', '2', '5000', '80', '100', '100', '100', '100', '100', '100', '0', '0', '0', '0', '1', '1', '1', '2', '1', '30', '1', '0', '1');

-- ----------------------------
-- Table structure for client_config_passive
-- ----------------------------
DROP TABLE IF EXISTS `client_config_passive`;
CREATE TABLE `client_config_passive` (
  `id` varchar(100) DEFAULT NULL COMMENT '技能ID'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of client_config_passive
-- ----------------------------
INSERT INTO `client_config_passive` VALUES ('PassiveSkill001');
INSERT INTO `client_config_passive` VALUES ('PassiveSkill002');
INSERT INTO `client_config_passive` VALUES ('PassiveSkill003');
INSERT INTO `client_config_passive` VALUES ('PassiveSkill004');
INSERT INTO `client_config_passive` VALUES ('PassiveSkill005');
INSERT INTO `client_config_passive` VALUES ('PassiveSkill006');
INSERT INTO `client_config_passive` VALUES ('PassiveSkill007');
INSERT INTO `client_config_passive` VALUES ('PassiveSkill008');
INSERT INTO `client_config_passive` VALUES ('PassiveSkill009');
INSERT INTO `client_config_passive` VALUES ('PassiveSkill010');
INSERT INTO `client_config_passive` VALUES ('PassiveSkill011');
INSERT INTO `client_config_passive` VALUES ('PassiveSkill012');
INSERT INTO `client_config_passive` VALUES ('PassiveSkill013');
INSERT INTO `client_config_passive` VALUES ('PassiveSkill014');
INSERT INTO `client_config_passive` VALUES ('PassiveSkill015');

-- ----------------------------
-- Table structure for client_config_subtask
-- ----------------------------
DROP TABLE IF EXISTS `client_config_subtask`;
CREATE TABLE `client_config_subtask` (
  `id` varchar(100) DEFAULT NULL COMMENT 'id',
  `battleData` varchar(100) DEFAULT NULL COMMENT '战斗数据ID',
  `diffs` varchar(100) DEFAULT NULL COMMENT '对应的每场战斗难度系数',
  `totalTimes` varchar(100) DEFAULT NULL COMMENT '对应的每场战斗时间限制（秒）',
  `name_ch` varchar(100) DEFAULT NULL COMMENT '中文任务名称',
  `expendSt` int(10) DEFAULT NULL COMMENT '消耗体力',
  `fightCount` int(10) DEFAULT NULL COMMENT '总挑战次数',
  `idExp` int(10) DEFAULT NULL COMMENT '账号获得经验',
  `heroExp` int(10) DEFAULT NULL COMMENT '英雄获得经验',
  `dropGold` int(10) DEFAULT NULL COMMENT '获得金币',
  `pointParentId` varchar(100) DEFAULT NULL COMMENT '所属系列任务 -->',
  `priority` float DEFAULT NULL COMMENT '优先级',
  `fightPower` int(10) DEFAULT NULL COMMENT '参考战力'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of client_config_subtask
-- ----------------------------
INSERT INTO `client_config_subtask` VALUES ('LevelData001', 'Battle0001,Battle0002', '1,2', '60,90', '伟大征程第一步', '5', '0', '100', '100', '300', 'Section0001', '1', '500');
INSERT INTO `client_config_subtask` VALUES ('LevelData002', 'Battle0003,Battle0004', '1,2', '60,90', '英杰降临', '6', '0', '150', '150', '300', 'Section0002', '1', '1000');
INSERT INTO `client_config_subtask` VALUES ('LevelData003', 'Battle0005,Battle0006', '1,2', '60,90', '钜鹿危机1', '5', '0', '200', '200', '400', 'Section0002', '2', '1500');
INSERT INTO `client_config_subtask` VALUES ('LevelData004', 'Battle0007,Battle0008,Battle0009', '1,2,2', '60,90,90', '钜鹿危机2', '5', '0', '250', '250', '400', 'Section0003', '1', '2000');
INSERT INTO `client_config_subtask` VALUES ('LevelData005', 'Battle0010,Battle0011', '1,2', '60,90', '破釜沉舟1', '6', '0', '300', '300', '500', 'Section0003', '2', '2500');
INSERT INTO `client_config_subtask` VALUES ('LevelData006', 'Battle0012,Battle0013,Battle0014', '1,1,1', '60,90,60', '破釜沉舟2', '5', '0', '350', '350', '500', 'Section0004', '1', '3000');
INSERT INTO `client_config_subtask` VALUES ('LevelData007', 'Battle0015,Battle0016', '1,2', '60,90', '出乎意料的战斗', '6', '0', '400', '400', '600', 'Section0005', '1', '3500');
INSERT INTO `client_config_subtask` VALUES ('LevelData008', 'Battle0017,Battle0018', '1,2', '60,90', '故地重游', '6', '0', '450', '450', '600', 'Section0006', '1', '4000');
INSERT INTO `client_config_subtask` VALUES ('LevelData009', 'Battle0019,Battle0020,Battle0021', '1,2,2', '60,90,60', '战神的挑战1', '6', '0', '500', '500', '700', 'Section0007', '2', '4500');
INSERT INTO `client_config_subtask` VALUES ('LevelData010', 'Battle0022,Battle0023,Battle0024', '1,2,2', '60,90,60', '战神的挑战2', '6', '0', '550', '550', '700', 'Section0008', '1', '5000');
INSERT INTO `client_config_subtask` VALUES ('LevelData011', 'Battle0025,Battle0026', '1,2', '60,90', '伟大征程第一步', '5', '0', '550', '550', '700', 'Section0008', '1', '5500');
INSERT INTO `client_config_subtask` VALUES ('LevelData012', 'Battle0027,Battle0028', '1,2', '60,90', '英杰降临', '6', '0', '550', '550', '700', 'Section0009', '2', '6000');
INSERT INTO `client_config_subtask` VALUES ('LevelData013', 'Battle0029,Battle0030', '1,2', '60,90', '钜鹿危机1', '5', '0', '550', '550', '700', 'Section0009', '1', '6500');
INSERT INTO `client_config_subtask` VALUES ('LevelData014', 'Battle0031,Battle0032,Battle0033', '1,2,2', '60,90,90', '钜鹿危机2', '5', '0', '550', '550', '700', 'Section0010', '2', '7000');
INSERT INTO `client_config_subtask` VALUES ('LevelData015', 'Battle0034,Battle0035', '1,2', '60,90', '破釜沉舟1', '6', '0', '550', '550', '700', 'Section0011', '1', '7500');
INSERT INTO `client_config_subtask` VALUES ('LevelData016', 'Battle0036,Battle0037,Battle0038', '1,1,1', '60,90,60', '破釜沉舟2', '5', '0', '550', '550', '700', 'Section0012', '2', '8000');
INSERT INTO `client_config_subtask` VALUES ('LevelData017', 'Battle0039,Battle0040', '1,2', '60,90', '出乎意料的战斗', '6', '0', '550', '550', '700', 'Section0013', '1', '8500');
INSERT INTO `client_config_subtask` VALUES ('LevelData018', 'Battle0041,Battle0042', '1,2', '60,90', '故地重游', '6', '0', '550', '550', '700', 'Section0014', '2', '9000');
INSERT INTO `client_config_subtask` VALUES ('LevelData019', 'Battle0043,Battle0044,Battle0045', '1,2,2', '60,90,60', '战神的挑战1', '6', '0', '550', '550', '700', 'Section0014', '1', '9500');
INSERT INTO `client_config_subtask` VALUES ('LevelData020', 'Battle0046,Battle0047,Battle0048', '1,2,2', '60,90,60', '战神的挑战2', '6', '0', '550', '550', '700', 'Section0015', '1', '10000');
INSERT INTO `client_config_subtask` VALUES ('LevelData021', 'Battle0049,Battle0050', '1,2', '60,90', '伟大征程第一步', '5', '0', '550', '550', '700', 'Section0015', '2', '10500');
INSERT INTO `client_config_subtask` VALUES ('LevelData022', 'Battle0051,Battle0052', '1,2', '60,90', '英杰降临', '6', '0', '550', '550', '700', 'Section0016', '1', '11000');
INSERT INTO `client_config_subtask` VALUES ('LevelData023', 'Battle0053,Battle0054', '1,2', '60,90', '钜鹿危机1', '5', '0', '550', '550', '700', 'Section0017', '2', '11500');
INSERT INTO `client_config_subtask` VALUES ('LevelData024', 'Battle0055,Battle0056,Battle0057', '1,2,2', '60,90,90', '钜鹿危机2', '5', '0', '550', '550', '700', 'Section0017', '1', '12000');
INSERT INTO `client_config_subtask` VALUES ('LevelData025', 'Battle0058,Battle0059', '1,2', '60,90', '破釜沉舟1', '6', '0', '550', '550', '700', 'Section0018', '1', '12500');
INSERT INTO `client_config_subtask` VALUES ('LevelData026', 'Battle0060,Battle0061,Battle0062', '1,1,1', '60,90,60', '破釜沉舟2', '5', '0', '550', '550', '700', 'Section0019', '2', '13000');
INSERT INTO `client_config_subtask` VALUES ('LevelData027', 'Battle0063,Battle0064', '1,2', '60,90', '出乎意料的战斗', '6', '0', '550', '550', '700', 'Section0020', '1', '13500');
INSERT INTO `client_config_subtask` VALUES ('LevelData028', 'Battle0065,Battle0066', '1,2', '60,90', '故地重游', '6', '0', '550', '550', '700', 'Section0021', '2', '14000');
INSERT INTO `client_config_subtask` VALUES ('LevelData029', 'Battle0067,Battle0068,Battle0069', '1,2,2', '60,90,60', '战神的挑战1', '6', '0', '550', '550', '700', 'Section0021', '1', '14500');
INSERT INTO `client_config_subtask` VALUES ('LevelData030', 'Battle0070,Battle0071,Battle0072', '1,2,2', '60,90,60', '战神的挑战2', '6', '0', '550', '550', '700', 'Section0022', '1', '15000');
INSERT INTO `client_config_subtask` VALUES ('LevelData031', 'Battle0073,Battle0074', '1,2', '60,90', '伟大征程第一步', '5', '0', '550', '550', '700', 'Section0023', '1', '15500');
INSERT INTO `client_config_subtask` VALUES ('LevelData032', 'Battle0075,Battle0076', '1,2', '60,90', '英杰降临', '6', '0', '550', '550', '700', 'Section0024', '2', '16000');
INSERT INTO `client_config_subtask` VALUES ('LevelData033', 'Battle0077,Battle0078', '1,2', '60,90', '钜鹿危机1', '5', '0', '550', '550', '700', 'Section0025', '1', '16500');
INSERT INTO `client_config_subtask` VALUES ('LevelData034', 'Battle0079,Battle0080,Battle0081', '1,2,2', '60,90,90', '钜鹿危机2', '5', '0', '550', '550', '700', 'Section0026', '1', '17000');
INSERT INTO `client_config_subtask` VALUES ('LevelData035', 'Battle0082,Battle0083', '1,2', '60,90', '破釜沉舟1', '6', '0', '550', '550', '700', 'Section0027', '2', '17500');
INSERT INTO `client_config_subtask` VALUES ('LevelData036', 'Battle0084,Battle0085,Battle0086', '1,1,1', '60,90,60', '破釜沉舟2', '5', '0', '550', '550', '700', 'Section0027', '1', '18000');
INSERT INTO `client_config_subtask` VALUES ('LevelData037', 'Battle0087,Battle0088', '1,2', '60,90', '出乎意料的战斗', '6', '0', '550', '550', '700', 'Section0028', '2', '18500');
INSERT INTO `client_config_subtask` VALUES ('LevelData038', 'Battle0089,Battle0090', '1,2', '60,90', '故地重游', '6', '0', '550', '550', '700', 'Section0029', '1', '19000');
INSERT INTO `client_config_subtask` VALUES ('LevelData039', 'Battle0091,Battle0092,Battle0093', '1,2,2', '60,90,60', '战神的挑战1', '6', '0', '550', '550', '700', 'Section0030', '1', '19500');
INSERT INTO `client_config_subtask` VALUES ('LevelData040', 'Battle0094,Battle0095,Battle0096', '1,2,2', '60,90,60', '战神的挑战2', '6', '0', '550', '550', '700', 'Section0031', '1', '20000');
INSERT INTO `client_config_subtask` VALUES ('LevelData041', 'Battle0097,Battle0098', '1,2', '60,90', '伟大征程第一步', '5', '0', '550', '550', '700', 'Section0032', '2', '20500');
INSERT INTO `client_config_subtask` VALUES ('LevelData042', 'Battle0099,Battle0100', '1,2', '60,90', '英杰降临', '6', '0', '550', '550', '700', 'Section0032', '1', '21000');
INSERT INTO `client_config_subtask` VALUES ('LevelData043', 'Battle0101,Battle0102', '1,2', '60,90', '钜鹿危机1', '5', '0', '550', '550', '700', 'Section0033', '1', '21500');
INSERT INTO `client_config_subtask` VALUES ('LevelData044', 'Battle0103,Battle0104,Battle0105', '1,2,2', '60,90,90', '钜鹿危机2', '5', '0', '550', '550', '700', 'Section0033', '2', '22000');
INSERT INTO `client_config_subtask` VALUES ('LevelData045', 'Battle0106,Battle0107', '1,2', '60,90', '破釜沉舟1', '6', '0', '550', '550', '700', 'Section0034', '1', '22500');
INSERT INTO `client_config_subtask` VALUES ('LevelData046', 'Battle0108,Battle0109,Battle0110', '1,1,1', '60,90,60', '破釜沉舟2', '5', '0', '550', '550', '700', 'Section0035', '2', '23000');
INSERT INTO `client_config_subtask` VALUES ('LevelData047', 'Battle0111,Battle0112', '1,2', '60,90', '出乎意料的战斗', '6', '0', '550', '550', '700', 'Section0036', '1', '23500');
INSERT INTO `client_config_subtask` VALUES ('LevelData048', 'Battle0113,Battle0114', '1,2', '60,90', '故地重游', '6', '0', '550', '550', '700', 'Section0036', '1', '24000');
INSERT INTO `client_config_subtask` VALUES ('LevelData049', 'Battle0115,Battle0116,Battle0117', '1,2,2', '60,90,60', '战神的挑战1', '6', '0', '550', '550', '700', 'Section0037', '2', '24500');
INSERT INTO `client_config_subtask` VALUES ('LevelData050', 'Battle0118,Battle0119,Battle0120', '1,2,2', '60,90,60', '战神的挑战2', '6', '0', '550', '550', '700', 'Section0038', '1', '25000');
INSERT INTO `client_config_subtask` VALUES ('LevelData051', 'Battle0121,Battle0122', '1,2', '60,90', '伟大征程第一步', '5', '0', '550', '550', '700', 'Section0038', '2', '25500');
INSERT INTO `client_config_subtask` VALUES ('LevelData052', 'Battle0123,Battle0124', '1,2', '60,90', '英杰降临', '6', '0', '550', '550', '700', 'Section0039', '1', '26000');
INSERT INTO `client_config_subtask` VALUES ('LevelData053', 'Battle0125,Battle0126', '1,2', '60,90', '钜鹿危机1', '5', '0', '550', '550', '700', 'Section0039', '1', '26500');
INSERT INTO `client_config_subtask` VALUES ('LevelData054', 'Battle0127,Battle0128,Battle0129', '1,2,2', '60,90,90', '钜鹿危机2', '5', '0', '550', '550', '700', 'Section0040', '1', '27000');
INSERT INTO `client_config_subtask` VALUES ('LevelData055', 'Battle0130,Battle0131', '1,2', '60,90', '破釜沉舟1', '6', '0', '550', '550', '700', 'Section0041', '2', '27500');
INSERT INTO `client_config_subtask` VALUES ('LevelData056', 'Battle0132,Battle0133,Battle0134', '1,1,1', '60,90,60', '破釜沉舟2', '5', '0', '550', '550', '700', 'Section0042', '1', '28000');
INSERT INTO `client_config_subtask` VALUES ('LevelData057', 'Battle0135,Battle0136', '1,2', '60,90', '出乎意料的战斗', '6', '0', '550', '550', '700', 'Section0042', '1', '28500');
INSERT INTO `client_config_subtask` VALUES ('LevelData058', 'Battle0137,Battle0138', '1,2', '60,90', '故地重游', '6', '0', '550', '550', '700', 'Section0043', '2', '29000');
INSERT INTO `client_config_subtask` VALUES ('LevelData059', 'Battle0139,Battle0140,Battle0141', '1,2,2', '60,90,60', '战神的挑战1', '6', '0', '550', '550', '700', 'Section0044', '1', '29500');
INSERT INTO `client_config_subtask` VALUES ('LevelData060', 'Battle0142,Battle0143,Battle0144', '1,2,2', '60,90,60', '战神的挑战2', '6', '0', '550', '550', '700', 'Section0044', '2', '30000');
INSERT INTO `client_config_subtask` VALUES ('LevelData061', 'Battle0145,Battle0146', '1,2', '60,90', '伟大征程第一步', '5', '0', '550', '550', '700', 'Section0045', '1', '30500');
INSERT INTO `client_config_subtask` VALUES ('LevelData062', 'Battle0147,Battle0148', '1,2', '60,90', '英杰降临', '6', '0', '550', '550', '700', 'Section0046', '1', '31000');
INSERT INTO `client_config_subtask` VALUES ('LevelData063', 'Battle0149,Battle0150', '1,2', '60,90', '钜鹿危机1', '5', '0', '550', '550', '700', 'Section0047', '1', '31500');
INSERT INTO `client_config_subtask` VALUES ('LevelData064', 'Battle0151,Battle0152,Battle0153', '1,2,2', '60,90,90', '钜鹿危机2', '5', '0', '550', '550', '700', 'Section0047', '2', '32000');
INSERT INTO `client_config_subtask` VALUES ('LevelData065', 'Battle0154,Battle0155', '1,2', '60,90', '破釜沉舟1', '6', '0', '550', '550', '700', 'Section0048', '1', '32500');
INSERT INTO `client_config_subtask` VALUES ('LevelData066', 'Battle0156,Battle0157,Battle0158', '1,1,1', '60,90,60', '破釜沉舟2', '5', '0', '550', '550', '700', 'Section0048', '1', '33000');
INSERT INTO `client_config_subtask` VALUES ('LevelData067', 'Battle0159,Battle0160', '1,2', '60,90', '出乎意料的战斗', '6', '0', '550', '550', '700', 'Section0049', '2', '33500');
INSERT INTO `client_config_subtask` VALUES ('LevelData068', 'Battle0161,Battle0162', '1,2', '60,90', '故地重游', '6', '0', '550', '550', '700', 'Section0050', '1', '34000');
INSERT INTO `client_config_subtask` VALUES ('LevelData069', 'Battle0163,Battle0164,Battle0165', '1,2,2', '60,90,60', '战神的挑战1', '6', '0', '550', '550', '700', 'Section0051', '2', '34500');
INSERT INTO `client_config_subtask` VALUES ('LevelData070', 'Battle0166,Battle0167,Battle0168', '1,2,2', '60,90,60', '战神的挑战2', '6', '0', '550', '550', '700', 'Section0052', '1', '35000');
INSERT INTO `client_config_subtask` VALUES ('LevelData071', 'Battle0169,Battle0170', '1,2', '60,90', '伟大征程第一步', '5', '0', '550', '550', '700', 'Section0053', '1', '35500');
INSERT INTO `client_config_subtask` VALUES ('LevelData072', 'Battle0171,Battle0172', '1,2', '60,90', '英杰降临', '6', '0', '550', '550', '700', 'Section0053', '2', '36000');
INSERT INTO `client_config_subtask` VALUES ('LevelData073', 'Battle0173,Battle0174', '1,2', '60,90', '钜鹿危机1', '5', '0', '550', '550', '700', 'Section0054', '1', '36500');
INSERT INTO `client_config_subtask` VALUES ('LevelData074', 'Battle0175,Battle0176,Battle0177', '1,2,2', '60,90,90', '钜鹿危机2', '5', '0', '550', '550', '700', 'Section0054', '2', '37000');
INSERT INTO `client_config_subtask` VALUES ('LevelData075', 'Battle0178,Battle0179', '1,2', '60,90', '破釜沉舟1', '6', '0', '550', '550', '700', 'Section0055', '1', '37500');
INSERT INTO `client_config_subtask` VALUES ('LevelData076', 'Battle0180,Battle0181,Battle0182', '1,1,1', '60,90,60', '破釜沉舟2', '5', '0', '550', '550', '700', 'Section0056', '1', '38000');
INSERT INTO `client_config_subtask` VALUES ('LevelData077', 'Battle0183,Battle0184', '1,2', '60,90', '出乎意料的战斗', '6', '0', '550', '550', '700', 'Section0057', '1', '38500');
INSERT INTO `client_config_subtask` VALUES ('LevelData078', 'Battle0185,Battle0186', '1,2', '60,90', '故地重游', '6', '0', '550', '550', '700', 'Section0058', '2', '39000');
INSERT INTO `client_config_subtask` VALUES ('LevelData079', 'Battle0187,Battle0188,Battle0189', '1,2,2', '60,90,60', '战神的挑战1', '6', '0', '550', '550', '700', 'Section0059', '1', '39500');
INSERT INTO `client_config_subtask` VALUES ('LevelData080', 'Battle0190,Battle0191,Battle0192', '1,2,2', '60,90,60', '战神的挑战2', '6', '0', '550', '550', '700', 'Section0059', '1', '40000');
INSERT INTO `client_config_subtask` VALUES ('LevelData081', 'Battle0193,Battle0194', '1,2', '60,90', '伟大征程第一步', '5', '0', '550', '550', '700', 'Section0060', '2', '40500');
INSERT INTO `client_config_subtask` VALUES ('LevelData082', 'Battle0195,Battle0196', '1,2', '60,90', '英杰降临', '6', '0', '550', '550', '700', 'Section0060', '1', '41000');
INSERT INTO `client_config_subtask` VALUES ('LevelData083', 'Battle0197,Battle0198', '1,2', '60,90', '钜鹿危机1', '5', '0', '550', '550', '700', 'Section0061', '2', '41500');
INSERT INTO `client_config_subtask` VALUES ('LevelData084', 'Battle0199,Battle0200,Battle0201', '1,2,2', '60,90,90', '钜鹿危机2', '5', '0', '550', '550', '700', 'Section0062', '1', '42000');
INSERT INTO `client_config_subtask` VALUES ('LevelData085', 'Battle0202,Battle0203', '1,2', '60,90', '破釜沉舟1', '6', '0', '550', '550', '700', 'Section0063', '1', '42500');
INSERT INTO `client_config_subtask` VALUES ('LevelData086', 'Battle0204,Battle0205,Battle0206', '1,1,1', '60,90,60', '破釜沉舟2', '5', '0', '550', '550', '700', 'Section0063', '1', '43000');
INSERT INTO `client_config_subtask` VALUES ('LevelData087', 'Battle0207,Battle0208', '1,2', '60,90', '出乎意料的战斗', '6', '0', '550', '550', '700', 'Section0064', '2', '43500');
INSERT INTO `client_config_subtask` VALUES ('LevelData088', 'Battle0209,Battle0210', '1,2', '60,90', '故地重游', '6', '0', '550', '550', '700', 'Section0065', '1', '44000');
INSERT INTO `client_config_subtask` VALUES ('LevelData089', 'Battle0211,Battle0212,Battle0213', '1,2,2', '60,90,60', '战神的挑战1', '6', '0', '550', '550', '700', 'Section0066', '1', '44500');
INSERT INTO `client_config_subtask` VALUES ('LevelData090', 'Battle0214,Battle0215,Battle0216', '1,2,2', '60,90,60', '战神的挑战2', '6', '0', '550', '550', '700', 'Section0066', '2', '45000');

-- ----------------------------
-- Table structure for client_config_task
-- ----------------------------
DROP TABLE IF EXISTS `client_config_task`;
CREATE TABLE `client_config_task` (
  `id` varchar(100) DEFAULT NULL COMMENT 'id',
  `seriesType` int(10) DEFAULT NULL COMMENT '类型',
  `seriesName_ch` varchar(100) DEFAULT NULL COMMENT '中文任务名',
  `rewardId` varchar(100) DEFAULT NULL COMMENT '奖励',
  `rewardCount` int(10) DEFAULT NULL COMMENT '奖励数量',
  `beginTime` varchar(100) DEFAULT NULL COMMENT '任务开放开始时间',
  `endTime` varchar(100) DEFAULT NULL COMMENT '任务开放结束时间',
  `timeLimitType` int(10) DEFAULT NULL COMMENT '时间限制类型',
  `dayParam` varchar(100) DEFAULT NULL COMMENT '普通日期参数',
  `weekParam` varchar(100) DEFAULT NULL COMMENT '每周日期参数',
  `timeParam` varchar(100) DEFAULT NULL COMMENT '时间段参数',
  `natureLimit` varchar(100) DEFAULT NULL COMMENT '属性限制',
  `forceLimit` varchar(100) DEFAULT NULL COMMENT '势力限制',
  `professionLimit` varchar(100) DEFAULT NULL COMMENT '职业限制',
  `pointParentId` varchar(100) DEFAULT NULL COMMENT '所属城市 -->',
  `nextPoint` varchar(100) DEFAULT NULL COMMENT '完成后开启据点id',
  `priority` float DEFAULT NULL COMMENT '优先级'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of client_config_task
-- ----------------------------
INSERT INTO `client_config_task` VALUES ('Section0001', '1', '逐鹿', 'HeroPiece001', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint001', 'MapPoint002', '1');
INSERT INTO `client_config_task` VALUES ('Section0002', '1', '心结', 'HeroPiece002', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint002', 'NA', '2');
INSERT INTO `client_config_task` VALUES ('Section0003', '1', '钜鹿终战', 'HeroPiece003', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint002', 'MapPoint003', '3');
INSERT INTO `client_config_task` VALUES ('Section0004', '2', '破釜沉舟', 'HeroPiece004', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint002', 'NA', '0');
INSERT INTO `client_config_task` VALUES ('Section0005', '2', '鸿门宴', 'HeroPiece005', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint002', 'NA', '1');
INSERT INTO `client_config_task` VALUES ('Section0006', '1', '平原', 'HeroPiece006', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint003', 'MapPoint004', '4');
INSERT INTO `client_config_task` VALUES ('Section0007', '1', '北海', 'HeroPiece007', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint004', 'MapPoint005', '5');
INSERT INTO `client_config_task` VALUES ('Section0008', '1', '反间计', 'HeroPiece008', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint005', 'NA', '6');
INSERT INTO `client_config_task` VALUES ('Section0009', '1', '徐州攻防战', 'HeroPiece009', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint005', 'MapPoint006', '7');
INSERT INTO `client_config_task` VALUES ('Section0010', '2', '辕门射戟', 'HeroPiece010', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint005', 'NA', '0');
INSERT INTO `client_config_task` VALUES ('Section0011', '2', '群英战吕布', 'HeroPiece011', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint005', 'NA', '1');
INSERT INTO `client_config_task` VALUES ('Section0012', '1', '小沛', 'HeroPiece012', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint006', 'MapPoint007', '8');
INSERT INTO `client_config_task` VALUES ('Section0013', '1', '许昌', 'HeroPiece013', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint007', 'MapPoint008', '9');
INSERT INTO `client_config_task` VALUES ('Section0014', '1', '迷失的大军', 'HeroPiece014', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint008', 'NA', '10');
INSERT INTO `client_config_task` VALUES ('Section0015', '1', '净化冤魂', 'HeroPiece015', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint008', 'MapPoint009', '11');
INSERT INTO `client_config_task` VALUES ('Section0016', '2', '精忠报国', 'HeroPiece016', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint008', 'NA', '0');
INSERT INTO `client_config_task` VALUES ('Section0017', '2', '大战朱仙镇', 'HeroPiece017', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint008', 'NA', '1');
INSERT INTO `client_config_task` VALUES ('Section0018', '1', '函谷关', 'HeroPiece018', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint009', 'MapPoint010', '12');
INSERT INTO `client_config_task` VALUES ('Section0019', '1', '潼关', 'HeroPiece019', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint010', 'MapPoint011', '13');
INSERT INTO `client_config_task` VALUES ('Section0020', '1', '邪教的末日', 'HeroPiece020', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint011', 'NA', '14');
INSERT INTO `client_config_task` VALUES ('Section0021', '1', '解放长安', 'HeroPiece021', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint011', 'MapPoint012', '15');
INSERT INTO `client_config_task` VALUES ('Section0022', '2', '西行之路', 'HeroPiece022', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint011', 'NA', '0');
INSERT INTO `client_config_task` VALUES ('Section0023', '1', '渭南', 'HeroPiece023', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint012', 'MapPoint013', '16');
INSERT INTO `client_config_task` VALUES ('Section0024', '1', '泾阳', 'HeroPiece024', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint013', 'MapPoint014', '17');
INSERT INTO `client_config_task` VALUES ('Section0025', '1', '覆灭的阿房宫', 'HeroPiece025', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint014', 'NA', '18');
INSERT INTO `client_config_task` VALUES ('Section0026', '1', '湮灭心魔', 'HeroPiece026', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint014', 'MapPoint015', '19');
INSERT INTO `client_config_task` VALUES ('Section0027', '2', '倒长城之战', 'HeroPiece027', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint014', 'NA', '0');
INSERT INTO `client_config_task` VALUES ('Section0028', '2', '刺秦', 'HeroPiece028', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint014', 'NA', '1');
INSERT INTO `client_config_task` VALUES ('Section0029', '1', '汉中', 'HeroPiece029', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint015', 'MapPoint016', '20');
INSERT INTO `client_config_task` VALUES ('Section0030', '1', '剑阁', 'HeroPiece030', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint016', 'MapPoint017', '21');
INSERT INTO `client_config_task` VALUES ('Section0031', '1', '失败的北伐军', 'HeroPiece031', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint017', 'NA', '22');
INSERT INTO `client_config_task` VALUES ('Section0032', '1', '消逝的执念', 'HeroPiece032', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint017', 'MapPoint018', '23');
INSERT INTO `client_config_task` VALUES ('Section0033', '2', '神话的破灭', 'HeroPiece033', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint017', 'NA', '0');
INSERT INTO `client_config_task` VALUES ('Section0034', '1', '昆仑关', 'HeroPiece034', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint018', 'MapPoint019', '24');
INSERT INTO `client_config_task` VALUES ('Section0035', '1', '神农架', 'HeroPiece035', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint019', 'MapPoint020', '25');
INSERT INTO `client_config_task` VALUES ('Section0036', '1', '实力的比拼', 'HeroPiece036', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint020', 'NA', '26');
INSERT INTO `client_config_task` VALUES ('Section0037', '1', '神射之王的末日', 'HeroPiece037', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint020', 'MapPoint021', '27');
INSERT INTO `client_config_task` VALUES ('Section0038', '2', '百步穿杨', 'HeroPiece038', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint020', 'NA', '0');
INSERT INTO `client_config_task` VALUES ('Section0039', '2', '三箭定天下', 'HeroPiece039', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint020', 'NA', '1');
INSERT INTO `client_config_task` VALUES ('Section0040', '1', '乌林', 'HeroPiece040', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint021', 'MapPoint022', '28');
INSERT INTO `client_config_task` VALUES ('Section0041', '1', '洞庭', 'HeroPiece041', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint022', 'MapPoint023', '29');
INSERT INTO `client_config_task` VALUES ('Section0042', '1', '医者毒心', 'HeroPiece042', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint023', 'NA', '30');
INSERT INTO `client_config_task` VALUES ('Section0043', '1', '惩戒华佗', 'HeroPiece043', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint023', 'MapPoint024', '31');
INSERT INTO `client_config_task` VALUES ('Section0044', '2', '五禽戏', 'HeroPiece044', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint023', 'NA', '0');
INSERT INTO `client_config_task` VALUES ('Section0045', '1', '赤壁', 'HeroPiece045', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint024', 'MapPoint025', '32');
INSERT INTO `client_config_task` VALUES ('Section0046', '1', '庐陵', 'HeroPiece046', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint025', 'MapPoint026', '33');
INSERT INTO `client_config_task` VALUES ('Section0047', '1', '神奇的占卜', 'HeroPiece047', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint026', 'NA', '34');
INSERT INTO `client_config_task` VALUES ('Section0048', '1', '迷信的后果', 'HeroPiece048', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint026', 'MapPoint027', '35');
INSERT INTO `client_config_task` VALUES ('Section0049', '2', '龙王的报复', 'HeroPiece049', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint026', 'NA', '0');
INSERT INTO `client_config_task` VALUES ('Section0050', '1', '钱塘', 'HeroPiece050', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint027', 'MapPoint028', '36');
INSERT INTO `client_config_task` VALUES ('Section0051', '1', '澎湖', 'HeroPiece051', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint028', 'MapPoint029', '37');
INSERT INTO `client_config_task` VALUES ('Section0052', '1', '上岸水兵不如菜', 'HeroPiece052', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint029', 'NA', '38');
INSERT INTO `client_config_task` VALUES ('Section0053', '1', '远方的来客', 'HeroPiece053', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint029', 'NA', '39');
INSERT INTO `client_config_task` VALUES ('Section0054', '1', '攻占台湾', 'HeroPiece054', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint029', 'MapPoint030', '40');
INSERT INTO `client_config_task` VALUES ('Section0055', '2', '宝岛守卫战', 'HeroPiece055', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint029', 'NA', '0');
INSERT INTO `client_config_task` VALUES ('Section0056', '1', '钓鱼岛', 'HeroPiece056', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint030', 'MapPoint031', '41');
INSERT INTO `client_config_task` VALUES ('Section0057', '1', '蓬莱', 'HeroPiece057', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint031', 'MapPoint032', '42');
INSERT INTO `client_config_task` VALUES ('Section0058', '1', '远征', 'HeroPiece058', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint032', 'NA', '43');
INSERT INTO `client_config_task` VALUES ('Section0059', '1', '强势征服', 'HeroPiece059', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint032', 'MapPoint033', '44');
INSERT INTO `client_config_task` VALUES ('Section0060', '2', '半岛攻势', 'HeroPiece060', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint032', 'NA', '0');
INSERT INTO `client_config_task` VALUES ('Section0061', '1', '辽东', 'HeroPiece061', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint033', 'MapPoint034', '45');
INSERT INTO `client_config_task` VALUES ('Section0062', '1', '山海关', 'HeroPiece062', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint034', 'MapPoint035', '46');
INSERT INTO `client_config_task` VALUES ('Section0063', '1', '秘密潜入', 'HeroPiece063', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint035', 'NA', '47');
INSERT INTO `client_config_task` VALUES ('Section0064', '1', '强势的突击', 'HeroPiece064', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint035', 'NA', '48');
INSERT INTO `client_config_task` VALUES ('Section0065', '1', '一战定乾坤', 'HeroPiece065', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint035', 'MapPoint036', '49');
INSERT INTO `client_config_task` VALUES ('Section0066', '2', '问鼎天下', 'HeroPiece066', '2', 'NA', 'NA', '0', 'NA', 'NA', 'NA', 'NA', 'NA', 'NA', 'MapPoint035', 'NA', '0');

-- ----------------------------
-- Table structure for global_account
-- ----------------------------
DROP TABLE IF EXISTS `global_account`;
CREATE TABLE `global_account` (
  `user_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `plantform_id` int(10) unsigned NOT NULL COMMENT '渠道或平台id',
  `plantform_sub_id` smallint(5) unsigned NOT NULL COMMENT '渠道或平台的 下级子类型',
  `plantform_account` varchar(63) NOT NULL COMMENT '拼接了平台信息的账户, 比如qq  那么就是open_id+qq平台id',
  `password` varchar(63) NOT NULL COMMENT '密码 todo md5',
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of global_account
-- ----------------------------
INSERT INTO `global_account` VALUES ('1', '1', '1', 'asdasd', '123456');

-- ----------------------------
-- Table structure for sys_battle
-- ----------------------------
DROP TABLE IF EXISTS `sys_battle`;
CREATE TABLE `sys_battle` (
  `battle_id` int(10) unsigned NOT NULL COMMENT '战斗ID',
  PRIMARY KEY (`battle_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_battle
-- ----------------------------
INSERT INTO `sys_battle` VALUES ('1');
INSERT INTO `sys_battle` VALUES ('2');
INSERT INTO `sys_battle` VALUES ('3');
INSERT INTO `sys_battle` VALUES ('4');
INSERT INTO `sys_battle` VALUES ('5');
INSERT INTO `sys_battle` VALUES ('6');
INSERT INTO `sys_battle` VALUES ('7');
INSERT INTO `sys_battle` VALUES ('8');
INSERT INTO `sys_battle` VALUES ('9');
INSERT INTO `sys_battle` VALUES ('10');
INSERT INTO `sys_battle` VALUES ('11');
INSERT INTO `sys_battle` VALUES ('12');
INSERT INTO `sys_battle` VALUES ('13');
INSERT INTO `sys_battle` VALUES ('14');
INSERT INTO `sys_battle` VALUES ('15');
INSERT INTO `sys_battle` VALUES ('16');
INSERT INTO `sys_battle` VALUES ('17');
INSERT INTO `sys_battle` VALUES ('18');
INSERT INTO `sys_battle` VALUES ('19');
INSERT INTO `sys_battle` VALUES ('20');
INSERT INTO `sys_battle` VALUES ('21');
INSERT INTO `sys_battle` VALUES ('22');
INSERT INTO `sys_battle` VALUES ('23');
INSERT INTO `sys_battle` VALUES ('24');
INSERT INTO `sys_battle` VALUES ('25');
INSERT INTO `sys_battle` VALUES ('26');
INSERT INTO `sys_battle` VALUES ('27');
INSERT INTO `sys_battle` VALUES ('28');
INSERT INTO `sys_battle` VALUES ('29');
INSERT INTO `sys_battle` VALUES ('30');
INSERT INTO `sys_battle` VALUES ('31');
INSERT INTO `sys_battle` VALUES ('32');
INSERT INTO `sys_battle` VALUES ('33');
INSERT INTO `sys_battle` VALUES ('34');
INSERT INTO `sys_battle` VALUES ('35');
INSERT INTO `sys_battle` VALUES ('36');
INSERT INTO `sys_battle` VALUES ('37');
INSERT INTO `sys_battle` VALUES ('38');
INSERT INTO `sys_battle` VALUES ('39');
INSERT INTO `sys_battle` VALUES ('40');
INSERT INTO `sys_battle` VALUES ('41');
INSERT INTO `sys_battle` VALUES ('42');
INSERT INTO `sys_battle` VALUES ('43');
INSERT INTO `sys_battle` VALUES ('44');
INSERT INTO `sys_battle` VALUES ('45');
INSERT INTO `sys_battle` VALUES ('46');
INSERT INTO `sys_battle` VALUES ('47');
INSERT INTO `sys_battle` VALUES ('48');
INSERT INTO `sys_battle` VALUES ('49');
INSERT INTO `sys_battle` VALUES ('50');
INSERT INTO `sys_battle` VALUES ('51');
INSERT INTO `sys_battle` VALUES ('52');
INSERT INTO `sys_battle` VALUES ('53');
INSERT INTO `sys_battle` VALUES ('54');
INSERT INTO `sys_battle` VALUES ('55');
INSERT INTO `sys_battle` VALUES ('56');
INSERT INTO `sys_battle` VALUES ('57');
INSERT INTO `sys_battle` VALUES ('58');
INSERT INTO `sys_battle` VALUES ('59');
INSERT INTO `sys_battle` VALUES ('60');
INSERT INTO `sys_battle` VALUES ('61');
INSERT INTO `sys_battle` VALUES ('62');
INSERT INTO `sys_battle` VALUES ('63');
INSERT INTO `sys_battle` VALUES ('64');
INSERT INTO `sys_battle` VALUES ('65');
INSERT INTO `sys_battle` VALUES ('66');
INSERT INTO `sys_battle` VALUES ('67');
INSERT INTO `sys_battle` VALUES ('68');
INSERT INTO `sys_battle` VALUES ('69');
INSERT INTO `sys_battle` VALUES ('70');
INSERT INTO `sys_battle` VALUES ('71');
INSERT INTO `sys_battle` VALUES ('72');
INSERT INTO `sys_battle` VALUES ('73');
INSERT INTO `sys_battle` VALUES ('74');
INSERT INTO `sys_battle` VALUES ('75');
INSERT INTO `sys_battle` VALUES ('76');
INSERT INTO `sys_battle` VALUES ('77');
INSERT INTO `sys_battle` VALUES ('78');
INSERT INTO `sys_battle` VALUES ('79');
INSERT INTO `sys_battle` VALUES ('80');
INSERT INTO `sys_battle` VALUES ('81');
INSERT INTO `sys_battle` VALUES ('82');
INSERT INTO `sys_battle` VALUES ('83');
INSERT INTO `sys_battle` VALUES ('84');
INSERT INTO `sys_battle` VALUES ('85');
INSERT INTO `sys_battle` VALUES ('86');
INSERT INTO `sys_battle` VALUES ('87');
INSERT INTO `sys_battle` VALUES ('88');
INSERT INTO `sys_battle` VALUES ('89');
INSERT INTO `sys_battle` VALUES ('90');
INSERT INTO `sys_battle` VALUES ('91');
INSERT INTO `sys_battle` VALUES ('92');
INSERT INTO `sys_battle` VALUES ('93');
INSERT INTO `sys_battle` VALUES ('94');
INSERT INTO `sys_battle` VALUES ('95');
INSERT INTO `sys_battle` VALUES ('96');
INSERT INTO `sys_battle` VALUES ('97');
INSERT INTO `sys_battle` VALUES ('98');
INSERT INTO `sys_battle` VALUES ('99');
INSERT INTO `sys_battle` VALUES ('100');
INSERT INTO `sys_battle` VALUES ('101');
INSERT INTO `sys_battle` VALUES ('102');
INSERT INTO `sys_battle` VALUES ('103');
INSERT INTO `sys_battle` VALUES ('104');
INSERT INTO `sys_battle` VALUES ('105');
INSERT INTO `sys_battle` VALUES ('106');
INSERT INTO `sys_battle` VALUES ('107');
INSERT INTO `sys_battle` VALUES ('108');
INSERT INTO `sys_battle` VALUES ('109');
INSERT INTO `sys_battle` VALUES ('110');
INSERT INTO `sys_battle` VALUES ('111');
INSERT INTO `sys_battle` VALUES ('112');
INSERT INTO `sys_battle` VALUES ('113');
INSERT INTO `sys_battle` VALUES ('114');
INSERT INTO `sys_battle` VALUES ('115');
INSERT INTO `sys_battle` VALUES ('116');
INSERT INTO `sys_battle` VALUES ('117');
INSERT INTO `sys_battle` VALUES ('118');
INSERT INTO `sys_battle` VALUES ('119');
INSERT INTO `sys_battle` VALUES ('120');
INSERT INTO `sys_battle` VALUES ('121');
INSERT INTO `sys_battle` VALUES ('122');
INSERT INTO `sys_battle` VALUES ('123');
INSERT INTO `sys_battle` VALUES ('124');
INSERT INTO `sys_battle` VALUES ('125');
INSERT INTO `sys_battle` VALUES ('126');
INSERT INTO `sys_battle` VALUES ('127');
INSERT INTO `sys_battle` VALUES ('128');
INSERT INTO `sys_battle` VALUES ('129');
INSERT INTO `sys_battle` VALUES ('130');
INSERT INTO `sys_battle` VALUES ('131');
INSERT INTO `sys_battle` VALUES ('132');
INSERT INTO `sys_battle` VALUES ('133');
INSERT INTO `sys_battle` VALUES ('134');
INSERT INTO `sys_battle` VALUES ('135');
INSERT INTO `sys_battle` VALUES ('136');
INSERT INTO `sys_battle` VALUES ('137');
INSERT INTO `sys_battle` VALUES ('138');
INSERT INTO `sys_battle` VALUES ('139');
INSERT INTO `sys_battle` VALUES ('140');
INSERT INTO `sys_battle` VALUES ('141');
INSERT INTO `sys_battle` VALUES ('142');
INSERT INTO `sys_battle` VALUES ('143');
INSERT INTO `sys_battle` VALUES ('144');
INSERT INTO `sys_battle` VALUES ('145');
INSERT INTO `sys_battle` VALUES ('146');
INSERT INTO `sys_battle` VALUES ('147');
INSERT INTO `sys_battle` VALUES ('148');
INSERT INTO `sys_battle` VALUES ('149');
INSERT INTO `sys_battle` VALUES ('150');
INSERT INTO `sys_battle` VALUES ('151');
INSERT INTO `sys_battle` VALUES ('152');
INSERT INTO `sys_battle` VALUES ('153');
INSERT INTO `sys_battle` VALUES ('154');
INSERT INTO `sys_battle` VALUES ('155');
INSERT INTO `sys_battle` VALUES ('156');
INSERT INTO `sys_battle` VALUES ('157');
INSERT INTO `sys_battle` VALUES ('158');
INSERT INTO `sys_battle` VALUES ('159');
INSERT INTO `sys_battle` VALUES ('160');
INSERT INTO `sys_battle` VALUES ('161');
INSERT INTO `sys_battle` VALUES ('162');
INSERT INTO `sys_battle` VALUES ('163');
INSERT INTO `sys_battle` VALUES ('164');
INSERT INTO `sys_battle` VALUES ('165');
INSERT INTO `sys_battle` VALUES ('166');
INSERT INTO `sys_battle` VALUES ('167');
INSERT INTO `sys_battle` VALUES ('168');
INSERT INTO `sys_battle` VALUES ('169');
INSERT INTO `sys_battle` VALUES ('170');
INSERT INTO `sys_battle` VALUES ('171');
INSERT INTO `sys_battle` VALUES ('172');
INSERT INTO `sys_battle` VALUES ('173');
INSERT INTO `sys_battle` VALUES ('174');
INSERT INTO `sys_battle` VALUES ('175');
INSERT INTO `sys_battle` VALUES ('176');
INSERT INTO `sys_battle` VALUES ('177');
INSERT INTO `sys_battle` VALUES ('178');
INSERT INTO `sys_battle` VALUES ('179');
INSERT INTO `sys_battle` VALUES ('180');
INSERT INTO `sys_battle` VALUES ('181');
INSERT INTO `sys_battle` VALUES ('182');
INSERT INTO `sys_battle` VALUES ('183');
INSERT INTO `sys_battle` VALUES ('184');
INSERT INTO `sys_battle` VALUES ('185');
INSERT INTO `sys_battle` VALUES ('186');
INSERT INTO `sys_battle` VALUES ('187');
INSERT INTO `sys_battle` VALUES ('188');
INSERT INTO `sys_battle` VALUES ('189');
INSERT INTO `sys_battle` VALUES ('190');
INSERT INTO `sys_battle` VALUES ('191');
INSERT INTO `sys_battle` VALUES ('192');
INSERT INTO `sys_battle` VALUES ('193');
INSERT INTO `sys_battle` VALUES ('194');
INSERT INTO `sys_battle` VALUES ('195');
INSERT INTO `sys_battle` VALUES ('196');
INSERT INTO `sys_battle` VALUES ('197');
INSERT INTO `sys_battle` VALUES ('198');
INSERT INTO `sys_battle` VALUES ('199');
INSERT INTO `sys_battle` VALUES ('200');
INSERT INTO `sys_battle` VALUES ('201');
INSERT INTO `sys_battle` VALUES ('202');
INSERT INTO `sys_battle` VALUES ('203');
INSERT INTO `sys_battle` VALUES ('204');
INSERT INTO `sys_battle` VALUES ('205');
INSERT INTO `sys_battle` VALUES ('206');
INSERT INTO `sys_battle` VALUES ('207');
INSERT INTO `sys_battle` VALUES ('208');
INSERT INTO `sys_battle` VALUES ('209');
INSERT INTO `sys_battle` VALUES ('210');
INSERT INTO `sys_battle` VALUES ('211');
INSERT INTO `sys_battle` VALUES ('212');
INSERT INTO `sys_battle` VALUES ('213');
INSERT INTO `sys_battle` VALUES ('214');
INSERT INTO `sys_battle` VALUES ('215');
INSERT INTO `sys_battle` VALUES ('216');
INSERT INTO `sys_battle` VALUES ('217');
INSERT INTO `sys_battle` VALUES ('218');
INSERT INTO `sys_battle` VALUES ('219');
INSERT INTO `sys_battle` VALUES ('220');
INSERT INTO `sys_battle` VALUES ('221');
INSERT INTO `sys_battle` VALUES ('222');
INSERT INTO `sys_battle` VALUES ('223');
INSERT INTO `sys_battle` VALUES ('224');
INSERT INTO `sys_battle` VALUES ('225');
INSERT INTO `sys_battle` VALUES ('226');
INSERT INTO `sys_battle` VALUES ('227');
INSERT INTO `sys_battle` VALUES ('228');
INSERT INTO `sys_battle` VALUES ('229');
INSERT INTO `sys_battle` VALUES ('230');
INSERT INTO `sys_battle` VALUES ('231');
INSERT INTO `sys_battle` VALUES ('232');
INSERT INTO `sys_battle` VALUES ('233');
INSERT INTO `sys_battle` VALUES ('234');
INSERT INTO `sys_battle` VALUES ('235');
INSERT INTO `sys_battle` VALUES ('236');
INSERT INTO `sys_battle` VALUES ('237');
INSERT INTO `sys_battle` VALUES ('238');
INSERT INTO `sys_battle` VALUES ('239');
INSERT INTO `sys_battle` VALUES ('240');
INSERT INTO `sys_battle` VALUES ('241');
INSERT INTO `sys_battle` VALUES ('242');

-- ----------------------------
-- Table structure for sys_burst_packet
-- ----------------------------
DROP TABLE IF EXISTS `sys_burst_packet`;
CREATE TABLE `sys_burst_packet` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `packet_id` int(10) unsigned NOT NULL COMMENT '包的id',
  `burst_type` tinyint(1) NOT NULL COMMENT '爆落类型 1 N选1  2N选N',
  `element_type_id_arr` varchar(511) NOT NULL COMMENT '[类型-id]列表 可能为道具 也可能为packet或者英雄等等 形如 1-203|2-15|3-33',
  `element_probability_arr` varchar(511) NOT NULL COMMENT '概率列表，每个元素的概率值 形如 5000|1000|20000',
  `element_num_range_arr` varchar(511) NOT NULL COMMENT '每种元素的数量范围  形如 0-1|2-4',
  PRIMARY KEY (`id`),
  UNIQUE KEY `packet_id` (`packet_id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_burst_packet
-- ----------------------------
INSERT INTO `sys_burst_packet` VALUES ('1', '1', '1', '8-1|8-2|8-3', '20000|30000|50000', '1-1|1-1|1-1');
INSERT INTO `sys_burst_packet` VALUES ('2', '2', '1', '8-1|8-2|8-3', '20000|30000|50000', '1-1|1-1|1-1');
INSERT INTO `sys_burst_packet` VALUES ('3', '3', '1', '8-1|8-2|8-3', '20000|30000|50000', '1-1|1-1|1-1');
INSERT INTO `sys_burst_packet` VALUES ('4', '4', '1', '8-1|8-2|8-3', '20000|30000|50000', '1-1|1-1|1-1');
INSERT INTO `sys_burst_packet` VALUES ('5', '5', '1', '8-1|8-2|8-3', '20000|30000|50000', '1-1|1-1|1-1');

-- ----------------------------
-- Table structure for sys_chapter
-- ----------------------------
DROP TABLE IF EXISTS `sys_chapter`;
CREATE TABLE `sys_chapter` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '为了运营后台容易增删改， 不代表chapter_id是因为excel表格上的顺序经常改变',
  `chapter_id` tinyint(3) unsigned NOT NULL COMMENT '章节id， 独立出来 而不用自增id字段 是因为表格顺序经常会改动 导致id会改变！但是这些数据会存储在玩家那里 所以要保证不能因为插入顺序而改变',
  `name` varchar(63) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `chapter_id` (`chapter_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_chapter
-- ----------------------------
INSERT INTO `sys_chapter` VALUES ('1', '1', '华夏');
INSERT INTO `sys_chapter` VALUES ('2', '2', '欧美');
INSERT INTO `sys_chapter` VALUES ('3', '3', '大和之章');

-- ----------------------------
-- Table structure for sys_chapter_mappoint
-- ----------------------------
DROP TABLE IF EXISTS `sys_chapter_mappoint`;
CREATE TABLE `sys_chapter_mappoint` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '据点id 可以是城市 也可以小点',
  `mappoint_id` int(10) unsigned NOT NULL COMMENT '据点id  城市或者小点的唯一标识！',
  `chapter_id` tinyint(3) unsigned NOT NULL COMMENT '所属章节id',
  `type` tinyint(3) unsigned NOT NULL COMMENT '类型 1.城市 2.小点',
  `name` varchar(63) DEFAULT NULL COMMENT '据点的名字 城市需要 小点不需要',
  `joined_mappoint_id_arr` varchar(255) DEFAULT NULL COMMENT '最大255字节 也就是说 连接点最多 255/4 = 63个！',
  `hidden` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否隐藏 0否 1是。 当隐藏的时候 即使有边链接到这个点，客户端判断这个点隐藏了 也要取消掉这些连线！',
  PRIMARY KEY (`id`),
  UNIQUE KEY `mappoint_id` (`mappoint_id`),
  KEY `chapter_id` (`chapter_id`)
) ENGINE=InnoDB AUTO_INCREMENT=36 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_chapter_mappoint
-- ----------------------------
INSERT INTO `sys_chapter_mappoint` VALUES ('1', '1', '1', '2', '逐鹿', '2', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('2', '2', '1', '1', '钜鹿', '1|3', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('3', '3', '1', '2', '平原', '2|4', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('4', '4', '1', '2', '北海', '3|5', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('5', '5', '1', '1', '徐州', '4|6', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('6', '6', '1', '2', '小沛', '5|7', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('7', '7', '1', '2', '许昌', '6|8', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('8', '8', '1', '1', '洛阳', '7|9', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('9', '9', '1', '2', '函谷关', '8|10', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('10', '10', '1', '2', '潼关', '9|11', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('11', '11', '1', '1', '长安', '10|12', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('12', '12', '1', '2', '渭南', '11|13', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('13', '13', '1', '2', '泾阳', '12|14', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('14', '14', '1', '1', '咸阳', '13|15', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('15', '15', '1', '2', '汉中', '14|16', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('16', '16', '1', '2', '剑阁', '15|17', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('17', '17', '1', '1', '成都', '16|18', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('18', '18', '1', '2', '昆仑关', '17|19', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('19', '19', '1', '2', '神农架', '18|20', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('20', '20', '1', '1', '襄阳', '19|21', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('21', '21', '1', '2', '乌林', '20|22', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('22', '22', '1', '2', '洞庭', '21|23', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('23', '23', '1', '1', '长沙', '22|24', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('24', '24', '1', '2', '赤壁', '23|25', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('25', '25', '1', '2', '庐陵', '24|26', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('26', '26', '1', '1', '金陵', '25|27', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('27', '27', '1', '2', '钱塘', '26|28', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('28', '28', '1', '2', '澎湖', '27|29', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('29', '29', '1', '1', '台湾', '28|30', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('30', '30', '1', '2', '钓鱼岛', '29|31', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('31', '31', '1', '2', '蓬莱', '30|32', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('32', '32', '1', '1', '汉城', '31|33', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('33', '33', '1', '2', '辽东', '32|34', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('34', '34', '1', '2', '山海关', '33|35', '0');
INSERT INTO `sys_chapter_mappoint` VALUES ('35', '35', '1', '1', '京城', '34', '0');

-- ----------------------------
-- Table structure for sys_equip
-- ----------------------------
DROP TABLE IF EXISTS `sys_equip`;
CREATE TABLE `sys_equip` (
  `id` int(10) unsigned NOT NULL,
  `equip_id` int(10) unsigned NOT NULL,
  `equip_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '装备类型  武器 = 1, 项链 = 2 , 衣服 = 3, 腰带= 4, 戒指 = 5 , 鞋子 = 6',
  `name_cn` varchar(255) NOT NULL DEFAULT '' COMMENT '名字',
  `quality` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '品质',
  `evolution_equip_id` int(10) unsigned NOT NULL COMMENT '进化后的装备id',
  `evolution_input` varchar(511) NOT NULL DEFAULT '' COMMENT '进化所需要的消耗 形如  type-id-num|type-id-num : 1-555-10|2-5864-2',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_equip
-- ----------------------------
INSERT INTO `sys_equip` VALUES ('1', '1', '1', '短剑', '1', '2', '8-1-1');
INSERT INTO `sys_equip` VALUES ('2', '2', '1', '青铜剑', '2', '3', '8-7-2');
INSERT INTO `sys_equip` VALUES ('3', '3', '1', '百炼铁剑', '3', '4', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('4', '4', '1', '流星剑', '4', '5', '8-13-3|8-20-3');
INSERT INTO `sys_equip` VALUES ('5', '5', '1', '含光剑', '5', '6', '8-24-2|8-20-4');
INSERT INTO `sys_equip` VALUES ('6', '6', '1', '承影剑', '6', '7', '8-24-2|8-30-2|8-20-4');
INSERT INTO `sys_equip` VALUES ('7', '7', '1', '龙泉宝剑', '7', '8', '8-30-3|8-37-3');
INSERT INTO `sys_equip` VALUES ('8', '8', '1', '断天青虹剑', '8', '9', '8-41-2|8-37-4');
INSERT INTO `sys_equip` VALUES ('9', '9', '1', '七星龙渊剑', '9', '10', '8-41-2|8-47-2|8-37-6');
INSERT INTO `sys_equip` VALUES ('10', '10', '1', '赤霄帝剑', '10', '11', '8-47-3|8-53-2|8-60-1');
INSERT INTO `sys_equip` VALUES ('11', '11', '1', '轩辕圣剑', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('12', '21', '1', '竹弓', '1', '22', '8-1-1');
INSERT INTO `sys_equip` VALUES ('13', '22', '1', '反曲弓', '2', '23', '8-7-2');
INSERT INTO `sys_equip` VALUES ('14', '23', '1', '复合弓', '3', '24', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('15', '24', '1', '强弩', '4', '25', '8-13-3|8-21-3');
INSERT INTO `sys_equip` VALUES ('16', '25', '1', '十字弩', '5', '26', '8-24-2|8-21-4');
INSERT INTO `sys_equip` VALUES ('17', '26', '1', '连弩', '6', '27', '8-24-2|8-30-2|8-21-4');
INSERT INTO `sys_equip` VALUES ('18', '27', '1', '神臂弓', '7', '28', '8-30-3|8-38-3');
INSERT INTO `sys_equip` VALUES ('19', '28', '1', '霸王弓', '8', '29', '8-41-2|8-38-4');
INSERT INTO `sys_equip` VALUES ('20', '29', '1', '飞将灵宝弓', '9', '30', '8-41-2|8-47-2|8-38-6');
INSERT INTO `sys_equip` VALUES ('21', '30', '1', '穿云震天弓', '10', '31', '8-47-3|8-53-2|8-61-1');
INSERT INTO `sys_equip` VALUES ('22', '31', '1', '落日神弓', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('23', '41', '1', '竹杖', '1', '42', '8-1-1');
INSERT INTO `sys_equip` VALUES ('24', '42', '1', '青铜杖', '2', '43', '8-7-2');
INSERT INTO `sys_equip` VALUES ('25', '43', '1', '梵谷铜杖', '3', '44', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('26', '44', '1', '玄铁杖', '4', '45', '8-13-3|8-23-3');
INSERT INTO `sys_equip` VALUES ('27', '45', '1', '伏魔铁杖', '5', '46', '8-24-2|8-23-4');
INSERT INTO `sys_equip` VALUES ('28', '46', '1', '达摩禅杖', '6', '47', '8-24-2|8-30-2|8-23-4');
INSERT INTO `sys_equip` VALUES ('29', '47', '1', '降龙神木杖', '7', '48', '8-30-3|8-40-3');
INSERT INTO `sys_equip` VALUES ('30', '48', '1', '龙魂夺魄杖', '8', '49', '8-41-2|8-40-4');
INSERT INTO `sys_equip` VALUES ('31', '49', '1', '四海神龙杖', '9', '50', '8-41-2|8-47-2|8-40-6');
INSERT INTO `sys_equip` VALUES ('32', '50', '1', '龙首拐杖', '10', '51', '8-47-3|8-53-2|8-63-1');
INSERT INTO `sys_equip` VALUES ('33', '51', '1', '九宝扶桑杖', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('34', '61', '1', '纸扇', '1', '62', '8-1-1');
INSERT INTO `sys_equip` VALUES ('35', '62', '1', '竹木扇', '2', '63', '8-7-2');
INSERT INTO `sys_equip` VALUES ('36', '63', '1', '檀香木扇', '3', '64', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('37', '64', '1', '羽扇', '4', '65', '8-13-3|8-22-3');
INSERT INTO `sys_equip` VALUES ('38', '65', '1', '尘尾扇', '5', '66', '8-24-2|8-22-4');
INSERT INTO `sys_equip` VALUES ('39', '66', '1', '孔雀羽尾扇', '6', '67', '8-24-2|8-30-2|8-22-4');
INSERT INTO `sys_equip` VALUES ('40', '67', '1', '火翼翅扇', '7', '68', '8-30-3|8-39-3');
INSERT INTO `sys_equip` VALUES ('41', '68', '1', '雷光白羽扇', '8', '69', '8-41-2|8-39-4');
INSERT INTO `sys_equip` VALUES ('42', '69', '1', '玄冰翼扇', '9', '70', '8-41-2|8-47-2|8-39-6');
INSERT INTO `sys_equip` VALUES ('43', '70', '1', '疾风无影扇', '10', '71', '8-47-3|8-53-2|8-62-1');
INSERT INTO `sys_equip` VALUES ('44', '71', '1', '五火神焰扇', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('45', '81', '1', '石矛', '1', '82', '8-1-1');
INSERT INTO `sys_equip` VALUES ('46', '82', '1', '铁矛', '2', '83', '8-7-2');
INSERT INTO `sys_equip` VALUES ('47', '83', '1', '镔铁矛', '3', '84', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('48', '84', '1', '短戟', '4', '85', '8-13-3|8-19-3');
INSERT INTO `sys_equip` VALUES ('49', '85', '1', '飞戟', '5', '86', '8-24-2|8-19-4');
INSERT INTO `sys_equip` VALUES ('50', '86', '1', '断刃戟', '6', '87', '8-24-2|8-30-2|8-19-4');
INSERT INTO `sys_equip` VALUES ('51', '87', '1', '烈风戟', '7', '88', '8-30-3|8-36-3');
INSERT INTO `sys_equip` VALUES ('52', '88', '1', '虎啸镇山戟', '8', '89', '8-41-2|8-36-4');
INSERT INTO `sys_equip` VALUES ('53', '89', '1', '风雷化神戟', '9', '90', '8-41-2|8-47-2|8-36-6');
INSERT INTO `sys_equip` VALUES ('54', '90', '1', '龙魄斩天戟', '10', '91', '8-47-3|8-53-2|8-59-1');
INSERT INTO `sys_equip` VALUES ('55', '91', '1', '天命神戟', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('56', '101', '1', '硬木矛', '1', '102', '8-1-1');
INSERT INTO `sys_equip` VALUES ('57', '102', '1', '铜矛', '2', '103', '8-7-2');
INSERT INTO `sys_equip` VALUES ('58', '103', '1', '青铜矛', '3', '104', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('59', '104', '1', '手戟', '4', '105', '8-13-3|8-19-3');
INSERT INTO `sys_equip` VALUES ('60', '105', '1', '镔铁戟', '5', '106', '8-24-2|8-19-4');
INSERT INTO `sys_equip` VALUES ('61', '106', '1', '月牙戟', '6', '107', '8-24-2|8-30-2|8-19-4');
INSERT INTO `sys_equip` VALUES ('62', '107', '1', '斩魄银戟', '7', '108', '8-30-3|8-36-3');
INSERT INTO `sys_equip` VALUES ('63', '108', '1', '虎威破魂戟', '8', '109', '8-41-2|8-36-4');
INSERT INTO `sys_equip` VALUES ('64', '109', '1', '雷火震天戟', '9', '110', '8-41-2|8-47-2|8-36-6');
INSERT INTO `sys_equip` VALUES ('65', '110', '1', '化龙擎天戟', '10', '111', '8-47-3|8-53-2|8-59-1');
INSERT INTO `sys_equip` VALUES ('66', '111', '1', '天龙破城戟', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('67', '121', '1', '木枪', '1', '122', '8-1-1');
INSERT INTO `sys_equip` VALUES ('68', '122', '1', '铜枪', '2', '123', '8-7-2');
INSERT INTO `sys_equip` VALUES ('69', '123', '1', '青铜枪', '3', '124', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('70', '124', '1', '长枪', '4', '125', '8-13-3|8-20-3');
INSERT INTO `sys_equip` VALUES ('71', '125', '1', '镔铁枪', '5', '126', '8-24-2|8-20-4');
INSERT INTO `sys_equip` VALUES ('72', '126', '1', '铁脊蛇矛', '6', '127', '8-24-2|8-30-2|8-20-4');
INSERT INTO `sys_equip` VALUES ('73', '127', '1', '霸王枪', '7', '128', '8-30-3|8-37-3');
INSERT INTO `sys_equip` VALUES ('74', '128', '1', '神威烈水枪', '8', '129', '8-41-2|8-37-4');
INSERT INTO `sys_equip` VALUES ('75', '129', '1', '风雪冰魄枪', '9', '130', '8-41-2|8-47-2|8-37-6');
INSERT INTO `sys_equip` VALUES ('76', '130', '1', '火龙断魂枪', '10', '131', '8-47-3|8-53-2|8-60-1');
INSERT INTO `sys_equip` VALUES ('77', '131', '1', '沥泉神枪', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('78', '141', '1', '短统', '1', '142', '8-1-1');
INSERT INTO `sys_equip` VALUES ('79', '142', '1', '三眼统', '2', '143', '8-7-2');
INSERT INTO `sys_equip` VALUES ('80', '143', '1', '迅雷火统', '3', '144', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('81', '144', '1', '火绳枪', '4', '145', '8-13-3|8-21-3');
INSERT INTO `sys_equip` VALUES ('82', '145', '1', '鲁密统', '5', '146', '8-24-2|8-21-4');
INSERT INTO `sys_equip` VALUES ('83', '146', '1', '燧发枪', '6', '147', '8-24-2|8-30-2|8-21-4');
INSERT INTO `sys_equip` VALUES ('84', '147', '1', '弗朗机手统', '7', '148', '8-30-3|8-38-3');
INSERT INTO `sys_equip` VALUES ('85', '148', '1', '迅捷手统', '8', '149', '8-41-2|8-38-4');
INSERT INTO `sys_equip` VALUES ('86', '149', '1', '忏悔', '9', '150', '8-41-2|8-47-2|8-38-6');
INSERT INTO `sys_equip` VALUES ('87', '150', '1', '野心', '10', '151', '8-47-3|8-53-2|8-61-1');
INSERT INTO `sys_equip` VALUES ('88', '151', '1', '裁决', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('89', '161', '1', '檀木球', '1', '162', '8-1-1');
INSERT INTO `sys_equip` VALUES ('90', '162', '1', '青铜珠', '2', '163', '8-7-2');
INSERT INTO `sys_equip` VALUES ('91', '163', '1', '闪光银珠', '3', '164', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('92', '164', '1', '祈福轮', '4', '165', '8-13-3|8-22-3');
INSERT INTO `sys_equip` VALUES ('93', '165', '1', '法王诵经轮', '5', '166', '8-24-2|8-22-4');
INSERT INTO `sys_equip` VALUES ('94', '166', '1', '转世经轮', '6', '167', '8-24-2|8-30-2|8-22-4');
INSERT INTO `sys_equip` VALUES ('95', '167', '1', '光耀戳目珠', '7', '168', '8-30-3|8-39-3');
INSERT INTO `sys_equip` VALUES ('96', '168', '1', '开天辟地珠', '8', '169', '8-41-2|8-39-4');
INSERT INTO `sys_equip` VALUES ('97', '169', '1', '混元宝珠', '9', '170', '8-41-2|8-47-2|8-39-6');
INSERT INTO `sys_equip` VALUES ('98', '170', '1', '立界定海珠', '10', '171', '8-47-3|8-53-2|8-62-1');
INSERT INTO `sys_equip` VALUES ('99', '171', '1', '日月神珠', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('100', '181', '1', '木葫芦', '1', '182', '8-1-1');
INSERT INTO `sys_equip` VALUES ('101', '182', '1', '雕花葫芦', '2', '183', '8-7-2');
INSERT INTO `sys_equip` VALUES ('102', '183', '1', '符咒葫芦', '3', '184', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('103', '184', '1', '刻印铜葫芦', '4', '185', '8-13-3|8-23-3');
INSERT INTO `sys_equip` VALUES ('104', '185', '1', '风水葫芦', '5', '186', '8-24-2|8-23-4');
INSERT INTO `sys_equip` VALUES ('105', '186', '1', '八宝铜葫芦', '6', '187', '8-24-2|8-30-2|8-23-4');
INSERT INTO `sys_equip` VALUES ('106', '187', '1', '翡翠玉葫', '7', '188', '8-30-3|8-40-3');
INSERT INTO `sys_equip` VALUES ('107', '188', '1', '玄雷冰葫', '8', '189', '8-41-2|8-40-4');
INSERT INTO `sys_equip` VALUES ('108', '189', '1', '九九散魄葫芦', '9', '190', '8-41-2|8-47-2|8-40-6');
INSERT INTO `sys_equip` VALUES ('109', '190', '1', '紫金霹雳葫', '10', '191', '8-47-3|8-53-2|8-63-1');
INSERT INTO `sys_equip` VALUES ('110', '191', '1', '太极水火葫', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('111', '201', '1', '纸面扇', '1', '202', '8-1-1');
INSERT INTO `sys_equip` VALUES ('112', '202', '1', '稠纸扇', '2', '203', '8-7-2');
INSERT INTO `sys_equip` VALUES ('113', '203', '1', '紫檀木扇', '3', '204', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('114', '204', '1', '清羽扇', '4', '205', '8-13-3|8-22-3');
INSERT INTO `sys_equip` VALUES ('115', '205', '1', '天鹅尾扇', '5', '206', '8-24-2|8-22-4');
INSERT INTO `sys_equip` VALUES ('116', '206', '1', '雕翎羽扇', '6', '207', '8-24-2|8-30-2|8-22-4');
INSERT INTO `sys_equip` VALUES ('117', '207', '1', '疾风扇羽', '7', '208', '8-30-3|8-39-3');
INSERT INTO `sys_equip` VALUES ('118', '208', '1', '雷切闪羽', '8', '209', '8-41-2|8-39-4');
INSERT INTO `sys_equip` VALUES ('119', '209', '1', '千羽光翼扇', '9', '210', '8-41-2|8-47-2|8-39-6');
INSERT INTO `sys_equip` VALUES ('120', '210', '1', '冰晶魔翼扇', '10', '211', '8-47-3|8-53-2|8-62-1');
INSERT INTO `sys_equip` VALUES ('121', '211', '1', '安倍晴明11', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('122', '221', '1', '护手双刃', '1', '222', '8-1-1');
INSERT INTO `sys_equip` VALUES ('123', '222', '1', '刺钩双刀', '2', '223', '8-7-2');
INSERT INTO `sys_equip` VALUES ('124', '223', '1', '残忍双刃', '3', '224', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('125', '224', '1', '绝情双刀', '4', '225', '8-13-3|8-19-3');
INSERT INTO `sys_equip` VALUES ('126', '225', '1', '合金双刀', '5', '226', '8-24-2|8-19-4');
INSERT INTO `sys_equip` VALUES ('127', '226', '1', '高科技合金双刀', '6', '227', '8-24-2|8-30-2|8-19-4');
INSERT INTO `sys_equip` VALUES ('128', '227', '1', '狂风双刀', '7', '228', '8-30-3|8-36-3');
INSERT INTO `sys_equip` VALUES ('129', '228', '1', '怒雷双斩刃', '8', '229', '8-41-2|8-36-4');
INSERT INTO `sys_equip` VALUES ('130', '229', '1', '风雷怒斩双刀', '9', '230', '8-41-2|8-47-2|8-36-6');
INSERT INTO `sys_equip` VALUES ('131', '230', '1', '月斩.风.雷', '10', '231', '8-47-3|8-53-2|8-59-1');
INSERT INTO `sys_equip` VALUES ('132', '231', '1', '宫本武藏11', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('133', '241', '1', '训练短刀', '1', '242', '8-1-1');
INSERT INTO `sys_equip` VALUES ('134', '242', '1', '小太刀', '2', '243', '8-7-2');
INSERT INTO `sys_equip` VALUES ('135', '243', '1', '野太刀', '3', '244', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('136', '244', '1', '小乌丸', '4', '245', '8-13-3|8-20-3');
INSERT INTO `sys_equip` VALUES ('137', '245', '1', '莳绘太刀', '5', '246', '8-24-2|8-20-4');
INSERT INTO `sys_equip` VALUES ('138', '246', '1', '餝太刀', '6', '247', '8-24-2|8-30-2|8-20-4');
INSERT INTO `sys_equip` VALUES ('139', '247', '1', '玉缠横刀', '7', '248', '8-30-3|8-37-3');
INSERT INTO `sys_equip` VALUES ('140', '248', '1', '童子切安纲', '8', '249', '8-41-2|8-37-4');
INSERT INTO `sys_equip` VALUES ('141', '249', '1', '鬼丸国纲', '9', '250', '8-41-2|8-47-2|8-37-6');
INSERT INTO `sys_equip` VALUES ('142', '250', '1', '大典太刀', '10', '251', '8-47-3|8-53-2|8-60-1');
INSERT INTO `sys_equip` VALUES ('143', '251', '1', '织田信长11', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('144', '261', '1', '木刀', '1', '262', '8-1-1');
INSERT INTO `sys_equip` VALUES ('145', '262', '1', '训练刀', '2', '263', '8-7-2');
INSERT INTO `sys_equip` VALUES ('146', '263', '1', '短刀', '3', '264', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('147', '264', '1', '长刀', '4', '265', '8-13-3|8-19-3');
INSERT INTO `sys_equip` VALUES ('148', '265', '1', '武士刀', '5', '266', '8-24-2|8-19-4');
INSERT INTO `sys_equip` VALUES ('149', '266', '1', '大太刀', '6', '267', '8-24-2|8-30-2|8-19-4');
INSERT INTO `sys_equip` VALUES ('150', '267', '1', '天国刃', '7', '268', '8-30-3|8-36-3');
INSERT INTO `sys_equip` VALUES ('151', '268', '1', '肥前国中吉', '8', '269', '8-41-2|8-36-4');
INSERT INTO `sys_equip` VALUES ('152', '269', '1', '长船巨刃', '9', '270', '8-41-2|8-47-2|8-36-6');
INSERT INTO `sys_equip` VALUES ('153', '270', '1', '大般若正宗', '10', '271', '8-47-3|8-53-2|8-59-1');
INSERT INTO `sys_equip` VALUES ('154', '271', '1', '村正妖刀', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('155', '281', '1', '水珠', '1', '282', '8-1-1');
INSERT INTO `sys_equip` VALUES ('156', '282', '1', '紫玉珠', '2', '283', '8-7-2');
INSERT INTO `sys_equip` VALUES ('157', '283', '1', '暗影丸', '3', '284', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('158', '284', '1', '转经轮', '4', '285', '8-13-3|8-23-3');
INSERT INTO `sys_equip` VALUES ('159', '285', '1', '炎铜藏经轮', '5', '286', '8-24-2|8-23-4');
INSERT INTO `sys_equip` VALUES ('160', '286', '1', '舍利子经轮', '6', '287', '8-24-2|8-30-2|8-23-4');
INSERT INTO `sys_equip` VALUES ('161', '287', '1', '水鲛珠', '7', '288', '8-30-3|8-40-3');
INSERT INTO `sys_equip` VALUES ('162', '288', '1', '风螺旋丸', '8', '289', '8-41-2|8-40-4');
INSERT INTO `sys_equip` VALUES ('163', '289', '1', '幻月珠', '9', '290', '8-41-2|8-47-2|8-40-6');
INSERT INTO `sys_equip` VALUES ('164', '290', '1', '万花螺旋珠', '10', '291', '8-47-3|8-53-2|8-63-1');
INSERT INTO `sys_equip` VALUES ('165', '291', '1', '辉夜姬11', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('166', '301', '1', '双短刀', '1', '302', '8-1-1');
INSERT INTO `sys_equip` VALUES ('167', '302', '1', '蝙蝠双刃', '2', '303', '8-7-2');
INSERT INTO `sys_equip` VALUES ('168', '303', '1', '大马士革双刀', '3', '304', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('169', '304', '1', '残酷双钩', '4', '305', '8-13-3|8-20-3');
INSERT INTO `sys_equip` VALUES ('170', '305', '1', '蝎尾毒刃', '5', '306', '8-24-2|8-20-4');
INSERT INTO `sys_equip` VALUES ('171', '306', '1', '虎牙双刃', '6', '307', '8-24-2|8-30-2|8-20-4');
INSERT INTO `sys_equip` VALUES ('172', '307', '1', '武士双刀', '7', '308', '8-30-3|8-37-3');
INSERT INTO `sys_equip` VALUES ('173', '308', '1', '大小太刀', '8', '309', '8-41-2|8-37-4');
INSERT INTO `sys_equip` VALUES ('174', '309', '1', '秃鹫狂刃', '9', '310', '8-41-2|8-47-2|8-37-6');
INSERT INTO `sys_equip` VALUES ('175', '310', '1', '蝴蝶刃.影.流', '10', '311', '8-47-3|8-53-2|8-60-1');
INSERT INTO `sys_equip` VALUES ('176', '311', '1', '雪蓧双刀', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('177', '321', '1', '蓝色木弓', '1', '322', '8-1-1');
INSERT INTO `sys_equip` VALUES ('178', '322', '1', '迅捷弓', '2', '323', '8-7-2');
INSERT INTO `sys_equip` VALUES ('179', '323', '1', '强力弓', '3', '324', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('180', '324', '1', '风击弩', '4', '325', '8-13-3|8-21-3');
INSERT INTO `sys_equip` VALUES ('181', '325', '1', '寒冰弩', '5', '326', '8-24-2|8-21-4');
INSERT INTO `sys_equip` VALUES ('182', '326', '1', '钢铁重弩', '6', '327', '8-24-2|8-30-2|8-21-4');
INSERT INTO `sys_equip` VALUES ('183', '327', '1', '神力弓', '7', '328', '8-30-3|8-38-3');
INSERT INTO `sys_equip` VALUES ('184', '328', '1', '冰河弓', '8', '329', '8-41-2|8-38-4');
INSERT INTO `sys_equip` VALUES ('185', '329', '1', '寒风啸弓', '9', '330', '8-41-2|8-47-2|8-38-6');
INSERT INTO `sys_equip` VALUES ('186', '330', '1', '飞花吹雪之弓', '10', '331', '8-47-3|8-53-2|8-61-1');
INSERT INTO `sys_equip` VALUES ('187', '331', '1', '浓姬11', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('188', '341', '1', '亚瑟王1', '1', '342', '8-1-1');
INSERT INTO `sys_equip` VALUES ('189', '342', '1', '亚瑟王2', '2', '343', '8-7-2');
INSERT INTO `sys_equip` VALUES ('190', '343', '1', '亚瑟王3', '3', '344', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('191', '344', '1', '亚瑟王4', '4', '345', '8-13-3|8-19-3');
INSERT INTO `sys_equip` VALUES ('192', '345', '1', '亚瑟王5', '5', '346', '8-24-2|8-19-4');
INSERT INTO `sys_equip` VALUES ('193', '346', '1', '亚瑟王6', '6', '347', '8-24-2|8-30-2|8-19-4');
INSERT INTO `sys_equip` VALUES ('194', '347', '1', '亚瑟王7', '7', '348', '8-30-3|8-36-3');
INSERT INTO `sys_equip` VALUES ('195', '348', '1', '亚瑟王8', '8', '349', '8-41-2|8-36-4');
INSERT INTO `sys_equip` VALUES ('196', '349', '1', '亚瑟王9', '9', '350', '8-41-2|8-47-2|8-36-6');
INSERT INTO `sys_equip` VALUES ('197', '350', '1', '亚瑟王10', '10', '351', '8-47-3|8-53-2|8-59-1');
INSERT INTO `sys_equip` VALUES ('198', '351', '1', '亚瑟王11', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('199', '361', '1', '拿破仑1', '1', '362', '8-1-1');
INSERT INTO `sys_equip` VALUES ('200', '362', '1', '拿破仑2', '2', '363', '8-7-2');
INSERT INTO `sys_equip` VALUES ('201', '363', '1', '拿破仑3', '3', '364', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('202', '364', '1', '拿破仑4', '4', '365', '8-13-3|8-21-3');
INSERT INTO `sys_equip` VALUES ('203', '365', '1', '拿破仑5', '5', '366', '8-24-2|8-21-4');
INSERT INTO `sys_equip` VALUES ('204', '366', '1', '拿破仑6', '6', '367', '8-24-2|8-30-2|8-21-4');
INSERT INTO `sys_equip` VALUES ('205', '367', '1', '拿破仑7', '7', '368', '8-30-3|8-38-3');
INSERT INTO `sys_equip` VALUES ('206', '368', '1', '拿破仑8', '8', '369', '8-41-2|8-38-4');
INSERT INTO `sys_equip` VALUES ('207', '369', '1', '拿破仑9', '9', '370', '8-41-2|8-47-2|8-38-6');
INSERT INTO `sys_equip` VALUES ('208', '370', '1', '拿破仑10', '10', '371', '8-47-3|8-53-2|8-61-1');
INSERT INTO `sys_equip` VALUES ('209', '371', '1', '拿破仑11', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('210', '381', '1', '一休1', '1', '382', '8-1-1');
INSERT INTO `sys_equip` VALUES ('211', '382', '1', '一休2', '2', '383', '8-7-2');
INSERT INTO `sys_equip` VALUES ('212', '383', '1', '一休3', '3', '384', '8-7-3|8-13-3');
INSERT INTO `sys_equip` VALUES ('213', '384', '1', '一休4', '4', '385', '8-13-3|8-22-3');
INSERT INTO `sys_equip` VALUES ('214', '385', '1', '一休5', '5', '386', '8-24-2|8-22-4');
INSERT INTO `sys_equip` VALUES ('215', '386', '1', '一休6', '6', '387', '8-24-2|8-30-2|8-22-4');
INSERT INTO `sys_equip` VALUES ('216', '387', '1', '一休7', '7', '388', '8-30-3|8-39-3');
INSERT INTO `sys_equip` VALUES ('217', '388', '1', '一休8', '8', '389', '8-41-2|8-39-4');
INSERT INTO `sys_equip` VALUES ('218', '389', '1', '一休9', '9', '390', '8-41-2|8-47-2|8-39-6');
INSERT INTO `sys_equip` VALUES ('219', '390', '1', '一休10', '10', '391', '8-47-3|8-53-2|8-62-1');
INSERT INTO `sys_equip` VALUES ('220', '391', '1', '一休11', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('221', '2001', '2', '铜质项链', '1', '2002', '8-2-1');
INSERT INTO `sys_equip` VALUES ('222', '2002', '2', '兽牙项圈', '2', '2003', '8-8-2');
INSERT INTO `sys_equip` VALUES ('223', '2003', '2', '虎牙项圈', '3', '2004', '8-8-3|8-14-3');
INSERT INTO `sys_equip` VALUES ('224', '2004', '2', '朴素的珍珠项链', '4', '2005', '8-14-3|8-19-2');
INSERT INTO `sys_equip` VALUES ('225', '2005', '2', '彩色珍珠项链', '5', '2006', '8-25-2|8-19-3');
INSERT INTO `sys_equip` VALUES ('226', '2006', '2', '闪耀金珍珠项链', '6', '2007', '8-25-2|8-31-2|8-19-3');
INSERT INTO `sys_equip` VALUES ('227', '2007', '2', '赤炎勇气项链', '7', '2008', '8-31-3|8-36-2');
INSERT INTO `sys_equip` VALUES ('228', '2008', '2', '狂热野性项链', '8', '2009', '8-42-2|8-36-3');
INSERT INTO `sys_equip` VALUES ('229', '2009', '2', '血魄蛮力项链', '9', '2010', '8-42-2|8-48-2|8-36-4');
INSERT INTO `sys_equip` VALUES ('230', '2010', '2', '护卫者项链', '10', '2011', '8-48-2|8-54-2|8-59-1');
INSERT INTO `sys_equip` VALUES ('231', '2011', '2', '守护光芒项链', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('232', '2021', '2', '铜质铭牌项链', '1', '2022', '8-2-1');
INSERT INTO `sys_equip` VALUES ('233', '2022', '2', '兽骨项圈', '2', '2023', '8-8-2');
INSERT INTO `sys_equip` VALUES ('234', '2023', '2', '犬牙项圈', '3', '2024', '8-8-3|8-14-3');
INSERT INTO `sys_equip` VALUES ('235', '2024', '2', '珍珠坠饰', '4', '2025', '8-14-3|8-20-2');
INSERT INTO `sys_equip` VALUES ('236', '2025', '2', '彩色珍珠坠饰', '5', '2026', '8-25-2|8-20-3');
INSERT INTO `sys_equip` VALUES ('237', '2026', '2', '金色珍珠吊坠', '6', '2027', '8-25-2|8-31-2|8-20-3');
INSERT INTO `sys_equip` VALUES ('238', '2027', '2', '烈炎项链', '7', '2028', '8-31-3|8-37-2');
INSERT INTO `sys_equip` VALUES ('239', '2028', '2', '狂野吊坠', '8', '2029', '8-42-2|8-37-3');
INSERT INTO `sys_equip` VALUES ('240', '2029', '2', '魔能项圈', '9', '2030', '8-42-2|8-48-2|8-37-4');
INSERT INTO `sys_equip` VALUES ('241', '2030', '2', '雷光坠饰', '10', '2031', '8-48-2|8-54-2|8-60-1');
INSERT INTO `sys_equip` VALUES ('242', '2031', '2', '涅槃项链', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('243', '2041', '2', '简陋的项链', '1', '2042', '8-2-1');
INSERT INTO `sys_equip` VALUES ('244', '2042', '2', '兽爪项圈', '2', '2043', '8-8-2');
INSERT INTO `sys_equip` VALUES ('245', '2043', '2', '虎爪项圈', '3', '2044', '8-8-3|8-14-3');
INSERT INTO `sys_equip` VALUES ('246', '2044', '2', '锐利尖石吊坠', '4', '2045', '8-14-3|8-21-2');
INSERT INTO `sys_equip` VALUES ('247', '2045', '2', '闪光宝石吊坠', '5', '2046', '8-25-2|8-21-3');
INSERT INTO `sys_equip` VALUES ('248', '2046', '2', '坚硬钻石吊坠', '6', '2047', '8-25-2|8-31-2|8-21-3');
INSERT INTO `sys_equip` VALUES ('249', '2047', '2', '灵巧项链', '7', '2048', '8-31-3|8-38-2');
INSERT INTO `sys_equip` VALUES ('250', '2048', '2', '追捕项链', '8', '2049', '8-42-2|8-38-3');
INSERT INTO `sys_equip` VALUES ('251', '2049', '2', '狂野追踪项链', '9', '2050', '8-42-2|8-48-2|8-38-4');
INSERT INTO `sys_equip` VALUES ('252', '2050', '2', '风行者项链', '10', '2051', '8-48-2|8-54-2|8-61-1');
INSERT INTO `sys_equip` VALUES ('253', '2051', '2', '风神之拥项链', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('254', '2061', '2', '铭文铜项链', '1', '2062', '8-2-1');
INSERT INTO `sys_equip` VALUES ('255', '2062', '2', '秘银项圈', '2', '2063', '8-8-2');
INSERT INTO `sys_equip` VALUES ('256', '2063', '2', '密银符文项圈', '3', '2064', '8-8-3|8-14-3');
INSERT INTO `sys_equip` VALUES ('257', '2064', '2', '神秘的珍珠项链', '4', '2065', '8-14-3|8-22-2');
INSERT INTO `sys_equip` VALUES ('258', '2065', '2', '多彩珍珠项链', '5', '2066', '8-25-2|8-22-3');
INSERT INTO `sys_equip` VALUES ('259', '2066', '2', '秘法珍珠项链', '6', '2067', '8-25-2|8-31-2|8-22-3');
INSERT INTO `sys_equip` VALUES ('260', '2067', '2', '部族诅咒项链', '7', '2068', '8-31-3|8-39-2');
INSERT INTO `sys_equip` VALUES ('261', '2068', '2', '远古奥秘项链', '8', '2069', '8-42-2|8-39-3');
INSERT INTO `sys_equip` VALUES ('262', '2069', '2', '死亡低语项链', '9', '2070', '8-42-2|8-48-2|8-39-4');
INSERT INTO `sys_equip` VALUES ('263', '2070', '2', '噩梦项链', '10', '2071', '8-48-2|8-54-2|8-62-1');
INSERT INTO `sys_equip` VALUES ('264', '2071', '2', '奥秘闪光项链', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('265', '2081', '2', '符文铜项链', '1', '2082', '8-2-1');
INSERT INTO `sys_equip` VALUES ('266', '2082', '2', '春雨水晶坠饰', '2', '2083', '8-8-2');
INSERT INTO `sys_equip` VALUES ('267', '2083', '2', '荣光坠饰', '3', '2084', '8-8-3|8-14-3');
INSERT INTO `sys_equip` VALUES ('268', '2084', '2', '光洁珍珠吊坠', '4', '2085', '8-14-3|8-23-2');
INSERT INTO `sys_equip` VALUES ('269', '2085', '2', '上古贝壳坠饰', '5', '2086', '8-25-2|8-23-3');
INSERT INTO `sys_equip` VALUES ('270', '2086', '2', '苦修者吊坠', '6', '2087', '8-25-2|8-31-2|8-23-3');
INSERT INTO `sys_equip` VALUES ('271', '2087', '2', '圣洁治愈坠饰', '7', '2088', '8-31-3|8-40-2');
INSERT INTO `sys_equip` VALUES ('272', '2088', '2', '无暇护佑吊坠', '8', '2089', '8-42-2|8-40-3');
INSERT INTO `sys_equip` VALUES ('273', '2089', '2', '光耀吊坠', '9', '2090', '8-42-2|8-48-2|8-40-4');
INSERT INTO `sys_equip` VALUES ('274', '2090', '2', '高尚者吊坠', '10', '2091', '8-48-2|8-54-2|8-63-1');
INSERT INTO `sys_equip` VALUES ('275', '2091', '2', '圣光守护坠饰', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('276', '3001', '3', '坚硬铁战甲', '1', '3002', '8-3-1');
INSERT INTO `sys_equip` VALUES ('277', '3002', '3', '士兵战甲', '2', '3003', '8-9-2');
INSERT INTO `sys_equip` VALUES ('278', '3003', '3', '尊贵护甲', '3', '3004', '8-9-2|8-15-2');
INSERT INTO `sys_equip` VALUES ('279', '3004', '3', '堡垒卫士铠甲', '4', '3005', '8-15-2|8-19-2');
INSERT INTO `sys_equip` VALUES ('280', '3005', '3', '战争使者护甲', '5', '3006', '8-26-2|8-19-2');
INSERT INTO `sys_equip` VALUES ('281', '3006', '3', '远古君王战甲', '6', '3007', '8-26-2|8-32-2|8-19-2');
INSERT INTO `sys_equip` VALUES ('282', '3007', '3', '狂怒英勇战衣', '7', '3008', '8-32-2|8-36-2');
INSERT INTO `sys_equip` VALUES ('283', '3008', '3', '无畏征服者战甲', '8', '3009', '8-43-2|8-36-2');
INSERT INTO `sys_equip` VALUES ('284', '3009', '3', '邪血霸主铠甲', '9', '3010', '8-43-2|8-49-2|8-36-3');
INSERT INTO `sys_equip` VALUES ('285', '3010', '3', '雷霆堡垒战甲', '10', '3011', '8-49-2|8-55-1|8-59-1');
INSERT INTO `sys_equip` VALUES ('286', '3011', '3', '正义仲裁铠甲', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('287', '3021', '3', '染尘的锁护甲', '1', '3022', '8-3-1');
INSERT INTO `sys_equip` VALUES ('288', '3022', '3', '游侠新手护甲', '2', '3023', '8-9-2');
INSERT INTO `sys_equip` VALUES ('289', '3023', '3', '探路者护甲', '3', '3024', '8-9-2|8-15-2');
INSERT INTO `sys_equip` VALUES ('290', '3024', '3', '银鳞链甲', '4', '3025', '8-15-2|8-20-2');
INSERT INTO `sys_equip` VALUES ('291', '3025', '3', '镀金银链甲', '5', '3026', '8-26-2|8-20-2');
INSERT INTO `sys_equip` VALUES ('292', '3026', '3', '邪恶龙鳞链甲', '6', '3027', '8-26-2|8-32-2|8-20-2');
INSERT INTO `sys_equip` VALUES ('293', '3027', '3', '不懈防御护甲', '7', '3028', '8-32-2|8-37-2');
INSERT INTO `sys_equip` VALUES ('294', '3028', '3', '疾风追猎者链甲', '8', '3029', '8-43-2|8-37-2');
INSERT INTO `sys_equip` VALUES ('295', '3029', '3', '尖啸者护甲', '9', '3030', '8-43-2|8-49-2|8-37-3');
INSERT INTO `sys_equip` VALUES ('296', '3030', '3', '无情暴君护甲', '10', '3031', '8-49-2|8-55-1|8-60-1');
INSERT INTO `sys_equip` VALUES ('297', '3031', '3', '浴火凤凰护甲', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('298', '3041', '3', '硬皮外套', '1', '3042', '8-4-1');
INSERT INTO `sys_equip` VALUES ('299', '3042', '3', '猎手皮衣', '2', '3043', '8-10-2');
INSERT INTO `sys_equip` VALUES ('300', '3043', '3', '游牧者外套', '3', '3044', '8-10-2|8-16-2');
INSERT INTO `sys_equip` VALUES ('301', '3044', '3', '染血的皮外衣', '4', '3045', '8-16-2|8-21-2');
INSERT INTO `sys_equip` VALUES ('302', '3045', '3', '畸形蟒皮外套', '5', '3046', '8-27-2|8-21-2');
INSERT INTO `sys_equip` VALUES ('303', '3046', '3', '狂躁野兽外衣', '6', '3047', '8-27-2|8-33-2|8-21-2');
INSERT INTO `sys_equip` VALUES ('304', '3047', '3', '狩猎者皮衣', '7', '3048', '8-33-2|8-38-2');
INSERT INTO `sys_equip` VALUES ('305', '3048', '3', '咆哮兽王外套', '8', '3049', '8-44-2|8-38-2');
INSERT INTO `sys_equip` VALUES ('306', '3049', '3', '追猎者皮衣', '9', '3050', '8-44-2|8-50-2|8-38-3');
INSERT INTO `sys_equip` VALUES ('307', '3050', '3', '疾风巨龙外套', '10', '3051', '8-50-2|8-56-1|8-61-1');
INSERT INTO `sys_equip` VALUES ('308', '3051', '3', '胜利斗士战衣', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('309', '3061', '3', '手织长袍', '1', '3062', '8-5-1');
INSERT INTO `sys_equip` VALUES ('310', '3062', '3', '冰雾法袍', '2', '3063', '8-11-2');
INSERT INTO `sys_equip` VALUES ('311', '3063', '3', '缚魂者长袍', '3', '3064', '8-11-2|8-17-2');
INSERT INTO `sys_equip` VALUES ('312', '3064', '3', '学者外套', '4', '3065', '8-17-2|8-22-2');
INSERT INTO `sys_equip` VALUES ('313', '3065', '3', '虚空法袍', '5', '3066', '8-28-2|8-22-2');
INSERT INTO `sys_equip` VALUES ('314', '3066', '3', '无尽黑夜法袍', '6', '3067', '8-28-2|8-34-2|8-22-2');
INSERT INTO `sys_equip` VALUES ('315', '3067', '3', '精神冲击长袍', '7', '3068', '8-34-2|8-39-2');
INSERT INTO `sys_equip` VALUES ('316', '3068', '3', '巨龙束缚长袍', '8', '3069', '8-45-2|8-39-2');
INSERT INTO `sys_equip` VALUES ('317', '3069', '3', '背叛者法袍', '9', '3070', '8-45-2|8-51-2|8-39-3');
INSERT INTO `sys_equip` VALUES ('318', '3070', '3', '时空梦魇法袍', '10', '3071', '8-51-2|8-57-1|8-62-1');
INSERT INTO `sys_equip` VALUES ('319', '3071', '3', '邪神毁灭者法袍', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('320', '3081', '3', '手工长袍', '1', '3082', '8-5-1');
INSERT INTO `sys_equip` VALUES ('321', '3082', '3', '贤者长袍', '2', '3083', '8-11-2');
INSERT INTO `sys_equip` VALUES ('322', '3083', '3', '救赎者长袍', '3', '3084', '8-11-2|8-17-2');
INSERT INTO `sys_equip` VALUES ('323', '3084', '3', '预言者长袍', '4', '3085', '8-17-2|8-23-2');
INSERT INTO `sys_equip` VALUES ('324', '3085', '3', '虔诚法袍', '5', '3086', '8-28-2|8-23-2');
INSERT INTO `sys_equip` VALUES ('325', '3086', '3', '振奋长袍', '6', '3087', '8-28-2|8-34-2|8-23-2');
INSERT INTO `sys_equip` VALUES ('326', '3087', '3', '忠诚信仰长袍', '7', '3088', '8-34-2|8-40-2');
INSERT INTO `sys_equip` VALUES ('327', '3088', '3', '赦免审判长袍', '8', '3089', '8-45-2|8-40-2');
INSERT INTO `sys_equip` VALUES ('328', '3089', '3', '忏悔者法袍', '9', '3090', '8-45-2|8-51-2|8-40-3');
INSERT INTO `sys_equip` VALUES ('329', '3090', '3', '无尽智慧法袍', '10', '3091', '8-51-2|8-57-1|8-63-1');
INSERT INTO `sys_equip` VALUES ('330', '3091', '3', '神圣使徒长袍', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('331', '4001', '4', '牢固的铁腰带', '1', '4002', '8-3-1');
INSERT INTO `sys_equip` VALUES ('332', '4002', '4', '战士腰带', '2', '4003', '8-9-2');
INSERT INTO `sys_equip` VALUES ('333', '4003', '4', '贵重的腰带', '3', '4004', '8-9-2|8-15-2');
INSERT INTO `sys_equip` VALUES ('334', '4004', '4', '坚强卫士腰带', '4', '4005', '8-15-2|8-19-2');
INSERT INTO `sys_equip` VALUES ('335', '4005', '4', '胜利者腰带', '5', '4006', '8-26-2|8-19-2');
INSERT INTO `sys_equip` VALUES ('336', '4006', '4', '远古王者腰带', '6', '4007', '8-26-2|8-32-2|8-19-2');
INSERT INTO `sys_equip` VALUES ('337', '4007', '4', '英勇腰带', '7', '4008', '8-32-2|8-36-2');
INSERT INTO `sys_equip` VALUES ('338', '4008', '4', '征服者腰带', '8', '4009', '8-43-2|8-36-2');
INSERT INTO `sys_equip` VALUES ('339', '4009', '4', '邪血腰带', '9', '4010', '8-43-2|8-49-2|8-36-3');
INSERT INTO `sys_equip` VALUES ('340', '4010', '4', '壁垒腰带', '10', '4011', '8-49-2|8-55-1|8-59-1');
INSERT INTO `sys_equip` VALUES ('341', '4011', '4', '正义仲裁腰带', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('342', '4021', '4', '破旧的链甲护腰', '1', '4022', '8-3-1');
INSERT INTO `sys_equip` VALUES ('343', '4022', '4', '游侠新兵腰带', '2', '4023', '8-9-2');
INSERT INTO `sys_equip` VALUES ('344', '4023', '4', '探索者腰带', '3', '4024', '8-9-2|8-15-2');
INSERT INTO `sys_equip` VALUES ('345', '4024', '4', '银色铜腰带', '4', '4025', '8-15-2|8-20-2');
INSERT INTO `sys_equip` VALUES ('346', '4025', '4', '镀金链甲腰带', '5', '4026', '8-26-2|8-20-2');
INSERT INTO `sys_equip` VALUES ('347', '4026', '4', '邪恶龙鳞腰带', '6', '4027', '8-26-2|8-32-2|8-20-2');
INSERT INTO `sys_equip` VALUES ('348', '4027', '4', '防御者腰带', '7', '4028', '8-32-2|8-37-2');
INSERT INTO `sys_equip` VALUES ('349', '4028', '4', '风之猎手腰带', '8', '4029', '8-43-2|8-37-2');
INSERT INTO `sys_equip` VALUES ('350', '4029', '4', '嚎叫者腰带', '9', '4030', '8-43-2|8-49-2|8-37-3');
INSERT INTO `sys_equip` VALUES ('351', '4030', '4', '无情打击腰带', '10', '4031', '8-49-2|8-55-1|8-60-1');
INSERT INTO `sys_equip` VALUES ('352', '4031', '4', '浴火重生腰带', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('353', '4041', '4', '硬皮腰带', '1', '4042', '8-4-1');
INSERT INTO `sys_equip` VALUES ('354', '4042', '4', '猎人皮带', '2', '4043', '8-10-2');
INSERT INTO `sys_equip` VALUES ('355', '4043', '4', '游牧者腰带', '3', '4044', '8-10-2|8-16-2');
INSERT INTO `sys_equip` VALUES ('356', '4044', '4', '染血的皮腰带', '4', '4045', '8-16-2|8-21-2');
INSERT INTO `sys_equip` VALUES ('357', '4045', '4', '畸形蟒皮腰带', '5', '4046', '8-27-2|8-21-2');
INSERT INTO `sys_equip` VALUES ('358', '4046', '4', '暴躁野兽皮带', '6', '4047', '8-27-2|8-33-2|8-21-2');
INSERT INTO `sys_equip` VALUES ('359', '4047', '4', '狩猎者腰带', '7', '4048', '8-33-2|8-38-2');
INSERT INTO `sys_equip` VALUES ('360', '4048', '4', '咆哮兽王腰带', '8', '4049', '8-44-2|8-38-2');
INSERT INTO `sys_equip` VALUES ('361', '4049', '4', '追猎者腰带', '9', '4050', '8-44-2|8-50-2|8-38-3');
INSERT INTO `sys_equip` VALUES ('362', '4050', '4', '疾风巨龙束带', '10', '4051', '8-50-2|8-56-1|8-61-1');
INSERT INTO `sys_equip` VALUES ('363', '4051', '4', '胜利斗士腰带', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('364', '4061', '4', '丝质腰带', '1', '4062', '8-5-1');
INSERT INTO `sys_equip` VALUES ('365', '4062', '4', '冰雾腰带', '2', '4063', '8-11-2');
INSERT INTO `sys_equip` VALUES ('366', '4063', '4', '缚魂者腰带', '3', '4064', '8-11-2|8-17-2');
INSERT INTO `sys_equip` VALUES ('367', '4064', '4', '学者的知识束带', '4', '4065', '8-17-2|8-22-2');
INSERT INTO `sys_equip` VALUES ('368', '4065', '4', '虚空腰带', '5', '4066', '8-28-2|8-22-2');
INSERT INTO `sys_equip` VALUES ('369', '4066', '4', '无尽黑夜腰带', '6', '4067', '8-28-2|8-34-2|8-22-2');
INSERT INTO `sys_equip` VALUES ('370', '4067', '4', '精神打击腰带', '7', '4068', '8-34-2|8-39-2');
INSERT INTO `sys_equip` VALUES ('371', '4068', '4', '巨龙束缚腰带', '8', '4069', '8-45-2|8-39-2');
INSERT INTO `sys_equip` VALUES ('372', '4069', '4', '背叛者腰带', '9', '4070', '8-45-2|8-51-2|8-39-3');
INSERT INTO `sys_equip` VALUES ('373', '4070', '4', '时空梦魇腰带', '10', '4071', '8-51-2|8-57-1|8-62-1');
INSERT INTO `sys_equip` VALUES ('374', '4071', '4', '邪神封印者腰带', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('375', '4081', '4', '丝绸腰带', '1', '4082', '8-5-1');
INSERT INTO `sys_equip` VALUES ('376', '4082', '4', '贤者腰带', '2', '4083', '8-11-2');
INSERT INTO `sys_equip` VALUES ('377', '4083', '4', '救赎者腰带', '3', '4084', '8-11-2|8-17-2');
INSERT INTO `sys_equip` VALUES ('378', '4084', '4', '预言束带', '4', '4085', '8-17-2|8-23-2');
INSERT INTO `sys_equip` VALUES ('379', '4085', '4', '虔诚腰带', '5', '4086', '8-28-2|8-23-2');
INSERT INTO `sys_equip` VALUES ('380', '4086', '4', '振奋腰带', '6', '4087', '8-28-2|8-34-2|8-23-2');
INSERT INTO `sys_equip` VALUES ('381', '4087', '4', '忠诚信仰腰带', '7', '4088', '8-34-2|8-40-2');
INSERT INTO `sys_equip` VALUES ('382', '4088', '4', '赦免审判腰带', '8', '4089', '8-45-2|8-40-2');
INSERT INTO `sys_equip` VALUES ('383', '4089', '4', '忏悔者束带', '9', '4090', '8-45-2|8-51-2|8-40-3');
INSERT INTO `sys_equip` VALUES ('384', '4090', '4', '无尽智慧束带', '10', '4091', '8-51-2|8-57-1|8-63-1');
INSERT INTO `sys_equip` VALUES ('385', '4091', '4', '神圣使徒腰带', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('386', '5001', '5', '简陋的铜戒指', '1', '5002', '8-6-1');
INSERT INTO `sys_equip` VALUES ('387', '5002', '5', '兽爪戒指', '2', '5003', '8-12-2');
INSERT INTO `sys_equip` VALUES ('388', '5003', '5', '虎趾戒指', '3', '5004', '8-12-3|8-18-3');
INSERT INTO `sys_equip` VALUES ('389', '5004', '5', '绿松石戒指', '4', '5005', '8-18-3|8-19-2');
INSERT INTO `sys_equip` VALUES ('390', '5005', '5', '坚硬宝石戒指', '5', '5006', '8-29-2|8-19-3');
INSERT INTO `sys_equip` VALUES ('391', '5006', '5', '璀璨钻石戒指', '6', '5007', '8-29-2|8-35-2|8-19-3');
INSERT INTO `sys_equip` VALUES ('392', '5007', '5', '蛮力戒指', '7', '5008', '8-35-3|8-36-2');
INSERT INTO `sys_equip` VALUES ('393', '5008', '5', '狂热指环', '8', '5009', '8-46-2|8-36-3');
INSERT INTO `sys_equip` VALUES ('394', '5009', '5', '血誓戒指', '9', '5010', '8-46-2|8-52-2|8-36-4');
INSERT INTO `sys_equip` VALUES ('395', '5010', '5', '护卫者戒指', '10', '5011', '8-52-2|8-58-2|8-59-1');
INSERT INTO `sys_equip` VALUES ('396', '5011', '5', '守护光芒指环', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('397', '5021', '5', '铜指环', '1', '5022', '8-6-1');
INSERT INTO `sys_equip` VALUES ('398', '5022', '5', '兽骨戒指', '2', '5023', '8-12-2');
INSERT INTO `sys_equip` VALUES ('399', '5023', '5', '犬牙指环', '3', '5024', '8-12-3|8-18-3');
INSERT INTO `sys_equip` VALUES ('400', '5024', '5', '蓝宝石戒指', '4', '5025', '8-18-3|8-20-2');
INSERT INTO `sys_equip` VALUES ('401', '5025', '5', '坚锐宝石戒指', '5', '5026', '8-29-2|8-20-3');
INSERT INTO `sys_equip` VALUES ('402', '5026', '5', '耀眼钻石戒指', '6', '5027', '8-29-2|8-35-2|8-20-3');
INSERT INTO `sys_equip` VALUES ('403', '5027', '5', '烈炎戒指', '7', '5028', '8-35-3|8-37-2');
INSERT INTO `sys_equip` VALUES ('404', '5028', '5', '狂野指环', '8', '5029', '8-46-2|8-37-3');
INSERT INTO `sys_equip` VALUES ('405', '5029', '5', '魔能戒指', '9', '5030', '8-46-2|8-52-2|8-37-4');
INSERT INTO `sys_equip` VALUES ('406', '5030', '5', '引雷戒指', '10', '5031', '8-52-2|8-58-2|8-60-1');
INSERT INTO `sys_equip` VALUES ('407', '5031', '5', '神之指环', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('408', '5041', '5', '破损的戒指', '1', '5042', '8-6-1');
INSERT INTO `sys_equip` VALUES ('409', '5042', '5', '利爪戒指', '2', '5043', '8-12-2');
INSERT INTO `sys_equip` VALUES ('410', '5043', '5', '猎豹指环', '3', '5044', '8-12-3|8-18-3');
INSERT INTO `sys_equip` VALUES ('411', '5044', '5', '尖石戒指', '4', '5045', '8-18-3|8-21-2');
INSERT INTO `sys_equip` VALUES ('412', '5045', '5', '闪光宝石戒指', '5', '5046', '8-29-2|8-21-3');
INSERT INTO `sys_equip` VALUES ('413', '5046', '5', '坚硬钻石戒指', '6', '5047', '8-29-2|8-35-2|8-21-3');
INSERT INTO `sys_equip` VALUES ('414', '5047', '5', '灵巧戒指', '7', '5048', '8-35-3|8-38-2');
INSERT INTO `sys_equip` VALUES ('415', '5048', '5', '追捕指环', '8', '5049', '8-46-2|8-38-3');
INSERT INTO `sys_equip` VALUES ('416', '5049', '5', '狂野追踪戒指', '9', '5050', '8-46-2|8-52-2|8-38-4');
INSERT INTO `sys_equip` VALUES ('417', '5050', '5', '风行者之戒', '10', '5051', '8-52-2|8-58-2|8-61-1');
INSERT INTO `sys_equip` VALUES ('418', '5051', '5', '风神之拥指环', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('419', '5061', '5', '铭文戒指', '1', '5062', '8-6-1');
INSERT INTO `sys_equip` VALUES ('420', '5062', '5', '恶念指环', '2', '5063', '8-12-2');
INSERT INTO `sys_equip` VALUES ('421', '5063', '5', '邪恶凝视指环', '3', '5064', '8-12-3|8-18-3');
INSERT INTO `sys_equip` VALUES ('422', '5064', '5', '红宝石指环', '4', '5065', '8-18-3|8-22-2');
INSERT INTO `sys_equip` VALUES ('423', '5065', '5', '铭刻宝石指环', '5', '5066', '8-29-2|8-22-3');
INSERT INTO `sys_equip` VALUES ('424', '5066', '5', '闪耀钻石指环', '6', '5067', '8-29-2|8-35-2|8-22-3');
INSERT INTO `sys_equip` VALUES ('425', '5067', '5', '禁言指环', '7', '5068', '8-35-3|8-39-2');
INSERT INTO `sys_equip` VALUES ('426', '5068', '5', '疯狂奥秘指环', '8', '5069', '8-46-2|8-39-3');
INSERT INTO `sys_equip` VALUES ('427', '5069', '5', '亡者嚎哭戒指', '9', '5070', '8-46-2|8-52-2|8-39-4');
INSERT INTO `sys_equip` VALUES ('428', '5070', '5', '梦魇指环', '10', '5071', '8-52-2|8-58-2|8-62-1');
INSERT INTO `sys_equip` VALUES ('429', '5071', '5', '神之怒火指环', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('430', '5081', '5', '符印指环', '1', '5082', '8-6-1');
INSERT INTO `sys_equip` VALUES ('431', '5082', '5', '春雨戒指', '2', '5083', '8-12-2');
INSERT INTO `sys_equip` VALUES ('432', '5083', '5', '荣光指环', '3', '5084', '8-12-3|8-18-3');
INSERT INTO `sys_equip` VALUES ('433', '5084', '5', '水晶指环', '4', '5085', '8-18-3|8-23-2');
INSERT INTO `sys_equip` VALUES ('434', '5085', '5', '闪光晶石戒指', '5', '5086', '8-29-2|8-23-3');
INSERT INTO `sys_equip` VALUES ('435', '5086', '5', '苦修者指环', '6', '5087', '8-29-2|8-35-2|8-23-3');
INSERT INTO `sys_equip` VALUES ('436', '5087', '5', '圣洁治愈指环', '7', '5088', '8-35-3|8-40-2');
INSERT INTO `sys_equip` VALUES ('437', '5088', '5', '无暇护佑指环', '8', '5089', '8-46-2|8-40-3');
INSERT INTO `sys_equip` VALUES ('438', '5089', '5', '光耀戒指', '9', '5090', '8-46-2|8-52-2|8-40-4');
INSERT INTO `sys_equip` VALUES ('439', '5090', '5', '高尚者指环', '10', '5091', '8-52-2|8-58-2|8-63-1');
INSERT INTO `sys_equip` VALUES ('440', '5091', '5', '圣光守护指环', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('441', '6001', '6', '耐磨的铁战靴', '1', '6002', '8-3-1');
INSERT INTO `sys_equip` VALUES ('442', '6002', '6', '士兵战靴', '2', '6003', '8-9-2');
INSERT INTO `sys_equip` VALUES ('443', '6003', '6', '尊贵铁靴', '3', '6004', '8-9-2|8-15-2');
INSERT INTO `sys_equip` VALUES ('444', '6004', '6', '堡垒卫士战靴', '4', '6005', '8-15-2|8-19-2');
INSERT INTO `sys_equip` VALUES ('445', '6005', '6', '战争使者战靴', '5', '6006', '8-26-2|8-19-2');
INSERT INTO `sys_equip` VALUES ('446', '6006', '6', '远古君王战靴', '6', '6007', '8-26-2|8-32-2|8-19-2');
INSERT INTO `sys_equip` VALUES ('447', '6007', '6', '狂怒战靴', '7', '6008', '8-32-2|8-36-2');
INSERT INTO `sys_equip` VALUES ('448', '6008', '6', '无畏者战靴', '8', '6009', '8-43-2|8-36-2');
INSERT INTO `sys_equip` VALUES ('449', '6009', '6', '血领主战靴', '9', '6010', '8-43-2|8-49-2|8-36-3');
INSERT INTO `sys_equip` VALUES ('450', '6010', '6', '雷霆战靴', '10', '6011', '8-49-2|8-55-1|8-59-1');
INSERT INTO `sys_equip` VALUES ('451', '6011', '6', '正义仲裁战靴', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('452', '6021', '6', '链甲战靴', '1', '6022', '8-3-1');
INSERT INTO `sys_equip` VALUES ('453', '6022', '6', '游侠新兵靴子', '2', '6023', '8-9-2');
INSERT INTO `sys_equip` VALUES ('454', '6023', '6', '探路者战靴', '3', '6024', '8-9-2|8-15-2');
INSERT INTO `sys_equip` VALUES ('455', '6024', '6', '银白战靴', '4', '6025', '8-15-2|8-20-2');
INSERT INTO `sys_equip` VALUES ('456', '6025', '6', '镀金链甲战靴', '5', '6026', '8-26-2|8-20-2');
INSERT INTO `sys_equip` VALUES ('457', '6026', '6', '邪恶龙鳞战靴', '6', '6027', '8-26-2|8-32-2|8-20-2');
INSERT INTO `sys_equip` VALUES ('458', '6027', '6', '防御者战靴', '7', '6028', '8-32-2|8-37-2');
INSERT INTO `sys_equip` VALUES ('459', '6028', '6', '疾风战靴', '8', '6029', '8-43-2|8-37-2');
INSERT INTO `sys_equip` VALUES ('460', '6029', '6', '哀嚎战靴', '9', '6030', '8-43-2|8-49-2|8-37-3');
INSERT INTO `sys_equip` VALUES ('461', '6030', '6', '暴君战靴', '10', '6031', '8-49-2|8-55-1|8-60-1');
INSERT INTO `sys_equip` VALUES ('462', '6031', '6', '浴火打击战靴', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('463', '6041', '6', '硬皮靴', '1', '6042', '8-4-1');
INSERT INTO `sys_equip` VALUES ('464', '6042', '6', '猎手软靴', '2', '6043', '8-10-2');
INSERT INTO `sys_equip` VALUES ('465', '6043', '6', '游牧者皮靴', '3', '6044', '8-10-2|8-16-2');
INSERT INTO `sys_equip` VALUES ('466', '6044', '6', '染血的皮靴', '4', '6045', '8-16-2|8-21-2');
INSERT INTO `sys_equip` VALUES ('467', '6045', '6', '畸形蟒皮软靴', '5', '6046', '8-27-2|8-21-2');
INSERT INTO `sys_equip` VALUES ('468', '6046', '6', '暴躁野兽皮靴', '6', '6047', '8-27-2|8-33-2|8-21-2');
INSERT INTO `sys_equip` VALUES ('469', '6047', '6', '狩猎者皮靴', '7', '6048', '8-33-2|8-38-2');
INSERT INTO `sys_equip` VALUES ('470', '6048', '6', '咆哮兽王软靴', '8', '6049', '8-44-2|8-38-2');
INSERT INTO `sys_equip` VALUES ('471', '6049', '6', '追猎者皮靴', '9', '6050', '8-44-2|8-50-2|8-38-3');
INSERT INTO `sys_equip` VALUES ('472', '6050', '6', '疾风巨龙皮靴', '10', '6051', '8-50-2|8-56-1|8-61-1');
INSERT INTO `sys_equip` VALUES ('473', '6051', '6', '胜利斗士软靴', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('474', '6061', '6', '丝质布靴', '1', '6062', '8-5-1');
INSERT INTO `sys_equip` VALUES ('475', '6062', '6', '冰雾布靴', '2', '6063', '8-11-2');
INSERT INTO `sys_equip` VALUES ('476', '6063', '6', '缚魂者布鞋', '3', '6064', '8-11-2|8-17-2');
INSERT INTO `sys_equip` VALUES ('477', '6064', '6', '学者的旅行靴', '4', '6065', '8-17-2|8-22-2');
INSERT INTO `sys_equip` VALUES ('478', '6065', '6', '虚空行走软靴', '5', '6066', '8-28-2|8-22-2');
INSERT INTO `sys_equip` VALUES ('479', '6066', '6', '无尽黑暗布靴', '6', '6067', '8-28-2|8-34-2|8-22-2');
INSERT INTO `sys_equip` VALUES ('480', '6067', '6', '精神冲击布靴', '7', '6068', '8-34-2|8-39-2');
INSERT INTO `sys_equip` VALUES ('481', '6068', '6', '巨龙锁魂布鞋', '8', '6069', '8-45-2|8-39-2');
INSERT INTO `sys_equip` VALUES ('482', '6069', '6', '背叛者布靴', '9', '6070', '8-45-2|8-51-2|8-39-3');
INSERT INTO `sys_equip` VALUES ('483', '6070', '6', '时空梦魇布鞋', '10', '6071', '8-51-2|8-57-1|8-62-1');
INSERT INTO `sys_equip` VALUES ('484', '6071', '6', '邪神欺诈者布靴', '11', '0', '');
INSERT INTO `sys_equip` VALUES ('485', '6081', '6', '丝绸软靴', '1', '6082', '8-5-1');
INSERT INTO `sys_equip` VALUES ('486', '6082', '6', '贤者布靴', '2', '6083', '8-11-2');
INSERT INTO `sys_equip` VALUES ('487', '6083', '6', '救赎者布鞋', '3', '6084', '8-11-2|8-17-2');
INSERT INTO `sys_equip` VALUES ('488', '6084', '6', '预言软靴', '4', '6085', '8-17-2|8-23-2');
INSERT INTO `sys_equip` VALUES ('489', '6085', '6', '虔诚布鞋', '5', '6086', '8-28-2|8-23-2');
INSERT INTO `sys_equip` VALUES ('490', '6086', '6', '振奋布靴', '6', '6087', '8-28-2|8-34-2|8-23-2');
INSERT INTO `sys_equip` VALUES ('491', '6087', '6', '忠诚信仰布靴', '7', '6088', '8-34-2|8-40-2');
INSERT INTO `sys_equip` VALUES ('492', '6088', '6', '赦免审判布鞋', '8', '6089', '8-45-2|8-40-2');
INSERT INTO `sys_equip` VALUES ('493', '6089', '6', '忏悔者布靴', '9', '6090', '8-45-2|8-51-2|8-40-3');
INSERT INTO `sys_equip` VALUES ('494', '6090', '6', '无尽智慧软靴', '10', '6091', '8-51-2|8-57-1|8-63-1');
INSERT INTO `sys_equip` VALUES ('495', '6091', '6', '神圣守护布鞋', '11', '0', '');

-- ----------------------------
-- Table structure for sys_equip_level_gold
-- ----------------------------
DROP TABLE IF EXISTS `sys_equip_level_gold`;
CREATE TABLE `sys_equip_level_gold` (
  `level` int(10) unsigned NOT NULL,
  `weapon` int(10) unsigned NOT NULL COMMENT '升到下一级需要的金币',
  `necklace` int(10) unsigned NOT NULL COMMENT '升到下一级需要的金币',
  `clothes` int(10) unsigned NOT NULL COMMENT '升到下一级需要的金币',
  `belt` int(10) unsigned NOT NULL COMMENT '升到下一级需要的金币',
  `ring` int(10) unsigned NOT NULL COMMENT '升到下一级需要的金币',
  `boots` int(10) unsigned NOT NULL COMMENT '升到下一级需要的金币',
  PRIMARY KEY (`level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_equip_level_gold
-- ----------------------------
INSERT INTO `sys_equip_level_gold` VALUES ('1', '12', '12', '12', '12', '12', '12');
INSERT INTO `sys_equip_level_gold` VALUES ('2', '15', '15', '15', '15', '15', '15');
INSERT INTO `sys_equip_level_gold` VALUES ('3', '15', '15', '15', '15', '15', '15');
INSERT INTO `sys_equip_level_gold` VALUES ('4', '20', '20', '20', '20', '20', '20');
INSERT INTO `sys_equip_level_gold` VALUES ('5', '25', '25', '25', '25', '25', '25');
INSERT INTO `sys_equip_level_gold` VALUES ('6', '35', '35', '35', '35', '35', '35');
INSERT INTO `sys_equip_level_gold` VALUES ('7', '45', '45', '45', '45', '45', '45');
INSERT INTO `sys_equip_level_gold` VALUES ('8', '45', '45', '45', '45', '45', '45');
INSERT INTO `sys_equip_level_gold` VALUES ('9', '50', '50', '50', '50', '50', '50');
INSERT INTO `sys_equip_level_gold` VALUES ('10', '55', '55', '55', '55', '55', '55');
INSERT INTO `sys_equip_level_gold` VALUES ('11', '65', '65', '65', '65', '65', '65');
INSERT INTO `sys_equip_level_gold` VALUES ('12', '80', '80', '80', '80', '80', '80');
INSERT INTO `sys_equip_level_gold` VALUES ('13', '85', '85', '85', '85', '85', '85');
INSERT INTO `sys_equip_level_gold` VALUES ('14', '100', '100', '100', '100', '100', '100');
INSERT INTO `sys_equip_level_gold` VALUES ('15', '110', '110', '110', '110', '110', '110');
INSERT INTO `sys_equip_level_gold` VALUES ('16', '125', '125', '125', '125', '125', '125');
INSERT INTO `sys_equip_level_gold` VALUES ('17', '135', '135', '135', '135', '135', '135');
INSERT INTO `sys_equip_level_gold` VALUES ('18', '150', '150', '150', '150', '150', '150');
INSERT INTO `sys_equip_level_gold` VALUES ('19', '170', '170', '170', '170', '170', '170');
INSERT INTO `sys_equip_level_gold` VALUES ('20', '210', '210', '210', '210', '210', '210');
INSERT INTO `sys_equip_level_gold` VALUES ('21', '250', '250', '250', '250', '250', '250');
INSERT INTO `sys_equip_level_gold` VALUES ('22', '290', '290', '290', '290', '290', '290');
INSERT INTO `sys_equip_level_gold` VALUES ('23', '335', '335', '335', '335', '335', '335');
INSERT INTO `sys_equip_level_gold` VALUES ('24', '380', '380', '380', '380', '380', '380');
INSERT INTO `sys_equip_level_gold` VALUES ('25', '495', '495', '495', '495', '495', '495');
INSERT INTO `sys_equip_level_gold` VALUES ('26', '550', '550', '550', '550', '550', '550');
INSERT INTO `sys_equip_level_gold` VALUES ('27', '685', '685', '685', '685', '685', '685');
INSERT INTO `sys_equip_level_gold` VALUES ('28', '798', '798', '798', '798', '798', '798');
INSERT INTO `sys_equip_level_gold` VALUES ('29', '905', '905', '905', '905', '905', '905');
INSERT INTO `sys_equip_level_gold` VALUES ('30', '1000', '1000', '1000', '1000', '1000', '1000');
INSERT INTO `sys_equip_level_gold` VALUES ('31', '1115', '1115', '1115', '1115', '1115', '1115');
INSERT INTO `sys_equip_level_gold` VALUES ('32', '1180', '1180', '1180', '1180', '1180', '1180');
INSERT INTO `sys_equip_level_gold` VALUES ('33', '1255', '1255', '1255', '1255', '1255', '1255');
INSERT INTO `sys_equip_level_gold` VALUES ('34', '1330', '1330', '1330', '1330', '1330', '1330');
INSERT INTO `sys_equip_level_gold` VALUES ('35', '1405', '1405', '1405', '1405', '1405', '1405');
INSERT INTO `sys_equip_level_gold` VALUES ('36', '1490', '1490', '1490', '1490', '1490', '1490');
INSERT INTO `sys_equip_level_gold` VALUES ('37', '1580', '1580', '1580', '1580', '1580', '1580');
INSERT INTO `sys_equip_level_gold` VALUES ('38', '1675', '1675', '1675', '1675', '1675', '1675');
INSERT INTO `sys_equip_level_gold` VALUES ('39', '1775', '1775', '1775', '1775', '1775', '1775');
INSERT INTO `sys_equip_level_gold` VALUES ('40', '1880', '1880', '1880', '1880', '1880', '1880');
INSERT INTO `sys_equip_level_gold` VALUES ('41', '1995', '1995', '1995', '1995', '1995', '1995');
INSERT INTO `sys_equip_level_gold` VALUES ('42', '2115', '2115', '2115', '2115', '2115', '2115');
INSERT INTO `sys_equip_level_gold` VALUES ('43', '2240', '2240', '2240', '2240', '2240', '2240');
INSERT INTO `sys_equip_level_gold` VALUES ('44', '2370', '2370', '2370', '2370', '2370', '2370');
INSERT INTO `sys_equip_level_gold` VALUES ('45', '2515', '2515', '2515', '2515', '2515', '2515');
INSERT INTO `sys_equip_level_gold` VALUES ('46', '2665', '2665', '2665', '2665', '2665', '2665');
INSERT INTO `sys_equip_level_gold` VALUES ('47', '2825', '2825', '2825', '2825', '2825', '2825');
INSERT INTO `sys_equip_level_gold` VALUES ('48', '2990', '2990', '2990', '2990', '2990', '2990');
INSERT INTO `sys_equip_level_gold` VALUES ('49', '3170', '3170', '3170', '3170', '3170', '3170');
INSERT INTO `sys_equip_level_gold` VALUES ('50', '3360', '3360', '3360', '3360', '3360', '3360');
INSERT INTO `sys_equip_level_gold` VALUES ('51', '3405', '3405', '3405', '3405', '3405', '3405');
INSERT INTO `sys_equip_level_gold` VALUES ('52', '3455', '3455', '3455', '3455', '3455', '3455');
INSERT INTO `sys_equip_level_gold` VALUES ('53', '3520', '3520', '3520', '3520', '3520', '3520');
INSERT INTO `sys_equip_level_gold` VALUES ('54', '3625', '3625', '3625', '3625', '3625', '3625');
INSERT INTO `sys_equip_level_gold` VALUES ('55', '3730', '3730', '3730', '3730', '3730', '3730');
INSERT INTO `sys_equip_level_gold` VALUES ('56', '3835', '3835', '3835', '3835', '3835', '3835');
INSERT INTO `sys_equip_level_gold` VALUES ('57', '3945', '3945', '3945', '3945', '3945', '3945');
INSERT INTO `sys_equip_level_gold` VALUES ('58', '4055', '4055', '4055', '4055', '4055', '4055');
INSERT INTO `sys_equip_level_gold` VALUES ('59', '4165', '4165', '4165', '4165', '4165', '4165');
INSERT INTO `sys_equip_level_gold` VALUES ('60', '4265', '4265', '4265', '4265', '4265', '4265');
INSERT INTO `sys_equip_level_gold` VALUES ('61', '4375', '4375', '4375', '4375', '4375', '4375');
INSERT INTO `sys_equip_level_gold` VALUES ('62', '4545', '4545', '4545', '4545', '4545', '4545');
INSERT INTO `sys_equip_level_gold` VALUES ('63', '4715', '4715', '4715', '4715', '4715', '4715');
INSERT INTO `sys_equip_level_gold` VALUES ('64', '4885', '4885', '4885', '4885', '4885', '4885');
INSERT INTO `sys_equip_level_gold` VALUES ('65', '5005', '5005', '5005', '5005', '5005', '5005');
INSERT INTO `sys_equip_level_gold` VALUES ('66', '5195', '5195', '5195', '5195', '5195', '5195');
INSERT INTO `sys_equip_level_gold` VALUES ('67', '5325', '5325', '5325', '5325', '5325', '5325');
INSERT INTO `sys_equip_level_gold` VALUES ('68', '5535', '5535', '5535', '5535', '5535', '5535');
INSERT INTO `sys_equip_level_gold` VALUES ('69', '5765', '5765', '5765', '5765', '5765', '5765');
INSERT INTO `sys_equip_level_gold` VALUES ('70', '5985', '5985', '5985', '5985', '5985', '5985');
INSERT INTO `sys_equip_level_gold` VALUES ('71', '6195', '6195', '6195', '6195', '6195', '6195');
INSERT INTO `sys_equip_level_gold` VALUES ('72', '6405', '6405', '6405', '6405', '6405', '6405');
INSERT INTO `sys_equip_level_gold` VALUES ('73', '6705', '6705', '6705', '6705', '6705', '6705');
INSERT INTO `sys_equip_level_gold` VALUES ('74', '7025', '7025', '7025', '7025', '7025', '7025');
INSERT INTO `sys_equip_level_gold` VALUES ('75', '7335', '7335', '7335', '7335', '7335', '7335');
INSERT INTO `sys_equip_level_gold` VALUES ('76', '7645', '7645', '7645', '7645', '7645', '7645');
INSERT INTO `sys_equip_level_gold` VALUES ('77', '7975', '7975', '7975', '7975', '7975', '7975');
INSERT INTO `sys_equip_level_gold` VALUES ('78', '8305', '8305', '8305', '8305', '8305', '8305');
INSERT INTO `sys_equip_level_gold` VALUES ('79', '8655', '8655', '8655', '8655', '8655', '8655');
INSERT INTO `sys_equip_level_gold` VALUES ('80', '10000', '10000', '10000', '10000', '10000', '10000');

-- ----------------------------
-- Table structure for sys_equip_quality_gold
-- ----------------------------
DROP TABLE IF EXISTS `sys_equip_quality_gold`;
CREATE TABLE `sys_equip_quality_gold` (
  `quality` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `weapon` int(10) unsigned NOT NULL,
  `necklace` int(10) unsigned NOT NULL,
  `clothes` int(10) unsigned NOT NULL,
  `belt` int(10) unsigned NOT NULL,
  `ring` int(10) unsigned NOT NULL,
  `boots` int(10) unsigned NOT NULL,
  PRIMARY KEY (`quality`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_equip_quality_gold
-- ----------------------------
INSERT INTO `sys_equip_quality_gold` VALUES ('1', '12', '12', '12', '12', '12', '12');
INSERT INTO `sys_equip_quality_gold` VALUES ('2', '15', '15', '15', '15', '15', '15');
INSERT INTO `sys_equip_quality_gold` VALUES ('3', '15', '15', '15', '15', '15', '15');
INSERT INTO `sys_equip_quality_gold` VALUES ('4', '20', '20', '20', '20', '20', '20');
INSERT INTO `sys_equip_quality_gold` VALUES ('5', '25', '25', '25', '25', '25', '25');
INSERT INTO `sys_equip_quality_gold` VALUES ('6', '35', '35', '35', '35', '35', '35');
INSERT INTO `sys_equip_quality_gold` VALUES ('7', '45', '45', '45', '45', '45', '45');
INSERT INTO `sys_equip_quality_gold` VALUES ('8', '45', '45', '45', '45', '45', '45');
INSERT INTO `sys_equip_quality_gold` VALUES ('9', '50', '50', '50', '50', '50', '50');
INSERT INTO `sys_equip_quality_gold` VALUES ('10', '55', '55', '55', '55', '55', '55');
INSERT INTO `sys_equip_quality_gold` VALUES ('11', '0', '0', '0', '0', '0', '0');

-- ----------------------------
-- Table structure for sys_hero
-- ----------------------------
DROP TABLE IF EXISTS `sys_hero`;
CREATE TABLE `sys_hero` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `hero_id` int(10) unsigned NOT NULL COMMENT '英雄id',
  `name_cn` varchar(63) NOT NULL,
  `area` tinyint(3) unsigned NOT NULL COMMENT '所属势力',
  `occupation` tinyint(3) unsigned NOT NULL,
  `star` tinyint(3) unsigned NOT NULL,
  `quality` tinyint(3) unsigned NOT NULL,
  `passive_skill1` int(10) unsigned NOT NULL,
  `passive_skill2` int(10) unsigned NOT NULL,
  `passive_skill3` int(10) unsigned NOT NULL,
  `weapon` int(10) unsigned NOT NULL,
  `necklace` int(10) unsigned NOT NULL,
  `clothes` int(10) unsigned NOT NULL,
  `belt` int(10) unsigned NOT NULL,
  `ring` int(10) unsigned NOT NULL,
  `boots` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_hero
-- ----------------------------
INSERT INTO `sys_hero` VALUES ('1', '1', '嬴政', '1', '1', '1', '1', '1', '2', '3', '1', '2021', '3021', '4021', '5021', '6021');
INSERT INTO `sys_hero` VALUES ('2', '2', '养由基', '1', '2', '1', '1', '1', '2', '3', '21', '2041', '3041', '4041', '5041', '6041');
INSERT INTO `sys_hero` VALUES ('3', '3', '唐玄奘', '1', '4', '1', '1', '1', '2', '3', '41', '2081', '3081', '4081', '5081', '6081');
INSERT INTO `sys_hero` VALUES ('4', '4', '诸葛亮', '1', '3', '1', '1', '1', '2', '3', '61', '2061', '3061', '4061', '5061', '6061');
INSERT INTO `sys_hero` VALUES ('5', '5', '吕布', '1', '1', '1', '1', '1', '2', '3', '81', '2001', '3001', '4001', '5001', '6001');
INSERT INTO `sys_hero` VALUES ('6', '6', '项羽', '1', '1', '1', '1', '1', '2', '3', '101', '2001', '3001', '4001', '5001', '6001');
INSERT INTO `sys_hero` VALUES ('7', '7', '岳飞', '1', '1', '1', '1', '1', '2', '3', '121', '2021', '3021', '4021', '5021', '6021');
INSERT INTO `sys_hero` VALUES ('8', '8', '郑成功', '1', '2', '1', '1', '1', '2', '3', '141', '2041', '3041', '4041', '5041', '6041');
INSERT INTO `sys_hero` VALUES ('9', '9', '袁天罡', '1', '3', '1', '1', '1', '2', '3', '161', '2061', '3061', '4061', '5061', '6061');
INSERT INTO `sys_hero` VALUES ('10', '10', '华佗', '1', '4', '1', '1', '1', '2', '3', '181', '2081', '3081', '4081', '5081', '6081');
INSERT INTO `sys_hero` VALUES ('11', '11', '安倍晴明', '1', '3', '1', '1', '1', '2', '3', '201', '2061', '3061', '4061', '5061', '6061');
INSERT INTO `sys_hero` VALUES ('12', '12', '宫本武藏', '2', '1', '1', '1', '1', '2', '3', '221', '2001', '3001', '4001', '5001', '6001');
INSERT INTO `sys_hero` VALUES ('13', '13', '织田信长', '2', '1', '1', '1', '1', '2', '3', '241', '2021', '3021', '4021', '5021', '6021');
INSERT INTO `sys_hero` VALUES ('14', '14', '服部半藏', '2', '2', '1', '1', '1', '2', '3', '261', '2001', '3001', '4001', '5001', '6001');
INSERT INTO `sys_hero` VALUES ('15', '15', '辉夜姬', '2', '4', '1', '1', '1', '2', '3', '281', '2081', '3081', '4081', '5081', '6081');
INSERT INTO `sys_hero` VALUES ('16', '16', '丰臣秀吉', '2', '1', '1', '1', '1', '2', '3', '301', '2021', '3021', '4021', '5021', '6021');
INSERT INTO `sys_hero` VALUES ('17', '17', '浓姬', '2', '2', '1', '1', '1', '2', '3', '321', '2041', '3041', '4041', '5041', '6041');
INSERT INTO `sys_hero` VALUES ('18', '18', '亚瑟王', '3', '1', '1', '1', '1', '2', '3', '341', '2001', '3001', '4001', '5001', '6001');
INSERT INTO `sys_hero` VALUES ('19', '25', '拿破仑', '3', '2', '1', '1', '1', '2', '3', '321', '2041', '3041', '4041', '5041', '6041');
INSERT INTO `sys_hero` VALUES ('20', '26', '一休', '2', '3', '1', '1', '1', '2', '3', '341', '2001', '3001', '4001', '5001', '6001');

-- ----------------------------
-- Table structure for sys_hero_fragment
-- ----------------------------
DROP TABLE IF EXISTS `sys_hero_fragment`;
CREATE TABLE `sys_hero_fragment` (
  `star` tinyint(3) unsigned NOT NULL COMMENT '星级',
  `fragment` int(10) unsigned NOT NULL COMMENT '召唤或者升星所需要的碎片数量',
  PRIMARY KEY (`star`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_hero_fragment
-- ----------------------------

-- ----------------------------
-- Table structure for sys_hero_level
-- ----------------------------
DROP TABLE IF EXISTS `sys_hero_level`;
CREATE TABLE `sys_hero_level` (
  `level` int(10) unsigned NOT NULL,
  `exp_for_next_level` int(10) unsigned NOT NULL,
  PRIMARY KEY (`level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_hero_level
-- ----------------------------
INSERT INTO `sys_hero_level` VALUES ('1', '12');
INSERT INTO `sys_hero_level` VALUES ('2', '15');
INSERT INTO `sys_hero_level` VALUES ('3', '15');
INSERT INTO `sys_hero_level` VALUES ('4', '20');
INSERT INTO `sys_hero_level` VALUES ('5', '25');
INSERT INTO `sys_hero_level` VALUES ('6', '35');
INSERT INTO `sys_hero_level` VALUES ('7', '45');
INSERT INTO `sys_hero_level` VALUES ('8', '45');
INSERT INTO `sys_hero_level` VALUES ('9', '50');
INSERT INTO `sys_hero_level` VALUES ('10', '55');
INSERT INTO `sys_hero_level` VALUES ('11', '65');
INSERT INTO `sys_hero_level` VALUES ('12', '80');
INSERT INTO `sys_hero_level` VALUES ('13', '85');
INSERT INTO `sys_hero_level` VALUES ('14', '100');
INSERT INTO `sys_hero_level` VALUES ('15', '110');
INSERT INTO `sys_hero_level` VALUES ('16', '125');
INSERT INTO `sys_hero_level` VALUES ('17', '135');
INSERT INTO `sys_hero_level` VALUES ('18', '150');
INSERT INTO `sys_hero_level` VALUES ('19', '170');
INSERT INTO `sys_hero_level` VALUES ('20', '210');
INSERT INTO `sys_hero_level` VALUES ('21', '250');
INSERT INTO `sys_hero_level` VALUES ('22', '290');
INSERT INTO `sys_hero_level` VALUES ('23', '335');
INSERT INTO `sys_hero_level` VALUES ('24', '380');
INSERT INTO `sys_hero_level` VALUES ('25', '495');
INSERT INTO `sys_hero_level` VALUES ('26', '550');
INSERT INTO `sys_hero_level` VALUES ('27', '685');
INSERT INTO `sys_hero_level` VALUES ('28', '798');
INSERT INTO `sys_hero_level` VALUES ('29', '905');
INSERT INTO `sys_hero_level` VALUES ('30', '1000');
INSERT INTO `sys_hero_level` VALUES ('31', '1115');
INSERT INTO `sys_hero_level` VALUES ('32', '1180');
INSERT INTO `sys_hero_level` VALUES ('33', '1255');
INSERT INTO `sys_hero_level` VALUES ('34', '1330');
INSERT INTO `sys_hero_level` VALUES ('35', '1405');
INSERT INTO `sys_hero_level` VALUES ('36', '1490');
INSERT INTO `sys_hero_level` VALUES ('37', '1580');
INSERT INTO `sys_hero_level` VALUES ('38', '1675');
INSERT INTO `sys_hero_level` VALUES ('39', '1775');
INSERT INTO `sys_hero_level` VALUES ('40', '1880');
INSERT INTO `sys_hero_level` VALUES ('41', '1995');
INSERT INTO `sys_hero_level` VALUES ('42', '2115');
INSERT INTO `sys_hero_level` VALUES ('43', '2240');
INSERT INTO `sys_hero_level` VALUES ('44', '2370');
INSERT INTO `sys_hero_level` VALUES ('45', '2515');
INSERT INTO `sys_hero_level` VALUES ('46', '2665');
INSERT INTO `sys_hero_level` VALUES ('47', '2825');
INSERT INTO `sys_hero_level` VALUES ('48', '2990');
INSERT INTO `sys_hero_level` VALUES ('49', '3170');
INSERT INTO `sys_hero_level` VALUES ('50', '3360');
INSERT INTO `sys_hero_level` VALUES ('51', '3405');
INSERT INTO `sys_hero_level` VALUES ('52', '3455');
INSERT INTO `sys_hero_level` VALUES ('53', '3520');
INSERT INTO `sys_hero_level` VALUES ('54', '3625');
INSERT INTO `sys_hero_level` VALUES ('55', '3730');
INSERT INTO `sys_hero_level` VALUES ('56', '3835');
INSERT INTO `sys_hero_level` VALUES ('57', '3945');
INSERT INTO `sys_hero_level` VALUES ('58', '4055');
INSERT INTO `sys_hero_level` VALUES ('59', '4165');
INSERT INTO `sys_hero_level` VALUES ('60', '4265');
INSERT INTO `sys_hero_level` VALUES ('61', '4375');
INSERT INTO `sys_hero_level` VALUES ('62', '4545');
INSERT INTO `sys_hero_level` VALUES ('63', '4715');
INSERT INTO `sys_hero_level` VALUES ('64', '4885');
INSERT INTO `sys_hero_level` VALUES ('65', '5005');
INSERT INTO `sys_hero_level` VALUES ('66', '5195');
INSERT INTO `sys_hero_level` VALUES ('67', '5325');
INSERT INTO `sys_hero_level` VALUES ('68', '5535');
INSERT INTO `sys_hero_level` VALUES ('69', '5765');
INSERT INTO `sys_hero_level` VALUES ('70', '5985');
INSERT INTO `sys_hero_level` VALUES ('71', '6195');
INSERT INTO `sys_hero_level` VALUES ('72', '6405');
INSERT INTO `sys_hero_level` VALUES ('73', '6705');
INSERT INTO `sys_hero_level` VALUES ('74', '7025');
INSERT INTO `sys_hero_level` VALUES ('75', '7335');
INSERT INTO `sys_hero_level` VALUES ('76', '7645');
INSERT INTO `sys_hero_level` VALUES ('77', '7975');
INSERT INTO `sys_hero_level` VALUES ('78', '8305');
INSERT INTO `sys_hero_level` VALUES ('79', '8655');
INSERT INTO `sys_hero_level` VALUES ('80', '0');

-- ----------------------------
-- Table structure for sys_hero_level_need
-- ----------------------------
DROP TABLE IF EXISTS `sys_hero_level_need`;
CREATE TABLE `sys_hero_level_need` (
  `level` int(10) unsigned NOT NULL,
  `exp_for_next_level` int(10) unsigned NOT NULL,
  PRIMARY KEY (`level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_hero_level_need
-- ----------------------------
INSERT INTO `sys_hero_level_need` VALUES ('1', '12');
INSERT INTO `sys_hero_level_need` VALUES ('2', '15');
INSERT INTO `sys_hero_level_need` VALUES ('3', '15');
INSERT INTO `sys_hero_level_need` VALUES ('4', '20');
INSERT INTO `sys_hero_level_need` VALUES ('5', '25');
INSERT INTO `sys_hero_level_need` VALUES ('6', '35');
INSERT INTO `sys_hero_level_need` VALUES ('7', '45');
INSERT INTO `sys_hero_level_need` VALUES ('8', '45');
INSERT INTO `sys_hero_level_need` VALUES ('9', '50');
INSERT INTO `sys_hero_level_need` VALUES ('10', '55');
INSERT INTO `sys_hero_level_need` VALUES ('11', '65');
INSERT INTO `sys_hero_level_need` VALUES ('12', '80');
INSERT INTO `sys_hero_level_need` VALUES ('13', '85');
INSERT INTO `sys_hero_level_need` VALUES ('14', '100');
INSERT INTO `sys_hero_level_need` VALUES ('15', '110');
INSERT INTO `sys_hero_level_need` VALUES ('16', '125');
INSERT INTO `sys_hero_level_need` VALUES ('17', '135');
INSERT INTO `sys_hero_level_need` VALUES ('18', '150');
INSERT INTO `sys_hero_level_need` VALUES ('19', '170');
INSERT INTO `sys_hero_level_need` VALUES ('20', '210');
INSERT INTO `sys_hero_level_need` VALUES ('21', '250');
INSERT INTO `sys_hero_level_need` VALUES ('22', '290');
INSERT INTO `sys_hero_level_need` VALUES ('23', '335');
INSERT INTO `sys_hero_level_need` VALUES ('24', '380');
INSERT INTO `sys_hero_level_need` VALUES ('25', '495');
INSERT INTO `sys_hero_level_need` VALUES ('26', '550');
INSERT INTO `sys_hero_level_need` VALUES ('27', '685');
INSERT INTO `sys_hero_level_need` VALUES ('28', '798');
INSERT INTO `sys_hero_level_need` VALUES ('29', '905');
INSERT INTO `sys_hero_level_need` VALUES ('30', '1000');
INSERT INTO `sys_hero_level_need` VALUES ('31', '1115');
INSERT INTO `sys_hero_level_need` VALUES ('32', '1180');
INSERT INTO `sys_hero_level_need` VALUES ('33', '1255');
INSERT INTO `sys_hero_level_need` VALUES ('34', '1330');
INSERT INTO `sys_hero_level_need` VALUES ('35', '1405');
INSERT INTO `sys_hero_level_need` VALUES ('36', '1490');
INSERT INTO `sys_hero_level_need` VALUES ('37', '1580');
INSERT INTO `sys_hero_level_need` VALUES ('38', '1675');
INSERT INTO `sys_hero_level_need` VALUES ('39', '1775');
INSERT INTO `sys_hero_level_need` VALUES ('40', '1880');
INSERT INTO `sys_hero_level_need` VALUES ('41', '1995');
INSERT INTO `sys_hero_level_need` VALUES ('42', '2115');
INSERT INTO `sys_hero_level_need` VALUES ('43', '2240');
INSERT INTO `sys_hero_level_need` VALUES ('44', '2370');
INSERT INTO `sys_hero_level_need` VALUES ('45', '2515');
INSERT INTO `sys_hero_level_need` VALUES ('46', '2665');
INSERT INTO `sys_hero_level_need` VALUES ('47', '2825');
INSERT INTO `sys_hero_level_need` VALUES ('48', '2990');
INSERT INTO `sys_hero_level_need` VALUES ('49', '3170');
INSERT INTO `sys_hero_level_need` VALUES ('50', '3360');
INSERT INTO `sys_hero_level_need` VALUES ('51', '3405');
INSERT INTO `sys_hero_level_need` VALUES ('52', '3455');
INSERT INTO `sys_hero_level_need` VALUES ('53', '3520');
INSERT INTO `sys_hero_level_need` VALUES ('54', '3625');
INSERT INTO `sys_hero_level_need` VALUES ('55', '3730');
INSERT INTO `sys_hero_level_need` VALUES ('56', '3835');
INSERT INTO `sys_hero_level_need` VALUES ('57', '3945');
INSERT INTO `sys_hero_level_need` VALUES ('58', '4055');
INSERT INTO `sys_hero_level_need` VALUES ('59', '4165');
INSERT INTO `sys_hero_level_need` VALUES ('60', '4265');
INSERT INTO `sys_hero_level_need` VALUES ('61', '4375');
INSERT INTO `sys_hero_level_need` VALUES ('62', '4545');
INSERT INTO `sys_hero_level_need` VALUES ('63', '4715');
INSERT INTO `sys_hero_level_need` VALUES ('64', '4885');
INSERT INTO `sys_hero_level_need` VALUES ('65', '5005');
INSERT INTO `sys_hero_level_need` VALUES ('66', '5195');
INSERT INTO `sys_hero_level_need` VALUES ('67', '5325');
INSERT INTO `sys_hero_level_need` VALUES ('68', '5535');
INSERT INTO `sys_hero_level_need` VALUES ('69', '5765');
INSERT INTO `sys_hero_level_need` VALUES ('70', '5985');
INSERT INTO `sys_hero_level_need` VALUES ('71', '6195');
INSERT INTO `sys_hero_level_need` VALUES ('72', '6405');
INSERT INTO `sys_hero_level_need` VALUES ('73', '6705');
INSERT INTO `sys_hero_level_need` VALUES ('74', '7025');
INSERT INTO `sys_hero_level_need` VALUES ('75', '7335');
INSERT INTO `sys_hero_level_need` VALUES ('76', '7645');
INSERT INTO `sys_hero_level_need` VALUES ('77', '7975');
INSERT INTO `sys_hero_level_need` VALUES ('78', '8305');
INSERT INTO `sys_hero_level_need` VALUES ('79', '8655');
INSERT INTO `sys_hero_level_need` VALUES ('80', '0');

-- ----------------------------
-- Table structure for sys_hero_quality
-- ----------------------------
DROP TABLE IF EXISTS `sys_hero_quality`;
CREATE TABLE `sys_hero_quality` (
  `quality` int(10) unsigned NOT NULL,
  `need_level` int(10) unsigned NOT NULL,
  `gold` int(10) unsigned NOT NULL,
  `hp_grow` int(10) unsigned NOT NULL,
  `str_grow` int(10) unsigned NOT NULL,
  `def_grow` int(10) unsigned NOT NULL,
  `mdef_grow` int(10) unsigned NOT NULL,
  PRIMARY KEY (`quality`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_hero_quality
-- ----------------------------

-- ----------------------------
-- Table structure for sys_hero_quality_need
-- ----------------------------
DROP TABLE IF EXISTS `sys_hero_quality_need`;
CREATE TABLE `sys_hero_quality_need` (
  `quality` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `need_level` int(10) unsigned NOT NULL,
  `gold` int(10) unsigned NOT NULL,
  PRIMARY KEY (`quality`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_hero_quality_need
-- ----------------------------
INSERT INTO `sys_hero_quality_need` VALUES ('1', '0', '12');
INSERT INTO `sys_hero_quality_need` VALUES ('2', '5', '15');
INSERT INTO `sys_hero_quality_need` VALUES ('3', '10', '15');
INSERT INTO `sys_hero_quality_need` VALUES ('4', '15', '20');
INSERT INTO `sys_hero_quality_need` VALUES ('5', '20', '25');
INSERT INTO `sys_hero_quality_need` VALUES ('6', '25', '35');
INSERT INTO `sys_hero_quality_need` VALUES ('7', '30', '45');
INSERT INTO `sys_hero_quality_need` VALUES ('8', '35', '45');
INSERT INTO `sys_hero_quality_need` VALUES ('9', '40', '50');
INSERT INTO `sys_hero_quality_need` VALUES ('10', '45', '55');
INSERT INTO `sys_hero_quality_need` VALUES ('11', '0', '0');

-- ----------------------------
-- Table structure for sys_hero_star_need
-- ----------------------------
DROP TABLE IF EXISTS `sys_hero_star_need`;
CREATE TABLE `sys_hero_star_need` (
  `star` tinyint(3) unsigned NOT NULL COMMENT '星级',
  `fragment` int(10) unsigned NOT NULL COMMENT '召唤或者升星所需要的碎片数量',
  PRIMARY KEY (`star`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_hero_star_need
-- ----------------------------
INSERT INTO `sys_hero_star_need` VALUES ('1', '10');
INSERT INTO `sys_hero_star_need` VALUES ('2', '20');
INSERT INTO `sys_hero_star_need` VALUES ('3', '50');
INSERT INTO `sys_hero_star_need` VALUES ('4', '100');
INSERT INTO `sys_hero_star_need` VALUES ('5', '320');

-- ----------------------------
-- Table structure for sys_item
-- ----------------------------
DROP TABLE IF EXISTS `sys_item`;
CREATE TABLE `sys_item` (
  `id` int(10) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL COMMENT '道具ID',
  `use_type` tinyint(3) unsigned NOT NULL COMMENT '使用类型1.进化材料 2.经验值道具 3.扫荡券 4.杂物 5.材料碎片 6.英雄碎片',
  `name_cn` varchar(255) NOT NULL DEFAULT '' COMMENT '道具名字',
  `quality` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '品质',
  `overlap_num` int(10) unsigned NOT NULL COMMENT '最大可堆叠数量',
  `sell_gold` int(10) unsigned NOT NULL COMMENT '卖出时价格',
  `use_output` varchar(511) NOT NULL DEFAULT '' COMMENT '使用后的产出 形如  type-id-num|type-id-num : 1-555-10|2-5864-2',
  `compose_input` varchar(511) NOT NULL DEFAULT '' COMMENT '合成此道具所需要的消耗 形如  type-id-num|type-id-num : 1-555-10|2-5864-2',
  PRIMARY KEY (`id`),
  UNIQUE KEY `item_id` (`item_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_item
-- ----------------------------
INSERT INTO `sys_item` VALUES ('1', '1', '1', '碎铜块', '2', '999', '1500', '', '');
INSERT INTO `sys_item` VALUES ('2', '2', '1', '旧铜链', '2', '999', '1500', '', '');
INSERT INTO `sys_item` VALUES ('3', '3', '1', '破铁片', '2', '999', '1500', '', '');
INSERT INTO `sys_item` VALUES ('4', '4', '1', '烂皮毛', '2', '999', '1500', '', '');
INSERT INTO `sys_item` VALUES ('5', '5', '1', '亚麻布', '2', '999', '1500', '', '');
INSERT INTO `sys_item` VALUES ('6', '6', '1', '小珍珠', '2', '999', '1500', '', '');
INSERT INTO `sys_item` VALUES ('7', '7', '1', '重铸的青铜块', '2', '999', '1500', '', '');
INSERT INTO `sys_item` VALUES ('8', '8', '1', '劣质银丝', '2', '999', '1500', '', '');
INSERT INTO `sys_item` VALUES ('9', '9', '1', '淬火的铁块', '2', '999', '1500', '', '');
INSERT INTO `sys_item` VALUES ('10', '10', '1', '轻薄兽皮', '2', '999', '1500', '', '');
INSERT INTO `sys_item` VALUES ('11', '11', '1', '旧棉布', '2', '999', '1500', '', '');
INSERT INTO `sys_item` VALUES ('12', '12', '1', '劣质铜构件', '2', '999', '1500', '', '');
INSERT INTO `sys_item` VALUES ('13', '13', '1', '火焰碎片', '3', '999', '2000', '', '8-64-1|8-70-1|12-0-2000');
INSERT INTO `sys_item` VALUES ('14', '14', '1', '彩色珍珠', '3', '999', '2000', '', '8-65-1|8-71-1|12-0-2000');
INSERT INTO `sys_item` VALUES ('15', '15', '1', '弱效酸液', '3', '999', '2000', '', '8-66-1|8-72-1|12-0-2000');
INSERT INTO `sys_item` VALUES ('16', '16', '1', '硝石粉末', '3', '999', '2000', '', '8-67-1|8-73-1|12-0-2000');
INSERT INTO `sys_item` VALUES ('17', '17', '1', '银丝线', '3', '999', '2000', '', '8-68-1|8-74-1|12-0-2000');
INSERT INTO `sys_item` VALUES ('18', '18', '1', '粗糙的宝石', '3', '999', '2000', '', '8-69-1|8-75-1|12-0-2000');
INSERT INTO `sys_item` VALUES ('19', '19', '1', '硫化黑铁锭', '3', '999', '2000', '', '8-76-1|8-81-1|8-86-1|12-0-3000');
INSERT INTO `sys_item` VALUES ('20', '20', '1', '镀银青铜', '3', '999', '2000', '', '8-77-1|8-82-1|8-87-1|12-0-3000');
INSERT INTO `sys_item` VALUES ('21', '21', '1', '坚韧皮带', '3', '999', '2000', '', '8-78-1|8-83-1|8-88-1|12-0-3000');
INSERT INTO `sys_item` VALUES ('22', '22', '1', '丝绸', '3', '999', '2000', '', '8-79-1|8-84-1|8-89-1|12-0-3000');
INSERT INTO `sys_item` VALUES ('23', '23', '1', '织布', '3', '999', '2000', '', '8-80-1|8-85-1|8-90-1|12-0-3000');
INSERT INTO `sys_item` VALUES ('24', '24', '1', '月光精华', '3', '999', '2000', '', '8-91-2|8-97-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('25', '25', '1', '金色珍珠', '3', '999', '2000', '', '8-92-2|8-98-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('26', '26', '1', '灵魂碎片', '3', '999', '2000', '', '8-93-2|8-99-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('27', '27', '1', '兽魂残渣', '3', '999', '2000', '', '8-94-2|8-100-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('28', '28', '1', '信念精华', '3', '999', '2000', '', '8-95-2|8-101-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('29', '29', '1', '精制宝石', '3', '999', '2000', '', '8-96-2|8-102-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('30', '30', '1', '战魂精魄', '4', '999', '2500', '', '8-103-2|8-109-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('31', '31', '1', '镶金银链', '4', '999', '2500', '', '8-104-2|8-110-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('32', '32', '1', '战旗碎片', '4', '999', '2500', '', '8-105-2|8-111-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('33', '33', '1', '野兽皮毛', '4', '999', '2500', '', '8-106-2|8-112-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('34', '34', '1', '魔能核心', '4', '999', '2500', '', '8-107-2|8-113-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('35', '35', '1', '强化银质构件', '4', '999', '2500', '', '8-108-2|8-114-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('36', '36', '1', '合金钢铁', '4', '999', '2500', '', '8-115-2|8-120-2|8-125-2|12-0-6000');
INSERT INTO `sys_item` VALUES ('37', '37', '1', '魔化黑铁', '4', '999', '2500', '', '8-116-2|8-121-2|8-126-2|12-0-6000');
INSERT INTO `sys_item` VALUES ('38', '38', '1', '硬质厚皮', '4', '999', '2500', '', '8-117-2|8-122-2|8-127-2|12-0-6000');
INSERT INTO `sys_item` VALUES ('39', '39', '1', '魔化丝绸布', '4', '999', '2500', '', '8-118-2|8-123-2|8-128-2|12-0-6000');
INSERT INTO `sys_item` VALUES ('40', '40', '1', '灵魂织布', '4', '999', '2500', '', '8-119-2|8-124-2|8-129-2|12-0-6000');
INSERT INTO `sys_item` VALUES ('41', '41', '1', '勇者精粹', '4', '999', '2500', '', '8-130-2|8-136-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('42', '42', '1', '神秘宝石', '4', '999', '2500', '', '8-131-2|8-137-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('43', '43', '1', '硬化合剂', '4', '999', '2500', '', '8-132-2|8-138-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('44', '44', '1', '精炼硝石', '4', '999', '2500', '', '8-133-2|8-139-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('45', '45', '1', '金丝线', '4', '999', '2500', '', '8-134-2|8-140-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('46', '46', '1', '贵重珠宝', '4', '999', '2500', '', '8-135-2|8-141-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('47', '47', '1', '恶魔头骨', '4', '999', '2500', '', '8-142-2|8-148-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('48', '48', '1', '哭泣的灵魂', '4', '999', '2500', '', '8-143-2|8-149-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('49', '49', '1', '勇气凭证', '4', '999', '2500', '', '8-144-2|8-150-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('50', '50', '1', '敏捷徽记', '4', '999', '2500', '', '8-145-2|8-151-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('51', '51', '1', '智慧勋章', '4', '999', '2500', '', '8-146-2|8-152-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('52', '52', '1', '符文金属构件', '4', '999', '2500', '', '8-147-2|8-153-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('53', '53', '1', '高强度合金', '5', '999', '3000', '', '8-154-2|8-160-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('54', '54', '1', '神圣光华', '5', '999', '3000', '', '8-155-2|8-161-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('55', '55', '1', '硬质合金', '5', '999', '3000', '', '8-156-2|8-162-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('56', '56', '1', '恶魔皮革', '5', '999', '3000', '', '8-157-2|8-163-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('57', '57', '1', '魔能丝绸', '5', '999', '3000', '', '8-158-2|8-164-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('58', '58', '1', '神之泪珠', '5', '999', '3000', '', '8-159-2|8-165-2|12-0-4000');
INSERT INTO `sys_item` VALUES ('59', '59', '1', '天火精华', '5', '999', '3000', '', '8-166-2|8-171-2|8-176-2|12-0-6000');
INSERT INTO `sys_item` VALUES ('60', '60', '1', '诡秘精魄', '5', '999', '3000', '', '8-167-2|8-99-2|8-177-2|12-0-6000');
INSERT INTO `sys_item` VALUES ('61', '61', '1', '风之气息', '5', '999', '3000', '', '8-168-2|8-173-2|8-178-2|12-0-6000');
INSERT INTO `sys_item` VALUES ('62', '62', '1', '奥秘符文', '5', '999', '3000', '', '8-169-2|8-174-2|8-179-2|12-0-6000');
INSERT INTO `sys_item` VALUES ('63', '63', '1', '虔诚之心', '5', '999', '3000', '', '8-170-2|8-175-2|8-180-2|12-0-6000');
INSERT INTO `sys_item` VALUES ('64', '64', '1', '油灯残座', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('65', '65', '1', '小蚌壳', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('66', '66', '1', '污水', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('67', '67', '1', '碎石块', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('68', '68', '1', '银丝', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('69', '69', '1', '原石', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('70', '70', '1', '火柴盒', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('71', '71', '1', '小尖头锤', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('72', '72', '1', '失效的硫酸', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('73', '73', '1', '硝粉', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('74', '74', '1', '棉线', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('75', '75', '1', '磨制工具', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('76', '76', '1', '黑铁块', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('77', '77', '1', '银粉', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('78', '78', '1', '小皮带', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('79', '79', '1', '线卷', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('80', '80', '1', '棉布', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('81', '81', '1', '硫化剂', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('82', '82', '1', '青铜块', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('83', '83', '1', '粗针线', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('84', '84', '1', '老旧机器', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('85', '85', '1', '残缺的织机', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('86', '86', '1', '毛刷', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('87', '87', '1', '喷头', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('88', '88', '1', '绞丝线', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('89', '89', '1', '绸带', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('90', '90', '1', '细线', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('91', '91', '1', '月华', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('92', '92', '1', '厚重的蚌壳', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('93', '93', '1', '残缺的灵魂', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('94', '94', '1', '消失的兽魂', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('95', '95', '1', '祷告书', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('96', '96', '1', '大块原石', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('97', '97', '1', '水盆', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('98', '98', '1', '尖头锤', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('99', '99', '1', '收集器', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('100', '100', '1', '骨渣', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('101', '101', '1', '乐器', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('102', '102', '1', '精制工具', '3', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('103', '103', '1', '魂魄', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('104', '104', '1', '金粉', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('105', '105', '1', '碎步', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('106', '106', '1', '厚兽皮', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('107', '107', '1', '石核', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('108', '108', '1', '银条', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('109', '109', '1', '精炼剂', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('110', '110', '1', '银链子', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('111', '111', '1', '旗帜', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('112', '112', '1', '野性之心', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('113', '113', '1', '魔药剂', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('114', '114', '1', '精确加工台', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('115', '115', '1', '钢铁', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('116', '116', '1', '黑铁锭', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('117', '117', '1', '厚重的皮', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('118', '118', '1', '丝绸布', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('119', '119', '1', '厚棉布', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('120', '120', '1', '金属元素', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('121', '121', '1', '恶魔药剂', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('122', '122', '1', '硬化剂', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('123', '123', '1', '魔能药剂', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('124', '124', '1', '遗忘的灵魂', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('125', '125', '1', '淬火台', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('126', '126', '1', '铁锤', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('127', '127', '1', '制作台', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('128', '128', '1', '小机器', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('129', '129', '1', '织机', '4', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('130', '130', '1', '勇者精魂', '4', '999', '2500', '', '8-1001-3|12-0-3000');
INSERT INTO `sys_item` VALUES ('131', '131', '1', '厚原石', '4', '999', '2500', '', '8-1002-3|12-0-3000');
INSERT INTO `sys_item` VALUES ('132', '132', '1', '强效硬化剂', '4', '999', '2500', '', '8-1003-3|12-0-3000');
INSERT INTO `sys_item` VALUES ('133', '133', '1', '硝石', '4', '999', '2500', '', '8-1004-3|12-0-3000');
INSERT INTO `sys_item` VALUES ('134', '134', '1', '金丝', '4', '999', '2500', '', '8-1005-3|12-0-3000');
INSERT INTO `sys_item` VALUES ('135', '135', '1', '大块厚原石', '4', '999', '2500', '', '8-1006-3|12-0-3000');
INSERT INTO `sys_item` VALUES ('136', '136', '1', '精炼药剂', '4', '999', '2500', '', '8-1007-3|12-0-3000');
INSERT INTO `sys_item` VALUES ('137', '137', '1', '坚硬的锤子', '4', '999', '2500', '', '8-1008-3|12-0-3000');
INSERT INTO `sys_item` VALUES ('138', '138', '1', '融合药剂', '4', '999', '2500', '', '8-1009-3|12-0-3000');
INSERT INTO `sys_item` VALUES ('139', '139', '1', '提炼药剂', '4', '999', '2500', '', '8-1010-3|12-0-3000');
INSERT INTO `sys_item` VALUES ('140', '140', '1', '丝线', '4', '999', '2500', '', '8-1011-3|12-0-3000');
INSERT INTO `sys_item` VALUES ('141', '141', '1', '精确磨制工具', '4', '999', '2500', '', '8-1012-3|12-0-3000');
INSERT INTO `sys_item` VALUES ('142', '142', '1', '恶魔残骸', '4', '999', '2500', '', '8-1013-4|12-0-4000');
INSERT INTO `sys_item` VALUES ('143', '143', '1', '胆小的灵魂', '4', '999', '2500', '', '8-1014-4|12-0-4000');
INSERT INTO `sys_item` VALUES ('144', '144', '1', '军工薄', '4', '999', '2500', '', '8-1015-4|12-0-4000');
INSERT INTO `sys_item` VALUES ('145', '145', '1', '鹰羽', '4', '999', '2500', '', '8-1016-4|12-0-4000');
INSERT INTO `sys_item` VALUES ('146', '146', '1', '奥数冠军奖章', '4', '999', '2500', '', '8-1017-4|12-0-4000');
INSERT INTO `sys_item` VALUES ('147', '147', '1', '精制构件', '4', '999', '2500', '', '8-1018-4|12-0-4000');
INSERT INTO `sys_item` VALUES ('148', '148', '1', '屠刀', '4', '999', '2500', '', '8-1019-4|12-0-4000');
INSERT INTO `sys_item` VALUES ('149', '149', '1', '皮鞭', '4', '999', '2500', '', '8-1020-4|12-0-4000');
INSERT INTO `sys_item` VALUES ('150', '150', '1', '奖励凭证', '4', '999', '2500', '', '8-1021-4|12-0-4000');
INSERT INTO `sys_item` VALUES ('151', '151', '1', '微风颗粒', '4', '999', '2500', '', '8-1022-4|12-0-4000');
INSERT INTO `sys_item` VALUES ('152', '152', '1', '笔记本', '4', '999', '2500', '', '8-1023-4|12-0-4000');
INSERT INTO `sys_item` VALUES ('153', '153', '1', '符文笔', '4', '999', '2500', '', '8-1024-4|12-0-4000');
INSERT INTO `sys_item` VALUES ('154', '154', '1', '合金钢', '5', '999', '3000', '', '8-1025-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('155', '155', '1', '祈祷台', '5', '999', '3000', '', '8-1026-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('156', '156', '1', '初级合金', '5', '999', '3000', '', '8-1027-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('157', '157', '1', '魔鬼残骸', '5', '999', '3000', '', '8-1028-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('158', '158', '1', '丝绸卷', '5', '999', '3000', '', '8-1029-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('159', '159', '1', '金色脸盆', '5', '999', '3000', '', '8-1030-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('160', '160', '1', '强化合剂', '5', '999', '3000', '', '8-1031-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('161', '161', '1', '信仰收集器', '5', '999', '3000', '', '8-1032-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('162', '162', '1', '弱效硬化剂', '5', '999', '3000', '', '8-1033-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('163', '163', '1', '剥皮刀', '5', '999', '3000', '', '8-1034-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('164', '164', '1', '强效魔能药剂', '5', '999', '3000', '', '8-1035-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('165', '165', '1', '神秘的水珠', '5', '999', '3000', '', '8-1036-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('166', '166', '1', '破碎的流星', '5', '999', '3000', '', '8-1037-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('167', '167', '1', '诡异气息', '5', '999', '3000', '', '8-1038-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('168', '168', '1', '微风气息', '5', '999', '3000', '', '8-1039-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('169', '169', '1', '秘术之心', '5', '999', '3000', '', '8-1040-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('170', '170', '1', '神圣经文', '5', '999', '3000', '', '8-1041-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('171', '171', '1', '提炼器', '5', '999', '3000', '', '8-1042-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('172', '172', '1', '收集器', '5', '999', '3000', '', '8-1043-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('173', '173', '1', '精确收集器', '5', '999', '3000', '', '8-1044-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('174', '174', '1', '奥法徽记', '5', '999', '3000', '', '8-1045-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('175', '175', '1', '圣洁祷告书', '5', '999', '3000', '', '8-1046-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('176', '176', '1', '神秘火焰', '5', '999', '3000', '', '8-1047-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('177', '177', '1', '暴躁的残魂', '5', '999', '3000', '', '8-1048-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('178', '178', '1', '狂暴的风精灵', '5', '999', '3000', '', '8-1049-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('179', '179', '1', '强效融合剂', '5', '999', '3000', '', '8-1050-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('180', '180', '1', '圣地祷告台', '5', '999', '3000', '', '8-1051-5|12-0-5000');
INSERT INTO `sys_item` VALUES ('181', '501', '2', '经验药水（小）', '1', '999', '100', '11-2-1000', '');
INSERT INTO `sys_item` VALUES ('182', '502', '2', '经验药水（中）', '2', '999', '200', '11-2-2000', '');
INSERT INTO `sys_item` VALUES ('183', '503', '2', '经验药水（大）', '3', '999', '300', '11-2-3000', '');
INSERT INTO `sys_item` VALUES ('184', '504', '2', '经验药水（特大）', '4', '999', '400', '11-2-4000', '');
INSERT INTO `sys_item` VALUES ('185', '505', '2', '经验药水（超大）', '5', '999', '500', '11-2-5000', '');
INSERT INTO `sys_item` VALUES ('186', '601', '3', '扫荡券', '3', '999', '100', '', '');
INSERT INTO `sys_item` VALUES ('187', '701', '4', '杂物1', '1', '999', '1000', '', '');
INSERT INTO `sys_item` VALUES ('188', '702', '4', '杂物2', '1', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('189', '703', '4', '杂物3', '2', '999', '3000', '', '');
INSERT INTO `sys_item` VALUES ('190', '704', '4', '杂物4', '2', '999', '4000', '', '');
INSERT INTO `sys_item` VALUES ('191', '705', '4', '杂物5', '3', '999', '5000', '', '');
INSERT INTO `sys_item` VALUES ('192', '706', '4', '杂物6', '3', '999', '6000', '', '');
INSERT INTO `sys_item` VALUES ('193', '707', '4', '杂物7', '4', '999', '7000', '', '');
INSERT INTO `sys_item` VALUES ('194', '708', '4', '杂物8', '4', '999', '8000', '', '');
INSERT INTO `sys_item` VALUES ('195', '709', '4', '杂物9', '5', '999', '9000', '', '');
INSERT INTO `sys_item` VALUES ('196', '710', '4', '杂物10', '5', '999', '10000', '', '');
INSERT INTO `sys_item` VALUES ('197', '1001', '5', '勇者精魂碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('198', '1002', '5', '厚原石碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('199', '1003', '5', '强效硬化剂碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('200', '1004', '5', '硝石碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('201', '1005', '5', '金丝碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('202', '1006', '5', '大块厚原石碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('203', '1007', '5', '精炼药剂碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('204', '1008', '5', '坚硬的锤子碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('205', '1009', '5', '融合药剂碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('206', '1010', '5', '提炼药剂碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('207', '1011', '5', '丝线碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('208', '1012', '5', '精确磨制工具碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('209', '1013', '5', '恶魔残骸碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('210', '1014', '5', '胆小的灵魂碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('211', '1015', '5', '军工薄碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('212', '1016', '5', '鹰羽碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('213', '1017', '5', '奥数冠军奖章碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('214', '1018', '5', '精制构件碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('215', '1019', '5', '屠刀碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('216', '1020', '5', '皮鞭碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('217', '1021', '5', '奖励凭证碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('218', '1022', '5', '微风颗粒碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('219', '1023', '5', '笔记本碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('220', '1024', '5', '符文笔碎片', '4', '999', '2000', '', '');
INSERT INTO `sys_item` VALUES ('221', '1025', '5', '合金钢碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('222', '1026', '5', '祈祷台碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('223', '1027', '5', '初级合金碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('224', '1028', '5', '魔鬼残骸碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('225', '1029', '5', '丝绸卷碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('226', '1030', '5', '金色脸盆碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('227', '1031', '5', '强化合剂碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('228', '1032', '5', '信仰收集器碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('229', '1033', '5', '弱效硬化剂碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('230', '1034', '5', '剥皮刀碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('231', '1035', '5', '强效魔能药剂碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('232', '1036', '5', '神秘的水珠碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('233', '1037', '5', '破碎的流星碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('234', '1038', '5', '诡异气息碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('235', '1039', '5', '微风气息碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('236', '1040', '5', '秘术之心碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('237', '1041', '5', '神圣经文碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('238', '1042', '5', '提炼器碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('239', '1043', '5', '收集器碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('240', '1044', '5', '精确收集器碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('241', '1045', '5', '奥法徽记碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('242', '1046', '5', '圣洁祷告书碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('243', '1047', '5', '神秘火焰碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('244', '1048', '5', '暴躁的残魂碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('245', '1049', '5', '狂暴的风精灵碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('246', '1050', '5', '强效融合剂碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('247', '1051', '5', '圣地祷告台碎片', '5', '999', '2500', '', '');
INSERT INTO `sys_item` VALUES ('248', '2001', '6', '嬴政碎片', '5', '999', '3000', '5-1-1', '');
INSERT INTO `sys_item` VALUES ('249', '2002', '6', '养由基碎片', '5', '999', '3000', '5-2-1', '');
INSERT INTO `sys_item` VALUES ('250', '2003', '6', '唐玄奘碎片', '5', '999', '3000', '5-3-1', '');
INSERT INTO `sys_item` VALUES ('251', '2004', '6', '诸葛亮碎片', '5', '999', '3000', '5-4-1', '');
INSERT INTO `sys_item` VALUES ('252', '2005', '6', '吕布碎片', '5', '999', '3000', '5-5-1', '');
INSERT INTO `sys_item` VALUES ('253', '2006', '6', '项羽碎片', '5', '999', '3000', '5-6-1', '');
INSERT INTO `sys_item` VALUES ('254', '2007', '6', '岳飞碎片', '5', '999', '3000', '5-7-1', '');
INSERT INTO `sys_item` VALUES ('255', '2008', '6', '郑成功碎片', '5', '999', '3000', '5-8-1', '');
INSERT INTO `sys_item` VALUES ('256', '2009', '6', '袁天罡碎片', '5', '999', '3000', '5-9-1', '');
INSERT INTO `sys_item` VALUES ('257', '2010', '6', '华佗碎片', '5', '999', '3000', '5-10-1', '');
INSERT INTO `sys_item` VALUES ('258', '2011', '6', '安倍睛明碎片', '5', '999', '3000', '5-11-1', '');
INSERT INTO `sys_item` VALUES ('259', '2012', '6', '宫本武藏碎片', '5', '999', '3000', '5-12-1', '');
INSERT INTO `sys_item` VALUES ('260', '2013', '6', '织田信长碎片', '5', '999', '3000', '5-13-1', '');
INSERT INTO `sys_item` VALUES ('261', '2014', '6', '服部半藏碎片', '5', '999', '3000', '5-14-1', '');
INSERT INTO `sys_item` VALUES ('262', '2015', '6', '辉夜姬碎片', '5', '999', '3000', '5-15-1', '');
INSERT INTO `sys_item` VALUES ('263', '2016', '6', '丰成秀吉碎片', '5', '999', '3000', '5-16-1', '');
INSERT INTO `sys_item` VALUES ('264', '2017', '6', '浓姬碎片', '5', '999', '3000', '5-17-1', '');
INSERT INTO `sys_item` VALUES ('265', '2018', '6', '亚瑟王碎片', '5', '999', '3000', '5-18-1', '');
INSERT INTO `sys_item` VALUES ('266', '2025', '6', '拿破仑碎片', '5', '999', '3000', '5-25-1', '');
INSERT INTO `sys_item` VALUES ('267', '2026', '6', '一休碎片', '5', '999', '3000', '5-26-1', '');

-- ----------------------------
-- Table structure for sys_monster
-- ----------------------------
DROP TABLE IF EXISTS `sys_monster`;
CREATE TABLE `sys_monster` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `monster_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_monster
-- ----------------------------

-- ----------------------------
-- Table structure for sys_shop_set
-- ----------------------------
DROP TABLE IF EXISTS `sys_shop_set`;
CREATE TABLE `sys_shop_set` (
  `shop_id` tinyint(3) unsigned NOT NULL COMMENT '所属的商店',
  `set_id` int(10) unsigned NOT NULL COMMENT '元素集id',
  `min_num` tinyint(3) unsigned NOT NULL COMMENT '物品集出现下限',
  `max_num` tinyint(3) unsigned NOT NULL COMMENT '物品集出现上限',
  PRIMARY KEY (`shop_id`,`set_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_shop_set
-- ----------------------------

-- ----------------------------
-- Table structure for sys_shop_set_element
-- ----------------------------
DROP TABLE IF EXISTS `sys_shop_set_element`;
CREATE TABLE `sys_shop_set_element` (
  `set_id` int(10) unsigned NOT NULL COMMENT '所属物品集id',
  `element_type` smallint(5) unsigned NOT NULL COMMENT '元素类型 参考类型表',
  `element_sub_type` tinyint(4) unsigned NOT NULL COMMENT '元素子类型',
  `element_id` int(10) unsigned NOT NULL COMMENT '元素id',
  `element_num` int(10) unsigned NOT NULL COMMENT '数量',
  PRIMARY KEY (`set_id`,`element_type`,`element_sub_type`,`element_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_shop_set_element
-- ----------------------------

-- ----------------------------
-- Table structure for sys_shop_set_level_weight
-- ----------------------------
DROP TABLE IF EXISTS `sys_shop_set_level_weight`;
CREATE TABLE `sys_shop_set_level_weight` (
  `shop_id` int(10) unsigned NOT NULL DEFAULT '0',
  `set_id` int(10) unsigned NOT NULL DEFAULT '0',
  `level_max` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '( , level_max]  没有到80的时候报错',
  `weight` int(11) DEFAULT NULL,
  PRIMARY KEY (`shop_id`,`set_id`,`level_max`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_shop_set_level_weight
-- ----------------------------

-- ----------------------------
-- Table structure for sys_skill_level_gold
-- ----------------------------
DROP TABLE IF EXISTS `sys_skill_level_gold`;
CREATE TABLE `sys_skill_level_gold` (
  `level` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `bisha` int(10) unsigned NOT NULL,
  `aoyi` int(10) unsigned NOT NULL,
  `passive1` int(10) unsigned NOT NULL,
  `passive2` int(10) unsigned NOT NULL,
  `passive3` int(10) unsigned NOT NULL,
  PRIMARY KEY (`level`)
) ENGINE=InnoDB AUTO_INCREMENT=81 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_skill_level_gold
-- ----------------------------
INSERT INTO `sys_skill_level_gold` VALUES ('1', '12', '12', '12', '12', '12');
INSERT INTO `sys_skill_level_gold` VALUES ('2', '15', '15', '15', '15', '15');
INSERT INTO `sys_skill_level_gold` VALUES ('3', '15', '15', '15', '15', '15');
INSERT INTO `sys_skill_level_gold` VALUES ('4', '20', '20', '20', '20', '20');
INSERT INTO `sys_skill_level_gold` VALUES ('5', '25', '25', '25', '25', '25');
INSERT INTO `sys_skill_level_gold` VALUES ('6', '35', '35', '35', '35', '35');
INSERT INTO `sys_skill_level_gold` VALUES ('7', '45', '45', '45', '45', '45');
INSERT INTO `sys_skill_level_gold` VALUES ('8', '45', '45', '45', '45', '45');
INSERT INTO `sys_skill_level_gold` VALUES ('9', '50', '50', '50', '50', '50');
INSERT INTO `sys_skill_level_gold` VALUES ('10', '55', '55', '55', '55', '55');
INSERT INTO `sys_skill_level_gold` VALUES ('11', '65', '65', '65', '65', '65');
INSERT INTO `sys_skill_level_gold` VALUES ('12', '80', '80', '80', '80', '80');
INSERT INTO `sys_skill_level_gold` VALUES ('13', '85', '85', '85', '85', '85');
INSERT INTO `sys_skill_level_gold` VALUES ('14', '100', '100', '100', '100', '100');
INSERT INTO `sys_skill_level_gold` VALUES ('15', '110', '110', '110', '110', '110');
INSERT INTO `sys_skill_level_gold` VALUES ('16', '125', '125', '125', '125', '125');
INSERT INTO `sys_skill_level_gold` VALUES ('17', '135', '135', '135', '135', '135');
INSERT INTO `sys_skill_level_gold` VALUES ('18', '150', '150', '150', '150', '150');
INSERT INTO `sys_skill_level_gold` VALUES ('19', '170', '170', '170', '170', '170');
INSERT INTO `sys_skill_level_gold` VALUES ('20', '210', '210', '210', '210', '210');
INSERT INTO `sys_skill_level_gold` VALUES ('21', '250', '250', '250', '250', '250');
INSERT INTO `sys_skill_level_gold` VALUES ('22', '290', '290', '290', '290', '290');
INSERT INTO `sys_skill_level_gold` VALUES ('23', '335', '335', '335', '335', '335');
INSERT INTO `sys_skill_level_gold` VALUES ('24', '380', '380', '380', '380', '380');
INSERT INTO `sys_skill_level_gold` VALUES ('25', '495', '495', '495', '495', '495');
INSERT INTO `sys_skill_level_gold` VALUES ('26', '550', '550', '550', '550', '550');
INSERT INTO `sys_skill_level_gold` VALUES ('27', '685', '685', '685', '685', '685');
INSERT INTO `sys_skill_level_gold` VALUES ('28', '798', '798', '798', '798', '798');
INSERT INTO `sys_skill_level_gold` VALUES ('29', '905', '905', '905', '905', '905');
INSERT INTO `sys_skill_level_gold` VALUES ('30', '1000', '1000', '1000', '1000', '1000');
INSERT INTO `sys_skill_level_gold` VALUES ('31', '1115', '1115', '1115', '1115', '1115');
INSERT INTO `sys_skill_level_gold` VALUES ('32', '1180', '1180', '1180', '1180', '1180');
INSERT INTO `sys_skill_level_gold` VALUES ('33', '1255', '1255', '1255', '1255', '1255');
INSERT INTO `sys_skill_level_gold` VALUES ('34', '1330', '1330', '1330', '1330', '1330');
INSERT INTO `sys_skill_level_gold` VALUES ('35', '1405', '1405', '1405', '1405', '1405');
INSERT INTO `sys_skill_level_gold` VALUES ('36', '1490', '1490', '1490', '1490', '1490');
INSERT INTO `sys_skill_level_gold` VALUES ('37', '1580', '1580', '1580', '1580', '1580');
INSERT INTO `sys_skill_level_gold` VALUES ('38', '1675', '1675', '1675', '1675', '1675');
INSERT INTO `sys_skill_level_gold` VALUES ('39', '1775', '1775', '1775', '1775', '1775');
INSERT INTO `sys_skill_level_gold` VALUES ('40', '1880', '1880', '1880', '1880', '1880');
INSERT INTO `sys_skill_level_gold` VALUES ('41', '1995', '1995', '1995', '1995', '1995');
INSERT INTO `sys_skill_level_gold` VALUES ('42', '2115', '2115', '2115', '2115', '2115');
INSERT INTO `sys_skill_level_gold` VALUES ('43', '2240', '2240', '2240', '2240', '2240');
INSERT INTO `sys_skill_level_gold` VALUES ('44', '2370', '2370', '2370', '2370', '2370');
INSERT INTO `sys_skill_level_gold` VALUES ('45', '2515', '2515', '2515', '2515', '2515');
INSERT INTO `sys_skill_level_gold` VALUES ('46', '2665', '2665', '2665', '2665', '2665');
INSERT INTO `sys_skill_level_gold` VALUES ('47', '2825', '2825', '2825', '2825', '2825');
INSERT INTO `sys_skill_level_gold` VALUES ('48', '2990', '2990', '2990', '2990', '2990');
INSERT INTO `sys_skill_level_gold` VALUES ('49', '3170', '3170', '3170', '3170', '3170');
INSERT INTO `sys_skill_level_gold` VALUES ('50', '3360', '3360', '3360', '3360', '3360');
INSERT INTO `sys_skill_level_gold` VALUES ('51', '3405', '3405', '3405', '3405', '3405');
INSERT INTO `sys_skill_level_gold` VALUES ('52', '3455', '3455', '3455', '3455', '3455');
INSERT INTO `sys_skill_level_gold` VALUES ('53', '3520', '3520', '3520', '3520', '3520');
INSERT INTO `sys_skill_level_gold` VALUES ('54', '3625', '3625', '3625', '3625', '3625');
INSERT INTO `sys_skill_level_gold` VALUES ('55', '3730', '3730', '3730', '3730', '3730');
INSERT INTO `sys_skill_level_gold` VALUES ('56', '3835', '3835', '3835', '3835', '3835');
INSERT INTO `sys_skill_level_gold` VALUES ('57', '3945', '3945', '3945', '3945', '3945');
INSERT INTO `sys_skill_level_gold` VALUES ('58', '4055', '4055', '4055', '4055', '4055');
INSERT INTO `sys_skill_level_gold` VALUES ('59', '4165', '4165', '4165', '4165', '4165');
INSERT INTO `sys_skill_level_gold` VALUES ('60', '4265', '4265', '4265', '4265', '4265');
INSERT INTO `sys_skill_level_gold` VALUES ('61', '4375', '4375', '4375', '4375', '4375');
INSERT INTO `sys_skill_level_gold` VALUES ('62', '4545', '4545', '4545', '4545', '4545');
INSERT INTO `sys_skill_level_gold` VALUES ('63', '4715', '4715', '4715', '4715', '4715');
INSERT INTO `sys_skill_level_gold` VALUES ('64', '4885', '4885', '4885', '4885', '4885');
INSERT INTO `sys_skill_level_gold` VALUES ('65', '5005', '5005', '5005', '5005', '5005');
INSERT INTO `sys_skill_level_gold` VALUES ('66', '5195', '5195', '5195', '5195', '5195');
INSERT INTO `sys_skill_level_gold` VALUES ('67', '5325', '5325', '5325', '5325', '5325');
INSERT INTO `sys_skill_level_gold` VALUES ('68', '5535', '5535', '5535', '5535', '5535');
INSERT INTO `sys_skill_level_gold` VALUES ('69', '5765', '5765', '5765', '5765', '5765');
INSERT INTO `sys_skill_level_gold` VALUES ('70', '5985', '5985', '5985', '5985', '5985');
INSERT INTO `sys_skill_level_gold` VALUES ('71', '6195', '6195', '6195', '6195', '6195');
INSERT INTO `sys_skill_level_gold` VALUES ('72', '6405', '6405', '6405', '6405', '6405');
INSERT INTO `sys_skill_level_gold` VALUES ('73', '6705', '6705', '6705', '6705', '6705');
INSERT INTO `sys_skill_level_gold` VALUES ('74', '7025', '7025', '7025', '7025', '7025');
INSERT INTO `sys_skill_level_gold` VALUES ('75', '7335', '7335', '7335', '7335', '7335');
INSERT INTO `sys_skill_level_gold` VALUES ('76', '7645', '7645', '7645', '7645', '7645');
INSERT INTO `sys_skill_level_gold` VALUES ('77', '7975', '7975', '7975', '7975', '7975');
INSERT INTO `sys_skill_level_gold` VALUES ('78', '8305', '8305', '8305', '8305', '8305');
INSERT INTO `sys_skill_level_gold` VALUES ('79', '8655', '8655', '8655', '8655', '8655');
INSERT INTO `sys_skill_level_gold` VALUES ('80', '10000', '10000', '10000', '10000', '10000');

-- ----------------------------
-- Table structure for sys_skill_passive
-- ----------------------------
DROP TABLE IF EXISTS `sys_skill_passive`;
CREATE TABLE `sys_skill_passive` (
  `passive_skill_id` int(10) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`passive_skill_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_skill_passive
-- ----------------------------
INSERT INTO `sys_skill_passive` VALUES ('1', '测试技能1');
INSERT INTO `sys_skill_passive` VALUES ('2', '测试技能2');
INSERT INTO `sys_skill_passive` VALUES ('3', '测试技能3');

-- ----------------------------
-- Table structure for sys_sub_task
-- ----------------------------
DROP TABLE IF EXISTS `sys_sub_task`;
CREATE TABLE `sys_sub_task` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `sub_task_id` int(10) unsigned NOT NULL COMMENT '子任务ID，全局唯一',
  `task_id` int(10) unsigned NOT NULL COMMENT '所属父任务，可以没有。 比如 小点上没有父任务 所以这里为空',
  `priority` float unsigned NOT NULL COMMENT '优先级， 数字越大 任务越后',
  `spend_health` int(10) unsigned NOT NULL COMMENT '消耗体力值',
  `burst_packet_id` int(10) unsigned NOT NULL COMMENT '爆落方案ID',
  `auto_battle_burst_packet_id` int(10) unsigned NOT NULL COMMENT '扫荡爆落方案ID',
  `extra_packet_id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '额外爆落包',
  `battle_id_time_arr` varchar(511) NOT NULL COMMENT '所包含的每场战斗的id和时间限制（秒）： 形如 1-500|2-600',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=91 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_sub_task
-- ----------------------------
INSERT INTO `sys_sub_task` VALUES ('1', '1', '1', '1', '6', '1', '1', '0', '1-60|2-90');
INSERT INTO `sys_sub_task` VALUES ('2', '2', '2', '1', '6', '2', '2', '0', '3-60|4-90');
INSERT INTO `sys_sub_task` VALUES ('3', '3', '2', '2', '6', '3', '3', '0', '5-60|6-90');
INSERT INTO `sys_sub_task` VALUES ('4', '4', '3', '1', '6', '4', '4', '0', '7-60|8-90|9-90');
INSERT INTO `sys_sub_task` VALUES ('5', '5', '3', '2', '6', '5', '5', '0', '10-60|11-90');
INSERT INTO `sys_sub_task` VALUES ('6', '6', '4', '1', '6', '1', '1', '0', '12-60|13-90|14-60');
INSERT INTO `sys_sub_task` VALUES ('7', '7', '5', '1', '6', '2', '2', '0', '15-60|16-90');
INSERT INTO `sys_sub_task` VALUES ('8', '8', '6', '1', '6', '3', '3', '0', '17-60|18-90');
INSERT INTO `sys_sub_task` VALUES ('9', '9', '7', '2', '6', '4', '4', '0', '19-60|20-90|21-60');
INSERT INTO `sys_sub_task` VALUES ('10', '10', '8', '1', '6', '5', '5', '0', '22-60|23-90|24-60');
INSERT INTO `sys_sub_task` VALUES ('11', '11', '8', '1', '6', '1', '1', '0', '25-60|26-90');
INSERT INTO `sys_sub_task` VALUES ('12', '12', '9', '2', '6', '2', '2', '0', '27-60|28-90');
INSERT INTO `sys_sub_task` VALUES ('13', '13', '9', '1', '6', '3', '3', '0', '29-60|30-90');
INSERT INTO `sys_sub_task` VALUES ('14', '14', '10', '2', '6', '4', '4', '0', '31-60|32-90|33-90');
INSERT INTO `sys_sub_task` VALUES ('15', '15', '11', '1', '6', '5', '5', '0', '34-60|35-90');
INSERT INTO `sys_sub_task` VALUES ('16', '16', '12', '2', '6', '1', '1', '0', '36-60|37-90|38-60');
INSERT INTO `sys_sub_task` VALUES ('17', '17', '13', '1', '6', '2', '2', '0', '39-60|40-90');
INSERT INTO `sys_sub_task` VALUES ('18', '18', '14', '2', '6', '3', '3', '0', '41-60|42-90');
INSERT INTO `sys_sub_task` VALUES ('19', '19', '14', '1', '6', '4', '4', '0', '43-60|44-90|45-60');
INSERT INTO `sys_sub_task` VALUES ('20', '20', '15', '1', '6', '5', '5', '0', '46-60|47-90|48-60');
INSERT INTO `sys_sub_task` VALUES ('21', '21', '15', '2', '6', '1', '1', '0', '49-60|50-90');
INSERT INTO `sys_sub_task` VALUES ('22', '22', '16', '1', '6', '2', '2', '0', '51-60|52-90');
INSERT INTO `sys_sub_task` VALUES ('23', '23', '17', '2', '6', '3', '3', '0', '53-60|54-90');
INSERT INTO `sys_sub_task` VALUES ('24', '24', '17', '1', '6', '4', '4', '0', '55-60|56-90|57-90');
INSERT INTO `sys_sub_task` VALUES ('25', '25', '18', '1', '6', '5', '5', '0', '58-60|59-90');
INSERT INTO `sys_sub_task` VALUES ('26', '26', '19', '2', '6', '1', '1', '0', '60-60|61-90|62-60');
INSERT INTO `sys_sub_task` VALUES ('27', '27', '20', '1', '6', '2', '2', '0', '63-60|64-90');
INSERT INTO `sys_sub_task` VALUES ('28', '28', '21', '2', '6', '3', '3', '0', '65-60|66-90');
INSERT INTO `sys_sub_task` VALUES ('29', '29', '21', '1', '6', '4', '4', '0', '67-60|68-90|69-60');
INSERT INTO `sys_sub_task` VALUES ('30', '30', '22', '1', '6', '5', '5', '0', '70-60|71-90|72-60');
INSERT INTO `sys_sub_task` VALUES ('31', '31', '23', '1', '6', '1', '1', '0', '73-60|74-90');
INSERT INTO `sys_sub_task` VALUES ('32', '32', '24', '2', '6', '2', '2', '0', '75-60|76-90');
INSERT INTO `sys_sub_task` VALUES ('33', '33', '25', '1', '6', '3', '3', '0', '77-60|78-90');
INSERT INTO `sys_sub_task` VALUES ('34', '34', '26', '1', '6', '4', '4', '0', '79-60|80-90|81-90');
INSERT INTO `sys_sub_task` VALUES ('35', '35', '27', '2', '6', '5', '5', '0', '82-60|83-90');
INSERT INTO `sys_sub_task` VALUES ('36', '36', '27', '1', '6', '1', '1', '0', '84-60|85-90|86-60');
INSERT INTO `sys_sub_task` VALUES ('37', '37', '28', '2', '6', '2', '2', '0', '87-60|88-90');
INSERT INTO `sys_sub_task` VALUES ('38', '38', '29', '1', '6', '3', '3', '0', '89-60|90-90');
INSERT INTO `sys_sub_task` VALUES ('39', '39', '30', '1', '6', '4', '4', '0', '91-60|92-90|93-60');
INSERT INTO `sys_sub_task` VALUES ('40', '40', '31', '1', '6', '5', '5', '0', '94-60|95-90|96-60');
INSERT INTO `sys_sub_task` VALUES ('41', '41', '32', '2', '6', '1', '1', '0', '97-60|98-90');
INSERT INTO `sys_sub_task` VALUES ('42', '42', '32', '1', '6', '2', '2', '0', '99-60|100-90');
INSERT INTO `sys_sub_task` VALUES ('43', '43', '33', '1', '6', '3', '3', '0', '101-60|102-90');
INSERT INTO `sys_sub_task` VALUES ('44', '44', '33', '2', '6', '4', '4', '0', '103-60|104-90|105-90');
INSERT INTO `sys_sub_task` VALUES ('45', '45', '34', '1', '6', '5', '5', '0', '106-60|107-90');
INSERT INTO `sys_sub_task` VALUES ('46', '46', '35', '2', '6', '1', '1', '0', '108-60|109-90|110-60');
INSERT INTO `sys_sub_task` VALUES ('47', '47', '36', '1', '6', '2', '2', '0', '111-60|112-90');
INSERT INTO `sys_sub_task` VALUES ('48', '48', '36', '1', '6', '3', '3', '0', '113-60|114-90');
INSERT INTO `sys_sub_task` VALUES ('49', '49', '37', '2', '6', '4', '4', '0', '115-60|116-90|117-60');
INSERT INTO `sys_sub_task` VALUES ('50', '50', '38', '1', '6', '5', '5', '0', '118-60|119-90|120-60');
INSERT INTO `sys_sub_task` VALUES ('51', '51', '38', '2', '6', '1', '1', '0', '121-60|122-90');
INSERT INTO `sys_sub_task` VALUES ('52', '52', '39', '1', '6', '2', '2', '0', '123-60|124-90');
INSERT INTO `sys_sub_task` VALUES ('53', '53', '39', '1', '6', '3', '3', '0', '125-60|126-90');
INSERT INTO `sys_sub_task` VALUES ('54', '54', '40', '1', '6', '4', '4', '0', '127-60|128-90|129-90');
INSERT INTO `sys_sub_task` VALUES ('55', '55', '41', '2', '6', '5', '5', '0', '130-60|131-90');
INSERT INTO `sys_sub_task` VALUES ('56', '56', '42', '1', '6', '1', '1', '0', '132-60|133-90|134-60');
INSERT INTO `sys_sub_task` VALUES ('57', '57', '42', '1', '6', '2', '2', '0', '135-60|136-90');
INSERT INTO `sys_sub_task` VALUES ('58', '58', '43', '2', '6', '3', '3', '0', '137-60|138-90');
INSERT INTO `sys_sub_task` VALUES ('59', '59', '44', '1', '6', '4', '4', '0', '139-60|140-90|141-60');
INSERT INTO `sys_sub_task` VALUES ('60', '60', '44', '2', '6', '5', '5', '0', '142-60|143-90|144-60');
INSERT INTO `sys_sub_task` VALUES ('61', '61', '45', '1', '6', '1', '1', '0', '145-60|146-90');
INSERT INTO `sys_sub_task` VALUES ('62', '62', '46', '1', '6', '2', '2', '0', '147-60|148-90');
INSERT INTO `sys_sub_task` VALUES ('63', '63', '47', '1', '6', '3', '3', '0', '149-60|150-90');
INSERT INTO `sys_sub_task` VALUES ('64', '64', '47', '2', '6', '4', '4', '0', '151-60|152-90|153-90');
INSERT INTO `sys_sub_task` VALUES ('65', '65', '48', '1', '6', '5', '5', '0', '154-60|155-90');
INSERT INTO `sys_sub_task` VALUES ('66', '66', '48', '1', '6', '1', '1', '0', '156-60|157-90|158-60');
INSERT INTO `sys_sub_task` VALUES ('67', '67', '49', '2', '6', '2', '2', '0', '159-60|160-90');
INSERT INTO `sys_sub_task` VALUES ('68', '68', '50', '1', '6', '3', '3', '0', '161-60|162-90');
INSERT INTO `sys_sub_task` VALUES ('69', '69', '51', '2', '6', '4', '4', '0', '163-60|164-90|165-60');
INSERT INTO `sys_sub_task` VALUES ('70', '70', '52', '1', '6', '5', '5', '0', '166-60|167-90|168-60');
INSERT INTO `sys_sub_task` VALUES ('71', '71', '53', '1', '6', '1', '1', '0', '169-60|170-90');
INSERT INTO `sys_sub_task` VALUES ('72', '72', '53', '2', '6', '2', '2', '0', '171-60|172-90');
INSERT INTO `sys_sub_task` VALUES ('73', '73', '54', '1', '6', '3', '3', '0', '173-60|174-90');
INSERT INTO `sys_sub_task` VALUES ('74', '74', '54', '2', '6', '4', '4', '0', '175-60|176-90|177-90');
INSERT INTO `sys_sub_task` VALUES ('75', '75', '55', '1', '6', '5', '5', '0', '178-60|179-90');
INSERT INTO `sys_sub_task` VALUES ('76', '76', '56', '1', '6', '1', '1', '0', '180-60|181-90|182-60');
INSERT INTO `sys_sub_task` VALUES ('77', '77', '57', '1', '6', '2', '2', '0', '183-60|184-90');
INSERT INTO `sys_sub_task` VALUES ('78', '78', '58', '2', '6', '3', '3', '0', '185-60|186-90');
INSERT INTO `sys_sub_task` VALUES ('79', '79', '59', '1', '6', '4', '4', '0', '187-60|188-90|189-60');
INSERT INTO `sys_sub_task` VALUES ('80', '80', '59', '1', '6', '5', '5', '0', '190-60|191-90|192-60');
INSERT INTO `sys_sub_task` VALUES ('81', '81', '60', '2', '6', '1', '1', '0', '193-60|194-90');
INSERT INTO `sys_sub_task` VALUES ('82', '82', '60', '1', '6', '2', '2', '0', '195-60|196-90');
INSERT INTO `sys_sub_task` VALUES ('83', '83', '61', '2', '6', '3', '3', '0', '197-60|198-90');
INSERT INTO `sys_sub_task` VALUES ('84', '84', '62', '1', '6', '4', '4', '0', '199-60|200-90|201-90');
INSERT INTO `sys_sub_task` VALUES ('85', '85', '63', '1', '6', '5', '5', '0', '202-60|203-90');
INSERT INTO `sys_sub_task` VALUES ('86', '86', '63', '1', '6', '1', '1', '0', '204-60|205-90|206-60');
INSERT INTO `sys_sub_task` VALUES ('87', '87', '64', '2', '6', '2', '2', '0', '207-60|208-90');
INSERT INTO `sys_sub_task` VALUES ('88', '88', '65', '1', '6', '3', '3', '0', '209-60|210-90');
INSERT INTO `sys_sub_task` VALUES ('89', '89', '66', '1', '6', '4', '4', '0', '211-60|212-90|213-60');
INSERT INTO `sys_sub_task` VALUES ('90', '90', '66', '2', '6', '5', '5', '0', '214-60|215-90|216-60');

-- ----------------------------
-- Table structure for sys_synthetic_formula
-- ----------------------------
DROP TABLE IF EXISTS `sys_synthetic_formula`;
CREATE TABLE `sys_synthetic_formula` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `synthetic_formula_id` int(10) unsigned NOT NULL,
  `target_element_type` tinyint(3) unsigned NOT NULL COMMENT '合成物品类型 英雄  道具 装备 等等',
  `target_element_id` int(10) unsigned NOT NULL COMMENT '合成元素id',
  `src_element_type_id_num_arr` varchar(1023) NOT NULL COMMENT '元素类型-id-数量 如：1-234-1|1-555-5',
  PRIMARY KEY (`id`),
  UNIQUE KEY `synthetic_formula_id` (`synthetic_formula_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_synthetic_formula
-- ----------------------------

-- ----------------------------
-- Table structure for sys_task
-- ----------------------------
DROP TABLE IF EXISTS `sys_task`;
CREATE TABLE `sys_task` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `task_id` int(10) unsigned NOT NULL COMMENT '任务id，全局唯一',
  `type` tinyint(2) unsigned NOT NULL COMMENT '类型 1.主线 2.支线 3.活动',
  `name` varchar(63) NOT NULL COMMENT '名字',
  `desc` varchar(255) DEFAULT NULL COMMENT '描述',
  `mappoint_id` int(10) unsigned DEFAULT NULL COMMENT '任务所属的据点，有的有，有的没有',
  `priority` float unsigned DEFAULT NULL COMMENT '任务优先级， 值越小越在前。控制 主线任务的顺序，支线任务的显示顺序 如:某城市中有多个任务；活动任务列表 ',
  `has_enter_condition` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否有进入限制 0没有 1有； 假如有的话，这个任务的进入条件写在./task/enter_condition/id.xml中  比如一天或者一周的次数限制  等级 职业等等限制',
  `prize_element_type_id_num_arr` varchar(1023) DEFAULT NULL COMMENT '所有子任务完成后的奖励物品类型-id-数量',
  `unlock_mappoint_id_arr` varchar(63) NOT NULL DEFAULT '' COMMENT '任务完成时解锁的据点ID',
  `background_img` varchar(63) NOT NULL DEFAULT '',
  `hidden` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '是否隐藏 0否 1是',
  PRIMARY KEY (`id`),
  UNIQUE KEY `instance_id` (`task_id`),
  KEY `type` (`type`),
  KEY `mappoint_id` (`mappoint_id`)
) ENGINE=InnoDB AUTO_INCREMENT=67 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_task
-- ----------------------------
INSERT INTO `sys_task` VALUES ('1', '1', '1', '逐鹿', '', '1', '1', '0', '11-0-10000', '2', '', '0');
INSERT INTO `sys_task` VALUES ('2', '2', '1', '心结', '', '2', '2', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('3', '3', '1', '钜鹿终战', '', '2', '3', '0', '11-0-10000', '3', '', '0');
INSERT INTO `sys_task` VALUES ('4', '4', '2', '破釜沉舟', '', '2', '0', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('5', '5', '2', '鸿门宴', '', '2', '1', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('6', '6', '1', '平原', '', '3', '4', '0', '11-0-10000', '4', '', '0');
INSERT INTO `sys_task` VALUES ('7', '7', '1', '北海', '', '4', '5', '0', '11-0-10000', '5', '', '0');
INSERT INTO `sys_task` VALUES ('8', '8', '1', '反间计', '', '5', '6', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('9', '9', '1', '徐州攻防战', '', '5', '7', '0', '11-0-10000', '6', '', '0');
INSERT INTO `sys_task` VALUES ('10', '10', '2', '辕门射戟', '', '5', '0', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('11', '11', '2', '群英战吕布', '', '5', '1', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('12', '12', '1', '小沛', '', '6', '8', '0', '11-0-10000', '7', '', '0');
INSERT INTO `sys_task` VALUES ('13', '13', '1', '许昌', '', '7', '9', '0', '11-0-10000', '8', '', '0');
INSERT INTO `sys_task` VALUES ('14', '14', '1', '迷失的大军', '', '8', '10', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('15', '15', '1', '净化冤魂', '', '8', '11', '0', '11-0-10000', '9', '', '0');
INSERT INTO `sys_task` VALUES ('16', '16', '2', '精忠报国', '', '8', '0', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('17', '17', '2', '大战朱仙镇', '', '8', '1', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('18', '18', '1', '函谷关', '', '9', '12', '0', '11-0-10000', '10', '', '0');
INSERT INTO `sys_task` VALUES ('19', '19', '1', '潼关', '', '10', '13', '0', '11-0-10000', '11', '', '0');
INSERT INTO `sys_task` VALUES ('20', '20', '1', '邪教的末日', '', '11', '14', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('21', '21', '1', '解放长安', '', '11', '15', '0', '11-0-10000', '12', '', '0');
INSERT INTO `sys_task` VALUES ('22', '22', '2', '西行之路', '', '11', '0', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('23', '23', '1', '渭南', '', '12', '16', '0', '11-0-10000', '13', '', '0');
INSERT INTO `sys_task` VALUES ('24', '24', '1', '泾阳', '', '13', '17', '0', '11-0-10000', '14', '', '0');
INSERT INTO `sys_task` VALUES ('25', '25', '1', '覆灭的阿房宫', '', '14', '18', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('26', '26', '1', '湮灭心魔', '', '14', '19', '0', '11-0-10000', '15', '', '0');
INSERT INTO `sys_task` VALUES ('27', '27', '2', '倒长城之战', '', '14', '0', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('28', '28', '2', '刺秦', '', '14', '1', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('29', '29', '1', '汉中', '', '15', '20', '0', '11-0-10000', '16', '', '0');
INSERT INTO `sys_task` VALUES ('30', '30', '1', '剑阁', '', '16', '21', '0', '11-0-10000', '17', '', '0');
INSERT INTO `sys_task` VALUES ('31', '31', '1', '失败的北伐军', '', '17', '22', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('32', '32', '1', '消逝的执念', '', '17', '23', '0', '11-0-10000', '18', '', '0');
INSERT INTO `sys_task` VALUES ('33', '33', '2', '神话的破灭', '', '17', '0', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('34', '34', '1', '昆仑关', '', '18', '24', '0', '11-0-10000', '19', '', '0');
INSERT INTO `sys_task` VALUES ('35', '35', '1', '神农架', '', '19', '25', '0', '11-0-10000', '20', '', '0');
INSERT INTO `sys_task` VALUES ('36', '36', '1', '实力的比拼', '', '20', '26', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('37', '37', '1', '神射之王的末日', '', '20', '27', '0', '11-0-10000', '21', '', '0');
INSERT INTO `sys_task` VALUES ('38', '38', '2', '百步穿杨', '', '20', '0', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('39', '39', '2', '三箭定天下', '', '20', '1', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('40', '40', '1', '乌林', '', '21', '28', '0', '11-0-10000', '22', '', '0');
INSERT INTO `sys_task` VALUES ('41', '41', '1', '洞庭', '', '22', '29', '0', '11-0-10000', '23', '', '0');
INSERT INTO `sys_task` VALUES ('42', '42', '1', '医者毒心', '', '23', '30', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('43', '43', '1', '惩戒华佗', '', '23', '31', '0', '11-0-10000', '24', '', '0');
INSERT INTO `sys_task` VALUES ('44', '44', '2', '五禽戏', '', '23', '0', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('45', '45', '1', '赤壁', '', '24', '32', '0', '11-0-10000', '25', '', '0');
INSERT INTO `sys_task` VALUES ('46', '46', '1', '庐陵', '', '25', '33', '0', '11-0-10000', '26', '', '0');
INSERT INTO `sys_task` VALUES ('47', '47', '1', '神奇的占卜', '', '26', '34', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('48', '48', '1', '迷信的后果', '', '26', '35', '0', '11-0-10000', '27', '', '0');
INSERT INTO `sys_task` VALUES ('49', '49', '2', '龙王的报复', '', '26', '0', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('50', '50', '1', '钱塘', '', '27', '36', '0', '11-0-10000', '28', '', '0');
INSERT INTO `sys_task` VALUES ('51', '51', '1', '澎湖', '', '28', '37', '0', '11-0-10000', '29', '', '0');
INSERT INTO `sys_task` VALUES ('52', '52', '1', '上岸水兵不如菜', '', '29', '38', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('53', '53', '1', '远方的来客', '', '29', '39', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('54', '54', '1', '攻占台湾', '', '29', '40', '0', '11-0-10000', '30', '', '0');
INSERT INTO `sys_task` VALUES ('55', '55', '2', '宝岛守卫战', '', '29', '0', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('56', '56', '1', '钓鱼岛', '', '30', '41', '0', '11-0-10000', '31', '', '0');
INSERT INTO `sys_task` VALUES ('57', '57', '1', '蓬莱', '', '31', '42', '0', '11-0-10000', '32', '', '0');
INSERT INTO `sys_task` VALUES ('58', '58', '1', '远征', '', '32', '43', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('59', '59', '1', '强势征服', '', '32', '44', '0', '11-0-10000', '33', '', '0');
INSERT INTO `sys_task` VALUES ('60', '60', '2', '半岛攻势', '', '32', '0', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('61', '61', '1', '辽东', '', '33', '45', '0', '11-0-10000', '34', '', '0');
INSERT INTO `sys_task` VALUES ('62', '62', '1', '山海关', '', '34', '46', '0', '11-0-10000', '35', '', '0');
INSERT INTO `sys_task` VALUES ('63', '63', '1', '秘密潜入', '', '35', '47', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('64', '64', '1', '强势的突击', '', '35', '48', '0', '11-0-10000', '', '', '0');
INSERT INTO `sys_task` VALUES ('65', '65', '1', '一战定乾坤', '', '35', '49', '0', '11-0-10000', '36', '', '0');
INSERT INTO `sys_task` VALUES ('66', '66', '2', '问鼎天下', '', '35', '0', '0', '11-0-10000', '', '', '0');

-- ----------------------------
-- Table structure for sys_user_level
-- ----------------------------
DROP TABLE IF EXISTS `sys_user_level`;
CREATE TABLE `sys_user_level` (
  `level` int(10) unsigned NOT NULL COMMENT '当前等级',
  `exp_for_next_level` int(10) unsigned NOT NULL COMMENT '下一级所需要的经验',
  PRIMARY KEY (`level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of sys_user_level
-- ----------------------------
INSERT INTO `sys_user_level` VALUES ('1', '12');
INSERT INTO `sys_user_level` VALUES ('2', '15');
INSERT INTO `sys_user_level` VALUES ('3', '15');
INSERT INTO `sys_user_level` VALUES ('4', '20');
INSERT INTO `sys_user_level` VALUES ('5', '25');
INSERT INTO `sys_user_level` VALUES ('6', '35');
INSERT INTO `sys_user_level` VALUES ('7', '45');
INSERT INTO `sys_user_level` VALUES ('8', '45');
INSERT INTO `sys_user_level` VALUES ('9', '50');
INSERT INTO `sys_user_level` VALUES ('10', '55');
INSERT INTO `sys_user_level` VALUES ('11', '65');
INSERT INTO `sys_user_level` VALUES ('12', '80');
INSERT INTO `sys_user_level` VALUES ('13', '85');
INSERT INTO `sys_user_level` VALUES ('14', '100');
INSERT INTO `sys_user_level` VALUES ('15', '110');
INSERT INTO `sys_user_level` VALUES ('16', '125');
INSERT INTO `sys_user_level` VALUES ('17', '135');
INSERT INTO `sys_user_level` VALUES ('18', '150');
INSERT INTO `sys_user_level` VALUES ('19', '170');
INSERT INTO `sys_user_level` VALUES ('20', '210');
INSERT INTO `sys_user_level` VALUES ('21', '250');
INSERT INTO `sys_user_level` VALUES ('22', '290');
INSERT INTO `sys_user_level` VALUES ('23', '335');
INSERT INTO `sys_user_level` VALUES ('24', '380');
INSERT INTO `sys_user_level` VALUES ('25', '495');
INSERT INTO `sys_user_level` VALUES ('26', '550');
INSERT INTO `sys_user_level` VALUES ('27', '685');
INSERT INTO `sys_user_level` VALUES ('28', '798');
INSERT INTO `sys_user_level` VALUES ('29', '905');
INSERT INTO `sys_user_level` VALUES ('30', '1000');
INSERT INTO `sys_user_level` VALUES ('31', '1115');
INSERT INTO `sys_user_level` VALUES ('32', '1180');
INSERT INTO `sys_user_level` VALUES ('33', '1255');
INSERT INTO `sys_user_level` VALUES ('34', '1330');
INSERT INTO `sys_user_level` VALUES ('35', '1405');
INSERT INTO `sys_user_level` VALUES ('36', '1490');
INSERT INTO `sys_user_level` VALUES ('37', '1580');
INSERT INTO `sys_user_level` VALUES ('38', '1675');
INSERT INTO `sys_user_level` VALUES ('39', '1775');
INSERT INTO `sys_user_level` VALUES ('40', '1880');
INSERT INTO `sys_user_level` VALUES ('41', '1995');
INSERT INTO `sys_user_level` VALUES ('42', '2115');
INSERT INTO `sys_user_level` VALUES ('43', '2240');
INSERT INTO `sys_user_level` VALUES ('44', '2370');
INSERT INTO `sys_user_level` VALUES ('45', '2515');
INSERT INTO `sys_user_level` VALUES ('46', '2665');
INSERT INTO `sys_user_level` VALUES ('47', '2825');
INSERT INTO `sys_user_level` VALUES ('48', '2990');
INSERT INTO `sys_user_level` VALUES ('49', '3170');
INSERT INTO `sys_user_level` VALUES ('50', '3360');
INSERT INTO `sys_user_level` VALUES ('51', '3405');
INSERT INTO `sys_user_level` VALUES ('52', '3455');
INSERT INTO `sys_user_level` VALUES ('53', '3520');
INSERT INTO `sys_user_level` VALUES ('54', '3625');
INSERT INTO `sys_user_level` VALUES ('55', '3730');
INSERT INTO `sys_user_level` VALUES ('56', '3835');
INSERT INTO `sys_user_level` VALUES ('57', '3945');
INSERT INTO `sys_user_level` VALUES ('58', '4055');
INSERT INTO `sys_user_level` VALUES ('59', '4165');
INSERT INTO `sys_user_level` VALUES ('60', '4265');
INSERT INTO `sys_user_level` VALUES ('61', '4375');
INSERT INTO `sys_user_level` VALUES ('62', '4545');
INSERT INTO `sys_user_level` VALUES ('63', '4715');
INSERT INTO `sys_user_level` VALUES ('64', '4885');
INSERT INTO `sys_user_level` VALUES ('65', '5005');
INSERT INTO `sys_user_level` VALUES ('66', '5195');
INSERT INTO `sys_user_level` VALUES ('67', '5325');
INSERT INTO `sys_user_level` VALUES ('68', '5535');
INSERT INTO `sys_user_level` VALUES ('69', '5765');
INSERT INTO `sys_user_level` VALUES ('70', '5985');
INSERT INTO `sys_user_level` VALUES ('71', '6195');
INSERT INTO `sys_user_level` VALUES ('72', '6405');
INSERT INTO `sys_user_level` VALUES ('73', '6705');
INSERT INTO `sys_user_level` VALUES ('74', '7025');
INSERT INTO `sys_user_level` VALUES ('75', '7335');
INSERT INTO `sys_user_level` VALUES ('76', '7645');
INSERT INTO `sys_user_level` VALUES ('77', '7975');
INSERT INTO `sys_user_level` VALUES ('78', '8305');
INSERT INTO `sys_user_level` VALUES ('79', '8655');
INSERT INTO `sys_user_level` VALUES ('80', '10000');

-- ----------------------------
-- Table structure for user
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
  `health_last_update_timestamp` int(10) unsigned NOT NULL COMMENT '体力数据上次保存的时间',
  `skill_point` int(10) unsigned NOT NULL,
  `skill_point_update_timestamp` int(10) unsigned NOT NULL,
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES ('5', 'world hero', '80', '0', '0', '10000', '10000', '54', '1433379945', '0', '1433327541');
INSERT INTO `user` VALUES ('6', 'world hero', '75', '0', '0', '10000', '2907455', '89', '1433856349', '0', '1433391098');
INSERT INTO `user` VALUES ('7', 'world hero', '1', '1000', '0', '10000', '8822485', '0', '1434440375', '0', '1433898913');

-- ----------------------------
-- Table structure for user_all_server_mail
-- ----------------------------
DROP TABLE IF EXISTS `user_all_server_mail`;
CREATE TABLE `user_all_server_mail` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `title` varchar(50) NOT NULL,
  `content` varchar(1000) NOT NULL,
  `send_time` datetime NOT NULL,
  `type` tinyint(1) NOT NULL COMMENT '领取类和非领取类',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_all_server_mail
-- ----------------------------
INSERT INTO `user_all_server_mail` VALUES ('1', 'zjjjj', '1sssss', '2015-05-26 20:16:58', '0');
INSERT INTO `user_all_server_mail` VALUES ('2', 's', 'ss', '2015-05-26 20:51:47', '0');
INSERT INTO `user_all_server_mail` VALUES ('3', '123', '456', '2015-06-14 20:33:37', '1');
INSERT INTO `user_all_server_mail` VALUES ('4', '123', '456', '2015-06-14 20:35:15', '1');
INSERT INTO `user_all_server_mail` VALUES ('5', '123', '456', '2015-06-14 20:35:31', '1');
INSERT INTO `user_all_server_mail` VALUES ('6', '123', '456', '2015-06-14 20:41:33', '1');
INSERT INTO `user_all_server_mail` VALUES ('7', '123', '456', '2015-06-14 20:41:51', '1');
INSERT INTO `user_all_server_mail` VALUES ('8', '123', '456', '2015-06-14 20:41:52', '1');
INSERT INTO `user_all_server_mail` VALUES ('9', '123', '456', '2015-06-14 20:41:52', '1');
INSERT INTO `user_all_server_mail` VALUES ('10', '123', '456', '2015-06-14 20:42:47', '1');
INSERT INTO `user_all_server_mail` VALUES ('11', '123', '456', '2015-06-14 20:43:24', '1');
INSERT INTO `user_all_server_mail` VALUES ('12', '123', '456', '2015-06-14 20:43:25', '1');
INSERT INTO `user_all_server_mail` VALUES ('13', '123', '456', '2015-06-14 20:43:39', '1');
INSERT INTO `user_all_server_mail` VALUES ('14', '123', '456', '2015-06-14 20:53:03', '1');
INSERT INTO `user_all_server_mail` VALUES ('15', '123', '456', '2015-06-14 20:58:46', '1');

-- ----------------------------
-- Table structure for user_all_server_mail_attach
-- ----------------------------
DROP TABLE IF EXISTS `user_all_server_mail_attach`;
CREATE TABLE `user_all_server_mail_attach` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `type` int(10) NOT NULL COMMENT '元素类型(金币,道具，经验等)',
  `element_id` int(10) NOT NULL,
  `num` int(10) NOT NULL,
  `mail_id` int(10) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_all_server_mail_attach
-- ----------------------------
INSERT INTO `user_all_server_mail_attach` VALUES ('1', '1', '1', '1', '4');
INSERT INTO `user_all_server_mail_attach` VALUES ('2', '1', '1', '1', '5');
INSERT INTO `user_all_server_mail_attach` VALUES ('3', '1', '1', '1', '6');
INSERT INTO `user_all_server_mail_attach` VALUES ('4', '1', '1', '1', '7');
INSERT INTO `user_all_server_mail_attach` VALUES ('5', '1', '1', '1', '8');
INSERT INTO `user_all_server_mail_attach` VALUES ('6', '1', '1', '1', '9');
INSERT INTO `user_all_server_mail_attach` VALUES ('7', '1', '1', '1', '10');
INSERT INTO `user_all_server_mail_attach` VALUES ('8', '1', '1', '1', '11');
INSERT INTO `user_all_server_mail_attach` VALUES ('9', '1', '1', '1', '8');
INSERT INTO `user_all_server_mail_attach` VALUES ('10', '1', '1', '1', '12');
INSERT INTO `user_all_server_mail_attach` VALUES ('11', '1', '1', '1', '10');
INSERT INTO `user_all_server_mail_attach` VALUES ('12', '1', '1', '1', '13');
INSERT INTO `user_all_server_mail_attach` VALUES ('13', '1', '1', '1', '12');
INSERT INTO `user_all_server_mail_attach` VALUES ('14', '1', '1', '1', '0');
INSERT INTO `user_all_server_mail_attach` VALUES ('15', '1', '1', '1', '0');

-- ----------------------------
-- Table structure for user_bin
-- ----------------------------
DROP TABLE IF EXISTS `user_bin`;
CREATE TABLE `user_bin` (
  `user_id` int(10) unsigned NOT NULL COMMENT '用户id',
  `bag_bin` blob,
  `hero_bin` blob,
  `team_bin` blob,
  `task_bin` blob,
  `friend_bin` blob,
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_bin
-- ----------------------------
INSERT INTO `user_bin` VALUES ('1', 0x0A090800100120EC022801, null, null, 0x0A0408161003, null);

-- ----------------------------
-- Table structure for user_hero
-- ----------------------------
DROP TABLE IF EXISTS `user_hero`;
CREATE TABLE `user_hero` (
  `user_id` int(10) unsigned NOT NULL COMMENT '用户id',
  `hero_id` int(10) unsigned NOT NULL,
  `star` tinyint(3) unsigned NOT NULL COMMENT '星级 可能升星过',
  `quality` tinyint(3) unsigned NOT NULL,
  `level` smallint(5) unsigned NOT NULL,
  `cur_exp` int(10) unsigned NOT NULL,
  `got_timestamp` int(10) unsigned NOT NULL COMMENT '获取到的时间',
  PRIMARY KEY (`user_id`,`hero_id`),
  KEY `user_id` (`user_id`),
  KEY `hero_id` (`hero_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_hero
-- ----------------------------
INSERT INTO `user_hero` VALUES ('5', '1', '1', '1', '1', '0', '1433327541');
INSERT INTO `user_hero` VALUES ('5', '2', '1', '1', '1', '0', '1433327541');
INSERT INTO `user_hero` VALUES ('5', '3', '1', '1', '1', '0', '1433327541');
INSERT INTO `user_hero` VALUES ('5', '4', '1', '1', '1', '0', '1433327541');
INSERT INTO `user_hero` VALUES ('5', '5', '1', '1', '1', '0', '1433330301');
INSERT INTO `user_hero` VALUES ('5', '6', '1', '1', '1', '0', '1433330301');
INSERT INTO `user_hero` VALUES ('5', '7', '1', '1', '1', '0', '1433330301');
INSERT INTO `user_hero` VALUES ('5', '8', '1', '1', '1', '0', '1433330301');
INSERT INTO `user_hero` VALUES ('6', '13', '1', '1', '1', '0', '1433404257');
INSERT INTO `user_hero` VALUES ('6', '18', '1', '1', '1', '0', '1433391098');
INSERT INTO `user_hero` VALUES ('7', '17', '1', '1', '1', '0', '1433898913');

-- ----------------------------
-- Table structure for user_hero_equip
-- ----------------------------
DROP TABLE IF EXISTS `user_hero_equip`;
CREATE TABLE `user_hero_equip` (
  `user_id` int(10) unsigned NOT NULL COMMENT '用户id',
  `hero_id` int(10) unsigned NOT NULL,
  `equip_id` int(10) unsigned NOT NULL,
  `level` smallint(10) unsigned NOT NULL,
  PRIMARY KEY (`user_id`,`hero_id`,`equip_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_hero_equip
-- ----------------------------
INSERT INTO `user_hero_equip` VALUES ('5', '1', '1', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '1', '2021', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '1', '3021', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '1', '4021', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '1', '5021', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '2', '21', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '2', '2041', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '2', '3041', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '2', '4041', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '2', '5041', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '3', '41', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '3', '2081', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '3', '3081', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '3', '4081', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '3', '5081', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '4', '61', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '4', '2061', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '4', '3061', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '4', '4061', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '4', '5061', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '5', '81', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '5', '2001', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '5', '3001', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '5', '4001', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '5', '5001', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '6', '101', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '6', '2001', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '6', '3001', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '6', '4001', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '6', '5001', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '7', '121', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '7', '2021', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '7', '3021', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '7', '4021', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '7', '5021', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '8', '141', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '8', '2041', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '8', '3041', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '8', '4041', '1');
INSERT INTO `user_hero_equip` VALUES ('5', '8', '5041', '1');
INSERT INTO `user_hero_equip` VALUES ('6', '18', '341', '75');
INSERT INTO `user_hero_equip` VALUES ('6', '18', '2001', '75');
INSERT INTO `user_hero_equip` VALUES ('6', '18', '3001', '75');
INSERT INTO `user_hero_equip` VALUES ('6', '18', '4001', '75');
INSERT INTO `user_hero_equip` VALUES ('6', '18', '5001', '75');
INSERT INTO `user_hero_equip` VALUES ('6', '18', '6001', '75');
INSERT INTO `user_hero_equip` VALUES ('7', '17', '322', '1');
INSERT INTO `user_hero_equip` VALUES ('7', '17', '2041', '1');
INSERT INTO `user_hero_equip` VALUES ('7', '17', '3041', '1');
INSERT INTO `user_hero_equip` VALUES ('7', '17', '4041', '1');
INSERT INTO `user_hero_equip` VALUES ('7', '17', '5041', '1');
INSERT INTO `user_hero_equip` VALUES ('7', '17', '6041', '1');

-- ----------------------------
-- Table structure for user_hero_skill
-- ----------------------------
DROP TABLE IF EXISTS `user_hero_skill`;
CREATE TABLE `user_hero_skill` (
  `user_id` int(10) unsigned NOT NULL COMMENT '用户id',
  `hero_id` int(10) unsigned NOT NULL,
  `bisha_level` int(10) unsigned NOT NULL,
  `aoyi_level` int(10) unsigned NOT NULL,
  `passive1_level` int(10) unsigned NOT NULL COMMENT '0时 表示未开启该技能！',
  `passive2_level` int(10) unsigned NOT NULL COMMENT '0时 表示未开启该技能！',
  `passive3_level` int(10) unsigned NOT NULL COMMENT '0时 表示未开启该技能！',
  PRIMARY KEY (`user_id`,`hero_id`,`bisha_level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_hero_skill
-- ----------------------------
INSERT INTO `user_hero_skill` VALUES ('5', '1', '1', '1', '1', '1', '1');
INSERT INTO `user_hero_skill` VALUES ('5', '2', '1', '1', '1', '1', '1');
INSERT INTO `user_hero_skill` VALUES ('5', '3', '1', '1', '1', '1', '1');
INSERT INTO `user_hero_skill` VALUES ('5', '4', '1', '1', '1', '1', '1');
INSERT INTO `user_hero_skill` VALUES ('5', '5', '1', '1', '1', '1', '1');
INSERT INTO `user_hero_skill` VALUES ('5', '6', '1', '1', '1', '1', '1');
INSERT INTO `user_hero_skill` VALUES ('5', '7', '1', '1', '1', '1', '1');
INSERT INTO `user_hero_skill` VALUES ('5', '8', '1', '1', '1', '1', '1');
INSERT INTO `user_hero_skill` VALUES ('6', '18', '1', '1', '1', '1', '1');
INSERT INTO `user_hero_skill` VALUES ('7', '17', '1', '1', '1', '1', '1');

-- ----------------------------
-- Table structure for user_item
-- ----------------------------
DROP TABLE IF EXISTS `user_item`;
CREATE TABLE `user_item` (
  `user_id` int(10) unsigned NOT NULL,
  `item_id` int(10) unsigned NOT NULL,
  `num` int(10) unsigned NOT NULL,
  PRIMARY KEY (`user_id`,`item_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_item
-- ----------------------------
INSERT INTO `user_item` VALUES ('7', '1', '33');
INSERT INTO `user_item` VALUES ('7', '2', '35');
INSERT INTO `user_item` VALUES ('7', '13', '2');
INSERT INTO `user_item` VALUES ('7', '64', '1');
INSERT INTO `user_item` VALUES ('7', '68', '999');
INSERT INTO `user_item` VALUES ('7', '70', '1');

-- ----------------------------
-- Table structure for user_mail
-- ----------------------------
DROP TABLE IF EXISTS `user_mail`;
CREATE TABLE `user_mail` (
  `mail_id` int(10) NOT NULL AUTO_INCREMENT,
  `user_id` int(10) NOT NULL,
  `send_time` datetime NOT NULL COMMENT '邮件发送时间',
  `title` varchar(30) NOT NULL,
  `content` varchar(1000) NOT NULL,
  `status` tinyint(1) NOT NULL,
  `type` tinyint(2) NOT NULL COMMENT '领取类和非领取类',
  PRIMARY KEY (`mail_id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB AUTO_INCREMENT=48 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_mail
-- ----------------------------
INSERT INTO `user_mail` VALUES ('26', '7', '2015-06-11 16:57:56', 'ç³»ç»Ÿé‚®ä»¶', 'å…³æœå¥–åŠ±é€šçŸ¥', '1', '0');
INSERT INTO `user_mail` VALUES ('27', '7', '2015-06-11 16:59:08', 'ç³»ç»Ÿé‚®ä»¶', 'å…³æœå¥–åŠ±é€šçŸ¥', '1', '0');
INSERT INTO `user_mail` VALUES ('30', '7', '2015-06-13 15:09:17', 'ç³»ç»Ÿé‚®ä»¶', 'èƒŒåŒ…æˆ–ç¢Žç‰‡å åŠ æ•°é‡å·²æ»¡ï¼Œé€šè¿‡é‚®ä»¶å‘é€', '1', '0');
INSERT INTO `user_mail` VALUES ('45', '7', '2015-06-15 03:26:34', 'ç³»ç»Ÿé‚®ä»¶', 'èƒŒåŒ…æˆ–ç¢Žç‰‡å åŠ æ•°é‡å·²æ»¡ï¼Œé€šè¿‡é‚®ä»¶å‘é€', '1', '0');
INSERT INTO `user_mail` VALUES ('46', '7', '2015-06-15 03:30:02', 'ç³»ç»Ÿé‚®ä»¶', 'èƒŒåŒ…æˆ–ç¢Žç‰‡å åŠ æ•°é‡å·²æ»¡ï¼Œé€šè¿‡é‚®ä»¶å‘é€', '1', '0');
INSERT INTO `user_mail` VALUES ('47', '7', '2015-06-15 03:30:02', 'ç³»ç»Ÿé‚®ä»¶', 'èƒŒåŒ…æˆ–ç¢Žç‰‡å åŠ æ•°é‡å·²æ»¡ï¼Œé€šè¿‡é‚®ä»¶å‘é€', '1', '0');

-- ----------------------------
-- Table structure for user_mail_attach
-- ----------------------------
DROP TABLE IF EXISTS `user_mail_attach`;
CREATE TABLE `user_mail_attach` (
  `id` int(10) NOT NULL,
  `type` int(10) NOT NULL COMMENT '元素类型(金币,道具，经验等)',
  `element_id` int(10) NOT NULL,
  `num` int(10) NOT NULL,
  `mail_id` int(10) NOT NULL,
  `user_id` int(10) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_mail_attach
-- ----------------------------
INSERT INTO `user_mail_attach` VALUES ('26', '8', '68', '1024', '26', '7');
INSERT INTO `user_mail_attach` VALUES ('27', '8', '68', '25', '27', '7');
INSERT INTO `user_mail_attach` VALUES ('30', '8', '68', '81', '30', '7');
INSERT INTO `user_mail_attach` VALUES ('48', '8', '88', '21', '45', '7');
INSERT INTO `user_mail_attach` VALUES ('49', '8', '68', '79', '46', '7');
INSERT INTO `user_mail_attach` VALUES ('50', '8', '45', '24', '47', '7');

-- ----------------------------
-- Table structure for user_sub_task
-- ----------------------------
DROP TABLE IF EXISTS `user_sub_task`;
CREATE TABLE `user_sub_task` (
  `user_id` int(10) unsigned NOT NULL,
  `sub_task_id` int(10) unsigned NOT NULL,
  `star` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '评价星级 0 1 2 3 4 5',
  `last_enter_time` datetime NOT NULL COMMENT '最后一次进此任务的时间',
  `last_day_enter_num` int(10) unsigned NOT NULL COMMENT '最后一次进此任务的那天当天 进入了多少次',
  `total_enter_num` int(10) unsigned NOT NULL COMMENT '一共进入了多少次',
  PRIMARY KEY (`user_id`,`sub_task_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_sub_task
-- ----------------------------
INSERT INTO `user_sub_task` VALUES ('5', '1', '3', '2015-06-04 09:05:45', '131', '131');
INSERT INTO `user_sub_task` VALUES ('6', '1', '3', '2015-06-09 21:29:22', '61', '61');
INSERT INTO `user_sub_task` VALUES ('7', '1', '3', '2015-06-16 15:40:04', '89', '89');

-- ----------------------------
-- Table structure for user_unreceived_mail
-- ----------------------------
DROP TABLE IF EXISTS `user_unreceived_mail`;
CREATE TABLE `user_unreceived_mail` (
  `user_id` int(10) NOT NULL,
  `user_all_server_mail_id` int(10) NOT NULL COMMENT '未接收的全服邮件id',
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user_unreceived_mail
-- ----------------------------

-- ----------------------------
-- Procedure structure for fill_battle_data
-- ----------------------------
DROP PROCEDURE IF EXISTS `fill_battle_data`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `fill_battle_data`()
BEGIN
	#Routine body goes here...

DECLARE v INT;

SET v = 1;

REPEAT

INSERT INTO `sys_battle` SET `battle_id` = v;

SET v = v + 1;

UNTIL v >= 242

END REPEAT;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for get_unreceive_user_all_server_mails
-- ----------------------------
DROP PROCEDURE IF EXISTS `get_unreceive_user_all_server_mails`;
DELIMITER ;;
CREATE DEFINER=`wh`@`127.0.0.1` PROCEDURE `get_unreceive_user_all_server_mails`(IN `inUserID` int)
    COMMENT '获取尚未接收的全服邮件'
BEGIN
	#Routine body goes here...
	select * from user_all_server_mail where id in(select user_all_server_mail_id from user_unreceived_mail where user_id = inUserID);
	select * from user_all_server_mail_attach where id in(select user_all_server_mail_id from user_unreceived_mail where user_id = inUserID);
	delete from user_unreceived_mail where user_id = inUserID;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for insert_user_all_server_mail
-- ----------------------------
DROP PROCEDURE IF EXISTS `insert_user_all_server_mail`;
DELIMITER ;;
CREATE DEFINER=`wh`@`127.0.0.1` PROCEDURE `insert_user_all_server_mail`(IN `in_mail_id` int)
BEGIN
	 DECLARE int_user_id INT DEFAULT 0;
	 DECLARE  _DONE int default 0; 

	#Routine body goes here...
	DECLARE cur CURSOR FOR
		SELECT user_id FROM user;  #查询不在线的
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET _DONE = 1;#错误定义，标记循环结束
	OPEN cur;
	REPEAT
		FETCH cur INTO int_user_id;
		IF NOT _DONE THEN
			INSERT INTO user_unreceived_mail VALUES(int_user_id, in_mail_id);
		END IF;
	UNTIL _DONE END REPEAT;
  CLOSE cur;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for public_set_user_item
-- ----------------------------
DROP PROCEDURE IF EXISTS `public_set_user_item`;
DELIMITER ;;
CREATE DEFINER=`wh`@`127.0.0.1` PROCEDURE `public_set_user_item`(IN `in_num_type` tinyint,IN `in_user_id` int,IN `in_item_id` int,IN `in_num` int)
BEGIN
#Rutine body goes here...

	DECLARE int_num INT DEFAULT -1;
	DECLARE int_sub_num INT;
#INCREASE(增加)
	IF in_num_type = 1 THEN
		INSERT INTO user_item SET user_id = in_user_id, item_id = in_item_id, num = in_num ON DUPLICATE KEY UPDATE num = num + in_num;
#DECREASE(减少)
	ELSEIF in_num_type  = 2 THEN
		SELECT num INTO int_num FROM user_item   WHERE user_id = in_user_id AND item_id = in_item_id;
		IF int_num = -1 THEN 
			SELECT -1 AS RET; 
		ELSE 
			SET int_sub_num = int_num - in_num;
			IF int_sub_num < 0 THEN
					SELECT -2 AS RET;
			ELSEIF int_sub_num = 0 THEN
				DELETE FROM user_item WHERE user_id = in_user_id AND item_id = in_item_id;
			ELSEIF int_sub_num > 0  THEN
				UPDATE user_item SET num = int_sub_num WHERE user_id = in_user_id AND item_id = in_item_id;
			END IF;
		END IF;
#TOTAL(全量)
	ELSEIF in_num_type  = 3 THEN
		INSERT INTO user_item SET user_id = in_user_id, item_id = in_item_id, num = in_num ON DUPLICATE KEY UPDATE num = num + in_num;
	END IF;
END
;;
DELIMITER ;
