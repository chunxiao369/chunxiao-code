# default greedy match
./capture "sip=(.*), " capture.txt p 5
# no-greedy match
./capture "sip=(.*?), " capture.txt p 5
# capture mul-times
./capture "sip=(.*),dip=(.*)" capture.txt p 5

hyperscan不支持零宽断言
./capture "(?<=href=\").{1,200}" capture.txt h 5
ERROR: Unable to compile pattern "(?<=href=").{1,200}": Zero-width assertions are not supported.

(?=exp) 正预测先行断言
它断言自身出现的位置的后面能匹配表达式exp。
\b\w+(?=ing\b)，匹配以ing结尾的单词的前面部分(除了ing以外的部分)，
如查找I'm singing while you're dancing.时，它会匹配sing和danc。

(?<=exp)正回顾后发断言
它断言自身出现的位置的前面能匹配表达式exp。
比如(?<=\bre)\w+\b会匹配以re开头的单词的后半部分(除了re以外的部分)，
例如在查找reading a book时，它匹配ading。

./capture "(?<=href=\").{1,200}(?=\")" capture.txt p 5
   '''href="www.baidu.com">百度一下

!感叹号符号在bash中会被解释，即Event Designators，用来引用历史命令。

可以换到dash中执行 dash ./capture "Windows(?!2000)" capture.txt p 6
或者使用单引号来执行 ./capture 'Windows(?!2000)' capture.txt p 6

    关闭history expansion
    [username@hostname ~]$ set +o histexpand
    [username@hostname ~]$ echo hello!world!anything
    hello!world!anything
    打开history expansion
    [username@hostname ~]$ set -o histexpand
    [username@hostname ~]$ echo hello!world!anything
    -bash: !world!anything: event not found

(?!exp) 负预测先行断言
“Windows(?!95|98|NT|2000)”能匹配“Windows3.1”中的“Windows”，但不能匹配“Windows2000”中的“Windows”

(?<!exp) 负回顾后发断言

(?:pattern) 这个意思还未清楚，待补充

    分组语法 捕获

    (exp) 匹配exp,并捕获文本到自动命名的组里
    (?<name>exp) 匹配exp,并捕获文本到名称为name的组里，也可以写成(?'name'exp)
    (?:exp) 匹配exp,不捕获匹配的文本

    位置指定
    (?=exp) 匹配exp前面的位置
    (?<=exp) 匹配exp后面的位置
    (?!exp) 匹配后面跟的不是exp的位置
    (?<!exp) 匹配前面不是exp的位置

这样记：带<号一般写在提取内容的前面，不带<号写在后面
