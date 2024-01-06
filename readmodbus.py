from pymodbus.client import ModbusTcpClient
import time
# Define the Modbus TCP server details
SERVER_IP = '192.168.1.100'
SERVER_PORT = 502

# Create a Modbus TCP client
client = ModbusTcpClient(SERVER_IP, port=SERVER_PORT)

def read_data(init_address,num_address,slave_id,interval):
    # Connect to the Modbus server
    if not client.connect():
        print("Unable to connect to the Modbus server")
        exit(1)
    response = client.read_holding_registers(init_address, num_address,slave_id)
    if not response.isError():
        print("Received data:", response.registers)
    else:
        print("Modbus error:", response)
    time.sleep(interval)

while True:
    try:
        read_data(0,10,1,0.8)
    except Exception as e:
        print("Error:", e)
    finally:
        # Close the connection
        client.close()
