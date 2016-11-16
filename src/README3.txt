
The #if 0
I'm getting rid of all of that.
I'd also like to get rid of if OLD
it's the old interface ... not interested in it.

The code is quite confused because of the portability and includng all the old stuff

the old stuff used a linked list stack.

fourcc (incredibly) is simply a uint32_t. It could mean "four character container"
menaing that it's really a four character string but without the \0 and all contained in a uint32.

batch mode and mixmodeare just two ways of handling volume result data from more than one file.

If it's only one file, well all its sample's volumeslevels are used to decide the final result.

However,if it's several files ...what sort of final result are you menat to give?

Mixmode is that all the files should be brought to same level so thatthey all sound the same.
batch mode is where their relative differences are kept constant which would make theludestfilein the pack
would decide theincrease in level of the other files.


in the maiin normalise this, we the getoptions rigmarolle s fairly long,
then we get the incongrusive otpion checkand finally we get to compute levels.

The whole design of the prog isbased on two pass, with compute-levels being the first pass. and then adjsting them in the second-

structs with leading _
As is good practice, it's prefereable to refer to structs via pointers to them. So, in this prog, the structs are called by names with leading underscored but only the pointers to them are actually typedef'd, and these DO NOT have a leading underscore. That seems to be the logic and it's not a bad one. However, I'm not too sure if it only really applies to the AFtype functions, most of them in wiener_af.c and which relate to the file handling.

AFfilehandle ... Imight begettig a bitcarried away with perl's interpretationof filehandle
but it's looking like the readied-up, fullyopen FILE ptrfrom whichbits maybeextracted. In other words the operative file source (as opposed to sink).


Gen Structure
-------------

Despite the talk of mp3 files, most of the code is given over to WAV files, in other words, RIFF.
MP3· is pretty much delegated else where.

I'm slowly coming to terms with the general structure of the code.

It can be run as a first pass ...whereby only information about the files is given,
or a a two pass where actual adjustments occur.

The hierarchy of the functionality is as follows:

normalize.c
riff.c
volume.c
adjust.c
wiener_af.c

"af" coudl stand for audiofile,but also audio filter.

There first pass doesn't really need to go beyond volume.c
while the second pass jumps from normalize.c to adjust.c due to the apply_gain call.

The issue of the gain
---------------------
Amazingly this really does seem to be just one number!

the -a option allows you to define the amplitude you want to st th elevel to if 

db and dbFS (decibel full scale)

fancy name frames
-----------------
throughout, thr riff file is dealt with in terms of frames. This sounds fancy and maybe in mp3
it's justifiied but in RIFF it's merely the bytes per sample (most often 2, occasionaly 3 or 4) multiplied 
by the number of channels. SO for hte usual stereo 16bit samples it's 4 bytes, 2 bytes 
each per channel. No heders no nothing. That's a frame for you.

Fd's FILE's, riff_streams
-------------------------
the program certainly make all this very complicated. The riff_stream actually is just a FILE pointer


the mystery of _riff_t
unusually, when looking in riff.h, this appears at the top without any predefinition. Neither are there previous #include's
so how cna some thing be announced and yet not throw up any errors in 
incredibly enough however, riff.h and riff.c will compile to a object file!
An unusual style quirk is the _riff_t is the normal struct, which as usual, you'll want to deal with in pointer terms.
but instead of defined vairables based on a pointer of _riff_t, a new typedef, without the leading underscore is defined as a pointer
to it. A particular style, I expect.
