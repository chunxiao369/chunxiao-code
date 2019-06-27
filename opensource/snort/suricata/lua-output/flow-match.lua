flow_all = 0
flow_match = 0
udp_flow_num= 0
tcp_flow_num= 0
flow_dic={}
match_percent=0
function init (args)
    local needs = {}
    needs["filter"] = "alerts"
    needs["type"] = "flow"
    return needs
end

function setup (args)
    flow_all = 0
    flow_match = 0
	udp_flow_num=0
	tcp_flow_num=0
    flow_dic={}
    match_percent=0
end

function log(args)
    ts = SCPacketTimeString()
    alproto = SCFlowAppLayerProto()
    --if alproto ~= nil then
    --    print (alproto)
    --end
    ipver, srcip, dstip, proto, sp, dp = SCFlowTuple()
    flow_all = flow_all + 1
    has_alerts = SCFlowHasAlerts()
    tscnt, tsbytes, tccnt, tcbytes = SCFlowStats()
    	
    flowid = SCFlowId()
    idstr = string.format("%.0f",flowid)
	--print ("idstr" .. idstr) 
	
    if flow_dic[idstr]==nil then
        --print ("no key")
		if proto == 6 then
			flow_type="tcp"
			flow_num=tcp_flow_num
		end
		
		if proto == 17 then
			flow_type="udp"
			flow_num=tcp_flow_num
		end
		
        flow_dic[idstr]={
        no_match_tscnt=0,
        no_match_tccnt=0,
        no_match_tsbytes=0,
        no_match_tcbytes=0,
        match_tscnt=0,
        match_tccnt=0,
        match_tsbytes=0,
        match_tcbytes=0,
		flowtuple={srcip, dstip, proto, sp, dp},
		flow_sque={flow_type,flow_num}
        }
		
		if flow_type=="tcp" then
			tcp_flow_num=tcp_flow_num+1
		elseif 	flow_type=="udp" then
			udp_flow_num=udp_flow_num+1
		end
		 
    end
    
    current_tscnt=tscnt-flow_dic[idstr]['no_match_tscnt']-flow_dic[idstr]['match_tscnt']
    current_tccnt=tccnt-flow_dic[idstr]['no_match_tccnt']-flow_dic[idstr]['match_tccnt']
    current_tsbytes=tsbytes-flow_dic[idstr]['no_match_tsbytes']-flow_dic[idstr]['match_tsbytes']
    current_tcbytes=tcbytes-flow_dic[idstr]['no_match_tcbytes']-flow_dic[idstr]['match_tcbytes']
    
    --print ("19/05/29-11:37:04", idstr, srcip, dstip, sp, dp, proto, 
    --       current_tscnt, current_tsbytes, current_tccnt, current_tcbytes, 1, "command",
    --       "username", "segs", "sessionid")
	
    if has_alerts then
        flow_match = flow_match + 1
        flow_dic[idstr]['match_tscnt']=current_tscnt+flow_dic[idstr]['match_tscnt']
        flow_dic[idstr]['match_tccnt']=current_tccnt+flow_dic[idstr]['match_tccnt']
        flow_dic[idstr]['match_tsbytes']=current_tsbytes+flow_dic[idstr]['match_tsbytes']
        flow_dic[idstr]['match_tcbytes']=current_tcbytes+flow_dic[idstr]['match_tcbytes']
        --print ("match:    " .. current_tscnt, current_tsbytes, current_tccnt, current_tcbytes)
    else
        flow_dic[idstr]['no_match_tscnt']=current_tscnt+flow_dic[idstr]['no_match_tscnt']
        flow_dic[idstr]['no_match_tccnt']=current_tccnt+flow_dic[idstr]['no_match_tccnt']
        flow_dic[idstr]['no_match_tsbytes']=current_tsbytes+flow_dic[idstr]['no_match_tsbytes']
        flow_dic[idstr]['no_match_tcbytes']=current_tcbytes+flow_dic[idstr]['no_match_tcbytes']
        --print ("match:    " .. current_tscnt, current_tsbytes, current_tccnt, current_tcbytes)
    end
end

function deinit (args)
    Bytes_total=0
    Bytes_match=0
	Bytes_no_match=0
    for k,v in pairs(flow_dic) do 
        Bytes_total=Bytes_total+flow_dic[k]['match_tsbytes']+flow_dic[k]['match_tcbytes']+flow_dic[k]['no_match_tsbytes']+flow_dic[k]['no_match_tcbytes']
        Bytes_match=Bytes_match+flow_dic[k]['match_tsbytes']+flow_dic[k]['match_tcbytes']
    end
	
	--print ("----------No_match Flow-----------------------------------------Percent----------")
	for k,v in pairs(flow_dic) do
		Bytes_no_match=flow_dic[k]['no_match_tsbytes']+flow_dic[k]['no_match_tcbytes']
		if Bytes_no_match ~= 0 then
			no_match_percent=Bytes_no_match/Bytes_total
			no_match_percent=no_match_percent*100
			--print ("Flow : " .. flow_dic[k]["flowtuple"][1],flow_dic[k]["flowtuple"][2],flow_dic[k]["flowtuple"][3],flow_dic[k]["flowtuple"][4],flow_dic[k]["flowtuple"][5])
			--str=string.format("Bytes No_match percent %f %%",no_match_percent)
			if	flow_dic[k]["flowtuple"][5] >= flow_dic[k]["flowtuple"][4] then
				user_port=flow_dic[k]["flowtuple"][5]
			else
				user_port=flow_dic[k]["flowtuple"][4]
			end
			str=string.format("ip.addr==%s	&&	%s.port==%s		%f %%",flow_dic[k]["flowtuple"][2],flow_dic[k]["flow_sque"][1],user_port,no_match_percent)
			--print(str)
		end
	end
	--print ("--------------------------------------------------------------------------")
	
    match_percent=Bytes_match/Bytes_total
    match_percent=match_percent*100
    print("flow all: " .. flow_all .. ", flow match: " .. flow_match .. ", total bytes: " .. Bytes_total)
    --print ("Bytes_match: " .. Bytes_match)
    str=string.format("Total Bytes match percent %f %%",match_percent)
    --print(str)

end
