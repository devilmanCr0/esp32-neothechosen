# Greetings, this is a crypto challenge for the slug x gator ultimate collabora~ esp32 egg hunt

## In this challenge, participators are tasked on finding a unique and hidden URI in this web
### This uses RSA's chosen cipher attack to retrieve the flag

The attack works when the end user is given the ability to encrypt/decrypt any message they want (except for the ciphertext they're trying to reveal)
More information on the other [document](chosen-cipher.md)

Due to the nature of the intended solution, you may want to consider adding padding values to the flag. Usually the trailing characters get muddied out during the arithmetic process. 

You need the BigNumber library to set this arduino sketch on your end
http://www.gammon.com.au/Arduino/BigNumber.zip
(Drag and drop to your Libraries folder)

There's a function that should be customized on your end to facilitate the blinking lights


