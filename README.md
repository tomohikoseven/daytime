「ふつうのLinuxプログラミング」の例題(daytime)を、  
機能を変えず、自分好みにリファクタリングした。  
そのときの生成物を置く。

# 主要ファイル
 daytime.c      daytime本体  
 SConstruct     daytime本体のビルドに必要なファイル  
 daytime_port.c サーバのポート番号とポート番号長  
 daytimed.c     サーバプログラム(オリジナルに一部修正)  
 Makefile       サーバビルド用Makefile  
 make.sh        サーバビルド用シェルスクリプト

# ビルド方法
 cat.c および SConstruct があるカレントディレクトリで以下を実行する。  
 $ scons .  
 $ ./make.sh

# 実行方法
 $ ./daytimed &  
 $ ./daytime

# 実行結果
Sun Feb 24 21:56:10 2013  ( 日時が表示される )  
[3]   終了                  ./daytimed  ( サーバが終了 )

# sconsのインストール方法
 $ sudo apt-get install scons  

