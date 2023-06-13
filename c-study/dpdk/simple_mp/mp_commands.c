/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation
 */
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <inttypes.h>
#include <stdio.h>
#include <termios.h>
#include <errno.h>
#include <sys/queue.h>

#include <rte_common.h>
#include <rte_errno.h>
#include <rte_memory.h>
#include <rte_eal.h>
#include <rte_branch_prediction.h>
#include <rte_launch.h>
#include <rte_log.h>
#include <rte_per_lcore.h>
#include <rte_lcore.h>
#include <rte_ring.h>
#include <rte_debug.h>
#include <rte_mempool.h>
#include <rte_string_fns.h>

#include <cmdline_rdline.h>
#include <cmdline_parse.h>
#include <cmdline_parse_string.h>
#include <cmdline_socket.h>
#include <cmdline.h>
#include "mp_commands.h"

/**********************************************************/

struct cmd_send_result {
	cmdline_fixed_string_t action;
	cmdline_fixed_string_t message;
};

#define CMD_TEST  "embedway_test_string"
#define CMD_ASYNC "embedway_async"
enum sal_req_result {
	REQ_RESULT_SUCCESS,
	REQ_RESULT_FAIL
};

struct sal_emsg_t {
    int t;
    int id;
    int result;
};
static int
handle_sync_response(const struct rte_mp_msg *request,
		const struct rte_mp_reply *reply )
{
    int i = 0;
	struct sal_emsg_t *mpreq = (struct sal_emsg_t *)request->param;

    printf("req............msg id: %d.\n", mpreq->id);
    printf("req............msg t : %d.\n\n", mpreq->t);
    printf("rsp............recv num: %d.\n", reply->nb_received);
    for (i = 0; i < reply->nb_received; i++ ) {
        mpreq = (struct sal_emsg_t *)reply->msgs[i].param;
        printf("rsp............msg  t : %d.\n", mpreq->t);
        printf("rsp............msg id : %d.\n", mpreq->id);
    }

	return 0;
}

int sal_mp_send_async(void)
{
    int ret = 0;
    struct rte_mp_msg sr_msg;
    struct sal_emsg_t *sr = (struct sal_emsg_t*)sr_msg.param;
    struct timespec ts;

    memset(&sr_msg, 0, sizeof(sr_msg));

    /* we can do something, so send sync request asynchronously */
    sr_msg.num_fds = 0;
    sr_msg.len_param = sizeof(*sr);
    strlcpy(sr_msg.name, CMD_ASYNC, sizeof(sr_msg.name));
    sr->t = 6;
    sr->id = 666;

    ts.tv_nsec = 0;
    ts.tv_sec = 5;

    do {
        ret = rte_mp_request_async(&sr_msg, &ts, handle_sync_response);
    } while (ret != 0 && rte_errno == EEXIST);
    if (ret != 0) {
        printf("Couldn't send sync request\n");
        return -1;
    }
    return 0;
}

int sal_mp_send_common(void)
{
    struct rte_mp_msg resp_msg;
    struct sal_emsg_t *resp = (struct sal_emsg_t *)resp_msg.param;

    resp_msg.num_fds = 0;
    resp_msg.len_param = sizeof(*resp);
    strlcpy(resp_msg.name, CMD_TEST, sizeof(resp_msg.name));

    resp->t = 2;
    resp->id = 222;
    resp->result = REQ_RESULT_FAIL;

    if (rte_mp_sendmsg(&resp_msg)) {
        printf("Couldn't send response\n");
        return -1;
    }
    return 0;
}

int recv_cnt = 0;
int cur_id = 0;

static int handle_response(const struct rte_mp_msg *msg, const void *peer  __rte_unused)
{
    const struct sal_emsg_t *m = (const struct sal_emsg_t *)msg->param;
    recv_cnt++;
    cur_id = m->id;
    /*
    printf("recv      t: %d\n", m->t);
    printf("recv     id: %d\n", m->id);
    printf("recv result: %d\n", m->result);
    */
	return 0;
}

static int handle_sync(const struct rte_mp_msg *msg, const void *peer)
{
    struct rte_mp_msg reply;
    const struct sal_emsg_t *req =
        (const struct sal_emsg_t *)msg->param;
    struct sal_emsg_t *resp =
        (struct sal_emsg_t *)reply.param;
    int ret;

    if (req->t != 6) {
        printf("Unexpected request from primary\n");
        return -1;
    }

    memset(&reply, 0, sizeof(reply));
    recv_cnt++;
    cur_id = req->id;

    reply.num_fds = 0;
    strlcpy(reply.name, msg->name, sizeof(reply.name));
    reply.len_param = sizeof(*resp);

    resp->t = 7;
    resp->id = 7777;
    resp->result = 0;

    return rte_mp_reply(&reply, peer);
}

