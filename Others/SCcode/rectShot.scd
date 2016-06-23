(//SynthDef
SynthDef(\bufRead,
	{| bufnum = 0, gate = 1, amp = 1, pan = 0, rate = 1, loop = 1, trig = 1 |
		var sig;
		sig = PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum)*rate, trigger: trig, loop: loop, doneAction: 2);
	Out.ar(0, Pan2.ar(sig, pan, amp));
}).add;

SynthDef(\gunShot,
	{| bufnum = 0, gate = 1, amp = 1, pan = 0, rate = 1, loop = 0, trig = 1 |
		var sig;
		sig = PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum)*rate, trigger: trig, loop: loop, doneAction: 2);
	Out.ar(0, Pan2.ar(sig, pan, amp));
}).add;


);//Synthdef END



(// Audio buffers

//------------------------ bomb & gun ------------------------//
~bombBuf = Buffer.read(s, "/Users/giy/Desktop/rectShot/gunShotSound/explosion.wav");

~gunBuf = Array.newClear(4);
~gunBuf[0] = Buffer.read(s, "/Users/giy/Desktop/rectShot/gunShotSound/shot_1.wav");
~gunBuf[1] = Buffer.read(s, "/Users/giy/Desktop/rectShot/gunShotSound/shot_2.wav");
~gunBuf[2] = Buffer.read(s, "/Users/giy/Desktop/rectShot/gunShotSound/shot_3.wav");
~gunBuf[3] = Buffer.read(s, "/Users/giy/Desktop/rectShot/gunShotSound/shot_4.wav"); // laser


//------------------------ voices ------------------------//
~mainMixBuf = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/MIX.wav");

~voiceBuf = Array.newClear(20);
~voiceBuf[0] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/youWillBeShot.wav");
~voiceBuf[1] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/portraitRightWarning.wav");
~voiceBuf[2] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/publicService.wav");
~voiceBuf[3] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/underBus.wav");
~voiceBuf[4] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/mySon.wav");
~voiceBuf[5] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/dogChild2.wav");
~voiceBuf[6] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/dogChild.wav");
~voiceBuf[7] = Buffer.read(s, "/Users/giy/Desktop/rectShot/wav/theyAreWatching.wav");
)


//Main
(
//Oscfuncs
var mainMixSyn, voiceSyn, gunSyn, bombSyn,
ofAddr, oscfn, titleBuf, soundBuf, buf, syn, synFrag, synBlobs, len, lenDiv, lenDivSec,
fragContNum,
maxBox, mainAmpLimit;

maxBox = 80;
mainAmpLimit = 0.6;

fragContNum = Array.newClear(10);
10.do{|i| fragContNum[i] = 0};

ofAddr = NetAddr("127.0.0.1", 20000); // oF addr

oscfn = Array.newClear(10);

voiceSyn = Array.newClear(20);
gunSyn = Array.newClear(4);
// voiceBuf = Array.newClear(20);

soundBuf = Array.newClear(10);
buf = Array.newClear(10);
// syn = Array.newClear(10);
syn = Array2D.new(10, 10);
synFrag = Array.newClear(100);
synBlobs = Array.newClear(20);
len = Array.newClear(6);
lenDiv = Array.newClear(6);
lenDivSec = Array.newClear(6);





// titleBuf = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/title.wav");
// buf[0] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/aya.wav");
// buf[1] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/han.wav");
// buf[2] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/sewol.wav");
// buf[3] = Buffer.read(s, "/Users/teum11/Desktop/lonewolf_sound/park.wav");


mainMixSyn = Synth(\bufRead, [\bufnum, ~mainMixBuf.bufnum, \amp, 0]);

voiceSyn[0] = Synth(\bufRead, [\bufnum, ~voiceBuf[0].bufnum, \trig, 0, \rate, 0]);
voiceSyn[1] = Synth(\bufRead, [\bufnum, ~voiceBuf[1].bufnum, \trig, 0, \rate, 0]);
voiceSyn[2] = Synth(\bufRead, [\bufnum, ~voiceBuf[2].bufnum, \trig, 0, \rate, 0]);
voiceSyn[3] = Synth(\bufRead, [\bufnum, ~voiceBuf[3].bufnum, \trig, 0, \rate, 0]);
voiceSyn[4] = Synth(\bufRead, [\bufnum, ~voiceBuf[4].bufnum, \trig, 0, \rate, 0]);
voiceSyn[5] = Synth(\bufRead, [\bufnum, ~voiceBuf[5].bufnum, \trig, 0, \rate, 0]);
voiceSyn[6] = Synth(\bufRead, [\bufnum, ~voiceBuf[6].bufnum, \trig, 0, \rate, 0]);
voiceSyn[7] = Synth(\bufRead, [\bufnum, ~voiceBuf[7].bufnum, \trig, 0, \rate, 0]);


/*bombSyn = Synth(\gunShot, [\bufnum, ~bombBuf.bufnum, \amp, 0]);
gunSyn[0] = Synth(\gunShot, [\bufnum, ~gunBuf[0].bufnum]);
gunSyn[1] = Synth(\gunShot, [\bufnum, ~gunBuf[1].bufnum]);
gunSyn[2] = Synth(\gunShot, [\bufnum, ~gunBuf[2].bufnum]);
gunSyn[3] = Synth(\gunShot, [\bufnum, ~gunBuf[3].bufnum]);*/



//MainMix
oscfn[0] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	//msg[1] : number of box

	var mainAmp = msg[1]/maxBox;
	if (mainAmp >= mainAmpLimit, {mainAmp = mainAmpLimit;}); //Maximum amp limit
	("mainAmp: " + mainAmp).postln;

	mainMixSyn.set(\amp, mainAmp);

}, '/boxNum');


