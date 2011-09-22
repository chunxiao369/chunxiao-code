#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>

typedef struct _port_trans {
    int cli_port;
    int want_vlan;
    uint64_t bcm_pmap;
} port_trans_t;

int main(int argc, char *argv[])
{
    xmlDocPtr doc;
    xmlNodePtr curNode;
    xmlChar *szKey;
    xmlChar *szAttr;
    char *szDocName;
    int port;
    int port_num;
    int w_vlan;
    int i = 0;
    uint64_t port_map;
    port_trans_t *port_array = NULL;

    if (argc != 2) {
        printf("Usage: %s docname\n", argv[0]);
        return (0);
    }
    szDocName = argv[1];
    doc = xmlReadFile(szDocName, "utf-8", XML_PARSE_RECOVER);
    if (NULL == doc) {
        fprintf(stderr, "Document not parsed successfully. \n");
        return -1;
    }
    curNode = xmlDocGetRootElement(doc);
    if (NULL == curNode) {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(doc);
        return -1;
    }
    if (xmlStrcmp(curNode->name, BAD_CAST "root")) {
        fprintf(stderr, "document of the wrong type, root node != root");
        xmlFreeDoc(doc);
        return -1;
    }
    curNode = curNode->xmlChildrenNode;
    xmlNodePtr propNodePtr = curNode;
    while (curNode != NULL) {
        if (strcmp((char *)curNode->name, "text") == 0) {
            curNode = curNode->next;
            continue;
        }
        if (xmlHasProp(curNode, BAD_CAST "port_num")) {
            propNodePtr = curNode;
            szAttr = xmlGetProp(propNodePtr, BAD_CAST "port_num");
            //printf("                port num: %s.\n", szAttr);
            port_num = atoi((char *)szAttr);
            port_array = malloc(port_num * sizeof(port_trans_t));
            xmlFree(szAttr);
            i = 0;
            curNode = curNode->next;
            continue;
        }
        if (xmlHasProp(curNode, BAD_CAST "want_vlan_id")) {
            //printf("node name: %s.\n", curNode->name);
            port = atoi((char *)curNode->name + strlen("port"));
            port_array[i].cli_port = port;

            szKey = xmlNodeGetContent(curNode);
            //printf("     content: %s\n", szKey);
            port_map = strtoul((char *)szKey, NULL, 16);
            port_array[i].bcm_pmap = port_map;
            xmlFree(szKey);

            propNodePtr = curNode;
            szAttr = xmlGetProp(propNodePtr, BAD_CAST "want_vlan_id");
            //printf("                want vlan id: %s.\n", szAttr);
            w_vlan = atoi((char *)szAttr);
            port_array[i].want_vlan = w_vlan;
            i++;
            xmlFree(szAttr);
        }
        curNode = curNode->next;
    }
    for (i = 0; i < port_num; i++) {
            printf("%d  ", port_array[i].cli_port);
            printf("%#llx  ", port_array[i].bcm_pmap);
            printf("%d\n", port_array[i].want_vlan);
    }
    xmlFreeDoc(doc);
    free(port_array);
    return 0;
}
