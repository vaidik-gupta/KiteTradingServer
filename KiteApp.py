from kiteconnect import KiteConnect, KiteTicker
import threading
import logging

logging.basicConfig(level=logging.INFO)

class KiteApp:
    def __init__(self, api_key: str, access_token: str):
        self.api_key = api_key
        self.access_token = access_token
        self.kite = KiteConnect(api_key=self.api_key)
        self.kite.set_access_token(self.access_token)
        self.kws = KiteTicker(self.api_key, self.access_token)
        self.kws.on_ticks = self.on_ticks
        self.kws.on_connect = self.on_connect
        self.kws.on_close = self.on_close
        self.kws.on_error = self.on_error
        self.subscribed_tokens = set()  # Keep track of current subscriptions
        self.token_ltp = {}



    # Core WebSocket callbacks
    def on_ticks(self, ws, ticks):
        for tick in ticks:
            self.token_ltp[tick['instrument_token']] = tick['last_price']

    def on_connect(self, ws, response):
        logging.info("Connected to KiteTicker")
        # Initially, do not subscribe
        pass

    def on_close(self, ws, code, reason):
        logging.info(f"WebSocket closed: {code} - {reason}")

    def on_error(self, ws, code, reason):
        logging.error(f"WebSocket error: {code} - {reason}")

    def start(self):
        # Start WebSocket in a separate thread
        threading.Thread(target=self.kws.connect, kwargs={"threaded": True}, daemon=True).start()

    def subscribe(self, token):
        if not token in self.subscribed_tokens:
            self.kws.subscribe(list(token))

    def unsubscribe(self, token):

        if token in self.subscribed_tokens:
            self.subscribed_tokens.remove(token)

