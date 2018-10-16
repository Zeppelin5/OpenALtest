#include <iostream>
#include <al/alut.h>//alut includes al.h
#include <math.h>
#include <windows.h>
#define PI 3.14159265359

#define TEST_ERROR(_msg)		\
	error = alGetError();		\
	if (error != AL_NO_ERROR) {	\
		fprintf(stderr, _msg "\n");	\
		return -1;		\
			}

ALboolean EnableSource2 = AL_FALSE;

//OS����40��windows7
void main()
{
	ALuint source1;     //buffer source
	alutInit(NULL, NULL);
	//ALuint source1;
	alGenSources(1, &source1);
	//��������10217��Music��RevolvingDoorMONO
	ALuint buffer1 = alutCreateBufferFromFile("RevolvingDoorMONO.WAV");//Ҫʵ�ֿռ仯Ч��������Ϊ�������ļ���monaural��RevolvingDoorMONO
	alSourcei(source1, AL_BUFFER, buffer1);

	//�Ƿ񲥷ŵڶ�������
	ALuint source2;
	if (EnableSource2){
		alGenSources(1, &source2);
		ALuint buffer2 = alutCreateBufferFromFile("10217.WAV");
		alSourcei(source2, AL_BUFFER, buffer2);
		alSourcePlay(source2);
		alSourcei(source2, AL_LOOPING, AL_TRUE);
	}

	alSourcei(source1, AL_SOURCE_RELATIVE, 0); // set to relative positioning so we can set everything to 0
	if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_SOURCE_RELATIVE error" << std::endl;
	alSourcef(source1, AL_ROLLOFF_FACTOR, 1.0); // 0 to disable attenuation  ����˥��ģ��
	if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_ROLLOFF_FACTOR error" << std::endl;
	alSourcef(source1, AL_REFERENCE_DISTANCE, 1); // doesn't matter

	alSourcef(source1, AL_MAX_DISTANCE, 5.f);//����AL_MAX_DISTANCE����λ�󣬽�����gainֵ������AL_MIN_GAIN�������cone֮�⣬�������ٳ���AL_MIN_GAIN�� 
	alSourcef(source1, AL_MIN_GAIN, 0.2f);//����AL_MIN_GAIN
	alSourcef(source1, AL_MAX_GAIN, 1.f);//����AL_MAX_GAIN����ʹ��ñ�1��Ҳ���Զ��س�1,�������EEROR

	alDistanceModel(AL_INVERSE_DISTANCE);//����ģ��
	//gain = AL_REFERENCE_DISTANCE / (AL_REFERENCE_DISTANCE + AL_ROLLOFF_FACTOR * (distance �C AL_REFERENCE_DISTANCE));

	alSourcef(source1, AL_PITCH, 1.f);//�������߳���
	if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_PITCH error" << std::endl;
	alSourcef(source1, AL_GAIN, 1.f);//��������,������AL_MIN_GAINӰ�죬��ʹ��ñ�min�ͣ�ʵ��gainҲ�ᱣ��min���ֵ
	if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_GAIN error" << std::endl;

	//set source direction
	alSource3f(source1, AL_DIRECTION, 0.f, 0.f, 1.f);//��z��������
	//set source position
	alSource3f(source1, AL_POSITION, 0.f, 0.f, 0.f);//��ʼλ��Ϊԭ��
	//set source velocity
	alSource3f(source1, AL_VELOCITY, 0.f, 0.f, 0.f);//������ЧӦӰ��Ƶ��

	alSourcef(source1, AL_CONE_OUTER_GAIN, 0.5f);//the gain when outside the oriented cone
	if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_CONE_OUTER_GAIN error" << std::endl;
	alSourcef(source1, AL_CONE_INNER_ANGLE, 90.f);//�ڽ�    *PI / 180.f
	if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_CONE_INNER_ANGLE error" << std::endl;
	alSourcef(source1, AL_CONE_OUTER_ANGLE, 150.f);
	if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_CONE_OUTER_ANGLE error" << std::endl;

	//set current listener position
	alListener3f(AL_POSITION, -0.0f,0.f,2.f);
	//-2.f,0.f,1.f �����������Ҷ������ϴ�
	//2.f,0.f,1.f ���������ң���������ϴ�
	//0.f,0.f,-1.f �������ں�����Ϊԭ����AL_CONE_OUTER_GAIN��,����Ҫ����AL_MIN_GAINϵ��

	//set current listener orientation
	ALfloat orivec[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	alListenerfv(AL_ORIENTATION, orivec);
	//���ú�listenerλ��
	//////////////////////////////////////////////////////////////////////////

	//tell the sound to loop continuously  
	alSourcei(source1, AL_LOOPING, AL_TRUE);
	
	//play the sound  
	alSourcePlay(source1);//have its state changed to AL_PLAYING�����²���

	ALfloat gain,outer_gain,min_gain,max_gain,max_distance; 
	alGetSourcef(source1, AL_GAIN, &gain); std::cout << "AL_GAIN: " << gain << std::endl;
	alGetSourcef(source1, AL_CONE_OUTER_GAIN, &outer_gain); std::cout << "AL_CONE_OUTER_GAIN: " << outer_gain << std::endl;
	alGetSourcef(source1, AL_MIN_GAIN, &min_gain); std::cout << "AL_MIN_GAIN: " << min_gain << std::endl;
	alGetSourcef(source1, AL_MAX_GAIN, &max_gain); std::cout << "AL_MAX_GAIN: " << max_gain << std::endl;
	alGetSourcef(source1, AL_MAX_DISTANCE, &max_distance); std::cout << "AL_MAX_DISTANCE: " << max_distance << std::endl;
	std::cout << std::endl << "����׶��Ǻ���Զ˥������֮�� gain * outer_gain * min_gain: " << gain*outer_gain*min_gain << std::endl;
	std::cout << std::endl;

	ALint state;
	ALfloat play_it = 0;
	do{
		//std::cout << "playing......" << "\b\b\b\b\b\b\b\b\b\b\b\b\b";
		//float updatex = -sinf(play_it*PI / 180.f);
		//float upadtez = cosf(play_it*PI / 180.f);
		//alSource3f(source1, AL_DIRECTION, updatex, 0.f, upadtez);
		//if ((alGetError()) != AL_NO_ERROR)std::cout << "AL_POSITION error" << std::endl;
		//play_it=play_it+0.1f;

		alGetSourcei(source1, AL_SOURCE_STATE, &state);
		if (EnableSource2)
			alGetSourcei(source2, AL_SOURCE_STATE, &state);
	} while (state == AL_PLAYING);

	// To stop the sound  
	alSourceStop(source1);
	//delete our source  
	alDeleteSources(1, &source1);
	////delete our buffer  
	alDeleteBuffers(1, &buffer1);
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
