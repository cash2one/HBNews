/*
Navicat MySQL Data Transfer

Source Server         : wh
Source Server Version : 50624
Source Host           : 127.0.0.1:3307
Source Database       : wh

Target Server Type    : MYSQL
Target Server Version : 50624
File Encoding         : 65001

Date: 2015-10-30 23:34:59
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Procedure structure for `_give_out_of_date_mail_attach_to_user`
-- ----------------------------
DROP PROCEDURE IF EXISTS `_give_out_of_date_mail_attach_to_user`;
DELIMITER ;;
CREATE DEFINER=`wh`@`127.0.0.1` PROCEDURE `_give_out_of_date_mail_attach_to_user`(IN `in_mail_id` int)
    COMMENT '将过期邮件中道具加给玩家'
BEGIN
	#Routine body goes here...
	#！！！物品类型硬编码
		DECLARE _DONE INT DEFAULT 0;
		DECLARE int_type INT DEFAULT 0;
		DECLARE int_element_id INT DEFAULT 0;
		DECLARE int_num INT DEFAULT 0;
		DECLARE int_user_id INT DEFAULT 0;	
		DECLARE int_item_max_overlap INT DEFAULT 0;

		DECLARE int_num_raw INT DEFAULT 0;	
    DECLARE cur CURSOR FOR SELECT t1.`type`, t1.`element_id`, t1.`num`, t1.`user_id`, t2.`overlap_num` 
															FROM `user_mail_attach` AS t1 
															LEFT JOIN `sys_item` AS t2 
															ON t1.`element_id` = t2.`item_id`  
															WHERE t1.`mail_id` = in_mail_id;
		DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET _DONE = 1;
   
		OPEN cur;
		REPEAT
			FETCH cur INTO int_type, int_element_id, int_num, int_user_id, int_item_max_overlap;
			IF NOT _DONE THEN
				IF int_type = 8 THEN #道具

					SET int_num_raw = 0;

					SELECT num INTO int_num_raw FROM  user_item WHERE user_id = int_user_id AND item_id = int_element_id;
					
					IF int_num_raw < int_item_max_overlap THEN 
						IF int_num_raw + int_num > int_item_max_overlap THEN
							SET int_num = int_item_max_overlap - int_num_raw;
						END IF;
						INSERT INTO user_item SET user_id = int_user_id, item_id = int_element_id, num = int_num ON DUPLICATE KEY UPDATE num = num + int_num;
					END IF;
					
				ELSEIF int_type = 11 THEN #经验
					UPDATE `user` SET `exp` = `exp` + int_num WHERE `user_id` = int_user_id;
				ELSEIF int_type = 12 THEN #金币
					UPDATE `user` SET `gold` = `gold` + int_num WHERE `user_id` = int_user_id;
				ELSEIF int_type = 13 THEN #钻石
					UPDATE `user` SET `diamond` = `diamond` + int_num WHERE `user_id` = int_user_id;
				ELSEIF int_type = 14 THEN #体力
					UPDATE `user` SET `health_cur` = `health_cur` + int_num WHERE `user_id` = int_user_id;
				END IF;

				SET _DONE = 0;
			END IF;
		UNTIL _DONE  END REPEAT;
    CLOSE cur;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `add_friend`
-- ----------------------------
DROP PROCEDURE IF EXISTS `add_friend`;
DELIMITER ;;
CREATE DEFINER=`wh`@`127.0.0.1` PROCEDURE `add_friend`(IN `in_user_id` int,IN `in_user_friend_id` int)
TOP:BEGIN
	#Routine body goes here...
	DECLARE friend_confirm_num INT DEFAULT 0;#好友待确认列表数量
	DECLARE friend_cur_num INT DEFAULT 0;
	SELECT `value` INTO friend_confirm_num FROM `sys_setting` WHERE `key` = 'Friend_Confirm_Max_Num';
	#查看待确认列表数目
	IF EXISTS(SELECT * FROM `user` WHERE user_id = in_user_friend_id)
	THEN SELECT COUNT(*) INTO friend_cur_num  FROM `user_friend_apply` WHERE `apply_user_id` = in_user_friend_id;
	ELSE
		SELECT -1 AS RET;
		LEAVE TOP;
	END IF;

	

	IF friend_cur_num >= friend_confirm_num THEN
		SELECT -1 AS RET;
	ELSE
		INSERT INTO `user_friend_apply` SET `user_id` = in_user_id, `apply_user_id` = in_user_friend_id;
		SELECT 0 AS RET;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `add_or_update_shop_buy`
-- ----------------------------
DROP PROCEDURE IF EXISTS `add_or_update_shop_buy`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `add_or_update_shop_buy`(IN InUserID int, IN InElementType int, IN InElementID int, IN InNum int, IN InLastBuyTime int)
BEGIN
	#Routine body goes here...
	INSERT INTO `user_shop_buy` 
	SET `user_id` = InUserID,
		`element_type` = InElementType,
		`element_id` = InElementID,
		`last_day_buy_num` = InNum,
		`last_buy_time` = InLastBuyTime
	ON DUPLICATE KEY 
	UPDATE `last_day_buy_num` = InNum, `last_buy_time` = InLastBuyTime
	;	
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `add_or_update_shop_item`
-- ----------------------------
DROP PROCEDURE IF EXISTS `add_or_update_shop_item`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `add_or_update_shop_item`(IN InUserID int, IN InShopType int, IN InPos int, IN InElementType int, IN InElementID int, IN InElementNum int, IN InConsumeType int, IN InConsumeID int, IN InConsumeNum int, IN InFlag int, IN InIsSold int)
BEGIN
	#Routine body goes here...
	INSERT INTO `user_shop_item` 
	SET `user_id` = InUserID,
		`shop_type` = InShopType ,
		`pos` = InPos ,
		`element_type` = InElementType,
		`element_id` = InElementID,
		`element_num` = InElementNum ,
		`consume_type` = InConsumeType,
		`consume_id` = InConsumeID,
		`consume_num` = InConsumeNum ,
		`flag` = InFlag,
		`is_sold` = InIsSold
	ON DUPLICATE KEY UPDATE 
		`element_type` = InElementType,
		`element_id` = InElementID,
		`element_num` = InElementNum ,
		`consume_type` = InConsumeType,
		`consume_id` = InConsumeID,
		`consume_num` = InConsumeNum ,
		`flag` = InFlag,
		`is_sold` = InIsSold
	;	
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `add_receive_sys_mail`
-- ----------------------------
DROP PROCEDURE IF EXISTS `add_receive_sys_mail`;
DELIMITER ;;
CREATE DEFINER=`wh`@`127.0.0.1` PROCEDURE `add_receive_sys_mail`(IN `in_userID` int,IN `in_num` int)
BEGIN
	#Routine body goes here...
		DECLARE int_mailid INT DEFAULT 0;
		DECLARE _DONE INT DEFAULT 0;
		
		DECLARE cur CURSOR FOR SELECT id FROM `user_sys_mail` ORDER BY id desc limit in_num;
		DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET _DONE = 1;

		OPEN cur;
		REPEAT 
			FETCH	cur INTO int_mailid;
			IF NOT _DONE THEN
				IF NOT EXISTS(SELECT * FROM `user_received_sys_mail` WHERE user_id = in_userID AND user_sys_mail_id = int_mailid)
					THEN 
						INSERT INTO `user_received_sys_mail` SET `user_id` = in_userID, `user_sys_mail_id` = int_mailid;
				END IF;
			END IF;
		UNTIL _DONE END REPEAT;
		CLOSE cur;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `copy_arena_data_2_db`
-- ----------------------------
DROP PROCEDURE IF EXISTS `copy_arena_data_2_db`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `copy_arena_data_2_db`()
BEGIN
	#Routine body goes here...
	DECLARE  var_max_user_id INT DEFAULT 0;
	#SELECT MAX(user_id) INTO var_max_user_id FROM `user`;

/*
*/
#插入user
INSERT INTO `user` (`user_id`, `name`, `level`, `exp`, `vip`, `diamond`, `gold`, `head_id`, `head_frame_id`, `health_cur`, `health_last_update_timestamp`, `skill_point`, `skill_point_update_timestamp`, `invoke_gold_free_left_num`, `invoke_gold_free_update_timestamp`, `invoke_gold_n_total_num`, `invoke_diamond_free_left_num`, `invoke_diamond_free_update_timestamp`, `invoke_diamond_n_total_num`) 
SELECT var_max_user_id + a.`user_id`, a.`name`, a.`level`, a.`exp`, a.`vip`, a.`diamond`, a.`gold`, a.`head_id`, a.`head_frame_id`, a.`health_cur`, a.`health_last_update_timestamp`, a.`skill_point`, a.`skill_point_update_timestamp`, a.`invoke_gold_free_left_num`, a.`invoke_gold_free_update_timestamp`, a.`invoke_gold_n_total_num`, a.`invoke_diamond_free_left_num`, a.`invoke_diamond_free_update_timestamp`, a.`invoke_diamond_n_total_num` FROM `arena_user` AS a ;


