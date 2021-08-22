import os

path = os.listdir('../file/')
f1 = open('../file/'+path[0],'r').readlines()#需要整理的文件
N=4
result = [] # 保存全排列结果
resultd ={}
for a in ('ACGT'):
    for b in ('ACGT'):
        for c in ('ACGT'):
            for d in ('ACGT'):
                result.append(f'{a}{b}{c}{d}')
def aaa():
	for i in range(len(result)):
		resultd[result[i]] = 0
with open("../test.txt", "a") as f:
    for i in f1:
        if i.startswith('>'):
            pass
        else:
            aaa()
            str2 = i.strip("\n")
            for i in range(0,len(str2),4):
                str_tam = str2[i:N+i]
                if str_tam in resultd.keys():
                    resultd[str_tam] += 1
            f.write(str(resultd.values()).strip('dict_values([').strip('])').replace(',', '')+'\n')
    f.close()

