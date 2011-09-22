#define NUM 13
#include <stdio.h>
#include <stdlib.h>
#include "test.h"

static string_function cmd_list[] = {
	{"exit",NULL},
	{"init", init_list},
	{"destroy", destroy_list},
	{"clear", clear_list},
	{"isempty", list_empty},
	{"length", list_length},
	{"getelem", get_element},
	{"locelem", locate_element},
	{"priorelem", prior_element},
	{"nextelem", next_element},
	{"getelenode", get_elem_node},
	{"insert", list_insert},
	{"delete", list_delete},
	{"show",},
};

int find_command_order(char *q);
int get_cmd_head(char *p);
int trim(char *p);
int cmd_process(char *cmd, char ***cmd_parameter);
int list_insert_wrapper(List * list, char **cmd_parameter);
int list_delete_wrapper(List * list, char **cmd_parameter);
int show(List list);

int main(int argc, char *argv[])
{
	int a = 10, b = 20, c = 30, d = 40;
	int len = -1;
	int i = 0;
	int flag = -1;
	int order = 1000;
	List list;
	char *cmd = (char *)malloc(sizeof(char) * 100);
	char *q = (char *)malloc(sizeof(char) * 100);

	char **cmd_parameter;

	test();
	printf("start test...\n");

	printf("CLI>");
	//gets(cmd);
    scanf("%s",cmd);
	//printf("p1 = %s\n",cmd);
	trim(cmd);
	strcpy(q, cmd);
	len = cmd_process(q, &cmd_parameter);
	if (len == 0) {
		printf("cmd length is %d\n", len);
		get_cmd_head(q);
		order = find_command_order(q);
	} else {
		//printf("cmd = %s\n",cmd);
		//printf("q = %s\n",q);
		printf("cmd length is %d\n", len);
		order = find_command_order(cmd_parameter[0]);
		//printf("order = %d\n",order);
	}

	while (1) {
		switch (order) {
		case 0:
			if (list != NULL)
				destroy_list(&list);
			exit(0);
			break;
		case 1:
			flag = init_list(&list);
			if (flag == 1)
				printf("[OK]\n");
			break;
		case 2:
			destroy_list(&list);
			if (list == NULL)
				printf("[OK]\n");
			break;
		case 3:
			clear_list(list);
			if (list->next == NULL && list->prev == NULL)
				printf("[OK]\n");
			break;
		case 4:
			flag = list_empty(list);
			if (flag == -1)
				printf("The list isn't exist!\n");
			else if (flag == 0)
				printf("The list is empty!");
			else
				printf("The list isn't empty!\n");
			break;
		case 5:
			len = list_length(list);
			printf("len = %d\n", len);
			break;

		case 11:	//printf("The main:cmd_parameter is %s\n",(*cmd_parameter)[2]);
			printf("1");
			list_insert_wrapper(&list, cmd_parameter);
			printf("2");
			break;
		/*

		case 6:
			//get_element();
			break;
		*/
			/*
			   case 7: locatelement();
			   break;
			   case 8: priorelement();
			   break;
			   case 9: nextelement();
			   break;
			   case 10:getelemnode();

			   //case 11:
			 */
		case 12:
			list_delete_wrapper(&list, cmd_parameter);
			break;
		case 13:
			show(list);
			break;
		/*
		default:
			printf("Input wrong cmd,Please input again!");
			break;
		*/
		}
		printf("\nCLI>");
		//gets(cmd);
        scanf("%s",cmd);
		trim(cmd);
		strcpy(q, cmd);
		len = cmd_process(q, &cmd_parameter);
		if (len == 0) {
			get_cmd_head(q);
			order = find_command_order(q);
		} else
			order = find_command_order(cmd_parameter[0]);
		//printf("order = %d\n",order);
	}
	free(cmd);
	free(q);
	free(cmd_parameter);
}

//****************************************************************
int find_command_order(char *q)
{
	int i = 0;
	int len = NUM + 1;
	for (; i < len; i++) {
		//printf("q = %s\n",q);
		//printf("cmd_list = %s\n",cmd_list[i].cmd_string);
		if (strcmp(q, cmd_list[i].cmd_string) == 0)
			break;
	}
	if (i < len)
		return i;
	else
		return 1000;
}

int get_cmd_head(char *p)
{
	int i = 0;
	for (; i < strlen(p); i++) {
		if (p[i] == ' ') {
			p[i] = '\0';
			break;
		}

	}
	//printf("first = %s\n",p);

	return i;
}

int trim(char *p)
{

	char *pcpy, *tail = NULL;
	//printf("trim = %s",p);
	if (p == NULL)
		return 0;
	for (pcpy = p; *p; p++) {
		if (*p != ' ' && *p != '\t') {
			*pcpy++ = *p;
			tail = pcpy;
		} else {
			if (tail)
				*pcpy++ = *p;
		}
	}
	if (tail)
		*tail = 0;
	else
		*pcpy = 0;
	return 1;
}

