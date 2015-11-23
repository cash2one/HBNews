// opxm.cpp : 定义控制台应用程序的入口点。
//自动生成库
#include "targetver.h"
#include <tchar.h>
//关联库
#include <iostream>
#include <string>
//引入项目工程
#include "tinyxml.h"
#include "tinystr.h"
using namespace std;
/*
参考地址：
	http://blog.chinaunix.net/uid-105540-id-191419.html
	http://qaohao.iteye.com/blog/496237

描述：
	像TiXmlDeclaration指的就是<?xml version="1.0" encoding="UTF-8"?>，
	像TiXmlComment指的就是<!--one item behalfs one contacted person.-->、 <!--more contacted persons.-->，
	像TiXmlDocument指的就是整个xml文档，
	像TiXmlElement指的就是<phonebook>、<item>、<name>、<addr>等等这些节点，
	像TiXmlText指的就是‘gougou’、‘15840330481’这些夹在<item>与</item>、<name>与</name>、<addr>与</addr>之间的文本文字，
	像TiXmlAttribute指的就是<?xml version="1.0" encoding="UTF-8"?>节点中version、encoding，
	除此之外就是TiXmlUnknown。
注：
	本xml编码格式调试<?xml version="1.0" encoding="GB18030"?> 文件格式ANSI；也可自由配置;
	根据task_enter_condition.xml解析
*/
 void Parsexml()  
{   
	//step 1 : Load the xml file	 
    string fileXml = "task_enter_condition.xml";
    TiXmlDocument doc( fileXml.c_str() );
    doc.LoadFile();

    // step 2 (optional) : Print the file's content 预览文件内容
    /*doc.Print();*/
    // step 3 : Parse the xml file 解析 xml 
    if ( !doc.Parse( fileXml.c_str() ) )
    {
        cout << doc.ErrorDesc() << endl;
    }   
    // step 3.1 : Get the RootElement 获得根目录
    const TiXmlElement *root = doc.RootElement();
	string Type,NoLessThan,NoMoreThan,attribute_title;//设定元素变量

	/***
	 *step 3.2 : Get RootElement's ChildElement 获取节点顺序从最外层到里层，包括节点的属性元素
	 *xml 第一层<EnterCondition>根目录
	 ***/
	 for ( const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
	 {	
		 // <BaseEnterCondition>第2层 Read the ChildElement		 
        if ( ( child->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( child->Value(),"BaseEnterCondition") ) )
        {  			
			//得到属性为BaseEnterCondition的结果集
			const TiXmlElement *child_menu = ( const TiXmlElement * ) child;
            
			//<Condition> xml 第3层Condition
			 for ( const TiXmlNode *child2 = child_menu->FirstChild(); child2; child2 = child2->NextSibling() )
			 {
				 if ( ( child2->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( child2->Value(),"Condition") ) )
				 {					  
					//得到属性为Condition的结果集
					 const TiXmlElement *child_menu2 = ( const TiXmlElement * ) child2;	
					//<Item type="" NoLessThan="" NoMoreThan="">xml 第4层Item				
					attribute_title=child_menu2->Attribute("Type");
					for ( const TiXmlNode *child3 = child_menu2->FirstChild(); child3; child3 = child3->NextSibling() )
					{
						if ( ( child3->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( child3->Value(),"Item") ) )
						{	
							//得到属性为Item的结果集						
							const  TiXmlElement *child_menu3 = ( const TiXmlElement * ) child3;
							Type = child_menu3->Attribute("Type");
							NoLessThan = child_menu3->Attribute("NoLessThan");
							NoMoreThan = child_menu3->Attribute("NoMoreThan");						 
						}
						cout << "\tType : "    << Type << endl
						<< "\tNoLessThan : "   << NoLessThan << endl
						<< "\tNoMoreThan : " << NoMoreThan << endl << endl;
					}////:~</Time>
					
				 }			 
			 }  		 
		}//: </BaseEnterCondition>
		/*
			在根目录里面还有和<BaseEnterCondition>同一级目录的<TaskEenterCondition>//:~第二层
			<TaskEenterCondition><Condition><Item Type="" NoLessThan="" NoMoreThan=""></Item></Condition>(</TaskEenterCondition>:~A total of four<Condition>
		*/		 
		if(( child->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( child->Value(),"TaskEenterCondition") )){
			//得到TaskEenterCondition 属性
			const TiXmlElement *child2_menu = ( const TiXmlElement * ) child;
			//循环属性<TaskEenterCondition>里面的节点
			for ( const TiXmlNode *Task_child = child2_menu->FirstChild(); Task_child; Task_child = Task_child->NextSibling() )
			{
				//循环属性<TaskEenterCondition>里面的节点有<Condition>
				if ( ( Task_child->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( Task_child->Value(),"Condition") ) )
				{
					//得到Condition 节点 
					const TiXmlElement *child3_menu = ( const TiXmlElement * ) Task_child;
					//获得Condition 节点属性值
					attribute_title =  child3_menu->Attribute("Type");
					//循环<Condition>节点
					for( const TiXmlNode *Task_child1 = child3_menu->FirstChild(); Task_child1; Task_child1 = Task_child1->NextSibling())
					{	
						//如果<Condition>节点里有<Item>
						if ( ( Task_child1->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( Task_child1->Value(),"Item") ) )
						{
							//得到<Item>节点
							const TiXmlElement *child4_menu = ( const TiXmlElement * ) Task_child1;
							//获得<Item>节点里的属性值 xml的格式是 <Item Type ="" NoLessThan="" NoMoreThan=""/>
							Type = child4_menu->Attribute("Type");
							NoLessThan = child4_menu->Attribute("NoLessThan");
							NoMoreThan = child4_menu->Attribute("NoMoreThan");	
						}
						//打印属性值
						cout << "\tType : "    << Type << endl
						<< "\tNoLessThan : "   << NoLessThan << endl
						<< "\tNoMoreThan : " << NoMoreThan << endl << endl;
					}
				}
			
			}
		
		}

	 }//:</EnterCondition>~for 	 	 
}  
  

  

int _tmain(int argc, _TCHAR* argv[])
{	 
	Parsexml();
	return 0;
}

