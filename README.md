Welcome to the Siftone repo !
=============================

Siftone is a sifteo communication system to send sound messages to a phone
(or a computer). Here are the steps to use it:

1. download the Sifteo SDK: http://developers.sifteo.com

2. move this repo in the example folder and `cd` in it

3. start the sifteo cubes and execute:
  - `make run`      # to run the app in the simulator, or...
  - `make install`  # to install it in the sifteo base.

4. To try it with a smartphone, here are 2 apps that work:
 - android: https://play.google.com/store/apps/details?id=dk.rafaelcouto.PhoneTone_Extractor
 - iPhone*: https://itunes.apple.com/app/dtmf-reader/id392514798?mt=8

Enjoy !


*Warning: the iPhone app doesn't work with the default speed, you need a digit period at least twice a long.
See the variable called "durationSec" here: https://github.com/honnet/siftone/blob/master/siftone.h#L49

PS: a demo is available here: http://youtu.be/jnJJuw-4oGU
