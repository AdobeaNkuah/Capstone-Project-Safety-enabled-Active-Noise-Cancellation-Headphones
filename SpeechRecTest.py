import speech_recognition as sr

r = sr.Recognizer()
key = ["Eddie", "Riley", "Adobea", "Gavin", "Stop"]
flag = 0

# Using a Microphone as Input
mic = sr.Microphone(device_index = 0)
"""
while(flag == 0):
	with mic as source:
		audio = r.listen(source)

	response = r.recognize_google(audio, language = "en-US")

	if any(word in response for word in key):
		print("Key Word Detected")
		flag = 1
		break
"""
print("Listening...")

with mic as source:
	audio = r.listen(source)

response = r.recognize_google(audio, language = "en-US")

if any(word in response for word in key):
	print("Key Word Detected")
	flag = 1
else:
	print("No Keyword Detected, but Riley doesn't know how to make me loop over yet")












"""
with sr.Microphone(device_index=2) as source:
	print("Say something!")
	audio = r.listen(source)

# recognize speech using Google Speech Recognition
try:
	# for testing purposes, we're just using the default API key
	# to use another API key, use `r.recognize_google(audio, key="GOOGLE_SPEECH_RECOGNITION_API_KEY")`
	# instead of `r.recognize_google(audio)`
	print("Google Speech Recognition thinks you said in English: -  " + r.recognize_google(audio, language = "en-US"))
	print("Google Speech Recognition thinks you said in Turkish: -  " + r.recognize_google(audio, language = "tr-TR"))
except sr.UnknownValueError:
	print("Google Speech Recognition could not understand audio")
except sr.RequestError as e:
	print("Could not request results from Google Speech Recognition service; {0}".format(e))
	"""

#print("Google Speech Recognition thinks you said: -  \n" + r.recognize_google(audio, language = "en-US"))

"""
# Using an Audio file as input
harvard = sr.AudioFile('harvard.wav')
with harvard as source:
	audio = r.record(source)
"""