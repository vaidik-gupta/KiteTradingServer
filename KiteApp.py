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


# --- TestKiteApp for testing purposes ---
import random

class TestKiteApp(KiteApp):
    def __init__(self, test_db, *args, **kwargs):
        """
        test_db: dict mapping instrument_token to list of tick prices
        """
        super().__init__(api_key=None, access_token=None)
        self.test_db = test_db
        self.tick_index = {token: 0 for token in test_db}
        self.token_ltp = {}
        self.subscribed_tokens = set()


    def subscribe(self, token):
        self.subscribed_tokens.add(token)

    def unsubscribe(self, token):
        self.subscribed_tokens.discard(token)

    def _simulate_tick(self, token):
        """
        Simulate a tick update for a token from the test_db
        """
        idx = self.tick_index[token]
        ticks = self.test_db[token]
        if idx < len(ticks):
            self.token_ltp[token] = ticks[idx]
            self.tick_index[token] += 1
        else:
            # Optionally loop or keep last value
            self.token_ltp[token] = ticks[-1]

    def start(self, min_delay=0.05, max_delay=0.2):
        """
        Starts a thread that waits a random amount of time before updating every subscribed tick.
        """
        def tick_worker():
            while True:
                delay = random.uniform(min_delay, max_delay)
                time.sleep(delay)
                for token in list(self.subscribed_tokens):
                    self.simulate_tick(token)
        thread = threading.Thread(target=tick_worker, daemon=True)
        thread.start()

    def reset(self):
        self.token_ltp = {}
        self.tick_index = {token: 0 for token in self.test_db}
        self.subscribed_tokens = set()

