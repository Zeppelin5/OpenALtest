#include <iostream>
#include <al/alut.h>//alut includes al.h
#include <math.h>
#include <windows.h>
using namespace std;

#define PI 3.14159265359

#define TEST_ERROR(_msg)		\
	error = alGetError();		\
	if (error != AL_NO_ERROR) {	\
		fprintf(stderr, _msg "\n");	\
		return -1;		\
			}

//ALboolean EnableSource2 = AL_TRUE;	//是否播放第二组声音
ALboolean EnableSource2 = AL_FALSE;	//是否播放第二组声音
ALboolean EnableSource3 = AL_TRUE;	//是否播放第三组声音

ALuint buffer2,buffer3;

void Init(){
	alutInit(NULL, NULL);
}

//OS音量40，windows7
void main()
{
	ALuint source1[10];     //buffer source

	Init();//分离到一个单独的函数或类成员中
	//alutInit(NULL, NULL);
	//ALuint source1;
	alGenSources(1, &source1[0]);
	//短哔声：10217，Music：RevolvingDoorMONO，中国军魂
	ALuint buffer1 = alutCreateBufferFromFile("RevolvingDoorMONO.WAV");//要实现空间化效果，必须为单声道文件（monaural）RevolvingDoorMONO

	if ((alGetError()) != AL_NO_ERROR)std::cout << "Buffer Create error!" << std::endl;

	alSourcei(source1[0], AL_BUFFER, buffer1);

	if (EnableSource2){
		alGenSources(2, &source1[1]);
		buffer2 = alutCreateBufferFromFile("E:\\VS2013\\Projects2013\\OpenALtest\\OpenALtest\\10217.WAV");
		alSourcei(source1[1], AL_BUFFER, buffer2);
		alSourcePlay(source1[1]);
		alSourcei(source1[1], AL_LOOPING, AL_TRUE);//第二个音频是否循环
	}

	if (EnableSource3){
		alGenSources(3, &source1[2]);
		ALuint temp = buffer1;
		alSourcei(source1[2], AL_BUFFER, temp);
		alSourcePlay(source1[2]);
		alSourcei(source1[2], AL_LOOPING, AL_TRUE);//第二个音频是否循环
	}

	alSourcei(source1[0], AL_SOURCE_RELATIVE, 0); // set to relative positioning so we can set everything to 0
	if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_SOURCE_RELATIVE error" << std::endl;
	alSourcef(source1[0], AL_ROLLOFF_FACTOR, 1.0); // 0 to disable attenuation  滚降衰减模型
	if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_ROLLOFF_FACTOR error" << std::endl;
	alSourcef(source1[0], AL_REFERENCE_DISTANCE, 1); // doesn't matter

	alSourcef(source1[0], AL_MAX_DISTANCE, 5.f);//超过AL_MAX_DISTANCE个单位后，将声音gain值限制在AL_MIN_GAIN（如果在cone之外，好像是再乘以AL_MIN_GAIN） 
	alSourcef(source1[0], AL_MIN_GAIN, 0.0f);//设置AL_MIN_GAIN
	alSourcef(source1[0], AL_MAX_GAIN, 5.f);//设置AL_MAX_GAIN，即使设得比1大，也会自动回成1,并报输出ERROR

	alDistanceModel(AL_INVERSE_DISTANCE);//距离模型
	//gain = AL_REFERENCE_DISTANCE / (AL_REFERENCE_DISTANCE + AL_ROLLOFF_FACTOR * (distance – AL_REFERENCE_DISTANCE));

	alSourcef(source1[0], AL_PITCH, 5.f);//调节音高乘数
	if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_PITCH error" << std::endl;
	alSourcef(source1[0], AL_GAIN, 1.f);//调节音量,但是受AL_MIN_GAIN影响，即使设得比min低，实际gain也会保持min设的值
	if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_GAIN error" << std::endl;

	//set source direction
	alSource3f(source1[0], AL_DIRECTION, 0.f, 0.f, 1.f);//朝z轴正方向
	//set source position
	alSource3f(source1[0], AL_POSITION, 0.f, 0.f, 0.f);//初始位置为原点
	//set source velocity
	alSource3f(source1[0], AL_VELOCITY, 0.f, 0.f, 0.f);//多普勒效应影响频率

	alSourcef(source1[0], AL_CONE_OUTER_GAIN, 0.0f);//the gain when outside the oriented cone
	if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_CONE_OUTER_GAIN error" << std::endl;
	alSourcef(source1[0], AL_CONE_INNER_ANGLE, 90.f);//内角    *PI / 180.f
	if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_CONE_INNER_ANGLE error" << std::endl;
	alSourcef(source1[0], AL_CONE_OUTER_ANGLE, 150.f);
	if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_CONE_OUTER_ANGLE error" << std::endl;

	//set current listener position
	alListener3f(AL_POSITION, -0.3f,0.f,2.f);
	//-1.f,0.f,1.f 监听者在左，右耳音量较大
	//1.f,0.f,1.f 监听者在右，左耳音量较大
	//0.f,0.f,-1.f 监听者在后，在外角之外，声音为原来的AL_CONE_OUTER_GAIN倍,好像还要乘以AL_MIN_GAIN系数

	//set current listener orientation
	ALfloat orivec[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	alListenerfv(AL_ORIENTATION, orivec);
	//设置好listener位置
	//////////////////////////////////////////////////////////////////////////

	//tell the sound to loop continuously  
	alSourcei(source1[0], AL_LOOPING, AL_TRUE);
	
	//play the sound  
	alSourcePlay(source1[0]);//have its state changed to AL_PLAYING，重新播放

	ALfloat gain,outer_gain,min_gain,max_gain,max_distance; 
	alGetSourcef(source1[0], AL_GAIN, &gain); std::cout << "AL_GAIN: " << gain << std::endl;
	alGetSourcef(source1[0], AL_CONE_OUTER_GAIN, &outer_gain); std::cout << "AL_CONE_OUTER_GAIN: " << outer_gain << std::endl;
	alGetSourcef(source1[0], AL_MIN_GAIN, &min_gain); std::cout << "AL_MIN_GAIN: " << min_gain << std::endl;
	alGetSourcef(source1[0], AL_MAX_GAIN, &max_gain); std::cout << "AL_MAX_GAIN: " << max_gain << std::endl;
	alGetSourcef(source1[0], AL_MAX_DISTANCE, &max_distance); std::cout << "AL_MAX_DISTANCE: " << max_distance << std::endl;
	std::cout << std::endl << "在音锥外角和最远衰减距离之外 gain * outer_gain * min_gain: " << gain*outer_gain*min_gain << std::endl;
	std::cout << std::endl;

	ALint sourceID;
	alGetSourcei(source1[0], AL_SOURCE_TYPE, &sourceID);
	std::cout << "AL_SOURCE_TYPE的ID： "<<sourceID << endl<<endl;

	ALint state;
	ALfloat tick = 0;
	ALfloat moveStart_posx = -2.f, moveStart_posy = 0.f, moveStart_posz = 0.f;
	do{
		std::cout << "playing......" << "\b\b\b\b\b\b\b\b\b\b\b\b\b";

		////旋转音锥
		//float updatex = -sinf(play_it*PI / 180.f);
		//float upadtez = cosf(play_it*PI / 180.f);
		//alSource3f(source1, AL_DIRECTION, updatex, 0.f, upadtez);
		//if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_POSITION error" << std::endl;
		//play_it=play_it+0.1f;

		////平移声源
		//alSource3f(source1, AL_POSITION, moveStart_posx, moveStart_posy, moveStart_posz);
		//moveStart_posx = moveStart_posx + 0.00000011f; moveStart_posz = moveStart_posz + 0.00000015f;

		alSourcef(source1[0], AL_PITCH, 1.2f);//调节音高乘数
		alGetSourcei(source1[0], AL_SOURCE_STATE, &state);

		if (tick == 2500)
			alSourcePause(source1[0]);//停止播放
		tick++;
		if (tick == 3500)
			alSourcePlay(source1[0]);//重新播放

		//是否播第二个音频资源
		if (EnableSource2)
			alGetSourcei(source1[1], AL_SOURCE_STATE, &state);

		if (EnableSource3)
			alGetSourcei(source1[2], AL_SOURCE_STATE, &state);

	} while (state == AL_PLAYING || state==AL_PAUSED);//结束时state自动变成AL_STOPPED,source1、2共享一个state

	// To stop the sound  
	alSourceStop(source1[0]);
	//delete our source  
	alDeleteSources(1, &source1[0]);
	////delete our buffer  
	alDeleteBuffers(1, &buffer1);
	if ((alGetError()) != AL_NO_ERROR)std::cout << "delete buffer1 error " << std::endl;

	if (EnableSource2){
		alSourceStop(source1[1]);
		alDeleteBuffers(1, &buffer2);
		if ((alGetError()) != AL_NO_ERROR)std::cout << "delete buffer2 error" << std::endl;
		alDeleteSources(1, &source1[1]);
		if ((alGetError()) != AL_NO_ERROR)std::cout << "delete source error" << std::endl;


	}
}



//alutInit(NULL, NULL);
//ALuint source1;
//alGenSources(1, &source1);

//ALuint buffer1 = alutCreateBufferFromFile("Stonefire.WAV");
//alSourcei(source1, AL_BUFFER, buffer1);
//alSourcePlay(source1);

//ALint state;
//do{
//	alGetSourcei(source1, AL_SOURCE_STATE, &state);
//} while (state == AL_PLAYING);

//alDeleteSources(1, &source1);
//alDeleteBuffers(1, &buffer1);
//alutExit();
