--alerts = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
alerts = {}
function init (args)
    local needs = {}
    print ("-----------------------------------------------------")
    print ("    Copyright©2019 EmbedWay IAA software")
    needs["type"] = "packet"
    needs["filter"] = "alerts"
    return needs
end

function setup (args)
    --for i=1,1500000 do
    --    alerts[i] = 0
    --end
end

function log(args)
    ts = SCPacketTimeString()
    ipver, srcip, dstip, proto, sp, dp = SCPacketTuple()
    sid, rev, gid = SCRuleIds()
    msg = SCRuleMsg()
    if sid ~= nil then
        if alerts[sid] == nil then
	    alerts[sid] = 1
        else
            alerts[sid] = alerts[sid] + 1
        end
    end
end

local apps = {}
apps[54]   = "qq"
apps[112]  = "weixin"
apps[189]  = "meituan"
apps[228]  = "douyin"
apps[1704] = "kuaishou"

function deinit (args)
    for key, value in pairs(alerts) do
        if alerts[key] == 0 then
        else
            print ("application [" .. string.format("%16s", apps[key]) .. "] match times:", string.format("%4d", alerts[key]))
        end
    end
    print ("-----------------------------------------------------")
end
