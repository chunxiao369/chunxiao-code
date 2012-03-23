    required int64 a10_found = 1
    required int64 a10_found_pps = 2
    required int64 a10_no_imsi = 3
    required int64 a10_no_imsi_pps = 4
    required int64 match_rule = 5
    required int64 total_pkts = 6
    required int64 total_bps = 7
    required int64 send_pkts = 8
    required int64 send_bps = 9

    required int64 not_gre = 11
    required int64 not_gre_pps = 12

    required int64 other = 21
    required int64 other_pps = 22

    required int64 port_drop = 31
    required int64 pip_drop = 32

    required int64 ipfrag = 41
    required int64 ipfrag_not_order = 42
    required int64 ipfrag_expired = 43
    required int64 ipfrag_error = 44
    required int64 ipfrag_ressembled = 45

    required int64 gre_release = 51
    required int64 gre_expire = 52
    required int64 radius_pkts = 53

    required int64 ppp_total = 54
    required int64 ppp_not_order = 55
    required int64 ppp_at_once = 56
    required int64 ppp_frag = 57
    required int64 ppp_frag_expire = 58
    required int64 ppp_frag_err = 59
    required int64 ppp_frag_rsmb = 60

    required int64 ppp_uncomp = 61
    required int64 ppp_mp_un = 62
    required int64 ppp_mp_cm = 63
    required int64 ppp_vj_un = 64
    required int64 ppp_vj_cm = 65
    required int64 ppp_mp_un_vj_un = 66
    required int64 ppp_mp_un_vj_cm = 67
    required int64 ppp_mp_cm_vj_un = 68
    required int64 ppp_mp_cm_vj_cm = 69

    required int64 ppp_mp_error = 71
    required int64 ppp_vj_error = 72
    required int64 ppp_other_error = 73
:1,11s/required\sint64\s\(.*\)\s=\s[0-9]\{1,2\}/rt->u.gre.\1 = gre->\1/g
