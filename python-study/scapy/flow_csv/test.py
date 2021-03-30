from flowmeter.flowmeter import Flowmeter

feature_gen = Flowmeter(
    offline = "tcp1.pcap",
    outfunc = None,
    outfile = "output.csv")

feature_gen.run()

