#ifndef INCLUDED_const_h
#define INCLUDED_const_h

#define T 30
#define M 91
#define Nc 3
#define Nv 3
#define Lambda 0.01
#define Interval 30
#define Datasize 0.1  //user数:500,500 Lamda:0.01 Interval:30の時0.1くらいでトラヒックはサチらない
#define P 1.0 //全車両に対するそのセンサ車両の割合
#define U0 1  //1の時が程よく反応率が数１０％になる
#define Tave 10  //通信のスループットで移動平均取る時間
#define Stoprate 1.2 //通信端末数がこれ以上増えたら停止
#define H 10  //taxiデータの読み込み開始時刻

#endif
