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
        print (ts, srcip, sp, dstip, dp)
        -- do something
        --print ("----------request header")
        a = HttpGetRequestHeaders();
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
        print ("----------request body")
        a, o, e = HttpGetRequestBody();
        for n, v in ipairs(a) do
            --print ("----------------------------ipairs----------------")
            multipart_data = Multipart(v, content_type_header)
            internal_data = multipart_data._data

            index_sender = internal_data.indexes["sender"]
            index_from = internal_data.indexes["from"]
            index_to = internal_data.indexes["to"]
            index_cc = internal_data.indexes["cc"]
            index_subj = internal_data.indexes["subj"]
            index_att = internal_data.indexes["att_swf"]
            if index_sender ~= nil then
                print (internal_data.data[index_sender].name .. " : " .. internal_data.data[index_sender].value)
            end
            if index_from ~= nil then
                print (internal_data.data[index_from].name .. " : " .. internal_data.data[index_from].value)
            end
            if index_to ~= nil then
                print (internal_data.data[index_to].name .. " : " .. internal_data.data[index_to].value)
            end
            if index_cc ~= nil then
                print (internal_data.data[index_cc].name .. " : " .. internal_data.data[index_cc].value)
            end
            if index_subj ~= nil then
                print (internal_data.data[index_subj].name .. " : " .. internal_data.data[index_subj].value)
            end
            if index_att ~= nil then
                print (internal_data.data[index_att].name .. " : " .. internal_data.data[index_att].value)
                json_txt = internal_data.data[index_att].value
                json_v = cjson.decode(json_txt)
                print ("attach file name: " .. json_v[1]["name"])
                print ("attach file size: " .. json_v[1]["size"])
            end
        end

        --print ("----------response")
        --a = HttpGetResponseHeaders();
        --for n, v in pairs(a) do
        --    print(n,v)
        --end
    else
    end

    --
    --
    --sid, rev, gid = SCRuleIds()
    --print (sid .. " xxxxxxxxxxxxx")
    --print ("embedway test")
    ----
    --print ("embedway test")
    ----
    --msg = SCRuleMsg()
    ----
    --print (ts, sid, rev, gid, srcip, sp, dstip, dp, msg, http_host)
    --print (ts .. " " .. http_host .. " [**] " .. http_uri .. " [**] " ..
    --       http_ua .. " [**] " .. srcip .. ":" .. sp .. " -> " ..
    --       dstip .. ":" .. dp .. "\n")
    --print (http_ua)
    --print ("--------------------------------embedway http session end\n\n")
end

function deinit (args)
end
