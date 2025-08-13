量化库ta-lib

python3.10 data_print.py

df['WILLR'] = talib.WILLR(df['最高'], df['最低'], df['收盘'], timeperiod=14)
WILLR < -80，超卖，威廉指标看涨
WILLR > -20，超买，看跌

df['MOM'] = talib.MOM(df['收盘'], timeperiod=10)
MOM 动量指标上升：说明股价上涨动力较强
MOM 动量指标下降：说明股价下跌动力较强
适合短线交易者参考。

df['OBV'] = talib.OBV(df['收盘'], df['成交量'])
能量潮指标
OBV 上升，表示资金流入，股价可能上涨
OBV 下降，表示资金流出，股价可能下跌
适用于判断资金的流向，结合 K 线走势可以提高判断准确度。

df['ATR'] = talib.ATR(df['最高'], df['最低'], df['收盘'], timeperiod=14)
平均真实波幅
ATR 代表市场的波动性，数值越大，波动越剧烈。
适用于设置止损点，波动大时止损应适当放宽，波动小时止损应收紧

df['CDLHAMMER'] = talib.CDLHAMMER(df['开盘'], df['最高'], df['最低'], df['收盘'])
锤子线：下影线长、实体短，出现在下跌趋势末端，可能反转向上
如果伴随较大的成交量，信号更加强烈！

df['CDLDOJI'] = talib.CDLDOJI(df['开盘'], df['最高'], df['最低'], df['收盘'])
十字星代表市场方向不明，可能意味着趋势反转！
出现在高位，可能下跌
出现在低位，可能上涨，需要结合其他指标进行确认，比如 RSI、MACD。
