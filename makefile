# 変数
includePath = -I ./ -I SerialClient/ -pthread # インクルードパス

product = serterm # カレントディレクトリに生成される実行ファイル名
destination = /usr/local/bin# 実行ファイルの配置場所

# 実行ファイル生成
$(product): main.c SerialClient.dll
	gcc $(includePath) SerialClient.dll main.c -o main

SerialClient.dll: SerialClient/client.h SerialClient/client.c SerialClient/recvThread.c
	gcc $(includePath) -c SerialClient/client.c
	gcc $(includePath) -c SerialClient/recvThread.c
	gcc client.o recvThread.o -fPIC -shared -o SerialClient.dll -pthread

# 内部ファイル削除
clean:
	rm -f *.o *.dll $(product)

# インストール
install: $(product)
	install -s $(product) $(destination)

# アンインストール
uninstall:
	rm $(destination)/$(product)