#插入user_arena
INSERT INTO `user_arena` (`rank`, `user_id`, `score`, `hero_id1`, `hero_id2`, `hero_id3`, `hero_id4`, `hero_id5`, `last_challenge_time`, `last_day_challenge_num`, `history_highest_rank`, `last_time_buy_arena_challenge`, `last_day_buy_arena_challenge_num`)
SELECT a.`rank`, var_max_user_id + a.`user_id`, a.`score`, a.`hero_id1`, a.`hero_id2`, a.`hero_id3`, a.`hero_id4`, a.`hero_id5`, a.`last_challenge_time`, a.`last_day_challenge_num`, a.`history_highest_rank`, a.`last_time_buy_arena_challenge`, a.`last_day_buy_arena_challenge_num` FROM `arena_user_arena` AS a ;


#插入user_hero
INSERT INTO `user_hero` (`user_id`, `hero_id`, `star`, `quality`, `level`, `cur_exp`, `got_timestamp`)
SELECT var_max_user_id + a.`user_id`, a.`hero_id`, a.`star`, a.`quality`, a.`level`, a.`cur_exp`, a.`got_timestamp` FROM `arena_user_hero` AS a ;



#插入user_hero_equip
INSERT INTO `user_hero_equip` (`user_id`, `hero_id`, `equip_id`, `level`)
SELECT var_max_user_id + a.`user_id`, a.`hero_id`, a.`equip_id`, a.`level` FROM `arena_user_hero_equip` AS a ;


