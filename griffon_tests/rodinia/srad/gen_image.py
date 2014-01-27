import sys
import random

size = sys.argv[1]
f = open("image_"+size+".pgm", "w")
f.write("P2\n")
f.write(size+" "+size+"\n")
f.write("255\n")
for i in range(int(size)):
	for j in range(int(size)):
		if j != 0:
			f.write(" ")
		f.write(str(random.randint(0,255)))
	f.write("\n")
f.close()
print "Finished write to : 'image_"+size+".pgm'"