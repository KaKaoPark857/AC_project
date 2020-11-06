#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" This application is Copyright ⓒ KT Corp. All rights reserved. """

"""The Python implementation of GiGA Genie gRPC client"""

import grpc
import time                # Import necessary modules

import gigagenieRPC_pb2
import gigagenieRPC_pb2_grpc

import os
import datetime
import hmac
import hashlib
from threading import Timer

from ctypes import *

button = False

import serial

###############################################ALSA 모듈 에러 메시지 삭제 코드
ERROR_HANDLER_FUNC = CFUNCTYPE(None, c_char_p, c_int, c_char_p, c_int, c_char_p)
def py_error_handler(filename, line, function, err, fmt):
	pass
c_error_handler = ERROR_HANDLER_FUNC(py_error_handler)
asound = cdll.LoadLibrary('libasound.so')
asound.snd_lib_error_set_handler(c_error_handler)
###############################################

state1 = 0 #1번 잠금상태시 열림 동작 방지
state2 = 0 #2번 잠금상태시 열림 동작 방지
###############################################아두이노 동작 명령 전달

port = "/dev/ttyACM0" #UART 통신
arduino = serial.Serial(port, 9600)
#블루투스 모듈 연결 코드 작성요망

def close1_AC(): #1번 닫힘
	m = '1'
	m = [m.encode('utf-8')]
	arduino.writelines(m)

def open1_AC(): #1번 열림
	m = '2'
	m = [m.encode('utf-8')]
	arduino.writelines(m)

def close2_AC(): #2번 닫힘
	m = '3'
	m = [m.encode('utf-8')]
	arduino.writelines(m)

def open2_AC(): #2번 열림
	m = '4'
	m = [m.encode('utf-8')]
	arduino.writelines(m)


def lock1_AC(): #1번 잠금
	m = '5'
	m = [m.encode('utf-8')]
	arduino.writelines(m)

def unlock1_AC(): #1번 잠금해제
	m = '6'
	m = [m.encode('utf-8')]
	arduino.writelines(m)

def lock2_AC(): #2번 잠금
	m = '7'
	m = [m.encode('utf-8')]
	arduino.writelines(m)

def unlock2_AC(): #2번 잠금해제
	m = '8'
	m = [m.encode('utf-8')]
	arduino.writelines(m)

################################################

def callback(channel):
	print("falling edge detected from pin {}".format(channel))
	global button
	button = True

############
import getVoice2Text as gv2t
import getText2VoiceStream as gt2vt

# Config for GiGA Genie gRPC

CLIENT_ID = 'Y2xpZW50X2lkMTU5MTYxOTAxOTk0Nw=='
CLIENT_KEY = 'Y2xpZW50X2tleTE1OTE2MTkwMTk5NDc='
CLIENT_SECRET = 'Y2xpZW50X3NlY3JldDE1OTE2MTkwMTk5NDc='
HOST = 'gate.gigagenie.ai' 
PORT = 4080 

### COMMON : Client Credentials ###
def getMetadata():
    timestamp = datetime.datetime.now().strftime("%Y%m%d%H%M%S%f")[:-3]
    message = CLIENT_ID + ':' + timestamp

    signature = hmac.new(CLIENT_SECRET.encode(), message.encode(), hashlib.sha256).hexdigest()

    metadata = [('x-auth-clientkey', CLIENT_KEY),
                ('x-auth-timestamp', timestamp),
                ('x-auth-signature', signature)]

    return metadata

def credentials(context, callback):
	callback(getMetadata(), None)

def getCredentials(): 
    sslCred = grpc.ssl_channel_credentials() 
    authCred = grpc.metadata_call_credentials(credentials) 
    return grpc.composite_channel_credentials(sslCred, authCred)

### KWS
import pyaudio
import audioop
from six.moves import queue

FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
CHUNK = 512

