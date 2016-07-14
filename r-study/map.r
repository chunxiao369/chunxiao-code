library(maps)
library(mapdata)
map("china")
map("china", col = "red4", ylim = c(18, 54), panel.first = grid())
title("中国地图")
#ggmap google map更为强大的地图
#http://cos.name/2013/01/drawing-map-in-r-era/
