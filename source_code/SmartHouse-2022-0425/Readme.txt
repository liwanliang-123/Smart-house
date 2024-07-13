该代码支持
	1.语音线程（控制继电器组)
	2.火灾线程（检测有火发出警报）
	3.socket线程（控制继电器组，控制摄像头线程）
	4.摄像头线程。

ps：	语音模块rx tx和树莓派的tx rx相连，因为语音模块需要5v供电，所以将其正负极接到usb转TTL上，然后插在电脑usb接口上进行供电。

	后续看看再加些传感器，完成智能家居项目。

	需要在该目录下放一张自己的照片，命名为yourself.jpg
运行： gcc bathroomLight.c livingroomLight.c restaurantLight.c upstairLight.c voiceContrl.c fire.c beep.c socketContrl.c camera.c mainPro.c -lwiringPi -lpthread -I /home/pi/httpHandler/curl-7.71.1/_install/include/ -L /home/pi/httpHandler/curl-7.71.1/_install/lib/ -lcurl -lssl -lcrypto -o test