#插入user_hero_skill
INSERT INTO `user_hero_skill` (`user_id`, `hero_id`, `bisha_level`, `aoyi_level`, `passive1_level`, `passive2_level`, `passive3_level`)
SELECT var_max_user_id + a.`user_id`, a.`hero_id`, a.`bisha_level`, a.`aoyi_level`, a.`passive1_level`, a.`passive2_level`, a.`passive3_level` FROM `arena_user_hero_skill` AS a ;
/*
*/

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `del_out_of_date_mail`
-- ----------------------------
DROP PROCEDURE IF EXISTS `del_out_of_date_mail`;
DELIMITER ;;
CREATE DEFINER=`wh`@`127.0.0.1` PROCEDURE `del_out_of_date_mail`()
    COMMENT '删除过期邮件'
BEGIN
	#Routine body goes here...
	
	DECLARE int_unreaded_mail_del_time INT DEFAULT 0;
	DECLARE int_readed_mail_del_time INT DEFAULT 0;

	DECLARE int_send_time INT DEFAULT 0;
	DECLARE int_staus INT DEFAULT 0;
	DECLARE int_mailid INT DEFAULT 0;

	DECLARE _DONE INT DEFAULT 0;
	DECLARE cur CURSOR FOR SELECT  `mail_id`, UNIX_TIMESTAMP(`send_time`), `status` FROM `user_mail`;
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET _DONE = 1;

	SELECT `value` INTO int_unreaded_mail_del_time FROM `sys_setting` WHERE `key` = 'Mail_UnReaded_Mail_Valid_Time';
	SELECT `value` INTO int_readed_mail_del_time FROM `sys_setting` WHERE `key` = 'Mail_Readed_Mail_Valid_Time';
	OPEN cur;
	REPEAT 
		FETCH	cur INTO int_mailid, int_send_time, int_staus;

		IF NOT _DONE THEN
			
			IF int_staus = 1 THEN#已读
				IF int_send_time + int_readed_mail_del_time < UNIX_TIMESTAMP(Now()) THEN
					#给玩家添加道具
					CALL _give_out_of_date_mail_attach_to_user(int_mailid);
					DELETE FROM `user_mail` WHERE `mail_id` = int_mailid;
					DELETE FROM `user_mail_attach` WHERE `mail_id` = int_mailid;
				END IF;
			ELSEIF int_staus = 2 THEN#未读
				IF int_send_time + int_unreaded_mail_del_time < UNIX_TIMESTAMP(Now()) THEN
					CALL _give_out_of_date_mail_attach_to_user(int_mailid);
					DELETE FROM `user_mail` WHERE `mail_id` = int_mailid;
					DELETE FROM `user_mail_attach` WHERE `mail_id` = int_mailid;
				END IF;
			END IF;	
		END IF;
	UNTIL _DONE END REPEAT;
	CLOSE cur;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `dvrt_add_friend`
