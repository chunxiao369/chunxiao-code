package main;

import (
    "fmt"
    "path"
    "strconv"
)

func main() {
    fmt.Println(path.Base("./a/b/c"));
    s := "0000:06:00.5"
    //first3 := s[0:3]
    last  := s[len(s)-1:]
    fmt.Println(last);
    b,error := strconv.Atoi(last)
    if error != nil{
        fmt.Println("字符串转换成整数失败")
    }
    b = b - 1
    fmt.Println(strconv.Itoa(b))
}
