﻿# Experimental braille converter
　日本語の漢字仮名交じり文章から、点字の分かち書き用データを生成します。今のところ、点訳精度はそれほどよくないです。まぁ、でもそれに見合ったコード料しかまだ書いてないので、これから賢くしていきたいです。
# Build
今のところWindowsでしかビルドできません、っていうかMeCabのwin32バイナリをそのまま突っ込んだりしていてry
msys2 をインストールして、その上にgccをインストールした状態で、
$ make
でビルドできます。
もしかしたら、これから boost が必要になるかもしれないので、適当にビルドして突っ込んでおいてください。あ、でも、unordered系はC++11に標準搭載になってmsysにも移植されたので、使うとしてもboost_formatぐらいになりそうだなあ。
# Usage
今のところ、入力ファイルはハードコーディングされています。test.txtです。出力先は標準出力なので、ファイルにしたい人は適当に >file.txt とかしてください。
brlcnv show で、原文を表示します。
brlcnv braille で、点訳結果を表示します。
brlcnv out で、点字データをoutbraille.bseに出力します。

# TODO
・点訳精度上げる
・いちいちコンパイルするのはあれなので、スクリプティングで点訳精度拡張を考える(とりあえず lua を持って来てごにょごにょやればいいかとか考えてるけどまだやってない、それよりなんで lua なんじゃ、っていうかなんで C++ なんじゃって声がどこかから聞こえてきそう)
・liblouis どーしようね

# final words
コードは汚いです。自分が読むことしかまだ考えてないのでインデント入れてないです。これをさいしょから解読できたら、貴方は眼が見えなくなっても人生ずっと生きていけます。嘘です。と思ったら、termでコードちょっと読まれてわーわーすいませんってなりました。部屋がすごい汚いときに友達がいきなり来ちゃったときの気分です。