-- ----------------------------
DROP PROCEDURE IF EXISTS `dvrt_add_friend`;
DELIMITER ;;
CREATE DEFINER=`wh`@`127.0.0.1` PROCEDURE `dvrt_add_friend`(IN `in_user_id` int,IN `in_user_friend_id` int)
TOP:BEGIN
	#Routine body goes here...
	DECLARE friend_confirm_num INT DEFAULT 0;#好友待确认列表数�?
	DECLARE friend_cur_num INT DEFAULT 0;
	SELECT `value` INTO friend_confirm_num FROM `sys_setting` WHERE `key` = 'Friend_Confirm_Max_Num';
	#查看待确认列表数�?
	IF EXISTS(
		SELECT * FROM `user` WHERE user_id = in_user_friend_id
	)
	THEN
		SELECT -1 AS RET;
		LEAVE TOP;
	END IF;

	SELECT COUNT(*) INTO friend_cur_num  FROM `user_friend_apply` WHERE `apply_user_id` = in_user_friend_id;

	IF friend_cur_num >= friend_confirm_num THEN
		SELECT -1 AS RET;
	ELSE
		INSERT INTO `user_friend_apply` SET `user_id` = in_user_id, `apply_user_id` = in_user_friend_id;
		SELECT 0 AS RET;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `edit_user_name`
-- ----------------------------
DROP PROCEDURE IF EXISTS `edit_user_name`;
DELIMITER ;;
CREATE DEFINER=`wh`@`127.0.0.1` PROCEDURE `edit_user_name`(IN `in_user_id` int, IN `in_user_name` varchar(63))
TOP:BEGIN
	#Routine body goes here...

	IF EXISTS
	(
		SELECT * FROM `user` WHERE binary `name` = in_user_name
	) THEN
			SELECT -1 AS RET;
			LEAVE TOP;
	END IF;
  UPDATE `user` SET `name` = in_user_name WHERE `user_id` = in_user_id;
	SELECT 0 AS RET;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `fill_battle_data`
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
-- Procedure structure for `get_user_mail`
-- ----------------------------
DROP PROCEDURE IF EXISTS `get_user_mail`;
DELIMITER ;;
CREATE DEFINER=`wh`@`127.0.0.1` PROCEDURE `get_user_mail`(IN `in_user_id` int)
    COMMENT '首先根据发送时间删除玩家的过期邮件，如果玩家邮件超过数量，根据发送时间删除超过数量的邮件'
BEGIN
	#Routine body goes here...
	DECLARE int_unreaded_mail_del_time INT DEFAULT 0;
	DECLARE int_readed_mail_del_time INT DEFAULT 0;

	DECLARE int_send_time INT DEFAULT 0;
	DECLARE int_staus INT DEFAULT 0;
	DECLARE int_mailid INT DEFAULT 0;
	DECLARE int_max_over_num_mail INT DEFAULT 0;
	DECLARE int_limit_num INT DEFAULT 0;

	DECLARE _DONE INT DEFAULT 0;
	DECLARE cur CURSOR FOR SELECT  `mail_id`, UNIX_TIMESTAMP(`send_time`), `status` FROM `user_mail` WHERE `user_id` = in_user_id;
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET _DONE = 1;
	
	SELECT `value` INTO int_unreaded_mail_del_time FROM `sys_setting` WHERE `key` = 'Mail_UnReaded_Mail_Valid_Time';
	SELECT `value` INTO int_readed_mail_del_time FROM `sys_setting` WHERE `key` = 'Mail_Readed_Mail_Valid_Time';
	SELECT `value` INTO int_max_over_num_mail FROM `sys_setting` WHERE `key` = 'Mail_Max_Num';
	#遍历邮件将无效邮件删除
	OPEN cur;
	REPEAT 
		FETCH	cur INTO int_mailid, int_send_time, int_staus;

		IF NOT _DONE THEN
			
			IF int_staus = 1 THEN#已读
				IF int_send_time + int_readed_mail_del_time < UNIX_TIMESTAMP(Now()) THEN
					#给玩家添加道具
					CALL _give_out_of_date_mail_attach_to_user(int_mailid);
					DELETE FROM `user_mail` WHERE `mail_id` = int_mailid;
					DELETE FROM `user_mail_attach` WHERE `mail_id` = int_mailid;
				END IF;
			ELSEIF int_staus = 2 THEN#未读
				IF int_send_time + int_unreaded_mail_del_time < UNIX_TIMESTAMP(Now()) THEN
					CALL _give_out_of_date_mail_attach_to_user(int_mailid);
					DELETE FROM `user_mail` WHERE `mail_id` = int_mailid;
					DELETE FROM `user_mail_attach` WHERE `mail_id` = int_mailid;
				END IF;
			END IF;	
		END IF;
	UNTIL _DONE END REPEAT;
	CLOSE cur;
	
	#获取玩家邮件信息
	SELECT `mail_id`, UNIX_TIMESTAMP(`send_time`)  as  `send_time`,  `content`, `status`, `type` FROM user_mail WHERE `user_id` = in_user_id ORDER BY  `send_time`;
	SELECT `mail_id`, `id`, `element_id`, `type`, `num` FROM `user_mail_attach` WHERE `user_id` = in_user_id;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `get_user_unreceived_sys_mail`
