#!/usr/bin/env python3

# NOTE: this example requires PyAudio because it uses the Microphone class

import time
import speech_recognition as sr

key = ["Eddie", "Riley", "Adobea", "Gavin", "stop", "hello"]
count_success = 0
count_fail = 0


# this is called from the background thread
def callback(recognizer, audio):
    count[0] = count[0] + 1
    if count[0] == 10:
        exit()
    # received audio data, now we'll recognize it using Google Speech Recognition
    try:
        # for testing purposes, we're just using the default API key
        # to use another API key, use `r.recognize_google(audio, key="GOOGLE_SPEECH_RECOGNITION_API_KEY")`
        # instead of `r.recognize_google(audio)`
        phrase = recognizer.recognize_google(audio)
        print(phrase)

        if any(word in phrase for word in key):
            print("Transparency mode activated")
            t_end = time.time() + 3  # Send Passthrough for 3 Seconds

            while time.time() < t_end:
                time.sleep(1)
            print("ANC mode activated")

    except sr.UnknownValueError:
        print("Speech Recognition could not understand audio")
    except sr.RequestError as e:
        print("Could not request results from Google Speech Recognition service; {0}".format(e))


r = sr.Recognizer()
# r.energy_threshold =4000
# r.pause_threshold = 0.5
m = sr.Microphone(device_index=0)

with m as source:
    while True:
        r.adjust_for_ambient_noise(source)  # we only need to calibrate once, before we start listening
        audio = r.listen(source, timeout=None, phrase_time_limit=3)

        try:
            # for testing purposes, we're just using the default API key
            # to use another API key, use `r.recognize_google(audio, key="GOOGLE_SPEECH_RECOGNITION_API_KEY")`
            # instead of `r.recognize_google(audio)`
            phrase = r.recognize_google(audio)
            print(phrase)

            if any(word in phrase for word in key):
                count_success = count_success + 1
                print("Count Success: " + str(count_success))
                print("Count Fail: " + str(count_fail))
            # t_end = time.time() + 1	# Send Passthrough for 3 Seconds

            ##                while time.time() < t_end:
            ##                        time.sleep(1)
            # print("ANC mode activated")
            else:
                count_fail = count_fail + 1
                print("Count Success: " + str(count_success))
                print("Count Fail: " + str(count_fail))

        except sr.UnknownValueError:
            continue
            #count_fail = count_fail + 1
            #print("Count Success: " + str(count_success))
            #print("Count Fail: " + str(count_fail))
        except sr.RequestError as e:
            print("Could not request results from Google Speech Recognition service; {0}".format(e))

# start listening in the background (note that we don't have to do this inside a `with` statement)
# stop_listening = r.listen_in_background(m, callback)    # calls the function "callback" when it hears a word
# `stop_listening` is now a function that, when called, stops background listening

