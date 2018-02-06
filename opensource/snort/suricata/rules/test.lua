function init (args)
    local needs = {}
    needs["http.request_user_agent"] = tostring(true)
    return needs
end

function match(args)
    a = tostring(args["http.request_user_agent"])
    print (a)
    --Mozilla/5.0 (X11; Linux x86_64; rv:58.0) Gecko/20100101 Firefox/58.0
    if #a > 0 then
        -- if a:find("^POST%s+/.*%.php%s+HTTP/1.0$") then
        string.find("Hello Lua user", "Lua", 1) 
        match_string = string.match(a, ".*Gecko/(%d+)")
        print (match_string)
        if match_string then
            print ("matched")
            return 1
        end
    end

    return 0
end

return 0
