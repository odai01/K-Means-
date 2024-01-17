#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cap.h"
#include <Python.h>

struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
    int cluster_number;
};

double* kalg(double *initial_centroids,double *data_points,int N, int d ,int k, int iter,double epsilon);
void AssignToCluster(int k,struct vector *vec,struct vector *centroids);
double UpdateCentroids(int k,int d,struct vector *centroids,struct vector *vec);
double distance(struct vector *vec1,struct vector *vec2);
void SumCords(struct cord *cords1,struct cord *cords2);
void DivideCords(struct cord *cords,double num);
int IsInCent(struct vector *vec,double *cents,int k ,int d);
void FreeCords(struct cord *cords);
void FreeVec(struct vector *vec);
void printvecs(struct vector *vec);
int main(){
    return 0;
}
double* kalg(double *initial_centroids,double *data_points,int N, int d ,int k, int iter,double epsilon)
{
    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;
    struct vector *first_centroid, *curr_centroid;
    int i=0,j=1,t=-1;
    double n,delta_uk=100000000;
    double *res=(double *)malloc(k*d*sizeof(double));
    
    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;
    for(i=0;i<N*d;i+=d)
    {
        for(j=0;j<d-1;j++){
            n=data_points[i+j];
            curr_cord->value = n;
            curr_cord->next = malloc(sizeof(struct cord));
            curr_cord = curr_cord->next;
            curr_cord->next = NULL;
        }
        j=d-1;
        n=data_points[i+j];
        curr_cord->value = n;
        curr_vec->cords = head_cord;
        curr_vec->cluster_number=-1;
        t=IsInCent(curr_vec,initial_centroids,k,d);
        if(t!=-1){
            curr_vec->cluster_number=t;
        }
        if(i==0){
            head_vec=curr_vec;
        }
        if(i!=N*d-d){
            curr_vec->next = malloc(sizeof(struct vector));
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;
        }
        if(i==N*d-d){curr_vec->next=NULL;}
    }
    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    first_centroid=malloc(sizeof(struct vector));
    curr_centroid=first_centroid;
    for(i=0;i<k*d;i+=d)
    {
        for(j=0;j<d-1;j++){
            n=initial_centroids[i+j];
            curr_cord->value = n;
            curr_cord->next = malloc(sizeof(struct cord));
            curr_cord = curr_cord->next;
            curr_cord->next = NULL;
        }
        j=d-1;
        n=initial_centroids[i+j];
        curr_cord->value = n;
        curr_centroid->cords = head_cord;
        if(i!=k*d-d){
            curr_centroid->next = malloc(sizeof(struct vector));
            curr_centroid = curr_centroid->next;
            curr_centroid->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;
        }
        if(i==k*d-d){curr_centroid->next=NULL;}
    }
    curr_centroid=first_centroid;
    curr_vec=head_vec;
    j=1;
    while(j<iter && delta_uk>=epsilon){
        while(curr_vec!=NULL){
            AssignToCluster(k,curr_vec,curr_centroid);
            curr_vec=curr_vec->next;
        }
        curr_centroid=first_centroid;
        curr_vec=head_vec;
        delta_uk=UpdateCentroids(k,d,curr_centroid,curr_vec);
        j+=1;
    }
    curr_centroid=first_centroid;
    for(i=0;i<k*d;i+=d){
        curr_cord=curr_centroid->cords;
        for(j=0;j<d;j++){
            res[i+j]=curr_cord->value;
            curr_cord=curr_cord->next;
        }
        curr_centroid=curr_centroid->next;
    }   
    curr_vec=head_vec;
    for(i=1;i<=N;i++){
        curr_cord=curr_vec->cords;
        FreeCords(curr_cord);
        curr_vec=curr_vec->next;
    }
    curr_centroid=first_centroid;
    for(i=1;i<=k;i++){ 
        curr_cord=curr_centroid->cords;
        FreeCords(curr_cord);
        curr_centroid=curr_centroid->next;
    }
    FreeVec(first_centroid);
    FreeVec(head_vec);
    return res;
}
void AssignToCluster(int k,struct vector *vec,struct vector *centroids){
    double argmin=distance(vec,centroids),dis;
    int i=0;
    vec->cluster_number=1;
    for(i=0;i<k;i++){
        dis=distance(vec,centroids);
        if(dis<argmin){
            argmin=dis;
            vec->cluster_number=i+1;
        }
        centroids=centroids->next;
    }
}
double UpdateCentroids(int k,int d,struct vector *centroids,struct vector *vec){
     
    struct vector *sumvec,*tempvec;
    struct cord *tempcord;
    int num=0,i=1,j=1;
    double delta=0,tempdist;
    for(i=1;i<=k;i++){
        num=0;
        tempvec=vec;
        sumvec= malloc(sizeof(struct vector));
        sumvec->next=NULL;
        sumvec->cords=malloc(sizeof(struct cord));
        tempcord=sumvec->cords;
        for(j=1;j<d;j++){
            tempcord->value=0;
            tempcord->next=malloc(sizeof(struct cord));
            tempcord=tempcord->next;
            tempcord->next=NULL;
        }
        while(tempvec!=NULL){
            if(tempvec->cluster_number==i){
                SumCords(sumvec->cords,tempvec->cords);
                num+=1;
                if(tempvec->cords==NULL){num-=1;}
            }
            tempvec=tempvec->next;
        }
        DivideCords(sumvec->cords,num);
        tempdist=distance(centroids,sumvec);
        if(tempdist>delta){
            delta=tempdist;
        }
        FreeCords(centroids->cords);
        centroids->cords=sumvec->cords;
        FreeVec(sumvec);    
        centroids=centroids->next;
    }
    return delta;
}
void SumCords(struct cord *cords1,struct cord *cords2){
    while(cords1!=NULL && cords2!=NULL){
        cords1->value+=cords2->value;
        cords1=cords1->next;
        cords2=cords2->next;
    }
}
void DivideCords(struct cord *cords,double num){
    while(cords!=NULL){
        cords->value=(double)(cords->value/num);
        cords=cords->next;
    }
}
double distance(struct vector *vec1,struct vector *vec2){
    struct cord *cords1=vec1->cords;
    struct cord *cords2=vec2->cords;
    double sum=0.0,res=0.0;
    double x=0.0;
    while(cords1!=NULL && cords2!=NULL){
        x=(cords1->value) - (cords2->value);
        sum+=pow(x,2);
        cords1=cords1->next;
        cords2=cords2->next;
    }
    res=sqrt(sum);
    return res;
}
int IsInCent(struct vector *vec,double *cents,int k ,int d){
    struct cord *temp=vec->cords;
    int flag=0,i=0,clust=1;
    for(i=0;i<k*d;i+=d){
        temp=vec->cords;
        flag=1;
        for(int j=0;j<d;j++){
            if(cents[i+j]!=temp->value){
                flag=0;
            }
            temp=temp->next;
        }
        if(flag==1){return clust;}
        clust+=1;
    }
    return -1;
}
void FreeCords(struct cord *cords){
    struct cord *temp;
    while(cords!=NULL){
        temp=cords->next;
        free(cords);
        cords=temp;
    }
}
void FreeVec(struct vector *vec){
    struct vector *temp;
    while(vec!=NULL){
        temp=vec->next;
        free(vec);
        vec=temp;
    }
}
void printvecs(struct vector *vec){
    struct cord *temp=vec->cords;
    int i=1;
    printf("%d: ",i);
    while(vec!=NULL){
        printf("%d: ",i);
        temp=vec->cords;
        while(temp!=NULL){
            printf("%f , ",temp->value);
            temp=temp->next;
        }
        vec=vec->next;
        i+=1;
        printf("\n");
    }
}