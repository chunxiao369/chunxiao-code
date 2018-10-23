cjson = require("cjson")
package.path = '/home/lance/Project/chunxiao-code/opensource/snort/suricata/lua-output/multipart.lua;'
Multipart = require("multipart")
content_type_header = "form-data; boundary=----WebKitFormBoundaryX4ThBgBtmXL30XI2"

function init (args)
    local needs = {}
    needs["filter"] = "alerts"
    needs["type"] = "flow"
    needs["protocol"] = "http"
    return needs
end

function setup (args)
end

function log(args)
    --print ("--------------------------------embedway http session start")
    ts = SCPacketTimeString()
    ipver, srcip, dstip, proto, sp, dp = SCFlowTuple()
    has_alerts = SCFlowHasAlerts()

    if has_alerts then
        --print (ts, srcip, sp, dstip, dp)
        -- do something
        --print ("----------request header")
        a = HttpGetRequestHeaders()
        for n, v in pairs(a) do
            if n == "Host" then
                --print(n,v)
            elseif n == "Cookie" then
                str = v
                str=str.."\n"
                str=str:gsub("%s(%S-)=","\n%1=")
                for name, value in string.gmatch(str, "(%S-)=(.-);*\n") do
                    --print (name, value)
                end
            end
        end
        http_uri = HttpGetRequestUriRaw()
        if http_uri == nil then
            http_uri = "<unknown>"
        end
        for qq_str, qq_num in string.gmatch(http_uri, "(%w+)=(%d+)") do
            if qq_str == "vuin" then
                print ("QQ number is: ", qq_num)
            end
        end

    else
    end
end

function deinit (args)
end
