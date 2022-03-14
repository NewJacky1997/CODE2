def move(beforedisk,nowlist,beforemove,ans,step,showABC):
    #根據上一次移動的情況得到這次可能移動的情況
    if beforemove[1] == 0:
        nowmove = [[1,0],[1,2],[2,0],[2,1]]
    elif beforemove[1] == 1:
        nowmove = [[0,1],[0,2],[2,0],[2,1]]
    elif beforemove[1] == 2:
        nowmove = [[0,1],[0,2],[1,0],[1,2]]
    else:
        if nowlist[0][0] % 2 == 0: 
            nowmove = [[0,1],beforemove]
        else:
            nowmove = [[0,2],beforemove]
    nowmove.remove(beforemove)
    i = 0
    nowmove2 = nowmove[:]
    while i < len(nowmove):
        if nowlist[nowmove[i][0]] == []:
            nowmove2.remove(nowmove[i])
        elif nowlist[nowmove[i][1]] != [] and nowlist[nowmove[i][0]][-1] > nowlist[nowmove[i][1]][-1]:
            nowmove2.remove(nowmove[i])
        i+=1
    nowmove = nowmove2[:]
    a = nowlist[nowmove[0][0]][-1]
        
    if len(nowmove) <= 1:
        nowlist[nowmove[0][1]].append(a)
        nowlist[nowmove[0][0]].remove(a)
        beforemove = nowmove[0]
    else:
        if (beforedisk - a) % 2 == 0:
            if nowlist[nowmove[0][1]] == [] or nowlist[nowmove[0][1]][-1] != beforedisk:
                nowlist[nowmove[0][1]].append(a)
                nowlist[nowmove[0][0]].remove(a)
                beforemove = nowmove[0]
            else:
                nowlist[nowmove[1][1]].append(a)
                nowlist[nowmove[1][0]].remove(a)
                beforemove = nowmove[1]
        else:
            if nowlist[nowmove[0][1]] == [] or nowlist[nowmove[0][1]][-1] != beforedisk:
                nowlist[nowmove[1][1]].append(a)
                nowlist[nowmove[1][0]].remove(a)
                beforemove = nowmove[1]
            else:
                nowlist[nowmove[0][1]].append(a)
                nowlist[nowmove[0][0]].remove(a)
                beforemove = nowmove[0]
    return a,beforemove,step+1

LL1 = []
strk = input("Enter the number of discs:")
for i in range(int(strk),0,-1):
    LL1.append(i)
LL2 = []
LL3 = []
LLL = [LL1,LL2,LL3]
ans = LL1[:]
step = 0
beforemove = [-1,-1]
beforedisk = 0
showABC = ['A','B','C']
while LLL[2] != ans:
    if beforemove[0] == -1:
        print("step",step,"now is",LLL)
    else:
        print("step",step,"disk",beforedisk,"from",showABC[beforemove[0]],"move to",showABC[beforemove[1]],"and now is",LLL)
    if LLL[2] == ans:
        break
    beforedisk, beforemove, step = move(beforedisk,LLL,beforemove,ans,step,showABC)
print("step",step,"disk",beforedisk,"from",showABC[beforemove[0]],"move to",showABC[beforemove[1]],"and now is",LLL)