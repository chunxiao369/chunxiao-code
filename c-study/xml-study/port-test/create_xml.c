
/*********************************************************************
    created:   2010/04/04
    filename:  CreateXmlFile.cpp
    author:    xuchunxiao
    depend:    libxml2.lib
    build:     nmake TARGET_NAME=CreateXmlFile
    purpose:   创建一个xml文件
*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>

int main()
{
    xmlDocPtr doc;
    xmlNodePtr root_node;
    xmlNodePtr node;
    xmlNodePtr content;
    int i = 0;
    char buf[32] = {0};
    //xmlNodePtr grandson;
    //定义文档和节点指针
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "root");
    //设置根节点
    xmlDocSetRootElement(doc, root_node);
    //创建一个节点，设置其内容和属性，然后加入根结点
    node = xmlNewNode(NULL, BAD_CAST "port_num");
    xmlAddChild(root_node, node);
    xmlNewProp(node, BAD_CAST "port_num", BAD_CAST "16");
    //创建一个节点，设置其内容和属性，然后加入根结点
    for (i = 1; i <= 16; i++) {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "port%d", i); 
        node = xmlNewNode(NULL, BAD_CAST buf);
        content = xmlNewText(BAD_CAST "NODE CONTENT");
        xmlAddChild(root_node, node);
        xmlAddChild(node, content);
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%d", i + 9); 
        xmlNewProp(node, BAD_CAST "want_vlan_id", BAD_CAST buf);
    }
    //创建一个儿子和孙子节点
    /*
    node = xmlNewNode(NULL, BAD_CAST "son");
    xmlAddChild(root_node, node);
    grandson = xmlNewNode(NULL, BAD_CAST "grandson");
    xmlAddChild(node, grandson);
    xmlAddChild(grandson, xmlNewText(BAD_CAST "This is a grandson node"));
    */
    //存储xml文档
    int nRel = xmlSaveFormatFileEnc("port_cfg.xml", doc, "UTF-8", 1);
    if (nRel != -1) {
        printf("一个xml文档被创建,写入%d个字节.\n", nRel);
    }
    //释放文档内节点动态申请的内存
    xmlFreeDoc(doc);

    return 1;
}
