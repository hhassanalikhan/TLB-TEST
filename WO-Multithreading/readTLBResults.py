import statistics
fx = open('tlbTest.txt','r')
content = fx.read()
fx.close()

content = content.split('\n')

arraySizes = {}
i = 0

possiblePageSize = []
while i < len(content):
    line = content[i]
    if line != '':
        #get array size
        arraySize = line.split(' ')[1]
        try:
            val = arraySizes[arraySize]
        except:
            arraySizes[arraySize] = []
        if arraySize not in possiblePageSize:
            possiblePageSize.append(arraySize)
        #get number of operations
        line = content[i+1]
        line = line.split(' ')[3]
        # print(line)
        if line == '128000000':
            line = content[i+2]
            timeTaken = float(line.split(' ')[4])
            arraySizes[arraySize].append(timeTaken)
    i += 3

keys = list(arraySizes.keys())
# keys.sort()

tlbNumber = 0
prevTime = 0
prevSize = ''
printedSize = []
for size in possiblePageSize:
    
    timeAverage = round(sum(arraySizes[size])/(max(1,len(arraySizes[size]))), 2)
    if timeAverage != 0:
        if (prevTime == 0):
            print(size, timeAverage, 'seconds')
        elif (prevTime != 0 and prevTime * 1.3 < timeAverage) and prevSize not in printedSize:
            print(size, timeAverage, 'seconds')

    prevTime = timeAverage
    prevSize = size