function init (args)
    local needs = {}
    --needs["payload"] = tostring(true)
    needs["http.request_cookie"] = tostring(true)
    return needs
end

function match(args)
    match = 0
    --a = tostring(args["http.request_user_agent"])
    a = tostring(args["http.request_cookie"])
    print ("body len is: " .. #a .. ", type is: " .. type(a))
    --match_string = string.match(a, ".*lxlrttp=(%w+);.*")
    for k, v in string.gfind(a, "(lxlrttp)=(%w+);") do
        print (k, v)
        match = match + 1
    end
    if match then
        return 1
    else
        return 0
    end
end

return 0
