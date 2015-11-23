// opxm.cpp : �������̨Ӧ�ó������ڵ㡣
//�Զ����ɿ�
#include "targetver.h"
#include <tchar.h>
//������
#include <iostream>
#include <string>
//������Ŀ����
#include "tinyxml.h"
#include "tinystr.h"
using namespace std;
/*
�ο���ַ��
	http://blog.chinaunix.net/uid-105540-id-191419.html
	http://qaohao.iteye.com/blog/496237

������
	��TiXmlDeclarationָ�ľ���<?xml version="1.0" encoding="UTF-8"?>��
	��TiXmlCommentָ�ľ���<!--one item behalfs one contacted person.-->�� <!--more contacted persons.-->��
	��TiXmlDocumentָ�ľ�������xml�ĵ���
	��TiXmlElementָ�ľ���<phonebook>��<item>��<name>��<addr>�ȵ���Щ�ڵ㣬
	��TiXmlTextָ�ľ��ǡ�gougou������15840330481����Щ����<item>��</item>��<name>��</name>��<addr>��</addr>֮����ı����֣�
	��TiXmlAttributeָ�ľ���<?xml version="1.0" encoding="UTF-8"?>�ڵ���version��encoding��
	����֮�����TiXmlUnknown��
ע��
	��xml�����ʽ����<?xml version="1.0" encoding="GB18030"?> �ļ���ʽANSI��Ҳ����������;
	����task_enter_condition.xml����
*/
 void Parsexml()  
{   
	//step 1 : Load the xml file	 
    string fileXml = "task_enter_condition.xml";
    TiXmlDocument doc( fileXml.c_str() );
    doc.LoadFile();

    // step 2 (optional) : Print the file's content Ԥ���ļ�����
    /*doc.Print();*/
    // step 3 : Parse the xml file ���� xml 
    if ( !doc.Parse( fileXml.c_str() ) )
    {
        cout << doc.ErrorDesc() << endl;
    }   
    // step 3.1 : Get the RootElement ��ø�Ŀ¼
    const TiXmlElement *root = doc.RootElement();
	string Type,NoLessThan,NoMoreThan,attribute_title;//�趨Ԫ�ر���

	/***
	 *step 3.2 : Get RootElement's ChildElement ��ȡ�ڵ�˳�������㵽��㣬�����ڵ������Ԫ��
	 *xml ��һ��<EnterCondition>��Ŀ¼
	 ***/
	 for ( const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
	 {	
		 // <BaseEnterCondition>��2�� Read the ChildElement		 
        if ( ( child->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( child->Value(),"BaseEnterCondition") ) )
        {  			
			//�õ�����ΪBaseEnterCondition�Ľ����
			const TiXmlElement *child_menu = ( const TiXmlElement * ) child;
            
			//<Condition> xml ��3��Condition
			 for ( const TiXmlNode *child2 = child_menu->FirstChild(); child2; child2 = child2->NextSibling() )
			 {
				 if ( ( child2->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( child2->Value(),"Condition") ) )
				 {					  
					//�õ�����ΪCondition�Ľ����
					 const TiXmlElement *child_menu2 = ( const TiXmlElement * ) child2;	
					//<Item type="" NoLessThan="" NoMoreThan="">xml ��4��Item				
					attribute_title=child_menu2->Attribute("Type");
					for ( const TiXmlNode *child3 = child_menu2->FirstChild(); child3; child3 = child3->NextSibling() )
					{
						if ( ( child3->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( child3->Value(),"Item") ) )
						{	
							//�õ�����ΪItem�Ľ����						
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
			�ڸ�Ŀ¼���滹�к�<BaseEnterCondition>ͬһ��Ŀ¼��<TaskEenterCondition>//:~�ڶ���
			<TaskEenterCondition><Condition><Item Type="" NoLessThan="" NoMoreThan=""></Item></Condition>(</TaskEenterCondition>:~A total of four<Condition>
		*/		 
		if(( child->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( child->Value(),"TaskEenterCondition") )){
			//�õ�TaskEenterCondition ����
			const TiXmlElement *child2_menu = ( const TiXmlElement * ) child;
			//ѭ������<TaskEenterCondition>����Ľڵ�
			for ( const TiXmlNode *Task_child = child2_menu->FirstChild(); Task_child; Task_child = Task_child->NextSibling() )
			{
				//ѭ������<TaskEenterCondition>����Ľڵ���<Condition>
				if ( ( Task_child->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( Task_child->Value(),"Condition") ) )
				{
					//�õ�Condition �ڵ� 
					const TiXmlElement *child3_menu = ( const TiXmlElement * ) Task_child;
					//���Condition �ڵ�����ֵ
					attribute_title =  child3_menu->Attribute("Type");
					//ѭ��<Condition>�ڵ�
					for( const TiXmlNode *Task_child1 = child3_menu->FirstChild(); Task_child1; Task_child1 = Task_child1->NextSibling())
					{	
						//���<Condition>�ڵ�����<Item>
						if ( ( Task_child1->Type() == TiXmlNode::TINYXML_ELEMENT ) && ( !strcmp( Task_child1->Value(),"Item") ) )
						{
							//�õ�<Item>�ڵ�
							const TiXmlElement *child4_menu = ( const TiXmlElement * ) Task_child1;
							//���<Item>�ڵ��������ֵ xml�ĸ�ʽ�� <Item Type ="" NoLessThan="" NoMoreThan=""/>
							Type = child4_menu->Attribute("Type");
							NoLessThan = child4_menu->Attribute("NoLessThan");
							NoMoreThan = child4_menu->Attribute("NoMoreThan");	
						}
						//��ӡ����ֵ
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

