# -*- coding: utf-8 -*-
# @Author: Your name
# @Date:   2023-11-06 23:33:19
# @Last Modified by:   Your name
# @Last Modified time: 2023-11-07 17:06:30
import pickle
import sys
import datetime

current_time = datetime.datetime.now()
timestamp_str = current_time.strftime('%Y-%m-%d_%H-%M-%S')
filename = f"{timestamp_str}.log"

f_log= open(filename, 'w')
sys.stdout = f_log


addresses = set()

def read_addresses(file_path, addresses):
    
    with open(file_path, 'r') as file:

        inst_list = file.read().split("\n")
        if inst_list[-1] == '':
            inst_list.pop()

        for i, line in enumerate(inst_list):
            # Strip newlines and any whitespace
            address_str = line.strip()
            address_int = int(address_str, 16)

            if i + 1 < len(inst_list):
                nxt_address_str = inst_list[i+1].strip()
                nxt_address_int = int(nxt_address_str, 16)

                addresses.add((address_int, nxt_address_int))

file_path = 'instructionPairs.txt'
read_addresses(file_path, addresses)

try:
    with open('data.pkl', 'rb') as f:
        data_loaded = pickle.load(f)
        print("Previous Inst Pair count: ", len(data_loaded))
        addresses.update(data_loaded)
        print("New/distintive inst add: ", len(addresses) - len(data_loaded))
except FileNotFoundError:
    print("The file 'data.pkl' does not exist.")

with open('data.pkl', 'wb') as f:
    pickle.dump(addresses, f)

with open('submission.dot', 'w') as file:
    file.write("digraph controlflow {\n")
    for addr in addresses:
        file.write('"' + "0x%06x" % addr[0] + '" -> "' + "0x%06x" % addr[1] + '"\n')
    file.write("}\n")

f_log.close()
    

