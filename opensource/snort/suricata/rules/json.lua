function init (args)
    local needs = {}
    --needs["payload"] = tostring(true)
    needs["http.response_body"] = tostring(true)
    return needs
end

function match(args)
    --a = tostring(args["http.request_user_agent"])
    a = tostring(args["http.response_body"])
    print ("body len is: " .. #a .. ", type is: " .. type(a))
    -- match_string = string.match(a, ".*lxlrttp=(%w+);.*")
    -- match_string = string.match(a, ".*{.*\"country\":\"(.+)\",\"province.*")
    -- kv example
    match_string = string.match(a, ".*{.*\"country\":\"(.-)\",?.*")
    if match_string then
        print "country match"
        print (match_string)
    end

    -- json example
    file = io.open("/etc/suricata/rules/contacts.json", "r")
    file_str = file:read("*all")
    io.close(file)

    local cjson = require("cjson") 
    local obj = cjson.decode(file_str)
    print (obj)
    for i = 1, #obj.contactlist, 1 do
        io.write(obj.contactlist[i].display_name .. ":")
        if obj.contactlist[i].phone == nil then
        else
            size = #obj.contactlist[i].phone
            if (size > 0) then
                for j = 1, size, 1 do
                    io.write(" " .. obj.contactlist[i].phone[j].phoneNumber)
                end
            end
        end
        print ("")
    end
    --for k, v in string.gfind(file_str, "(\"display_name\"): (\".-\")") do
    --    print (k, v)
    --end

    if match_string then
        return 1
    end
end

return 0
