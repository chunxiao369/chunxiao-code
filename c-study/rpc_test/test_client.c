/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "test.h"


void
testprog_1(char *host)
{
             CLIENT *clnt;
             char * *result_1;
             char * test_1_arg;

             test_1_arg = (char *)malloc(128);
#ifndef DEBUG
             clnt = clnt_create (host, TESTPROG, VERSION, "udp");
             if (clnt == NULL) {
                     clnt_pcreateerror (host);
                     exit (1);
             }
#endif       /* DEBUG */

             result_1 = test_1(&test_1_arg, clnt);
             if (result_1 == (char **) NULL) {
                     clnt_perror (clnt, "call failed");
             }
             if (strcmp(*result_1, "Error") == 0) {
                     fprintf(stderr, "%s: could not get the time\n", host);
                     exit(1);
             }
             printf("收到消息 ... %s\n", *result_1);
#ifndef DEBUG
             clnt_destroy (clnt);
#endif        /* DEBUG */
}

int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	testprog_1 (host);
exit (0);
}
