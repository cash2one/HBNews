# -*- coding: UTF-8 -*-

from xls_deploy_tool import func
import sys
import os
import xlrd

base_path = "E:/OurGame/doc/配置表/server/"
config = []


'''请在此处写配置表(excel名字)'''
config_excel_name =[
	"奥义数值表.xls",
	"背包及碎片物品表.xls",
	"被动技能表.xls",
	"必杀数值表.xls",
	#掉落方案及包裹组.xls
    "剧情副本数据表.xls",
	#玩家等级表.xls
	"英雄怪物配置表.xls",
	"英雄装备表.xls",
	"VIP等级表.xls",
	"奖励显示列表.xls",
	"规则配置表.xls",
	"品质数值表.xls"
]

def parse_sheet(excel_name):
    data = xlrd.open_workbook((base_path + excel_name).decode('utf-8'))
    #print "data: ", data, "\n"
    #print "data.sheets(): ", data.sheets(), "\n"
    #print "data.sheet_names(): ", data.sheet_names(), "\n"

    for sheet_name in data.sheet_names():
        config.append((sheet_name, (base_path + excel_name).decode('utf-8'), 0))
        #print "type(sheet_name), type(base_path + excel_name)", type(sheet_name), " ",  type((base_path + excel_name).decode('utf-8')), "\n"

    #print "config: ", config, "\n"


if __name__ == '__main__' :

    for excel_name in config_excel_name:
        print "excel_name: ", excel_name, "\n"
        parse_sheet(excel_name)
    
    for lst in config:
    #for i in range(len(config)):
        
        if (len(lst) < 3):
            print "config error"
            sys.exit(-1)
        #print config[i]
        
        print lst[0], lst[1], lst[2], "\n"
        print type(lst[0]), type(lst[1]), type(lst[2]), "\n"
        func(lst[0], lst[1], lst[2])

    os.system("pause")
            