int cmd_process(char *cmd, char ***cmd_parameter)
{
	int len = 0;
	int i = 0;
	char *p = cmd, *q;
	for (; *p; p++)
		if (*p == ' ')
			len++;
	if (len != 0) {
		cmd_parameter = (char ***)malloc(sizeof(char **));
		*cmd_parameter = (char **)malloc(sizeof(char *) * (len + 1));
		p = (char *)malloc(sizeof(char) * 100);
		strcpy(p, cmd);
		q = p;
		for (; *q; q++) {
			p = q;
			while (*q != ' ' && *q != '\0')
				q++;
			if (*q == '\0') {
				len = q - p;
				//printf("string's len is %d\n",len);
				(*cmd_parameter)[i] = (char *)malloc(sizeof(char) * 50);	//*********
				(*cmd_parameter)[i++] = p;
				break;
			} else {
				len = q - p;
				(*cmd_parameter)[i] =
				    (char *)malloc(sizeof(char) * 50);
				*q = '\0';
				(*cmd_parameter)[i++] = p;
				p = q;
			}
		}

		(*cmd_parameter)[i] = (char *)malloc(sizeof(char) * 50);
		(*cmd_parameter)[i][0] = '\0';

	}

	/*
	 * for (--i;i>=0;i--)
	 printf("parameter[%d] = %s\n",i,(*cmd_parameter)[i]);
	 */
	//free(cmd_parameter);
	return i;

}

int list_insert_wrapper(List * list, char **cmd_parameter)
{
	int i = 0;
	while (cmd_parameter[i] != NULL) {
		printf("The cmd is %s\n", cmd_parameter[i++]);
		//printf("The cmd is %s\n",cmd_parameter[2]);
	}
	if (!strcmp(cmd_parameter[1], "-l")) {
		int l = 0;
		if (*(cmd_parameter[2]) == 'h')
			l = 0;
		else if (*(cmd_parameter[2]) == 't')
			l = list_length(*list);
		else {
			int i = 0;
			while (*(cmd_parameter[2] + i) != '\0')
				l = l * 10 + (*(cmd_parameter[2] + i++) - 48);
		}
		printf("The insert location is %d\n", l);

		//read the date for the cmd,and insert in the dllist
		if (!strcmp(cmd_parameter[3], "-i")) {
			//int *e = (int *)malloc(sizeof(int));
			int cpi = 4;
			while (*(cmd_parameter[cpi]) != '\0') {
				int i = 0;
				int *e = (int *)malloc(sizeof(int));
				while (*(cmd_parameter[cpi] + i) != '\0') {
					(*e) =
					    (*e) * 10 +
					    (*(cmd_parameter[cpi] + i++) - 48);
				}
				list_insert(list, l++, (element_type) e);
				cpi++;
			}

		} else if (!strcmp(cmd_parameter[3], "-f")) {
			//read the file to insert
		} else if (!strcmp(cmd_parameter[3], "-r")) {
			//random date
			int num = 0, i = 0, j = 0;
			//int *e = (int *)malloc(sizeof(int));
			printf("Hello\n");
			while (*(cmd_parameter[4] + j) != '\0') {
				num =
				    num * 10 + (*(cmd_parameter[4] + j++) - 48);
			}
			printf("The numbers is %d\n", num);
			for (; i < num; i++) {
				int *e = (int *)malloc(sizeof(int));
				*e = rand();
				list_insert(list, l++, (element_type) e);
			}

		}

		return 1;

	} else
		return -1;

	//return 1;

}

int list_delete_wrapper(List * list, char **cmd_parameter)
{

	if (!strcmp(cmd_parameter[1], "-c")) {
		if (!strcmp(cmd_parameter[2], "-l")) {
			int numj = 0, numi = 0, i = 0, j = 0;
			while (*(cmd_parameter[3] + j) != '\0')
				numj =
				    numj * 10 + (*(cmd_parameter[3] + j++) -
						 48);
			printf("The numj is done,%d\n", numj);
			if (!strcmp(cmd_parameter[4], "-n")) {
				while (*(cmd_parameter[5] + i) != '\0') {
					numi =
					    numi * 10 +
					    (*(cmd_parameter[5] + i++) - 48);
				}
				printf("The numi is done,%d\n", numi);
			}
			int *e = (int *)malloc(sizeof(int));
			//printf("Here!\n");
			for (i = 0; i < numi; i++) {
				list_delete(list, numj, (element_type *) (&e));
				printf("The deleted e is %d\n", *e);
			}
			//printf("done!\n");
		} else {
			printf("The cmd is wrong!\n");
			return -1;
		}
	} else if (!strcmp(cmd_parameter[1], "-u")) {
		int cpi = 2;
		int del = 0;
		while (*(cmd_parameter[cpi]) != '\0') {
			int i = 0;
			int l = 0;
			int *e = (int *)malloc(sizeof(int));
			while (*(cmd_parameter[cpi] + i) != '\0') {
				l = l * 10 + (*(cmd_parameter[cpi] + i++) - 48);
			}
			list_delete(list, (l - del), (element_type *)(&e));
			printf("D %d yuanshu\n",l);
			del++;
			cpi++;
		}
	} else {
		printf("The cmd is worng!\n");
		return -1;
	}
	return 1;

}

int show(List list)
{
	int i = 1;
	int len = list_length(list);
	ptr_to_node p;
	printf("The dllist is\n");
	for (; i <= len; i++) {
		p = get_elem_node(list, i);
		printf("%d\t", *((int *)p->element));

		if (i % 5 == 0)
			printf("\n");
	}
	printf("\n");
}
