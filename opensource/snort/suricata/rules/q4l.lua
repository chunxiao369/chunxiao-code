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
    match_string = string.match(a, ".*q4L(%w+)")
    if match_string then
        print (match_string)
        return 1
    end
    return 0
end

return 0
