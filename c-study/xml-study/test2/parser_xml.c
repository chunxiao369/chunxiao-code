/********************************************************************
    created:   2007/11/15
    created:   15:11:2007   11:47
    filename:  ParseXmlFile.cpp
    author:    xuchunxiao 
    depend:    libxml2.lib
    build:     nmake TARGET_NAME=ParseXmlFile
    purpose:   解析xml文件
*********************************************************************/
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
int main(int argc, char *argv[])
{
    xmlDocPtr doc;              //定义解析文件指针
    xmlNodePtr curNode;         //定义结点指针(你需要他为了在各个结点间移动)
    xmlNodePtr SubcurNode;      //定义子结点指针(你需要他为了在各个结点间移动)
    xmlChar *szKey;             //临时字符串变量
    char *szDocName;
    if (argc != 2) {
        printf("Usage: %s docname\n", argv[0]);
        return (0);
    }
    szDocName = argv[1];
    doc = xmlReadFile(szDocName, "GB2312", XML_PARSE_RECOVER);  //解析文件
    //检查解析文件是否成功，如果不成功，libxml将指一个注册的错误并停止。
    //一个常见错误是不适当的编码。XML标准文件除了用UTF-8或UTF-16外还可用其他编码保存。
    //如果文件是这样，libxml将自动地为你转换到UTF-8。更多关于XML编码信息包含在XML标准中.
    if (NULL == doc) {
        fprintf(stderr, "Document not parsed successfully. \n");
        return -1;
    }
    curNode = xmlDocGetRootElement(doc);    //确定文件根元素
    /*检查确认当前文件中包含内容 */
    if (NULL == curNode) {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(doc);
        return -1;
    }
    /*在这个例子中，我们需要确认文件是正确的类型。“def_user”是在这个示例中使用文件的根类型。 */
    if (xmlStrcmp(curNode->name, BAD_CAST "def_user")) {
        fprintf(stderr, "document of the wrong type, root node != root");
        xmlFreeDoc(doc);
        return -1;
    }
    curNode = curNode->xmlChildrenNode;
    while (curNode != NULL) {
        //取出节点中的内容
        if (xmlNodeIsText(curNode)) {
        //if ((!xmlStrcmp(curNode->name, (const xmlChar *)"text"))) {
            curNode = curNode->next;
            continue;
        }
        printf("group name: %s.\n", curNode->name);
        /*
        szKey = xmlNodeGetContent(curNode);
        printf("    content: %s\n", szKey);
        xmlFree(szKey);
        */
        SubcurNode = curNode->xmlChildrenNode;
        while (SubcurNode != NULL) {
            if (xmlNodeIsText(SubcurNode)) {
                SubcurNode = SubcurNode->next;
                continue;
            }
            printf("    node name: %s.\n", SubcurNode->name);
            //查找带有属性attribute的节点
            xmlAttrPtr attrPtr = SubcurNode->properties;
            if (attrPtr != NULL) {
                xmlChar *szAttr = xmlGetProp(SubcurNode, BAD_CAST "slot");
                printf("        attribute slot: %s.\n", szAttr);
                xmlFree(szAttr);
                szAttr = xmlGetProp(SubcurNode, BAD_CAST "module");
                printf("        attribute module: %s.\n", szAttr);
                xmlFree(szAttr);
            }
           SubcurNode = SubcurNode->next;
        }
        curNode = curNode->next;
    }
    //查找属性
    xmlFreeDoc(doc);
    return 0;
}
