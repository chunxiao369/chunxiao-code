from flowmeter.flowmeter import Flowmeter
import pandas as pd

feature_gen = Flowmeter("wechat.pcap")
df = feature_gen.build_feature_dataframe()
df.to_csv("output.csv")
