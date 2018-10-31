cjson = require("cjson")
zlib = require("zlib")
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
        a = HttpGetRequestBody()
        for n, v in pairs(a) do
            print (n, v)
            print ("length of value", #v)
            uncompress = zlib.inflate()
            result, eof, bytes_in, bytes_out = uncompress(v)
            print ("result: ", result)
            print ("length of result: ", #result)
        end
    else
    end
end

function deinit (args)
end
