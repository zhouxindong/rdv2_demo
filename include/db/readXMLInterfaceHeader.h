#pragma once
#include "..\..\extlibs\tinyxml2\tinyxml2.h"
//#include "tinyxml2/tinyxml2.h"
#include <iostream>
 
using namespace std;
using namespace tinyxml2;
//function：create a xml file
//param：xmlPath:xml文件路径
//return:0,成功，非0，失败

//string m_strUser;
//string m_strPassWd;
//string m_strTargetDb;
//string m_strTargetServer;

//int createXML(const char* xmlPath)
//{
//	const char* declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
//	tinyxml2::XMLDocument doc;
//	doc.Parse(declaration);//会覆盖xml所有内容
//
//						   //添加申明可以使用如下两行
//						   //XMLDeclaration* declaration=doc.NewDeclaration();
//						   //doc.InsertFirstChild(declaration);
//
//	XMLElement* root = doc.NewElement("DBUSER");
//	doc.InsertEndChild(root);
//
//	return doc.SaveFile(xmlPath);
//}
//
////用户类
//class User
//{
//public:
//	User() {
//		gender = 0;
//	};
//
//	User(const string& userName, const string& password, int gender, const string& mobile, const string& email) {
//		this->userName = userName;
//		this->password = password;
//		this->gender = gender;
//		this->mobile = mobile;
//		this->email = email;
//	};
//
//	string userName;
//	string password;
//	int gender;
//	string mobile;
//	string email;
//};
//
////function:insert XML node
////param:xmlPath:xml文件路径; user:用户对象
////return:0:成功; 非0:失败
//int insertXMLNode(const char* xmlPath, const User& user)
//{
//	tinyxml2::XMLDocument doc;
//	int res = doc.LoadFile(xmlPath);
//	if (res != 0)
//	{
//		cout << "load xml file failed" << endl;
//		return res;
//	}
//	XMLElement* root = doc.RootElement();
//
//	XMLElement* userNode = doc.NewElement("User");
//	userNode->SetAttribute("Name", user.userName.c_str());
//	userNode->SetAttribute("Password ", user.password.c_str());
//	root->InsertEndChild(userNode);
//
//	XMLElement* gender = doc.NewElement("Gender");
//	char p[2];//用于存储目标字符串的地址.经过调试，这里长度为2时，运行时才不会出现debug error
//	int len = sprintf(p, "%d", user.gender);
//	XMLText* genderText = doc.NewText(p);
//	//XMLText* genderText = doc.NewText(_itoa(user.gender,p,10));
//	//delete p;
//	gender->InsertEndChild(genderText);
//	userNode->InsertEndChild(gender);
//
//	XMLElement* mobile = doc.NewElement("Mobile");
//	mobile->InsertEndChild(doc.NewText(user.mobile.c_str()));
//	userNode->InsertEndChild(mobile);
//
//	XMLElement* email = doc.NewElement("Email");
//	email->InsertEndChild(doc.NewText(user.email.c_str()));
//	userNode->InsertEndChild(email);
//
//	return doc.SaveFile(xmlPath);
//}
//
////function:根据用户名获取用户节点
////param:root:xml文件根节点；userName：用户名
////return：用户节点
//XMLElement* queryUserNodeByName(XMLElement* root, const string& userName)
//{
//
//	XMLElement* userNode = root->FirstChildElement("User");
//	while (userNode != NULL)
//	{
//		if (userNode->Attribute("Name") == userName)
//			break;
//		userNode = userNode->NextSiblingElement();//下一个兄弟节点
//	}
//	return userNode;
//}
//
////在以上函数的基础上，获取用户信息的函数：
//User* queryUserByName(const char* xmlPath, const string& userName)
//{
//	tinyxml2::XMLDocument doc;
//	if (doc.LoadFile(xmlPath) != 0)
//	{
//		cout << "load xml file failed" << endl;
//		return NULL;
//	}
//	XMLElement* root = doc.RootElement();
//	XMLElement* userNode = queryUserNodeByName(root, userName);
//
//	if (userNode != NULL)  //searched successfully
//	{
//		User* user = new User();
//		user->userName = userName;
//		user->password = userNode->Attribute("Password");
//		XMLElement* genderNode = userNode->FirstChildElement("Gender");
//		user->gender = atoi(genderNode->GetText());
//		XMLElement* mobileNode = userNode->FirstChildElement("Mobile");
//		user->mobile = mobileNode->GetText();
//		XMLElement* emailNode = userNode->FirstChildElement("Email");
//		user->email = emailNode->GetText();
//		return user;
//	}
//	return NULL;
//}

namespace xmlInterface
{ 
//获取XML中数据库的配置信息
	extern int getDBConfigInfo(XMLElement* root, const char* paraName, string& strOut);

//获取XML中指定节点的属性信息
	extern int getAttributeInfo(XMLElement* curShcemeNode, const char* paraName, string& strOut);

//获取XML中scheme节点的个数
	extern int getSchemeNodeCount(const char* xmlPath);

	extern string getStringConfig(const char* xmlPath, const char* nodeName, const char* attributeName);

//获取XML中input_param节点的个数
	extern int getInputParamNodeCount(XMLElement* curShcemeNode);

//获取XML中output_param节点的个数
	extern int getOutputParamNodeCount(XMLElement* curShcemeNode);

}// namespace end