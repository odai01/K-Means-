import sys
import pandas as pd
import numpy as np
import mykmeanssp as kalg

def main_func(k,iter,eps,file_name1,file_name2):
    file1=pd.read_csv(file_name1,header=None)
    file2=pd.read_csv(file_name2,header=None)
    file1.columns=["key"]+file1.columns.tolist()[1:]
    file2.columns=["key"]+file2.columns.tolist()[1:]
    file=pd.merge(file1,file2,on="key")
    file=file.sort_values(by="key")
    ##file.values returns an np.array
    datapoints=file[file.columns[1:]].values
    temp_dp_for_cent=datapoints.copy()
    #check the iter and the N ..... 
    N=datapoints.shape[0]
    d=datapoints.shape[1]
    indexarr=np.arange(0,N)
    #indexarr=list(range(N))
    centroids=np.zeros((k,d))
    np.random.seed(0)
    temp=np.random.choice(indexarr)
    print(str(temp)+",",end="")
    centroids[0]=datapoints[temp]
    for i in range(k-1):
        pvals=np.zeros(N)
        indexarr=np.arange(0,N)
        #pvals=list(range(N))
        for j in indexarr:
            vec=datapoints[j]
            Dx=distance(vec,centroids[0])
            count=0
            for cent in centroids:
                if(count==i+1):
                    break
                tempdis=distance(vec,cent)
                if(tempdis<Dx):
                    Dx=tempdis   
                count+=1
            pvals[j]=Dx
        dsum=0 
        for p in pvals:
            dsum+=p
        pvals=pvals/dsum
        temp=np.random.choice(indexarr,p=pvals)
        if(i==k-2):
            print(str(temp))
        else:
            print(str(temp)+",",end="")
        centroids[i+1]=datapoints[temp]
    cents_list=[]
    points_list=[]
    for point in datapoints:
        for cord in point:
            points_list.append(cord)
    for point in centroids:
        for cord in point:
            cents_list.append(cord)

    res=list(kalg.fit(cents_list,points_list,N,d,k,iter,eps))
    for i in range(0,k*d,d):
        for j in range(d-1):
            print("%.4f," %res[i+j],end="")
        j=d-1
        print("%.4f" %res[i+j])

def distance(vec1, vec2):         ### calcuting the Euclidean Distance
    n = len(vec1)
    result = 0
    for j in range(n):
        sub = vec1[j]-vec2[j]
        poww = pow(sub,2)
        result += poww

    return pow(result,0.5)

def file_details(file_name1,file_name2):
    file1=pd.read_csv(file_name1,header=None)
    file2=pd.read_csv(file_name2,header=None)

    file1.columns=["key"]+file1.columns.tolist()[1:]
    file2.columns=["key"]+file2.columns.tolist()[1:]
    file=pd.merge(file1,file2,on="key")
    file=file.sort_values(by="key")
    datapoints=file[file.columns[1:]].values
    N=datapoints.shape[0]
    return N


##end of functions
inputs = sys.argv
inputs_length = len(inputs)    ### [not important, K, Iter,epsilon, file1, file2]
if(inputs_length==6):
    num_of_rows = file_details(inputs[4],inputs[5])       ### reading the files, get number of rows(N)
else:
    if(inputs_length==5):
        num_of_rows = file_details(inputs[3],inputs[4])

if(inputs_length==6):    ### [not important, K, Iter,epsilon, file1, file2]
    if ((num_of_rows <= int(inputs[1])) or (1000 <= int(inputs[2])) or(int(inputs[1])<=1) or(int(inputs[2])<=1) ):
        if (num_of_rows <= int(inputs[1]) or (int(inputs[1]) <= 1)):
            print("Invalid number of clusters!")
        if(1000 <= int(inputs[2]) or (int(inputs[2])<=1)):
            print("Invalid maximum iteration!")
    else:
        main_func(int(inputs[1]),int(inputs[2]),float(inputs[3]),inputs[4],inputs[5])

else:
    if (inputs_length==5):         ### [not important, K, epsilon, file1, file2]
        if (num_of_rows <= int(inputs[1]) or (int(inputs[1]) <= 1)):
            print("Invalid number of clusters!")
        else:
            main_func(int(inputs[1]),300,float(inputs[2]),inputs[3],inputs[4])
    else:
        print("An Error Has Occured")