oscfn[1] = OSCFunc({|msg, time, addr, recvPort|

    var idx;
	msg.postln;

	//msg[1] : box idx
	//msg[2] : true/false
    idx = msg[1] % 8;
	("idx: " + idx).postln;

	if (msg[2] == 1,
		{
			voiceSyn[idx].set(\rate, 1, \trig, 1);
		},
        {
			voiceSyn[idx].set(\rate, 0);
        });

}, '/canHit');


oscfn[2] = OSCFunc({|msg, time, addr, recvPort|

    var idx;
	msg.postln;

	//msg[1] : box idx
    idx = msg[1] % 8;

	voiceSyn[idx].set(\rate, 0);

}, '/brkBox');



//SHOT
oscfn[3] = OSCFunc({|msg, time, addr, recvPort|
	//msg[1] = 1 ~ 4 : choose gun sound 1 ~ 4

	var idx;
	// msg.postln;
	idx = msg[1] - 1; //1 ~ 4 to 0 ~ 3

	gunSyn[idx] = Synth(\gunShot, [\bufnum, ~gunBuf[idx].bufnum, \amp, 0.2]);

}, '/gunShot');


//EXPLOSION
oscfn[4] = OSCFunc({|msg, time, addr, recvPort|
	//msg[1] = 1

	bombSyn = Synth(\gunShot, [\bufnum, ~bombBuf.bufnum]);

}, '/explo');



oscfn[5] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	if(msg[1] == 1, {
		titleBuf.play;
	});

}, '/title');


oscfn[6] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	//msg[1] = 0 / 1
	//0 : slide to Left Screen (boxes scene)
	//1 : slide to Right Screen (polygon bodie scene)

	var idx;
	idx = msg[1];

	if (idx == 1,
		{
			//Mute
			20.do{|i| voiceSyn[i].set(\rate, 0);};
			mainMixSyn.set(\rate, 0);
		},
		{
			//Unmute
			20.do{|i| voiceSyn[i].set(\rate, 1);};
			mainMixSyn.set(\rate, 1);
		}
	);

}, '/slideScreen');


oscfn[7] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	/*
	msg[1] = blob num
	*/

	if(msg[1] == 0,
		{
			10.do{|i| synBlobs[i].release(2)};
			10.do{|i|10.do{|j| syn[i,j].free;};

			};

		},
		{ msg[1].do{|i| synBlobs[i] = Synth(\liedR, [\amp, 0.1]);};}

	);

}, '/creatBlobSyn');


oscfn[8] = OSCFunc({|msg, time, addr, recvPort|
	// msg.postln;

	/*
	msg[1] = 0 or 1
	*/

	if(msg[1] == 1,{

		10.do{|i|
			10.do{|j| syn[i,j].free;};

			synBlobs[i].free;
			buf[i].free;
			soundBuf[i].free;
			titleBuf.free;
		};
	});

}, '/blackout');



CmdPeriod.doOnce{
	10.do{|i| oscfn[i].free;};

	20.do{|i| voiceSyn[i].free;};



}


)