-- ----------------------------
DROP PROCEDURE IF EXISTS `get_user_unreceived_sys_mail`;
DELIMITER ;;
CREATE DEFINER=`wh`@`127.0.0.1` PROCEDURE `get_user_unreceived_sys_mail`(IN `in_user_id` int)
    COMMENT '首先获取玩家未接收的系统邮件的id，根据未接收邮件的id获取为接收邮件的主体和附件，将这些id插入到玩家已接收系统邮件中'
BEGIN
	#Routine body goes here...
	DECLARE int_mail_id INT DEFAULT 0;
	DECLARE  _DONE INT default 0;  
	DECLARE now INT DEFAULT UNIX_TIMESTAMP(Now());							
	
	#获取未接收邮件邮件主体
	SELECT `id`, UNIX_TIMESTAMP(`send_time`) AS `send_time`, `content`, `type`
	FROM `user_sys_mail` 
	WHERE UNIX_TIMESTAMP(`send_time`) +  `valid_time` >= now AND `id` NOT IN(SELECT `user_sys_mail_id` FROM `user_received_sys_mail` WHERE `user_id` = in_user_id);
	
	#获取未接收邮件附件
	SELECT `mail_id`, `element_id`, `type`, `num` 
	FROM `user_sys_mail_attach`  
	WHERE `mail_id` IN(
		SELECT `id` FROM `user_sys_mail` 
		WHERE UNIX_TIMESTAMP(`send_time`) + `valid_time` >= now AND `id` NOT IN(
			SELECT `user_sys_mail_id` FROM `user_received_sys_mail` WHERE `user_id` = in_user_id)
		);

	INSERT INTO `user_received_sys_mail`(user_id, user_sys_mail_id) SELECT in_user_id,`id` FROM `user_sys_mail` WHERE UNIX_TIMESTAMP(`send_time`) + `valid_time` >= now AND `id` NOT IN(SELECT `user_sys_mail_id` FROM `user_received_sys_mail` WHERE `user_id` = in_user_id);

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `is_affirm_friend_full`
-- ----------------------------
DROP PROCEDURE IF EXISTS `is_affirm_friend_full`;
DELIMITER ;;
CREATE DEFINER=`wh`@`127.0.0.1` PROCEDURE `is_affirm_friend_full`(IN `friend_user_ids` varchar(1000))
TOP:
BEGIN
	#Routine body goes here...
	#获取玩家的好友列表是否达到上限
	DECLARE friend_max_num int DEFAULT 0;
	DECLARE int_friend_num int DEFAULT 0;

	DECLARE _DONE INT DEFAULT 0;
	DECLARE cur CURSOR FOR SELECT count(*) FROM `user_friend` WHERE `user_id` in(friend_user_ids) GROUP BY user_id;
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET _DONE = 1;

	SELECT `value` INTO friend_max_num FROM `sys_setting` WHERE `key` = 'Friend_Max_Num';
	
	#循环检测值
	OPEN cur;
	REPEAT 
		FETCH	cur INTO int_friend_num;

		IF NOT _DONE THEN
			IF int_friend_num = friend_max_num THEN#判断对方列表会不会满
				SELECT -1 AS RET;
				LEAVE TOP;
			END IF;
		END IF;
	UNTIL _DONE END REPEAT;
	CLOSE cur;

	SELECT 0 AS RET;
END
;;
DELIMITER ;

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

-- ----------------------------
-- Procedure structure for `set_user_item`
-- ----------------------------
DROP PROCEDURE IF EXISTS `set_user_item`;
DELIMITER ;;
CREATE DEFINER=`wh`@`127.0.0.1` PROCEDURE `set_user_item`(IN `in_num_type` tinyint,IN `in_user_id` int,IN `in_item_id` int,IN `in_num` int)
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
		INSERT INTO user_item SET user_id = in_user_id, item_id = in_item_id, num = in_num ON DUPLICATE KEY UPDATE num =  in_num;
	END IF;
END
;;
DELIMITER ;
