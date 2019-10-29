from __future__ import absolute_import, division, print_function, unicode_literals

import os
import sklearn
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
from tensorflow import keras
from sklearn.model_selection import train_test_split
from sklearn.model_selection import KFold
from Lib import wave
from scipy import signal
from scipy.io import wavfile
from pydub import AudioSegment
from pydub.utils import which

AudioSegment.converter = which("ffmpeg")

wavlist = os.listdir(r"C:\Users\5upre\OneDrive\Documents\ELEC\ELEC 490\Sound Recognition Project\UrbanSound8K\audio")
kfolds = list([filename for filename in wavlist if filename.startswith("f")])
#kfolds = list(enumerate(kfolds))
#kf = KFold(n_splits=2)
fold = [None] * 10

for i, kfold in enumerate(kfolds):
    pathname = r"C:\Users\5upre\OneDrive\Documents\ELEC\ELEC 490\Sound Recognition Project\UrbanSound8K\audio" + f"\{kfold}"
    mainpathname = r"C:\Users\5upre\OneDrive\Documents\ELEC\ELEC 490\Sound Recognition Project\UrbanSound8K\audio_mono" + f"\{kfold}_mono"
    fold[i] = list(os.listdir(pathname))

for ds in fold:
    ds.remove('.DS_Store')

print(fold)
fold = list(enumerate(fold))

for j, wavlist in fold:
    pathname = r"C:\Users\5upre\OneDrive\Documents\ELEC\ELEC 490\Sound Recognition Project\UrbanSound8K\audio" + f"\{kfolds[j]}"
    mainpathname = r"C:\Users\5upre\OneDrive\Documents\ELEC\ELEC 490\Sound Recognition Project\UrbanSound8K\audio_mono" + f"\{kfolds[j]}_mono"
    for wavfile in wavlist:
        sound = AudioSegment.from_file(pathname + f"\{wavfile}")
        mono = sound.set_channels(1)
        idx = wavfile.index(".")
        wavfile_mono = wavfile[:idx] + "_mono" + wavfile[idx:]
        mono.export(mainpathname + f"\{wavfile_mono}", format="wav")

# for j in range(0, 10):
#     print(f"{fold[j]}\n")

    #X_train, X_test = train_test_split(fold, test_size=0.30, shuffle=False)

    # sample_rate, samples = wavfile.read('path-to-mono-audio-file.wav')
    # frequencies, times, spectrogram = signal.spectrogram(samples, sample_rate)



# plt.pcolormesh(times, frequencies, spectrogram)
# plt.imshow(spectrogram)
# plt.ylabel('Frequency [Hz]')
# plt.xlabel('Time [sec]')
# plt.show()



# wav_in_file = wave.Wave_read(wav)
# Wave_read.getnchannels()

# print(f"Training set: [{X_train}]")
# print(f"Testing set: [{X_test}]")