import os, re

file = 'm384-import-11.xml'
filenames = ['M384_0201_output.xml','M384_0202_output.xml']

counter = 2
outputfile = file

for fname in filenames:
    in_size = (os.stat(fname).st_size / 1000000)
    try:
        out_size = (os.stat(file).st_size / 1000000)
    except OSError:
        out_size = 0
    if (in_size + out_size) > 75:
        file = re.split('\.', outputfile)[0] + '_' + str(counter) + '.xml'
        counter = counter + 1
    with open(file, 'a') as outfile:
        with open(fname) as infile:
            for line in infile:
                outfile.write(line)
