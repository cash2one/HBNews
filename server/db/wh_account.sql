/*
Navicat MySQL Data Transfer

Source Server         : localhost-mysql5.6-3307
Source Server Version : 50617
Source Host           : localhost:3307
Source Database       : wh_account

Target Server Type    : MYSQL
Target Server Version : 50617
File Encoding         : 65001

Date: 2015-10-14 15:34:07
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Procedure structure for `public_login`
-- ----------------------------
DROP PROCEDURE IF EXISTS `public_login`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `public_login`(IN `in_plantform_id` int, IN `in_plantform_sub_id` int, IN `in_account` varchar(63), IN `in_password` varchar(63))
BEGIN
	#Routine body goes here...
DECLARE var_user_id INT DEFAULT NULL;

SELECT `user_id` INTO var_user_id FROM `global_account` 
WHERE `plantform_id` = in_plantform_id 
	AND `plantform_sub_id` = in_plantform_sub_id
	AND `account` = in_account
  AND `password` = in_password;

IF ISNULL(var_user_id) THEN
	SELECT -1 AS RET;
END IF;

  SELECT 0 AS RET, var_user_id AS user_id;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `public_login_if_no_register`
-- ----------------------------
DROP PROCEDURE IF EXISTS `public_login_if_no_register`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `public_login_if_no_register`(IN `in_plantform_id` int, IN `in_plantform_sub_id` int, IN `in_account` varchar(63), IN `in_password` varchar(63))
BEGIN
	#Routine body goes here...
DECLARE var_user_id INT DEFAULT NULL;
DECLARE var_password VARCHAR(255) DEFAULT "";

SELECT `user_id`, `password` INTO var_user_id, var_password FROM `global_account` 
WHERE `plantform_id` = in_plantform_id 
	AND `plantform_sub_id` = in_plantform_sub_id
	AND `account` = in_account
  #AND `password` = in_password
;

IF ISNULL(var_user_id) THEN #没有这个用户时
	INSERT INTO `global_account` 
	SET `plantform_id` = in_plantform_id,
			`plantform_sub_id` = in_plantform_sub_id,
			`account` = in_account,
			`password` = in_password,
			`register_time` = NOW()
	;

	SELECT 0 AS RET, LAST_INSERT_ID() AS user_id;
ELSE
	IF var_password = in_password THEN
		SELECT 0 AS RET, var_user_id AS user_id;
	ELSE
		SELECT -1 AS RET;
	END IF;
END IF;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `public_register`
-- ----------------------------
DROP PROCEDURE IF EXISTS `public_register`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `public_register`(IN `in_plantform_id` int, IN `in_plantform_sub_id` int, IN `in_account` varchar(63), IN `in_password` varchar(63))
TOP:BEGIN
	#Routine body goes here...

IF EXISTS
(
SELECT * FROM `global_account` 
WHERE `plantform_id` = in_plantform_id 
	AND `plantform_sub_id` = in_plantform_sub_id
	AND `account` = in_account
) THEN
		SELECT -1 AS RET;
		LEAVE TOP;
END IF;

INSERT INTO `global_account` SET 
`plantform_id` = in_plantform_id,
`plantform_sub_id` = in_plantform_sub_id,
`account` = in_account,
`password` = in_password,
`register_time` = NOW()
;

SELECT 0 AS RET, LAST_INSERT_ID() AS user_id;

END
;;
DELIMITER ;