# MicrophoneStream - original code in https://goo.gl/7Xy3TT
class MicrophoneStream(object):
	"""Opens a recording stream as a generator yielding the audio chunks."""
	def __init__(self, rate, chunk):
		self._rate = rate
		self._chunk = chunk

		# Create a thread-safe buffer of audio data
		self._buff = queue.Queue()
		self.closed = True

	def __enter__(self):
		self._audio_interface = pyaudio.PyAudio()
		self._audio_stream = self._audio_interface.open(
			format=pyaudio.paInt16,
			channels=1, rate=self._rate,
			input=True, frames_per_buffer=self._chunk,
			# Run the audio stream asynchronously to fill the buffer object.
			# This is necessary so that the input device's buffer doesn't
			# overflow while the calling thread makes network requests, etc.
			stream_callback=self._fill_buffer,
		)

		self.closed = False

		return self

	#def __exit__(self, type, value, traceback):
	def __exit__(self, type, value, traceback):		
		self._audio_stream.stop_stream()
		self._audio_stream.close()
		self.closed = True
		# Signal the generator to terminate so that the client's
		# streaming_recognize method will not block the process termination.
		self._buff.put(None)
		self._audio_interface.terminate()
	
	def _fill_buffer(self, in_data, frame_count, time_info, status_flags):
		"""Continuously collect data from the audio stream, into the buffer."""
		self._buff.put(in_data)
		return None, pyaudio.paContinue

	def generator(self):
		while not self.closed:
			# Use a blocking get() to ensure there's at least one chunk of
			# data, and stop iteration if the chunk is None, indicating the
			# end of the audio stream.
			chunk = self._buff.get()
			if chunk is None:
				return
			data = [chunk]

			# Now consume whatever other data's still buffered.
			while True:
				try:
					chunk = self._buff.get(block=False)
					if chunk is None:
						return
					data.append(chunk)
				except queue.Empty:
					break

			yield b''.join(data)
# [END audio_stream]

def print_rms(rms):
	out = ''
	for _ in xrange(int(round(rms/30))):
		out = out + '*'
	
	print (out)

# KWS

import ktkws
KWSID = ['기가지니', '지니야', '친구야', '자기야']

def detect():
	global button
	with MicrophoneStream(RATE, CHUNK) as stream:
		audio_generator = stream.generator()
		for content in audio_generator:
			rc = ktkws.detect(content)
			rms = audioop.rms(content,2)
			if (button == True):
				rc = 1
				button = False
			if (rc == 1):
				gt2vt.play_file("../data/sample_sound.wav")
				return 200

def kws_test():
	rc = ktkws.init("../data/kwsmodel.pack")
	print ('init rc = %d' % (rc))
	rc = ktkws.start()
	#print ('start rc = %d' % (rc))
	ktkws.set_keyword(KWSID.index('기가지니'))
	rc = detect()
	print ('detect rc = %d' % (rc))
	ktkws.stop()
	ktkws.reset()
	return rc

def control_action(text): ##명령어 입력

######1번 열림
	open1_door1 = '1번 열어'
	open1_door2 = '1번 열어줘'
	open1_door3 = '1번 서랍 열어'
	open1_door4 = '1번 서랍 열어줘'
	open1_door5 = '일번 열어'
	open1_door6 = '일번 열어줘'
######1번 닫힘
	close1_door1 = '1번 닫아'
	close1_door2 = '1번 닫아줘'
	close1_door3 = '1번 서랍 닫아'
	close1_door4 = '1번 서랍 닫아줘'
	close1_door5 = '일번 닫아'
	close1_door6 = '일번 닫아줘'
######1번 잠금
	lock1_door1 = '1번 잠가'
	lock1_door2 = '1번 잠가줘'
	lock1_door3 = '1번 서랍 잠가'
	lock1_door4 = '1번 서랍 잠가줘'
	lock1_door5 = '일번 서랍 잠가'
	lock1_door6 = '일번 서랍 잠가줘'
	lock1_door7 = '1번 잠궈'
	lock1_door8 = '1번 서랍 잠궈'
	lock1_door9 = '일번 잠궈'
	lock1_door10 = '일번 서랍 잠궈'

######2번 열림
	open2_door1 = '2번 열어'
	open2_door2 = '2번 열어줘'
	open2_door3 = '2번 서랍 열어'
	open2_door4 = '2번 서랍 열어줘'
	open2_door5 = '이번 열어'
	open2_door6 = '이번 열어줘'
######2번 닫힘
	close2_door1 = '2번 닫아'
	close2_door2 = '2번 닫아줘'
	close2_door3 = '2번 서랍 닫아'
	close2_door4 = '2번 서랍 닫아줘'
	close2_door5 = '이번 닫아'
	close2_door6 = '이번 닫아줘'
