#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <string>
#include <sstream>
#include <deque>

#include "cell.h"
#include "traffic.h"
#include "control.h"
#include "const.h"
#include "eval.h"

using namespace std;
int main(){
  int t,i,j;
  int h = H;
  int waittime;
  double alpha=1.0/60;
  int dave[M][Tave];
  int bcave[Nc][Tave];
  ofstream fout;
  string filename;

  //トラヒックの発生、Cellerはqc、Vehicleはtrav  =>traffic.cpp
  vector<trafficc> trac;
  inittrac(&trac);
  int qc[T][Nc]={0};
  initqc(qc, trac);
  vector<trafficv> trav;
  inittrav(&trav);

  //基地局の配置 =>cell.cpp
  cood cell[M];
  initcell(cell);
  vector<int> nj[M];
  setnj(nj);

  //ログデータ読み込み、ユーザの位置入手用
  cood uc[Nc],uv[Nv];
  int bc[Nc],bv[Nv];

  //待機時間変えて結果取得
  for(waittime=0;waittime<60;waittime+=30){
    filename = "stop_w:"+to_string(waittime)+".csv";
    fout.open(filename);
    //waittimeでループするためのtravのバックップ
    vector<trafficv> copytra;
    copytra=trav;
    //dave,bcave初期化
    for(t=0; t<Tave; t++){
      for(j=0;j<M ;j++){
        dave[j][t]=0;
      }
      for(i=0; i<Nc; i++){
        bcave[i][t]=-1;
      }
    }
    //制御時刻開始
    for(t=0;t<T;t++){
      //logデータ読み込んでユーザの位置を入手 =>cell.cpp
      readlog(uc,0,Nc,h,t);
      readlog(uv,Nc,Nv,h,t);
      setcell(cell,uc,bc,Nc);
      setcell(cell,uv,bv,Nv);
      for(i=0; i<Nc; i++){
        bcave[i][t%Tave]=bc[i];
      }
      //制御
      control(copytra, &qc[t][0],bc,bv,nj,t,waittime,alpha, dave);
      fout << eval(trac,dave,bcave,t) << ",";
    }
    fout << endl;
    fout.close();
  }
  return 0;
}
