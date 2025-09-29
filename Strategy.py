from BufferManager import BufferManager

class Strategy:

    buf_manager:BufferManager


    def __init__(self,buf_manager:BufferManager,init_subscribe_configs):
        self.buf_manager = buf_manager
        
        for config in init_subscribe_configs:
            token = config['token']
            size = config.get('size',100)
            period = config.get('period',1.0)
            self.buf_manager.add_subscription(token,size,period)
        

    
        
