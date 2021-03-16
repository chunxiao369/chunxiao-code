from flowmeter.flowmeter import Flowmeter

feature_gen = Flowmeter(
    offline = "wechat.pcap",
    outfunc = None,
    outfile = "output.csv")

feature_gen.run()

