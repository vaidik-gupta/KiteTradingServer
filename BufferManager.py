import time
import threading
from collections import deque
from KiteApp import *
import hashlib


class TickBuffer:
    def __init__(self, size: int = 100, period: float = 1.0):
        """
        size: max number of ticks
        period: seconds per tick aggregation (can be used later)
        """
        self.size = size
        self.period = period
        self.data = deque(maxlen=size)
        self.lock = threading.Lock()
        self.metrics = []  # Placeholder for metric objects

    def update(self, ltp):
        with self.lock:
            self.data.append(ltp)

    def get_data(self):
        with self.lock:
            return list(self.data)


class BufferManager:
    def __init__(self, kite_app: KiteApp):
        self.kite_app = kite_app
        self.subscribed = set()  
        self.buffers = {}
        self.token_ref = {}
        self.stop_flags = {}


    def _buffer_id(self,token,size,period):
        return f"{token}-{size}-{period}"
    
    def _buffer_workload(self,token,size,period,stop_event):
        """
        Poll LTP at every `period` seconds and append to buffer
        """
        buf_id = self._buffer_id(token, size, period)
        buf = self.buffers[buf_id]

        while not stop_event.is_set():
            ltp = self.kite_app.token_ltp.get(token)
            if ltp is not None:
                buf.update(ltp)
            time.sleep(period)
        

    def _init_buffer_thread(self,token,size,period):
        id = self._buffer_id(token,size,period)

        stop_event = threading.Event()
        thread = threading.Thread(
            target=self._buffer_workload,
            args=(token, size, period, stop_event),
            daemon=True
        )

        self.stop_flags[id] = stop_event
        thread.start()



    def _stop_buffer_thread(self,buffer_id):
        if buffer_id in self.stop_flags:
            self.stop_flags[buffer_id].set()
            self.stop_flags.pop(buffer_id)

    def add_subscription(self, token, size:int = 100, period:float = 1.0):
        """
        token: instrument token to subscribe
        buffer_configs: list of dicts with 'size' and 'period' keys
        Example: [{'size':100, 'period':1}, {'size':50, 'period':5}]
        """
        if token not in self.buffers:
            self.kite_app.subscribe(token)
            self.subscribed.add(token)
            self.token_ref[token] += 1

        id = self._buffer_id(token,size,period)
        buffer_obj = TickBuffer(size,period)
        self.buffers[id] = buffer_obj
        self._init_buffer_thread(token,size,period)
    


    def remove_subscription(self, buffer_id):
        token = buffer_id.split('-')[0]
        if buffer_id in self.buffers:
            self.buffers.pop(buffer_id)
            self.token_ref[token] -= 1
            self._stop_buffer_thread(buffer_id)
            if not self.token_ref[token]:
                self.kite_app.unsubscribe(token)
                self.subscribed.remove(token)



    
