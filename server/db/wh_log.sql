/*
Navicat MySQL Data Transfer

Source Server         : localhost-mysql5.6-3307
Source Server Version : 50617
Source Host           : localhost:3307
Source Database       : wh_log

Target Server Type    : MYSQL
Target Server Version : 50617
File Encoding         : 65001

Date: 2015-08-31 10:56:33
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `log_msg_cfg`
-- ----------------------------
DROP TABLE IF EXISTS `log_msg_cfg`;
CREATE TABLE `log_msg_cfg` (
  `server_type` int(10) unsigned NOT NULL COMMENT '1. as  2.gs',
  `req_msg_id` int(10) unsigned NOT NULL,
  `res_msg_id` int(11) unsigned NOT NULL,
  `req_msg_name` varchar(255) NOT NULL,
  `res_msg_name` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of log_msg_cfg
-- ----------------------------

-- ----------------------------
-- Table structure for `log_msg_user_receive`
-- ----------------------------
DROP TABLE IF EXISTS `log_msg_user_receive`;
CREATE TABLE `log_msg_user_receive` (
  `user_id` int(10) unsigned NOT NULL,
  `server_type` int(10) unsigned NOT NULL COMMENT '1. as  2.gs',
  `msg_id` int(10) unsigned NOT NULL,
  `time` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of log_msg_user_receive
-- ----------------------------

-- ----------------------------
-- Table structure for `log_msg_user_send`
-- ----------------------------
DROP TABLE IF EXISTS `log_msg_user_send`;
CREATE TABLE `log_msg_user_send` (
  `user_id` int(10) unsigned NOT NULL,
  `server_type` int(10) unsigned NOT NULL COMMENT '1. as  2.gs',
  `msg_id` int(10) unsigned NOT NULL,
  `time` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of log_msg_user_send
-- ----------------------------

-- ----------------------------
-- Procedure structure for `log_msg`
-- ----------------------------
DROP PROCEDURE IF EXISTS `log_msg`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `log_msg`(IN in_log_type INT, IN in_server_type INT, IN in_user_id INT, IN in_msg_id INT, IN in_time int)
BEGIN
	#Routine body goes here...

	IF in_log_type = 1 THEN
		INSERT INTO `log_msg_user_send` 
		SET `server_type` = in_server_type, 
			`user_id` = in_user_id, 
			`msg_id` = in_msg_id, 
			`time` = FROM_UNIXTIME(in_time)
		;
	ELSE
		INSERT INTO `log_msg_user_receive` 
		SET `server_type` = in_server_type, 
			`user_id` = in_user_id, 
			`msg_id` = in_msg_id, 
			`time` = FROM_UNIXTIME(in_time)
		;
	END IF;

END
;;
DELIMITER ;
