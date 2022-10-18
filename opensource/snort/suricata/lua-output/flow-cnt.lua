flow_all = 0
flow_match = 0
udp_flow_num= 0
tcp_flow_num= 0
other_flow_num= 0
local file = io.open("flow_cnt.csv", "a")

function init (args)
    local needs = {}
    needs["filter"] = "alerts"
    needs["type"] = "flow"
    return needs
end

function setup (args)
    flow_all = 0
    flow_match = 0
	udp_cnt=0
	tcp_cnt=0
	other_cnt=0
end

function log(args)
    ts = SCPacketTimeString()
    alproto = SCFlowAppLayerProto()
    --if alproto ~= nil then
    --    print (alproto)
    --end
    startts_s, lastts_s, startts_us, lastts_us = SCFlowTimestamps()
    ipver, srcip, dstip, proto, sp, dp = SCFlowTuple()
    flow_all = flow_all + 1
    has_alerts = SCFlowHasAlerts()
    tscnt, tsbytes, tccnt, tcbytes = SCFlowStats()

    flowid = SCFlowId()
    idstr = string.format("%.0f",flowid)
	--print ("idstr" .. idstr) 

	if proto == 6 then
		tcp_cnt = tcp_cnt+1
	elseif proto == 17 then
		udp_cnt = udp_cnt+1
    else
		other_cnt = other_cnt+1
	end

	--if proto == 50 then
	if proto == 6 or proto == 17 then
        --print (idstr, srcip, dstip, sp, dp, proto, 
        --       tscnt, tsbytes, tccnt, tcbytes,
        --       startts_s, lastts_s, startts_us, lastts_us)
        file:write(idstr, ",", srcip, ",", dstip, ",", sp, ",", dp, ",", proto, ",", 
               tscnt, ",", tsbytes, ",", tccnt, ",", tcbytes, ",", 
               startts_s, ",", lastts_s, ",", startts_us, ",", lastts_us, "\n")
    end

    if flow_all % 100 == 0 then
        file:flush()
    end
    if has_alerts then
        flow_match = flow_match + 1
    end
end

function deinit (args)
    file:close()
    print("  flow all: " .. flow_all .. ", flow udp: " .. udp_cnt.. 
          ", flow tcp: " .. tcp_cnt .. ", flow other: " .. other_cnt)
end
