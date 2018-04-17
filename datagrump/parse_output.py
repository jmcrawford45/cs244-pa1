import re, sys

expr = r'size: (\d*).*?capacity: (\d*\.\d*).*?throughput: (\d*\.\d*).*?signal delay: (\d*)'
f = sys.argv[1]
with open(f, 'r') as raw:
  raw_data = raw.read()
raw_data = raw_data.replace('\n', ' ')
data = list()
for match in re.finditer(expr, raw_data):
	data.append(match.group(1,3,4))
print data
