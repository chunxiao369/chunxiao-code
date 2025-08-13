#python3.10
import akshare as ak
import talib as ta

print(ak.__version__)
print(ta.__version__)
stock_zh_a_hist_df = ak.stock_zh_a_hist(symbol="603496", period="daily", start_date="20250301", end_date='20250724', adjust="")
print(stock_zh_a_hist_df)
stock_zh_a_hist_df['MA5'] = ta.MA(stock_zh_a_hist_df['收盘'], timeperiod=5)
stock_zh_a_hist_df['EMA5'] = ta.EMA(stock_zh_a_hist_df['收盘'], timeperiod=5)
print(stock_zh_a_hist_df['MA5'])
print(stock_zh_a_hist_df['EMA5'])



