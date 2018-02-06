
alerts = {0,0,0,0,0,0,0,0,0}
function init (args)
    local needs = {}
    needs["type"] = "packet"
    needs["filter"] = "alerts"
    --needs["protocol"] = "http"
    return needs
end

function setup (args)
end

function log(args)
    --http_ua = HttpGetRequestHeader("User-Agent")
    --if http_ua == nil then
    --    http_ua = "<useragent unknown>"
    --end
    --print (http_ua)

    --has_alerts = SCFlowHasAlerts()
    --print (has_alerts)
    --if has_alerts then
    --    print ("has alerts")
    --end
    --
    --http_uri = HttpGetRequestUriRaw()
    --if http_uri == nil then
    --    http_uri = "<unknown>"
    --end
    --http_uri = string.gsub(http_uri, "%c", ".")
    --
    --http_host = HttpGetRequestHost()
    --if http_host == nil then
    --    http_host = "<hostname unknown>"
    --end
    --http_host = string.gsub(http_host, "%c", ".")
    --
    ts = SCPacketTimeString()
    ipver, srcip, dstip, proto, sp, dp = SCFlowTuple()
    sid, rev, gid = SCRuleIds()
    msg = SCRuleMsg()
    --print (ts, sid, rev, gid, srcip, sp, dstip, dp, msg)
    --print (ts .. " " .. http_host .. " [**] " .. http_uri .. " [**] " ..
    --       http_ua .. " [**] " .. srcip .. ":" .. sp .. " -> " ..
    --       dstip .. ":" .. dp .. "\n")
    --print (http_ua)
    alerts[sid] = alerts[sid] + 1;
end

function deinit (args)
    for i = 1, #alerts do
        print ("ruleid " .. i .. " match:", alerts[i])
    end
end
