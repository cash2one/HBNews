/*
Navicat MySQL Data Transfer

Source Server         : localhost-mysql5.6-3307
Source Server Version : 50617
Source Host           : localhost:3307
Source Database       : wh_pay

Target Server Type    : MYSQL
Target Server Version : 50617
File Encoding         : 65001

Date: 2015-11-05 12:38:56
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `area_gameserver`
-- ----------------------------
DROP TABLE IF EXISTS `area_gameserver`;
CREATE TABLE `area_gameserver` (
  `id` int(10) unsigned NOT NULL COMMENT '服务器id',
  `ip` varchar(255) NOT NULL COMMENT 'gs的ip地址',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of area_gameserver
-- ----------------------------
INSERT INTO area_gameserver VALUES ('1', '127.0.0.1');

-- ----------------------------
-- Table structure for `commodity`
-- ----------------------------
DROP TABLE IF EXISTS `commodity`;
CREATE TABLE `commodity` (
  `id` int(10) unsigned NOT NULL,
  `pay_plantform` int(10) unsigned NOT NULL COMMENT '商品展现&支付平台',
  `priority` float unsigned NOT NULL COMMENT '展示优先级， 数字越大 展示越后',
  `name` varchar(255) NOT NULL COMMENT '商品名称',
  `present_price` float unsigned NOT NULL COMMENT '现价,也就是玩家支付的钱数',
  `original_price` float unsigned NOT NULL COMMENT '原价',
  `type_id_num_arr` varchar(511) NOT NULL COMMENT '商品内容 游戏物品数组，格式为：  type-id-num|type-id-num',
  PRIMARY KEY (`id`),
  KEY `pay_plantform` (`pay_plantform`),
  KEY `priority` (`priority`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of commodity
-- ----------------------------

-- ----------------------------
-- Table structure for `order`
-- ----------------------------
DROP TABLE IF EXISTS `order`;
CREATE TABLE `order` (
  `plantform_id` smallint(5) unsigned NOT NULL,
  `order_id` varchar(255) NOT NULL COMMENT '订单号',
  `area_id` smallint(6) NOT NULL COMMENT '服务器id 区id',
  `user_id` int(10) unsigned NOT NULL COMMENT '玩家id 账户id  一个账户可以进多个区',
  `create_time` datetime NOT NULL COMMENT '订单创建时间',
  `status` tinyint(3) unsigned NOT NULL COMMENT '当前状态 0待支付  1已支付未处理  2已支付正在处理 3.已支付已处理完',
  `commodity_id` int(10) unsigned NOT NULL COMMENT '商品id',
  `commodity_num` int(10) unsigned NOT NULL COMMENT '商品数量',
  `present_price` float unsigned NOT NULL COMMENT '现价,也就是玩家支付的钱数',
  `original_price` float unsigned NOT NULL COMMENT '原价',
  PRIMARY KEY (`plantform_id`,`order_id`),
  KEY `area_id` (`area_id`),
  KEY `user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of order
-- ----------------------------

-- ----------------------------
-- Table structure for `plantform`
-- ----------------------------
DROP TABLE IF EXISTS `plantform`;
CREATE TABLE `plantform` (
  `id` int(10) unsigned NOT NULL COMMENT '平台id',
  `plantform_name` varchar(255) NOT NULL COMMENT '平台名字',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of plantform
-- ----------------------------

-- ----------------------------
-- Procedure structure for `public_order_notify`
-- ----------------------------
DROP PROCEDURE IF EXISTS `public_order_notify`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `public_order_notify`(IN var_plantform_id int, IN var_order_id varchar(127), IN var_create_time int, IN var_status int, IN var_commodity_id int, IN var_commodity_num int, IN var_present_price float, IN var_original_price float)
TOP:
BEGIN
	#Routine body goes here...
	DECLARE var_old_status INT DEFAULT -1;

	/*
		enum ESTATUS{
		MIN = 0,
		JUST_PAIED = 1,			//收到已经支付成功的通知
		GS_PROCESING = 2,		//发给GS处理 还未回复
		GS_PROCESS_SUCCESS = 3,	//GS处理成功，交付物品
		GS_PROCESS_FAIL = 4,	//GS处理失败
		MAX = 5
	};
	*/

	SELECT `status` INTO var_old_status FROM `order` WHERE `order_id` = var_order_id;

	#只有不存在或者失败的订单才会返回0 继续后续发消息给gs进行处理！
	#并且处理之前设置好状态 以防重入

	#IF EXISTS(SELECT * FROM `order` WHERE `order_id` = var_order_id) THEN
	IF var_old_status = -1 OR ISNULL(var_old_status) THEN#订单不存在 则创建订单 并设置JUST_PAIED状态防止重入
		INSERT INTO `order` SET
			`plantform_id` = var_plantform_id,
			`order_id` = var_order_id,
			`create_time` = create_time,
			`status` = var_status,
			`commodity_id` = var_commodity_id,
			`commodity_num` = var_commodity_num,
			`present_price` = var_present_price,
			`original_price` = var_original_price
		;
	ELSEIF var_status = 4 THEN#如果是失败的订单 则进行处理 并设置JUST_PAIED状态防止重入
		 UPDATE `order` SET `status` = var_status WHERE `order_id` = var_order_id;
	ELSE
		SELECT -1 AS RET;
		LEAVE TOP;
	END IF;

	SELECT 0 AS RET;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `public_order_process`
-- ----------------------------
DROP PROCEDURE IF EXISTS `public_order_process`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `public_order_process`(IN var_order_id varchar(127), IN var_status int)
TOP:
BEGIN
	#Routine body goes here...
	DECLARE var_old_status INT DEFAULT -1;

	/*
		enum ESTATUS{
		MIN = 0,
		JUST_PAIED = 1,			//收到已经支付成功的通知
		GS_PROCESING = 2,		//发给GS处理 还未回复
		GS_PROCESS_SUCCESS = 3,	//GS处理成功，交付物品
		GS_PROCESS_FAIL = 4,	//GS处理失败
		MAX = 5
	};
	*/

	SELECT `status` INTO var_old_status FROM `order` WHERE `order_id` = var_order_id;

	#只有不存在或者失败的订单才会返回0 继续后续发消息给gs进行处理！
	#并且处理之前设置好状态 以防重入

	#IF EXISTS(SELECT * FROM `order` WHERE `order_id` = var_order_id) THEN
	IF var_status = -1 OR ISNULL(var_status) THEN#订单不存在 则不应该会被处理！
		SELECT -1 AS RET;
		LEAVE TOP;
	ELSEIF var_status != 1 THEN#订单状态不为JUST_PAIED  不正常！
		SELECT -2 AS RET;
		LEAVE TOP;
	ELSE
		UPDATE `order` SET `status` = var_status WHERE `order_id` = var_order_id;
	END IF;

	SELECT 0 AS RET;
END
;;
DELIMITER ;
