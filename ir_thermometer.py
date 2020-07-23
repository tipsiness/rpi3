import smbus
import time
import datetime
 
class MLX90614():
    MLX90614_TA=0x06
    MLX90614_TOBJ1=0x07
    MLX90614_TOBJ2=0x08
 
    def __init__(self, address=0x5a, bus_num=1):
        self.bus_num = bus_num
        self.address = address
        self.bus = smbus.SMBus(bus=bus_num)
 
    def read_reg(self, reg_addr):
        return self.bus.read_word_data(self.address, reg_addr)
 
    def data_to_temp(self, data):
        temp = (data*0.02) - 273.15
        return temp
        
    def get_amb_temp(self):
        data = self.read_reg(self.MLX90614_TA)
        return self.data_to_temp(data)
 
    def get_obj_temp(self):
        data = self.read_reg(self.MLX90614_TOBJ1)
        return self.data_to_temp(data)

if __name__ == "__main__":
    sensor = MLX90614()

    try:
        while True:
            curr_time = datetime.datetime.now().strftime('[%Y-%m-%d %H:%M:%S]')

            amb_temp = sensor.get_amb_temp()
            obj_temp = sensor.get_obj_temp()
            print("%s ambient temperature: %.2f, object temperatue: %.2f" %(curr_time, amb_temp, obj_temp))

            time.sleep(3)
    except KeyboardInterrupt:
        print("terminating...")

