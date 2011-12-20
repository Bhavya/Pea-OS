#!/usr/bin/python

## Example for merging S19/SREC files
# @author: Copyright 2009,2010 Thomas Reidemeister
# @date: 2009.12.14
# @file: merge.py
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
import sys
import binascii

# generates the checksum for a string
def srec_checksum(text):
    s = len(text) + 3 + sum( map(ord, text))
    return 255 - (0x00FF & s)

def hexlify(text):
    hex_map = lambda x : "%02X" % ord(x)
    return ''.join(map(hex_map, text))

# breadS7: read S7 records, i.e. the starting address
def read_srecord(fname, breadS7=False):
    buffer = ""
    f = open(fname, 'r')
    for record in f:
        # identify description record
        if record.startswith("S0"):
            # see: http://www.amelek.gda.pl/avr/uisp/srecord.htm for format description
            # found header, [S0:2][count:2][address:4] ...[checksum:2]
            read_len = int(record[2:4], 16)
            
            # read text content
            header = ''
            for i in range(2, read_len-1):
                index = 4+2*i
                ch = chr(int(record[index:index+2], 16))
                header += ch
            
            print "Reading file %s ..." % header
        elif record.startswith('S7'):
            if breadS7:
                buffer += record
        else:
            buffer += record
    f.close()
    return buffer
                
                

if __name__ == '__main__':
    if len(sys.argv) != 4:
        print "ERROR usage: merge.py <output.s19> <first.s19> <second.s19>"
        sys.exit(-1)
    
    output = sys.argv[1]
    
    srecsA = read_srecord(sys.argv[2])
    srecsB = read_srecord(sys.argv[3], True) # read the starting address of 2nd file
    
    # create a new header
    print "Composing new for file %s ..." % output
    header = "S0%02X0000%s%02X\r\n" %(len(output)+3, hexlify(output), srec_checksum(output))
    
    new_file = open(output, 'w')
    new_file.write(header) 
    new_file.write(srecsA)
    new_file.write(srecsB)
    new_file.close()
    print "finished."
    
    
