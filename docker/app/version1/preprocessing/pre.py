import os

path = os.listdir('../file/')
f1 = open('../file/'+path[0],'r').readlines()#需要整理的文件
N=4
n=0
label = []
result = [] # 保存全排列结果
for a in ('ACGT'):
	for b in ('ACGT'):
		for c in ('ACGT'):
			for d in ('ACGT'):
				result.append(f'{a}{b}{c}{d}')
with open("../test.txt", "a") as f:
	for i in f1:
		if i.startswith('>'):
			pass
		else:
			str2 = i.strip("\n")
			for i in range(0, len(str2), N):
				str_tam = str2[i:N + i]
				if str_tam in result:
					f.write(str(result.index(str_tam)+1)+' ')
				else :
					f.write(str(0)+' ')
				n = n + 1
			while(n<=7500):
				f.write(str(-1)+' ')
				n=n+1
			n =0
			f.write('\n')