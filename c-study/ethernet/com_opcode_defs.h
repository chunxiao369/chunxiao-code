#ifndef __COM_OPCODE_DEFS_H__
#define __COM_OPCODE_DEFS_H__
#include "api.h"

typedef enum {
    PA_STAP_OPCODE_DUMMY = 0,
    
    PA_STAP_OPCODE_SET_CONFIG,
    PA_STAP_OPCODE_SET_UD,
    
    PA_STAP_OPCODE_FIVE_TUPLE,
    PA_STAP_OPCODE_UD_RULE,
    PA_STAP_OPCODE_SIGPROTO,
    
    PA_STAP_OPCODE_SET_RULE,
    PA_STAP_OPCODE_GET_RULE,
            
    PA_STAP_OPCODE_INGRP_CHANGE,
    PA_STAP_OPCODE_OUTGRP_CHANGE,
    
    PA_STAP_OPCODE_SET_TIMESTAMP,
    PA_STAP_OPCODE_GET_TIMESTAMP,
    
    PA_STAP_OPCODE_MAX_NUM,

} pa_stap_opcode_t;

typedef struct {
#if !defined(__linux__)
    cvmx_raw_inst_front_t instr;
#else
    char   instr[ 24 ];
#endif
} pa_stap_common_front_t;

typedef struct {
#if !defined(__linux__)
    cvmx_raw_inst_front_t instr;
#else
    char   instr[ 24 ];
#endif
    uint64_t config_mask;
    pa_stap_config_t config;
} pa_stap_set_config_front_t;

typedef struct {
#if !defined(__linux__)
    cvmx_raw_inst_front_t instr;
#else
    char     instr[ 24 ];
#endif
    uint64_t opcode;
    pa_stap_set_ud_t ud;
} pa_stap_set_ud_front_t;

typedef struct {
#if !defined(__linux__)
    cvmx_raw_inst_front_t instr;
#else
    char   instr[ 24 ];
#endif
    uint64_t rule_id;
    uint64_t param_mask;
    pa_stap_rule_param_t param;
} pa_stap_set_rule_front_t;

typedef struct {
#if !defined(__linux__)
    cvmx_raw_inst_front_t instr;
#else
    char   instr[ 24 ];
#endif
    uint64_t rule_id;
    uint64_t clear_counter;
} pa_stap_get_rule_front_t;


typedef struct {
#if !defined(__linux__)
    cvmx_raw_inst_front_t instr;
#else
    char   instr[ 24 ];
#endif
    uint64_t timestamp;
    uint64_t flag;
} pa_stap_set_timestamp_front_t;

typedef struct {
#if !defined(__linux__)
    cvmx_raw_inst_front_t instr;
#else
    char     instr[ 24 ];
#endif
    uint64_t opcode;
    uint64_t rule_id;
    pa_stap_five_tuple_t five_tuple;
} pa_stap_five_tuple_front_t;

typedef struct {
#if !defined(__linux__)
    cvmx_raw_inst_front_t instr;
#else
    char     instr[ 24 ];
#endif
    uint64_t opcode;
    uint64_t rule_id;
    pa_stap_ud_rule_t ud;
} pa_stap_ud_rule_front_t;

typedef struct {
#if !defined(__linux__)
    cvmx_raw_inst_front_t instr;
#else
    char     instr[ 24 ];
#endif
    uint64_t opcode;
    uint64_t rule_id;
    uint32_t field_mask;
    uint32_t sigproto;
    uint32_t opc;
    uint32_t dpc;
} pa_stap_sigproto_front_t;

typedef struct {
#if !defined(__linux__)
    cvmx_raw_inst_front_t instr;
#else
    char     instr[ 24 ];
#endif
    uint64_t rule_id;
    uint64_t inportmap[PORTMAP_ARRAY];
} pa_stap_rule_ingroup_front_t;


typedef struct {
#if !defined(__linux__)
    cvmx_raw_inst_front_t instr;
#else
    char     instr[ 24 ];
#endif
    uint64_t rule_id;
    uint64_t outportmap;
    uint64_t dispatch_mode;
} pa_stap_rule_outgroup_front_t;


typedef pa_stap_common_front_t pa_stap_get_timestamp_front_t;

/*********************************************************
*
*                              response struct 
*
*********************************************************/
typedef struct {
#if !defined(__linux__)
    cvmx_raw_inst_front_t instr;
#else
    char   instr[ 24 ];
#endif
    uint64_t status;
} pa_stap_common_resp_t;

typedef pa_stap_common_resp_t pa_stap_set_config_resp_t;

typedef pa_stap_common_resp_t pa_stap_set_ud_resp_t;

typedef pa_stap_common_resp_t pa_stap_set_rule_resp_t;

typedef struct {
#if !defined(__linux__)
    cvmx_raw_inst_front_t instr;
#else
    char   instr[ 24 ];
#endif
    pa_stap_rule_info_t info;
    uint64_t status;
} pa_stap_get_rule_resp_t;

typedef pa_stap_common_resp_t pa_stap_set_timestamp_resp_t;

typedef struct {
#if !defined(__linux__)
    cvmx_raw_inst_front_t instr;
#else
    char   instr[ 24 ];
#endif
    uint64_t timestamp;
    uint64_t status;
} pa_stap_get_timestamp_resp_t;

typedef pa_stap_common_resp_t pa_stap_five_tuple_resp_t;
typedef pa_stap_common_resp_t pa_stap_ud_rule_resp_t;
typedef pa_stap_common_resp_t pa_stap_sigproto_resp_t;
typedef pa_stap_common_resp_t pa_stap_rule_ingroup_resp_t;
typedef pa_stap_common_resp_t pa_stap_rule_outgroup_resp_t;

#endif

