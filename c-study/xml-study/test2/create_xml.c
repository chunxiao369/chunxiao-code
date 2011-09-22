
/*********************************************************************
    created:   2010/04/04
    filename:  CreateXmlFile.cpp
    author:    xuchunxiao
    depend:    libxml2.lib
    build:     nmake TARGET_NAME=CreateXmlFile
    purpose:   创建一个xml文件
*********************************************************************/
#include <stdio.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>

int main()
{
    xmlNodePtr node;
    xmlNodePtr content;
    //定义文档和节点指针
    xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr def_user = xmlNewNode(NULL, BAD_CAST "def_user");
    //设置根节点
    xmlDocSetRootElement(doc, def_user);
    //在根节点中直接创建节点
    //xmlNewTextChild(def_user, NULL, BAD_CAST "def_group", BAD_CAST "group_name");
    /*
    //创建一个节点，设置其内容和属性，然后加入根结点
    node = xmlNewNode(NULL, BAD_CAST "node");
    xmlNodePtr content = xmlNewText(BAD_CAST "NODE CONTENT");
    xmlAddChild(def_user, node);
    xmlAddChild(node, content);
    xmlNewProp(node, BAD_CAST "attribute", BAD_CAST "yes");
    */
    // def_group and a node aaa
    node = xmlNewNode(NULL, BAD_CAST "def_group");
    /*
    content = xmlNewText(BAD_CAST "NODE CONTENT");
    xmlAddChild(node, content);
    */

    xmlAddChild(def_user, node);

    xmlNodePtr grandson = xmlNewNode(NULL, BAD_CAST "aaa");
    xmlNewProp(grandson, BAD_CAST "slot", BAD_CAST "1");
    xmlNewProp(grandson, BAD_CAST "module", BAD_CAST "2");
    xmlAddChild(grandson, xmlNewText(BAD_CAST "node aaa"));

    xmlAddChild(node, grandson);

    // def_group and a node aaa
    node = xmlNewNode(NULL, BAD_CAST "haha_group");
    xmlAddChild(def_user, node);
    grandson = xmlNewNode(NULL, BAD_CAST "bbb");
    xmlNewProp(grandson, BAD_CAST "slot", BAD_CAST "2");
    xmlNewProp(grandson, BAD_CAST "module", BAD_CAST "3");
    xmlAddChild(grandson, xmlNewText(BAD_CAST "node bbb"));
    xmlAddChild(node, grandson);
    //存储xml文档
    int nRel = xmlSaveFormatFileEnc("group.xml", doc, "UTF-8", 1);
    if (nRel != -1) {
        printf("一个xml文档被创建,写入%d个字节.\n", nRel);
    }
    //释放文档内节点动态申请的内存
    xmlFreeDoc(doc);

    return 1;
}
