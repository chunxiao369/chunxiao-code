/home/lance/.openclaw/openclaw.json
openclaw gateway --port 18789 --log-level debug
ssh -L 18789:localhost:18789 lance@192.168.3.60
http://127.0.0.1:18789/?token=98fcdfc5b08b538459112d25264fe296a5b03a1b95eb1de7

