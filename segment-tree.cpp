#include <iostream>
using namespace std;
void build(int a[],int l,int r,int * t,int v){
    if (l==r){
        t[v]=a[l];
        return ;
    }
    int m=(l+r)/2;
    build(a,l,m,t,v*2+1);
    build(a,m+1,r,t,v*2+2);
    t[v]=t[2*v+2]+t[2*v+1];
}
void update(int * t,int l,int r,int i,int v,int diff){//l and r are 0 and n-1 on first call . Also diff is 'new_val-a[i]' .
    if (l==r && l==i){
        t[v]+=diff;
        return ;
    }
    int mid=(l+r)/2;
    t[v]+=diff;
    if (i<=mid){
        update(t,l,mid,i,2*v+1,diff);
        return ;
    }
    update(t,mid+1,r,i,2*v+2,diff);
}
int sum(int t[],int l,int r,int lx,int rx,int v){ //lx and rx are obviously 0 and n-1 on first call .
    if (l <= lx && rx <= r){
        return t[v];
    }
    if (l>rx || r<lx){
        return 0;
    }
    int m=(lx+rx)/2;
    return sum(t,l,r,lx,m,2*v+1)+sum(t,l,r,m+1,rx,2*v+2);
}
void update_2(int * t,int l,int r,int i,int val,int v){ //the l and r are 0 and n-1 on first call . 
    if (l==r & l==i){
        t[v]=val;
        return ;
    }
    int mid=(l+r)/2;
    if (i<=mid){
        update_2(t,l,mid,i,val,2*v+1);
    }else{
        update_2(t,mid+1,r,i,val,2*v+2);
    }
    t[v]=t[2*v+1]+t[2*v+2];
    return ;
}
int main(){
    int a[]={1,2,3,4,5,6,7};
    int t[2*7-1];
    for (int i=0;i<13;i++){
        t[i]=0;
    }
    build(a,0,6,t,0);
    cout<<"the sum :"<<sum(t,1,5,0,6,0)<<endl;
    update(t,0,6,3,0,1);
    for (int i=0;i<13;i++)
        cout<<t[i]<<endl;
    cout<<"the sum :"<<sum(t,1,5,0,6,0)<<endl;
    update_2(t,0,6,2,30,0);
    cout<<"the new sum :"<<sum(t,1,5,0,6,0)<<endl;
    return 0;
}
