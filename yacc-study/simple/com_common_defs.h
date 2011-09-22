#ifndef __COM_COMMON_DEFS_H__
#define __COM_COMMON_DEFS_H__

#define RUN_ON_NBOX		100
#define RUN_ON_CARD		200

#ifndef RUN_TARGET
#define RUN_TARGET	RUN_ON_NBOX
#endif

#define FROM_LINUX_GROUP          	14
#define TO_LINUX_GROUP          	15

typedef enum {
	PA_STAP_HOST_ERRNO_INVALID_PARAM = 0x800300,
	PA_STAP_HOST_ERRNO_NO_MEMORY,
	PA_STAP_HOST_ERRNO_STATE_ERROR,
} pa_stap_host_errno_t;

#define PA_STAP_CONFIG_MASK_INIT            0x0001
#define PA_STAP_CONFIG_MASK_TIME_STAMP      0x0100
#define PA_STAP_CONFIG_MASK_DEL_RULE_ALL    0x0200
#define PA_STAP_CONFIG_MASK_MODE_CHANGE     0x0400

#define PA_STAP_5T_SIP                      0x10
#define PA_STAP_5T_DIP                      0x08
#define PA_STAP_5T_SPORT                    0x04
#define PA_STAP_5T_DPORT                    0x02
#define PA_STAP_5T_PROTOCOL                 0x01

#define PA_STAP_MAX_UD_NUM                  64
#define PA_STAP_USER_DEF_HEAD               0x01
#define PA_STAP_USER_DEF_L3                 0x02
#define PA_STAP_USER_DEF_L4                 0x04

#define PA_STAP_FIELD_MASK_SIGPRO           0x01
#define PA_STAP_FIELD_MASK_OPC              0x02
#define PA_STAP_FIELD_MASK_DPC              0x04

typedef struct {
	uint64_t flow_bucket;
	uint64_t flow_number;
	uint64_t timer_tick;
	uint64_t timer_max_tick;
	uint64_t maintain_once_time;
	uint64_t flow_expire_time;
	uint64_t imsi_number;
	uint64_t msisdn_number;
	
	uint64_t time_stamp_flag;
	uint64_t ingress_port_flag;
	uint64_t app_specific_flag;
} pa_stap_config_t;

typedef struct {
	uint64_t send_ok_pkts;
	uint64_t send_ok_bytes;
	uint64_t send_fail_pkts;
	uint64_t send_fail_bytes;
	uint64_t drop_user_pkts;
	uint64_t drop_user_bytes;
	uint64_t drop_error_pkts;
	uint64_t drop_error_bytes;
} pa_stap_counter_t;

typedef struct {
	uint64_t state;
	uint64_t rule_mask;
	pa_stap_config_t config;
	pa_stap_counter_t counter;
} pa_stap_info_t;

typedef enum {
	PA_STAP_MODIFY_OPTION_TIME_STAMP = 0x1,
	PA_STAP_MODIFY_OPTION_INGRESS_PORT = 0x2,
	PA_STAP_MODIFY_OPTION_APP_SPECIFIC = 0x4,
} pa_stap_modify_option_t;

typedef enum {
	PA_STAP_PROTOCOL_TYPE_UNKNOWN = 0,
	PA_STAP_PROTOCOL_TYPE_BSSAP   = 1,
	PA_STAP_PROTOCOL_TYPE_H248    = 2,
	PA_STAP_PROTOCOL_TYPE_RANAP   = 3,
	PA_STAP_PROTOCOL_TYPE_BICC    = 4,
	PA_STAP_PROTOCOL_TYPE_NUM     = 16,
} pa_stap_protocol_type_t;

typedef enum {
	PA_STAP_RULE_PARAM_MASK_PROTOCOL  = 0x1,
	PA_STAP_RULE_PARAM_MASK_INTERFACE = 0x2,
	PA_STAP_RULE_PARAM_MASK_STAGE1    = 0x4,
	PA_STAP_RULE_PARAM_MASK_ACTION    = 0x8,
	PA_STAP_RULE_PARAM_DEL_RULE       = 0x10,
} pa_stap_rule_param_mask_t;

typedef enum {
	PA_STAP_CLASSIFY_TYPE_ETHERTYPE = 0,
	PA_STAP_CLASSIFY_TYPE_TCP = 1,
	PA_STAP_CLASSIFY_TYPE_UDP = 2,
	PA_STAP_CLASSIFY_TYPE_NUM = 16,
} pa_stap_classify_type_t;

typedef struct {
	uint64_t action_type;
	uint64_t dispatch_mode;
} pa_stap_action_param_t;

typedef struct {
	uint64_t param_value;
	uint64_t reserve_01;
	uint64_t reserve_02;
	pa_stap_action_param_t action;
} pa_stap_rule_param_t;

typedef struct {
	uint64_t match_pkts;
	uint64_t match_bytes;
} pa_stap_rule_counter_t;

typedef struct {
	uint64_t field_flag;
	pa_stap_rule_param_t param;
	pa_stap_rule_counter_t counter;
} pa_stap_rule_info_t;


typedef enum {
	PA_STAP_CONDITION_OPCODE_ADD_VALUE = 0,
	PA_STAP_CONDITION_OPCODE_DEL_VALUE,
	PA_STAP_CONDITION_OPCODE_DEL_FIELD,
	PA_STAP_CONDITION_OPCODE_DEL_RULE,
	PA_STAP_CONDITION_OPCODE_DEL_ALL,
} pa_stap_condition_opcode_t;

typedef struct{
    uint32_t sip;
    uint32_t dip;
    uint16_t sport;
    uint16_t dport;
    uint8_t  protocol;  
	uint8_t  field_mask;
}pa_stap_five_tuple_t;

typedef struct{
	uint32_t ud_num;
	uint32_t ud_type;
	uint32_t offset;
}pa_stap_set_ud_t;

typedef struct{
	uint64_t ud_num_mask;
	uint8_t data[PA_STAP_MAX_UD_NUM];
	uint8_t mask[PA_STAP_MAX_UD_NUM];
}pa_stap_ud_rule_t;

#endif

