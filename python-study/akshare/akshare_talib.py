#python3.10
import akshare as ak
import talib as ta

def df_ma(df):
    # 移动平均线
    df['MA5'] = ta.MA(df['收盘'], timeperiod=5)
    df['MA20'] = ta.MA(df['收盘'], timeperiod=20)
    #print(type(df['MA5']))
    #print(type(df['MA20']))

    # 判断是否出现 MA5 上穿 MA20
    # 条件：昨天 MA5 ≤ MA20，今天 MA5 > MA20 ， 上穿通常解读为买入信号
    df['cross_up'] = (
        (df['MA5'] > df['MA20']) &
        (df['MA5'].shift(1) <= df['MA20'].shift(1))
    )

    # 查看是否出现上穿
    latest_cross = df['cross_up'].iloc[-1]
    print("MA5是否上穿MA20                : ", latest_cross)

def df_ema(df):
   # 指数移动平均线
   df['EMA5'] = ta.EMA(df['收盘'], timeperiod=5)
   #print(df['EMA5'])

def df_macd(df):
    #MACD（指数平滑异同移动平均线）
    df['DIF'], df['DEA'], df['MACD'] = ta.MACD(df['收盘'], fastperiod=12, slowperiod=26, signalperiod=9)

    # 判断 DIF 上穿 DEA（金叉）
    # 条件：昨日 DIF ≤ DEA，今日 DIF > DEA
    df['DIF_cross_up_DEA'] = (
        (df['DIF'] > df['DEA']) &
        (df['DIF'].shift(1) <= df['DEA'].shift(1))
    )

    # 是否出现金叉
    latest_golden_cross = df['DIF_cross_up_DEA'].iloc[-1]
    print("DIF是否上穿DEA                 : ", latest_golden_cross)

def df_bollinger_band(df):
    #布林带（Bollinger Bands）
    df['upper'], df['middle'], df['lower'] = ta.BBANDS(df['收盘'], timeperiod=20)
    # 判断是否触及上轨
    df['touch_upper'] = df['收盘'] >= df['upper']
    # 判断是否触及下轨
    df['touch_lower'] = df['收盘'] <= df['lower']

    # 查看结果
    latest_touch_upper = df['touch_upper'].iloc[-1]
    latest_touch_lower = df['touch_lower'].iloc[-1]

    print("是否触及布林上轨               : ", latest_touch_upper)
    print("是否触及布林下轨               : ", latest_touch_lower)

def df_rsi(df):
    #RSI（相对强弱指数）
    df['RSI'] = ta.RSI(df['收盘'], timeperiod=14)
    round_df = round(df['RSI'].iloc[-1], 2)
    is_overbought = round_df >= 70
    is_oversold   = round_df <= 30
    print("RSI", round_df, "是否 > 70            : " , is_overbought)
    print("RSI", round_df, "是否 < 30            : " , is_oversold)

def df_sar(df):
    # 计算 SAR
    df['SAR'] = ta.SAR(df['最高'], df['最低'], acceleration=0.02, maximum=0.2)

    # 判断 SAR 相对当日收盘的位置
    df['SAR_above_close'] = df['SAR'] > df['收盘']    # SAR 在 K 线上方
    df['SAR_below_close'] = df['SAR'] < df['收盘']    # SAR 在 K 线下方

    # 查看结果
    latest_above = df['SAR_above_close'].iloc[-1]
    latest_below = df['SAR_below_close'].iloc[-1]

    print("SAR在K线下方(看涨信号)         : ", latest_below)
    print("SAR在K线上方(看跌信号)         : ", latest_above)

def df_cdlmorningstar(df):
    # 1) 计算晨星标记
    df['CDLMORNINGSTAR'] = ta.CDLMORNINGSTAR(df['开盘'], df['最高'], df['最低'], df['收盘'], penetration=0.3)

    # 2) 定义下跌趋势：20 日均线向下（可选任何你认定的趋势规则）
    df['MA20'] = ta.MA(df['收盘'], timeperiod=20)
    # 最近 3 根 K 线均满足 MA20 向下，视为“末端下跌”
    df['down_trend'] = (
        (df['MA20'] < df['MA20'].shift(1)) &
        (df['MA20'].shift(1) < df['MA20'].shift(2)) &
        (df['MA20'].shift(2) < df['MA20'].shift(3))
    )

    # 3) 晨星出现在下跌趋势末端
    df['morning_star_end_of_down'] = (
        (df['CDLMORNINGSTAR'] == 100) &   # 出现晨星
        df['down_trend']                  # 同时处于下跌趋势末端
    )
    # 查看结果
    latest = df['morning_star_end_of_down'].iloc[-1]
    print("是否下跌趋势末端出现晨星       : ", latest)

print("------", "ak version:", ak.__version__, "ta version:", ta.__version__, "------")
# 002912
# 002439
# 603496
# 688017
df = ak.stock_zh_a_hist(symbol="688017", period="daily", start_date="20250501", end_date='20250905', adjust="")
#print(df)
df_ma(df)
df_macd(df)
df_bollinger_band(df)
df_rsi(df)
df_sar(df)
df_cdlmorningstar(df)


