hs_compile_ext_multi扩展参数，

flags: flags标记
min_offset: 用于标识最小匹配的offset
max_offset: 用于标识最大匹配的offset
比如正则表达式/foo.*bar/，如果指定min_offset是10，max_offset是15的话，
foobar和foo0123456789bar就不会匹配，而foo0123bar和foo0123456bar就会匹配。

min_length: 最小匹配多长
edit_distance: Levenshtein距离参数，用于模糊匹配. fuzzy match但是在hyperscan中的描述是approximately match
     * Allow patterns to approximately match within this edit distance. To use
     * this parameter, set the @ref HS_EXT_FLAG_EDIT_DISTANCE flag in the
     * hs_expr_ext::flags field.
比如正则表达式以“/foo(bar)+/”为例，
若以原始模式“foobarbar”为基准，“foobarbar”，“foobarb0r”，“fooarbar”，“foobarba”，“f0obarbar”，“fobarbar”都是在“编辑距离”为1下的有效匹配。
似乎没有基准的设置，所以匹配只要能够精确匹配即是基准。
比如正则表达式/foo.*bar/
如果edit_distance是2的话，foobar, fooba, fobr, fo_baz, foooobar和/foobar/都会匹配