######2번 잠금
	lock2_door1 = '2번 잠가'
	lock2_door2 = '2번 잠가줘'
	lock2_door3 = '2번 서랍 잠가'
	lock2_door4 = '2번 서랍 잠가줘'
	lock2_door5 = '이번 서랍 잠가'
	lock2_door6 = '이번 서랍 잠가줘'
	lock2_door7 = '2번 잠궈'
	lock2_door8 = '2번 서랍 잠궈'
	lock2_door9 = '이번 잠궈'
	lock2_door10 = '이번 서랍 잠궈'

	#cmd_init()
#####명령 조건문
	global state1
	global state2
	
	if (open1_door1 in text or open1_door2 in text or open1_door3 in text or open1_door4 in text or open1_door5 in text or open1_door6 in text):
		if(state1 == 3):
			print('1번 서랍 잠금을 해제합니다.') #터미널 창에 출력
			gt2vt.getText2VoiceStream("1번 서랍 잠금을 해제합니다.아","./number1_unlock.wav")
			gt2vt.play_file("./number1_unlock.wav")
			time.sleep(0.5)
			unlock1_AC()
			time.sleep(1)
			state1 = 1
			print('1번 서랍을 열겠습니다.') #터미널 창에 출력
			gt2vt.getText2VoiceStream("1번 서랍을 열겠습니다.아","./number1_open.wav") #명령어 실행시 AC 음성 입력
			gt2vt.play_file("./number1_open.wav") #AC 음성 출력
			time.sleep(0.5)
			open1_AC()
			time.sleep(1)
		elif(state1 == 2 or state1 == 0):
			state1 = 1
			print('1번 서랍을 열겠습니다.') #터미널 창에 출력
			gt2vt.getText2VoiceStream("1번 서랍을 열겠습니다.아","./number1_open.wav") #명령어 실행시 AC 음성 입력
			gt2vt.play_file("./number1_open.wav") #AC 음성 출력
			time.sleep(0.5)
			open1_AC() #작동
			time.sleep(1)
		else:
			print('1번 서랍은 이미 열려있습니다.') #터미널 창에 출력
			gt2vt.getText2VoiceStream("1번 서랍은 이미 열려있습니다.아","./number1_cannot_open.wav")
			gt2vt.play_file("./number1_cannot_open.wav")
			time.sleep(1)

	elif (close1_door1 in text or close1_door2 in text or close1_door3 in text or close1_door4 in text or close1_door5 in text or close1_door6 in text):
		if(state1 == 1 or state1 == 0):
			state1 = 2
			print('1번 서랍을 닫겠습니다.')
			gt2vt.getText2VoiceStream("1번 서랍을 닫겠습니다.아","./number1_close.wav")
			gt2vt.play_file("./number1_close.wav")
			time.sleep(0.5)
			close1_AC()
			time.sleep(1)
		elif(state1 == 2):
			print('1번 서랍은 이미 닫혀있습니다.') #터미널 창에 출력
			gt2vt.getText2VoiceStream("1번 서랍은 이미 닫혀있습니다.아","./number1_cannot_close1.wav")
			gt2vt.play_file("./number1_cannot_close1.wav")
			time.sleep(1)			

	elif (lock1_door1 in text or lock1_door2 in text or lock1_door3 in text or lock1_door4 in text or lock1_door5 in text or lock1_door6 in text or lock1_door7 in text or lock1_door8 in text or lock1_door9 in text or lock1_door10 in text):
		if(state1 == 2 or state1 == 0):
			state1 = 3
			print('1번 서랍을 잠그겠습니다.')
			gt2vt.getText2VoiceStream("1번 서랍을 잠그겠습니다.아","./number1_lock.wav")
			gt2vt.play_file("./number1_lock.wav")
			time.sleep(0.5)
			lock1_AC()
			time.sleep(1)
		elif(state1 == 3):
			print('1번 서랍은 이미 잠금 상태입니다.') #터미널 창에 출력
			gt2vt.getText2VoiceStream("1번 서랍은 이미 잠금 상태입니다.아","./number1_cannot_lock.wav")
			gt2vt.play_file("./number1_cannot_lock.wav")
			time.sleep(1)
		else:
			print('1번 서랍을 닫은 후 시도해주세요.') #터미널 창에 출력
			gt2vt.getText2VoiceStream("1번 서랍을 닫은 후 시도해주세요.아","./number1_cannot_close2.wav")
			gt2vt.play_file("./number1_cannot_close2.wav")
			time.sleep(1)			

	elif (open2_door1 in text or open2_door2 in text or open2_door3 in text or open2_door4 in text or open2_door5 in text or open2_door6 in text):
		if(state2 == 3):
			print('2번 서랍 잠금을 해제합니다.') #터미널 창에 출력
			gt2vt.getText2VoiceStream("2번 서랍 잠금을 해제합니다.아","./number2_unlock.wav")
			gt2vt.play_file("./number2_unlock.wav")
			time.sleep(0.5)
			unlock2_AC()
			time.sleep(1)
			state2 = 1
			print('2번 서랍을 열겠습니다.')
			gt2vt.getText2VoiceStream("2번 서랍을 열겠습니다.아","./number2_open.wav")
			gt2vt.play_file("./number2_open.wav")
			time.sleep(0.5)
			open2_AC()
			time.sleep(1)
		elif(state2 == 2 or state2 == 0):
			state2 = 1
			print('2번 서랍을 열겠습니다.')
			gt2vt.getText2VoiceStream("2번 서랍을 열겠습니다.아","./number2_open.wav")
			gt2vt.play_file("./number2_open.wav")
			time.sleep(0.5)
			open2_AC()
			time.sleep(1)
		else:
			print('2번 서랍은 이미 열려있습니다.') #터미널 창에 출력
			gt2vt.getText2VoiceStream("2번 서랍은 이미 열려있습니다.아","./number2_cannot_open.wav")
			gt2vt.play_file("./number2_cannot_open.wav")
			time.sleep(1)

	elif (close2_door1 in text or close2_door2 in text or close2_door3 in text or close2_door4 in text or close2_door5 in text or close2_door6 in text):
		if(state2 == 1 or state2 == 0):
			state2 = 2
			print('2번 서랍을 닫겠습니다.')
			gt2vt.getText2VoiceStream("2번 서랍을 닫겠습니다.아","./number2_close.wav")
			gt2vt.play_file("./number2_close.wav")
			time.sleep(0.5)
			close2_AC()
			time.sleep(1)
		elif(state2 == 2):
			print('2번 서랍은 이미 닫혀있습니다.') #터미널 창에 출력
			gt2vt.getText2VoiceStream("2번 서랍은 이미 닫혀있습니다.아","./number2_cannot_close1.wav")
			gt2vt.play_file("./number2_cannot_close1.wav")
			time.sleep(1)

	elif (lock2_door1 in text or lock2_door2 in text or lock2_door3 in text or lock2_door4 in text or lock2_door5 in text or lock2_door6 in text or lock2_door7 in text or lock2_door8 in text or lock2_door9 in text or lock2_door10 in text):
		if(state2 == 2 or state2 == 0):
			state2 = 3
			print('2번 서랍을 잠그겠습니다.')
			gt2vt.getText2VoiceStream("2번 서랍을 잠그겠습니다.아","./number2_lock.wav")
			gt2vt.play_file("./number2_lock.wav")
			time.sleep(0.5)
			lock2_AC()
			time.sleep(1)
		elif(state2 == 3):
			print('2번 서랍은 이미 잠금 상태입니다.') #터미널 창에 출력
			gt2vt.getText2VoiceStream("2번 서랍은 이미 잠금 상태입니다..아","./number2_cannot_lock.wav")
			gt2vt.play_file("./number2_cannot_lock.wav")
			time.sleep(1)
		else:
			print('2번 서랍을 닫은 후 시도해주세요.') #터미널 창에 출력
			gt2vt.getText2VoiceStream("2번 서랍을 닫은 후 시도해주세요.아","./number2_cannot_close2.wav")
			gt2vt.play_file("./number2_cannot_close2.wav")

	else:
		print('명령을 실행할 수 없습니다.')

def main():
	while 1:
		retry_game = True
		button = False
		recog=kws_test()
		say_exit1 = '종료'
		say_exit2 = '잘자'
		if recog == 200:
			gt2vt.getText2VoiceStream("안녕하세요? AC를 실행합니다.아","./AC_openning.wav")
			gt2vt.play_file("./AC_openning.wav")
			time.sleep(0.8)
			while 1:
				if retry_game:
					text = gv2t.getVoice2Text()
					control_action(text)
					if(say_exit1 in text or say_exit2 in text):
						gt2vt.getText2VoiceStream("AC를 종료합니다. 감사합니다.아","./AC_closing.wav")
						gt2vt.play_file("./AC_closing.wav")
						break
		else:
			print('지니를 불러주세요~')

if __name__ == '__main__':
	main()