int sal_mp_init(void)
{
    if (rte_mp_action_register(CMD_TEST, handle_response)) {
        printf("Couldn't register '%s' action\n", CMD_TEST);
        return -1;
    }
    if (rte_mp_action_register(CMD_ASYNC, handle_sync)) {
        printf("Couldn't register '%s' action\n", CMD_ASYNC);
        return -1;
    }
    return 0;
}
static void cmd_send_parsed(void *parsed_result,
		__rte_unused struct cmdline *cl,
		__rte_unused void *data)
{
	void *msg = NULL;
	struct cmd_send_result *res = parsed_result;

	if (rte_mempool_get(message_pool, &msg) < 0)
		rte_panic("Failed to get message buffer\n");
	strlcpy((char *)msg, res->message, STR_TOKEN_SIZE);
	if (rte_ring_enqueue(send_ring, msg) < 0) {
		printf("Failed to send message - message discarded\n");
		rte_mempool_put(message_pool, msg);
	}
    if (strcmp(res->message, "common") == 0) {
        sal_mp_send_common();
    } else if (strcmp(res->message, "async") == 0) {
        sal_mp_send_async();
    } else {
    }
}

cmdline_parse_token_string_t cmd_send_action =
	TOKEN_STRING_INITIALIZER(struct cmd_send_result, action, "send");
cmdline_parse_token_string_t cmd_send_message =
	TOKEN_STRING_INITIALIZER(struct cmd_send_result, message, NULL);

cmdline_parse_inst_t cmd_send = {
	.f = cmd_send_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "send a string to another process",
	.tokens = {        /* token list, NULL terminated */
			(void *)&cmd_send_action,
			(void *)&cmd_send_message,
			NULL,
	},
};

/**********************************************************/

struct cmd_quit_result {
	cmdline_fixed_string_t quit;
};

static void cmd_quit_parsed(__rte_unused void *parsed_result,
			    struct cmdline *cl,
			    __rte_unused void *data)
{
	quit = 1;
	cmdline_quit(cl);
}

cmdline_parse_token_string_t cmd_quit_quit =
	TOKEN_STRING_INITIALIZER(struct cmd_quit_result, quit, "quit");

cmdline_parse_inst_t cmd_quit = {
	.f = cmd_quit_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "close the application",
	.tokens = {        /* token list, NULL terminated */
		(void *)&cmd_quit_quit,
		NULL,
	},
};

/**********************************************************/

struct cmd_cnt_result {
	cmdline_fixed_string_t cnt;
};

static void cmd_cnt_parsed(__rte_unused void *parsed_result,
			    struct cmdline *cl,
			    __rte_unused void *data)
{
    printf("-----------------------\n");
    printf("rev_cnt: %d\n", recv_cnt);
    printf("cur_id: %d\n", cur_id);
}

cmdline_parse_token_string_t cmd_cnt_cnt =
	TOKEN_STRING_INITIALIZER(struct cmd_cnt_result, cnt, "cnt");

cmdline_parse_inst_t cmd_cnt = {
	.f = cmd_cnt_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "show cnt",
	.tokens = {        /* token list, NULL terminated */
		(void *)&cmd_cnt_cnt,
		NULL,
	},
};


/**********************************************************/

struct cmd_help_result {
	cmdline_fixed_string_t help;
};

static void cmd_help_parsed(__rte_unused void *parsed_result,
			    struct cmdline *cl,
			    __rte_unused void *data)
{
	cmdline_printf(cl, "Simple demo example of multi-process in RTE\n\n"
			"This is a readline-like interface that can be used to\n"
			"send commands to the simple app. Commands supported are:\n\n"
			"- send [string]\n" "- help\n" "- quit\n\n");
}

cmdline_parse_token_string_t cmd_help_help =
	TOKEN_STRING_INITIALIZER(struct cmd_help_result, help, "help");

cmdline_parse_inst_t cmd_help = {
	.f = cmd_help_parsed,  /* function to call */
	.data = NULL,      /* 2nd arg of func */
	.help_str = "show help",
	.tokens = {        /* token list, NULL terminated */
		(void *)&cmd_help_help,
		NULL,
	},
};

/****** CONTEXT (list of instruction) */
cmdline_parse_ctx_t simple_mp_ctx[] = {
		(cmdline_parse_inst_t *)&cmd_send,
		(cmdline_parse_inst_t *)&cmd_cnt,
		(cmdline_parse_inst_t *)&cmd_quit,
		(cmdline_parse_inst_t *)&cmd_help,
	NULL,
};
