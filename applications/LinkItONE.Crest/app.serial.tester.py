"""

Usage:
    app.serial.tester.py <port> [--baudrate=<baud>] [--interval=<secs>]

Options:
    --baudrate=<baud>   Baudrate to use [default: 115200]
    --interval=<secs>   Interval between requests [default: 10]

"""

from docopt import docopt
import serial
import datetime
import time

def read_response(ser):

    resp = ""

    while True:
        next_byte = ser.read()
        resp += next_byte.decode("utf-8")

        if resp[-1] == "t":
            break

    return resp

def print_with_timestamp(s):
    print("{}: {}".format(datetime.datetime.now(), s))

def start_logging(port, baudrate, interval):

    interval = int(interval)
    baudrate = int(baudrate)
    req_no = 0

    with serial.Serial(port, baudrate) as ser:
        while(True):
            if req_no == 0:
                ser.write("s00?".encode("utf-8"))
                req_no = 1
            else:
                ser.write("s01?".encode("utf-8"))
                req_no = 0
                
            resp = read_response(ser)
            print_with_timestamp(resp)

            time.sleep(interval)

if __name__ == "__main__":
    args = docopt(__doc__)
    start_logging(args["<port>"], args['--baudrate'], args['--interval'])
