#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <deque>

#include "control.h"
#include "const.h"
#include "traffic.h"

using namespace std;

//反応率計算
double calpr(int D, double datasize, int st, int t, int waittime, double alpha){
  double a,b;
  a=exp(U0*exp(-alpha*(int(D*datasize)+t+waittime-st)));
  b=exp(U0*exp(-alpha*(int(D*datasize)+t-st)));
  return a/(a+b);

}

//制御関数f
void f(vector<trafficv> &copytra,int D[],int bv[],vector<int> nj[],int t,int waittime,double alpha){
  double f;
  int i,j,k;
    for(j=0;j<M;j++){
      f=0;
      //周囲の混雑度を合計
      for(i=0;i<nj[j].size();i++){
        f+=D[nj[j][i]];
      }
      //自分の基地局と比較
      if(f/nj[j].size()*1.5<D[j]&&(D[j]-f/nj[j].size())>=10){ //周囲の混雑度より1.5倍かつ１０以上多いなら制御実行
        for(k=0;k<copytra.size();k++){
          if(bv[copytra[k].userid]==j){
            if(Uniform(1.0)<calpr(D[j],copytra[k].datasize, copytra[k].starttime, t, waittime, alpha)){
              copytra[k].wait(waittime);
            }
          }
        }
      }
    }
  }

void control(vector<trafficv> &copytra, int qc[],int bc[],int bv[],vector<int> nj[],int t,int waittime,double alpha, deque<int> dave[]){
  int i,j,k,l,L,Kv;
  Kv=copytra.size();
  int Dc[M]={0};
  int D[M];
  ofstream fout;
  string outfile;
  //出力用ファイルオープン
  outfile="D_wt"+to_string(waittime)+".csv";
  fout.open(outfile,ios::app);

  //Cellerの混雑度計算
  for(i=0;i<Nc;i++){
    if(qc[i] == 1){
      if(bc[i]!= -1){
        Dc[bc[i]]+=1;
      }
    }
  }
  //Vehicleの通信端末数計算
  for(j=0;j<M;j++){
    l=0;
    for(k=0;k<Kv;k++){
    if(t>=copytra[k].starttime2&&copytra[k].datasize>0&&bv[copytra[k].userid]==j){
        l++;
      }
    }
    //混雑度計算
    L=l;
    D[j]=Dc[j]+L;
    //Vehicleの計算処理
    for(k=0;k<Kv;k++){
      if(t>=copytra[k].starttime2&&copytra[k].datasize>0&&bv[copytra[k].userid]==j){
        copytra[k].datasize -= 1.0/D[j];
        copytra[k].starttime2++;
      }
    }
  }

  //制御判定
  if(waittime!=0){
    f(copytra,D,bv,nj,t,waittime,alpha);
  }

  //dave更新
  for(j=0; j<M; j++){
    if(dave[j].size()==10){
      dave[j].pop_back();
    }
    dave[j].push_front(D[j]);
  }

  //record
  for(j=0;j<M;j++){
    fout<<D[j]<<",";
  }
  fout<<endl;
  //recorded
  fout.close();
}
